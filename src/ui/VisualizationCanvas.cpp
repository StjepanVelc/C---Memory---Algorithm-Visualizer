#include "ui/VisualizationCanvas.h"

#include <QPainter>
#include <QSet>

#include <algorithm>
#include <cmath>

namespace
{
    QColor cellColor(bool highlighted, bool freed)
    {
        if (freed)
        {
            return QColor("#9ca3af");
        }
        if (highlighted)
        {
            return QColor("#fb923c");
        }
        return QColor("#60a5fa");
    }

    qreal lerp(qreal from, qreal to, qreal t)
    {
        return from + (to - from) * t;
    }
} // namespace

VisualizationCanvas::VisualizationCanvas(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(280);

    m_transitionTimer.setInterval(16);
    connect(&m_transitionTimer, &QTimer::timeout, this, &VisualizationCanvas::onTransitionTick);
}

void VisualizationCanvas::setFrame(const SimulationFrame &frame)
{
    m_previousFrame = m_frame;
    m_frame = frame;

    const bool canAnimatePointers = !m_previousFrame.pointerLinks.empty() && !m_frame.pointerLinks.empty() &&
                                    !m_previousFrame.stackCells.empty() && !m_previousFrame.heapCells.empty() &&
                                    !m_frame.stackCells.empty() && !m_frame.heapCells.empty();

    if (canAnimatePointers && m_pointerAnimationEnabled)
    {
        m_transitionProgress = 0.0;
        m_transitionTimer.start();
    }
    else
    {
        m_transitionProgress = 1.0;
        m_transitionTimer.stop();
    }

    update();
}

void VisualizationCanvas::setPointerAnimationEnabled(bool enabled)
{
    m_pointerAnimationEnabled = enabled;
    if (!m_pointerAnimationEnabled)
    {
        m_transitionProgress = 1.0;
        m_transitionTimer.stop();
        update();
    }
}

void VisualizationCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    m_profiler.restart();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor("#0f172a"));

    QRect content = rect().adjusted(18, 18, -18, -18);

    if (!m_frame.arrayValues.empty())
    {
        drawAlgorithmView(painter, content);
    }

    if (!m_frame.stackCells.empty() || !m_frame.heapCells.empty())
    {
        drawMemoryView(painter, content);
    }

    if (!m_frame.stackValues.empty() || !m_frame.queueValues.empty())
    {
        drawStackQueueView(painter, content);
    }

    if (!m_frame.linkedListValues.empty())
    {
        drawLinkedListView(painter, content);
    }

    if (!m_frame.graphNodes.empty())
    {
        drawGraphView(painter, content);
    }

    if (!m_frame.threadStates.empty())
    {
        drawConcurrencyView(painter, content);
    }

    drawLegend(painter, content);
    m_lastPaintMs = static_cast<double>(m_profiler.nsecsElapsed()) / 1000000.0;
    drawProfilingOverlay(painter, content);
}

void VisualizationCanvas::drawGraphView(QPainter &painter, const QRect &rect) const
{
    Q_UNUSED(rect);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(QColor("#38bdf8"), 2));

    for (const auto &edge : m_frame.graphEdges)
    {
        if (edge.from < 0 || edge.to < 0 ||
            edge.from >= static_cast<int>(m_frame.graphNodes.size()) ||
            edge.to >= static_cast<int>(m_frame.graphNodes.size()))
        {
            continue;
        }

        const auto &a = m_frame.graphNodes[static_cast<std::size_t>(edge.from)];
        const auto &b = m_frame.graphNodes[static_cast<std::size_t>(edge.to)];
        painter.drawLine(QPoint(a.x, a.y), QPoint(b.x, b.y));
    }

    for (const auto &node : m_frame.graphNodes)
    {
        QRect circle(node.x - 24, node.y - 24, 48, 48);
        QColor fill = QColor("#334155");
        if (node.visited)
        {
            fill = QColor("#16a34a");
        }
        if (node.active)
        {
            fill = QColor("#f59e0b");
        }

        painter.setPen(Qt::NoPen);
        painter.setBrush(fill);
        painter.drawEllipse(circle);

        painter.setPen(Qt::white);
        painter.drawText(circle, Qt::AlignCenter, node.label);
    }
}

