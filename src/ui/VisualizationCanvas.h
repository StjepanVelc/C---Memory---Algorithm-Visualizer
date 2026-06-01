#pragma once

#include <QTimer>
#include <QWidget>

#include "core/SimulationTypes.h"

class VisualizationCanvas final : public QWidget
{
    Q_OBJECT

public:
    explicit VisualizationCanvas(QWidget *parent = nullptr);

    void setFrame(const SimulationFrame &frame);
    void setPointerAnimationEnabled(bool enabled);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawAlgorithmView(QPainter &painter, const QRect &rect) const;
    void drawMemoryView(QPainter &painter, const QRect &rect) const;
    void drawStackQueueView(QPainter &painter, const QRect &rect) const;
    void drawLinkedListView(QPainter &painter, const QRect &rect) const;
    void drawConcurrencyView(QPainter &painter, const QRect &rect) const;
    void drawLegend(QPainter &painter, const QRect &rect) const;

    void onTransitionTick();

    SimulationFrame m_frame;
    SimulationFrame m_previousFrame;
    QTimer m_transitionTimer;
    double m_transitionProgress = 1.0;
    bool m_pointerAnimationEnabled = true;
};