void VisualizationCanvas::drawStackQueueView(QPainter &painter, const QRect &rect) const
{
    const bool hasStack = !m_frame.stackValues.empty();
    const bool hasQueue = !m_frame.queueValues.empty();
    const bool animate = m_transitionProgress < 1.0;

    if (hasStack)
    {
        const int cellWidth = 96;
        const int cellHeight = 44;
        const int x = rect.left() + 24;
        const int baseY = rect.bottom() - 20;

        painter.setPen(QColor("#e2e8f0"));
        painter.drawText(x, baseY - static_cast<int>(m_frame.stackValues.size()) * (cellHeight + 6) - 12, "Stack (top at top)");

        const int currentSize = static_cast<int>(m_frame.stackValues.size());
        const int previousSize = static_cast<int>(m_previousFrame.stackValues.size());
        const int maxSize = std::max(currentSize, previousSize);

        for (int i = 0; i < maxSize; ++i)
        {
            const int currentDrawIndex = currentSize - 1 - i;
            const int previousDrawIndex = previousSize - 1 - i;
            const bool existsCurrent = currentDrawIndex >= 0;
            const bool existsPrevious = previousDrawIndex >= 0;

            if (!existsCurrent && !existsPrevious)
            {
                continue;
            }

            const int currentY = baseY - (i + 1) * (cellHeight + 6);
            int drawY = currentY;
            qreal opacity = 1.0;
            int value = existsCurrent ? m_frame.stackValues[static_cast<std::size_t>(currentDrawIndex)]
                                      : m_previousFrame.stackValues[static_cast<std::size_t>(previousDrawIndex)];

            if (animate)
            {
                if (existsCurrent && !existsPrevious)
                {
                    drawY = static_cast<int>(lerp(currentY - 24, currentY, m_transitionProgress));
                    opacity = m_transitionProgress;
                }
                else if (!existsCurrent && existsPrevious)
                {
                    drawY = static_cast<int>(lerp(currentY, currentY - 24, m_transitionProgress));
                    opacity = 1.0 - m_transitionProgress;
                }
            }

            const QRect cell(x, drawY, cellWidth, cellHeight);
            const bool top = existsCurrent && currentDrawIndex == currentSize - 1;

            painter.save();
            painter.setOpacity(opacity);
            painter.setPen(Qt::NoPen);
            painter.setBrush(top ? QColor("#f59e0b") : QColor("#334155"));
            painter.drawRoundedRect(cell, 6, 6);

            painter.setPen(Qt::white);
            painter.drawText(cell, Qt::AlignCenter, QString::number(value));
            if (top)
            {
                painter.setPen(QColor("#cbd5e1"));
                painter.drawText(cell.adjusted(0, -16, 0, 0), Qt::AlignHCenter, "TOP");
            }
            painter.restore();
        }
    }

    if (hasQueue)
    {
        const int cellWidth = 92;
        const int cellHeight = 54;
        const int gap = 10;
        const int y = rect.bottom() - cellHeight - 20;
        const int startX = rect.left() + 160;

        painter.setPen(QColor("#e2e8f0"));
        painter.drawText(startX, y - 12, "Queue (front -> back)");

        for (int i = 0; i < static_cast<int>(m_frame.queueValues.size()); ++i)
        {
            qreal drawX = startX + i * (cellWidth + gap);
            if (animate)
            {
                int previousIndex = -1;
                if (i < static_cast<int>(m_previousFrame.queueValues.size()) &&
                    m_previousFrame.queueValues[static_cast<std::size_t>(i)] == m_frame.queueValues[static_cast<std::size_t>(i)])
                {
                    previousIndex = i;
                }
                else if (i + 1 < static_cast<int>(m_previousFrame.queueValues.size()) &&
                         m_previousFrame.queueValues[static_cast<std::size_t>(i + 1)] == m_frame.queueValues[static_cast<std::size_t>(i)])
                {
                    previousIndex = i + 1;
                }
                else if (i > 0 && i - 1 < static_cast<int>(m_previousFrame.queueValues.size()) &&
                         m_previousFrame.queueValues[static_cast<std::size_t>(i - 1)] == m_frame.queueValues[static_cast<std::size_t>(i)])
                {
                    previousIndex = i - 1;
                }

                if (previousIndex >= 0)
                {
                    const qreal previousX = startX + previousIndex * (cellWidth + gap);
                    drawX = lerp(previousX, drawX, m_transitionProgress);
                }
            }

            const QRect cell(static_cast<int>(drawX), y, cellWidth, cellHeight);
            const bool isFront = i == m_frame.queueFront;
            const bool isBack = i == m_frame.queueBack;

            painter.setPen(Qt::NoPen);
            painter.setBrush((isFront || isBack) ? QColor("#38bdf8") : QColor("#334155"));
            painter.drawRoundedRect(cell, 6, 6);

            painter.setPen(Qt::white);
            painter.drawText(cell.adjusted(0, 0, 0, -14), Qt::AlignCenter, QString::number(m_frame.queueValues[static_cast<std::size_t>(i)]));
            painter.setPen(QColor("#e2e8f0"));
            if (isFront && isBack)
            {
                painter.drawText(cell.adjusted(0, 24, 0, 0), Qt::AlignCenter, "FRONT/BACK");
            }
            else if (isFront)
            {
                painter.drawText(cell.adjusted(0, 24, 0, 0), Qt::AlignCenter, "FRONT");
            }
            else if (isBack)
            {
                painter.drawText(cell.adjusted(0, 24, 0, 0), Qt::AlignCenter, "BACK");
            }
        }
    }
}

void VisualizationCanvas::drawAlgorithmView(QPainter &painter, const QRect &rect) const
{
    painter.setPen(Qt::NoPen);

    const int n = static_cast<int>(m_frame.arrayValues.size());
    if (n == 0)
    {
        return;
    }

    const int gap = 8;
    const int blockWidth = (rect.width() - (n - 1) * gap) / n;
    const int blockHeight = 90;
    const int y = rect.top() + 24;

    QSet<int> highlighted;
    for (int index : m_frame.highlightedIndices)
    {
        highlighted.insert(index);
    }

    for (int i = 0; i < n; ++i)
    {
        const QRect block(rect.left() + i * (blockWidth + gap), y, blockWidth, blockHeight);
        const bool isHighlighted = highlighted.contains(i);

        painter.setBrush(isHighlighted ? QColor("#f59e0b") : QColor("#334155"));
        painter.drawRoundedRect(block, 8, 8);

        painter.setPen(Qt::white);
        painter.drawText(block, Qt::AlignCenter, QString::number(m_frame.arrayValues[static_cast<std::size_t>(i)]));
        painter.setPen(QColor("#cbd5e1"));
        painter.drawText(block.adjusted(0, 58, 0, 0), Qt::AlignHCenter, QString("idx %1").arg(i));
        painter.setPen(Qt::NoPen);
    }
}

void VisualizationCanvas::drawMemoryView(QPainter &painter, const QRect &rect) const
{
    const int boxWidth = (rect.width() - 40) / 2;
    const int boxHeight = 46;

    const int stackX = rect.left();
    const int heapX = rect.left() + boxWidth + 40;
    const int startY = rect.top() + 130;

    painter.setPen(QColor("#e2e8f0"));
    painter.drawText(stackX, startY - 12, "Stack");
    painter.drawText(heapX, startY - 12, "Heap");

    for (std::size_t i = 0; i < m_frame.stackCells.size(); ++i)
    {
        const auto &cell = m_frame.stackCells[i];
        const QRect r(stackX, startY + static_cast<int>(i) * (boxHeight + 8), boxWidth, boxHeight);
        painter.setBrush(cellColor(cell.highlighted, cell.freed));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(r, 6, 6);

        painter.setPen(Qt::white);
        painter.drawText(r.adjusted(10, 6, -10, -24), Qt::AlignLeft, QString("%1 = %2").arg(cell.name, cell.value));
        painter.setPen(QColor("#e2e8f0"));
        painter.drawText(r.adjusted(10, 24, -10, -4), Qt::AlignLeft, cell.address);
    }

    for (std::size_t i = 0; i < m_frame.heapCells.size(); ++i)
    {
        const auto &cell = m_frame.heapCells[i];
        const QRect r(heapX, startY + static_cast<int>(i) * (boxHeight + 8), boxWidth, boxHeight);
        painter.setBrush(cellColor(cell.highlighted, cell.freed));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(r, 6, 6);

        painter.setPen(Qt::white);
        painter.drawText(r.adjusted(10, 6, -10, -24), Qt::AlignLeft, QString("%1 = %2").arg(cell.name, cell.value));
        painter.setPen(QColor("#e2e8f0"));
        painter.drawText(r.adjusted(10, 24, -10, -4), Qt::AlignLeft, cell.address);
    }

    painter.setPen(QPen(QColor("#f97316"), 2));
    const int currentStackCount = static_cast<int>(m_frame.stackCells.size());
    const int currentHeapCount = static_cast<int>(m_frame.heapCells.size());
    const int previousStackCount = static_cast<int>(m_previousFrame.stackCells.size());
    const int previousHeapCount = static_cast<int>(m_previousFrame.heapCells.size());

    for (std::size_t i = 0; i < m_frame.pointerLinks.size(); ++i)
    {
        const auto &link = m_frame.pointerLinks[i];
        if (link.stackIndex < 0 || link.heapIndex < 0)
        {
            continue;
        }
        if (link.stackIndex >= currentStackCount || link.heapIndex >= currentHeapCount)
        {
            continue;
        }

        const int y1 = startY + link.stackIndex * (boxHeight + 8) + boxHeight / 2;
        const int y2 = startY + link.heapIndex * (boxHeight + 8) + boxHeight / 2;
        QPointF from(stackX + boxWidth, y1);
        QPointF to(heapX, y2);

        if (m_transitionProgress < 1.0 && i < m_previousFrame.pointerLinks.size())
        {
            const auto &previousLink = m_previousFrame.pointerLinks[i];
            if (previousLink.stackIndex >= 0 && previousLink.heapIndex >= 0 &&
                previousLink.stackIndex < previousStackCount && previousLink.heapIndex < previousHeapCount)
            {
                const QPointF previousFrom(stackX + boxWidth,
                                           startY + previousLink.stackIndex * (boxHeight + 8) + boxHeight / 2);
                const QPointF previousTo(heapX,
                                         startY + previousLink.heapIndex * (boxHeight + 8) + boxHeight / 2);

                from = previousFrom + (from - previousFrom) * m_transitionProgress;
                to = previousTo + (to - previousTo) * m_transitionProgress;
            }
        }

        painter.drawLine(from, to);

        const QPointF arrowBase = from + (to - from) * 0.85;
        const QPointF direction = to - arrowBase;
        const QPointF normal(-direction.y(), direction.x());

        const qreal len = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
        QPointF unitDir(0.0, 0.0);
        QPointF unitNormal(0.0, 0.0);
        if (len > 0.001)
        {
            unitDir = direction / len;
            unitNormal = normal / len;
        }

        const QPointF p1 = to;
        const QPointF p2 = to - unitDir * 10.0 + unitNormal * 5.0;
        const QPointF p3 = to - unitDir * 10.0 - unitNormal * 5.0;
        painter.setBrush(QColor("#f97316"));
        painter.drawPolygon(QPolygonF() << p1 << p2 << p3);

        painter.setPen(QColor("#f8fafc"));
        painter.drawText(QPointF((from.x() + to.x()) / 2.0 - 20.0, (from.y() + to.y()) / 2.0 - 6.0), link.label);
        painter.setPen(QPen(QColor("#f97316"), 2));
    }
}

void VisualizationCanvas::drawLinkedListView(QPainter &painter, const QRect &rect) const
{
    const int nodeHeight = 72;
    const int nodeWidth = 128;
    const int gap = 34;
    const int y = rect.top() + 120;
    const int n = static_cast<int>(m_frame.linkedListValues.size());
    const int previousCount = static_cast<int>(m_previousFrame.linkedListValues.size());
    const bool animate = m_transitionProgress < 1.0;

    if (n == 0 && previousCount == 0)
    {
        return;
    }

    const int maxCount = std::max(n, previousCount);
    const int totalWidth = maxCount * nodeWidth + std::max(0, maxCount - 1) * gap;
    const int startX = std::max(rect.left(), rect.left() + (rect.width() - totalWidth) / 2);

    painter.setPen(QColor("#e2e8f0"));
    painter.drawText(startX, y - 12, "Linked List (node + next)");

    std::vector<int> prevToCurrent(previousCount, -1);
    std::vector<int> currentToPrev(n, -1);
    std::vector<bool> currentTaken(n, false);

    for (int p = 0; p < previousCount; ++p)
    {
        const int previousValue = m_previousFrame.linkedListValues[static_cast<std::size_t>(p)];
        for (int c = 0; c < n; ++c)
        {
            if (!currentTaken[static_cast<std::size_t>(c)] &&
                m_frame.linkedListValues[static_cast<std::size_t>(c)] == previousValue)
            {
                prevToCurrent[static_cast<std::size_t>(p)] = c;
                currentToPrev[static_cast<std::size_t>(c)] = p;
                currentTaken[static_cast<std::size_t>(c)] = true;
                break;
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        qreal currentX = startX + i * (nodeWidth + gap);
        qreal drawX = currentX;
        qreal drawY = y;
        qreal opacity = 1.0;

        if (animate)
        {
            const int previousIndex = currentToPrev[static_cast<std::size_t>(i)];
            if (previousIndex >= 0)
            {
                const qreal previousX = startX + previousIndex * (nodeWidth + gap);
                drawX = lerp(previousX, currentX, m_transitionProgress);
            }
            else
            {
                drawY = lerp(y - 22, y, m_transitionProgress);
                opacity = m_transitionProgress;
            }
        }

        const QRect node(static_cast<int>(drawX), static_cast<int>(drawY), nodeWidth, nodeHeight);
        const bool highlighted = i == m_frame.linkedListHighlight;
        painter.save();
        painter.setOpacity(opacity);
        painter.setPen(Qt::NoPen);
        painter.setBrush(highlighted ? QColor("#f59e0b") : QColor("#334155"));
        painter.drawRoundedRect(node, 8, 8);

        painter.setPen(QColor("#1e293b"));
        painter.drawLine(node.center().x(), node.top() + 8, node.center().x(), node.bottom() - 8);

        painter.setPen(Qt::white);
        painter.drawText(node.adjusted(0, 0, -nodeWidth / 2, 0), Qt::AlignCenter, QString::number(m_frame.linkedListValues[static_cast<std::size_t>(i)]));

        const QString nextLabel = (i + 1 < n) ? "next" : "null";
        painter.drawText(node.adjusted(nodeWidth / 2, 0, 0, 0), Qt::AlignCenter, nextLabel);

        if (i < static_cast<int>(m_frame.linkedListAddresses.size()))
        {
            painter.setPen(QColor("#cbd5e1"));
            painter.drawText(node.adjusted(0, nodeHeight - 18, 0, 0), Qt::AlignHCenter, m_frame.linkedListAddresses[static_cast<std::size_t>(i)]);
        }

        if (i + 1 < n)
        {
            const QPoint from(node.right() + 4, node.center().y());
            const QPoint to(node.right() + gap - 8, node.center().y());
            painter.setPen(QPen(QColor("#38bdf8"), 2));
            painter.drawLine(from, to);
            painter.setBrush(QColor("#38bdf8"));
            painter.drawPolygon(QPolygon() << QPoint(to.x(), to.y()) << QPoint(to.x() - 8, to.y() - 4) << QPoint(to.x() - 8, to.y() + 4));
        }
        painter.restore();
    }

    if (animate)
    {
        for (int p = 0; p < previousCount; ++p)
        {
            if (prevToCurrent[static_cast<std::size_t>(p)] != -1)
            {
                continue;
            }

            const qreal previousX = startX + p * (nodeWidth + gap);
            const qreal drawY = lerp(y, y - 22, m_transitionProgress);
            const qreal opacity = 1.0 - m_transitionProgress;
            const QRect ghostNode(static_cast<int>(previousX), static_cast<int>(drawY), nodeWidth, nodeHeight);

            painter.save();
            painter.setOpacity(opacity);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor("#64748b"));
            painter.drawRoundedRect(ghostNode, 8, 8);
            painter.setPen(QColor("#f1f5f9"));
            painter.drawText(ghostNode, Qt::AlignCenter,
                             QString::number(m_previousFrame.linkedListValues[static_cast<std::size_t>(p)]));
            painter.restore();
        }
    }
}

void VisualizationCanvas::drawConcurrencyView(QPainter &painter, const QRect &rect) const
{
    const int cardWidth = 200;
    const int cardHeight = 64;
    const int gap = 14;
    const int startX = rect.left();
    const int y = rect.bottom() - cardHeight - 10;

    painter.setPen(QColor("#e2e8f0"));
    painter.drawText(startX, y - 10, QString("Mutex owner: %1").arg(m_frame.mutexOwner.isEmpty() ? "none" : m_frame.mutexOwner));

    for (std::size_t i = 0; i < m_frame.threadStates.size(); ++i)
    {
        const auto &thread = m_frame.threadStates[i];
        const QRect card(startX + static_cast<int>(i) * (cardWidth + gap), y, cardWidth, cardHeight);
        painter.setPen(Qt::NoPen);
        painter.setBrush(thread.active ? QColor("#16a34a") : QColor("#475569"));
        painter.drawRoundedRect(card, 8, 8);

        painter.setPen(Qt::white);
        painter.drawText(card.adjusted(10, 8, -10, -30), Qt::AlignLeft, thread.name);
        painter.drawText(card.adjusted(10, 30, -10, -8), Qt::AlignLeft, thread.state);
    }
}

void VisualizationCanvas::drawLegend(QPainter &painter, const QRect &rect) const
{
    const int legendWidth = 390;
    const int legendHeight = 82;
    const int margin = 8;

    int legendX = rect.right() - legendWidth - margin;
    int legendY = rect.bottom() - legendHeight - margin;

    // Concurrency cards are rendered near the bottom; keep legend at the top-right there.
    if (!m_frame.threadStates.empty())
    {
        legendY = rect.top() + margin;
    }

    const QRect box(legendX, legendY, legendWidth, legendHeight);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(15, 23, 42, 210));
    painter.drawRoundedRect(box, 8, 8);

    struct LegendItem
    {
        QColor color;
        QString label;
    };

    const std::vector<LegendItem> items{
        {QColor("#f59e0b"), "highlight"},
        {QColor("#60a5fa"), "active value"},
        {QColor("#9ca3af"), "freed/moved-from"},
        {QColor("#16a34a"), "active thread"}};

    int x = box.left() + 14;
    const int y = box.top() + 18;
    for (const auto &item : items)
    {
        painter.setBrush(item.color);
        painter.drawRoundedRect(QRect(x, y, 14, 14), 3, 3);
        painter.setPen(QColor("#e2e8f0"));
        painter.drawText(QRect(x + 20, y - 1, 85, 16), Qt::AlignLeft | Qt::AlignVCenter, item.label);
        painter.setPen(Qt::NoPen);
        x += 95;
    }

    const QRect statusBadge(box.left() + 14, box.top() + 52, 170, 20);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_pointerAnimationEnabled ? QColor("#16a34a") : QColor("#b91c1c"));
    painter.drawRoundedRect(statusBadge, 10, 10);
    painter.setPen(Qt::white);
    painter.drawText(statusBadge, Qt::AlignCenter, m_pointerAnimationEnabled ? "Pointer animation: ON" : "Pointer animation: OFF");
}

void VisualizationCanvas::drawProfilingOverlay(QPainter &painter, const QRect &rect) const
{
    const int boxWidth = 270;
    const int boxHeight = 58;
    const QRect box(rect.left() + 8, rect.top() + 8, boxWidth, boxHeight);

    int logicalItems = static_cast<int>(m_frame.arrayValues.size() +
                                        m_frame.stackCells.size() + m_frame.heapCells.size() +
                                        m_frame.stackValues.size() + m_frame.queueValues.size() +
                                        m_frame.linkedListValues.size() + m_frame.graphNodes.size() +
                                        m_frame.threadStates.size());
    const double animationCost = (1.0 - m_transitionProgress) * 100.0;

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(2, 6, 23, 210));
    painter.drawRoundedRect(box, 8, 8);
    painter.setPen(QColor("#cbd5e1"));
    painter.drawText(box.adjusted(10, 8, -10, -30), Qt::AlignLeft,
                     QString("Frame: %1 ms  |  Anim: %2%").arg(QString::number(m_lastPaintMs, 'f', 2), QString::number(animationCost, 'f', 0)));
    painter.drawText(box.adjusted(10, 30, -10, -8), Qt::AlignLeft,
                     QString("Approx visual items: %1").arg(logicalItems));
}

void VisualizationCanvas::onTransitionTick()
{
    m_transitionProgress = std::min(1.0, m_transitionProgress + 0.12);
    if (m_transitionProgress >= 1.0)
    {
        m_transitionTimer.stop();
    }
    update();
}
