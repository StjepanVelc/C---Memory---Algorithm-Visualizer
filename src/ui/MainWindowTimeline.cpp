#include "ui/MainWindow.h"

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QTabWidget>

#include "ui/MainWindowSupport.h"

namespace mw = ui::mainwindow;

namespace
{
    void styleField(QLineEdit *field, bool primary, bool secondary, const QString &placeholder)
    {
        if (field == nullptr)
        {
            return;
        }

        field->setPlaceholderText(placeholder);

        if (primary)
        {
            field->setStyleSheet("QLineEdit { background:#0f172a; color:#e2e8f0; border:2px solid #22d3ee; border-radius:6px; padding:6px; }");
            return;
        }

        if (secondary)
        {
            field->setStyleSheet("QLineEdit { background:#0f172a; color:#cbd5e1; border:1px solid #64748b; border-radius:6px; padding:6px; }");
            return;
        }

        field->setStyleSheet("QLineEdit { background:#0b1324; color:#94a3b8; border:1px solid #334155; border-radius:6px; padding:6px; }");
    }
}

void MainWindow::updateTooltipsForMode()
{
    const int tab = m_tabs->currentIndex();
    const QString mode = mw::modeNameForTab(tab);

    m_scenarioSelector->setToolTip(QString("%1 %2.").arg(t("tooltip.scenario", "Mode-aware scenarios for"), translateText(mode)));
    m_customValuesInput->setToolTip(t("tooltip.values", "Comma-separated values, e.g: 7, 2, 11, 5"));
    m_targetInput->setToolTip(t("tooltip.target", "Target value for search scenarios."));
    m_insertIndexInput->setToolTip(t("tooltip.insertIndex", "Linked list insert index."));
    m_insertValueInput->setToolTip(t("tooltip.insertValue", "Linked list inserted value."));
    m_deleteIndexInput->setToolTip(t("tooltip.deleteIndex", "Linked list delete index."));
    m_applyInputButton->setToolTip(t("tooltip.applyInput", "Apply custom input to current scenario."));
    m_savePresetButton->setToolTip(t("tooltip.savePreset", "Save current scenario input as reproducible JSON preset."));
    m_loadPresetButton->setToolTip(t("tooltip.loadPreset", "Load scenario input preset from JSON."));
    m_timelineList->setToolTip(t("tooltip.timeline", "Click an event to jump to the recorded frame."));
    updateInputGuidanceForScenario();
}

void MainWindow::updateInputGuidanceForScenario()
{
    const QVariant selectedData = m_scenarioSelector->currentData();
    if (!selectedData.isValid())
    {
        return;
    }

    const Scenario scenario = static_cast<Scenario>(selectedData.toInt());

    bool valuesPrimary = false;
    bool targetPrimary = false;
    bool listOpsPrimary = false;
    bool targetSecondary = false;

    QString hint;
    switch (scenario)
    {
    case Scenario::LinearSearch:
    case Scenario::BinarySearch:
        valuesPrimary = true;
        targetPrimary = true;
        hint = t("inputHint.search", "Required: values + target. Other fields are ignored.");
        break;
    case Scenario::BubbleSort:
    case Scenario::SelectionSort:
    case Scenario::InsertionSort:
        valuesPrimary = true;
        targetSecondary = true;
        hint = t("inputHint.sort", "Required: values. Target is optional, list index fields are ignored.");
        break;
    case Scenario::LinkedList:
        listOpsPrimary = true;
        hint = t("inputHint.linkedList", "Required: insert index/value + delete index. Values/target are ignored.");
        break;
    default:
        hint = t("inputHint.none", "This scenario does not use custom input fields.");
        break;
    }

    styleField(m_customValuesInput, valuesPrimary, false, t("input.values", "values: 7,2,11,5"));
    styleField(m_targetInput, targetPrimary, targetSecondary, t("input.target", "target"));
    styleField(m_insertIndexInput, listOpsPrimary, false, t("input.insertIndex", "ins idx"));
    styleField(m_insertValueInput, listOpsPrimary, false, t("input.insertValue", "ins val"));
    styleField(m_deleteIndexInput, listOpsPrimary, false, t("input.deleteIndex", "del idx"));

    if (m_inputGuidanceLabel != nullptr)
    {
        m_inputGuidanceLabel->setText(hint);
    }
}

void MainWindow::maybeShowTutorialOverlay()
{
    const QString mode = mw::modeNameForTab(m_tabs->currentIndex());
    if (m_tutorialShown[mode])
    {
        return;
    }

    m_tutorialShown[mode] = true;
    QMessageBox::information(this,
                             QString("%1: %2").arg(t("tutorial.title", "Mini tutorial"), translateText(mode)),
                             t("tutorial.body", "1) Select scenario\n2) Optionally enter custom input\n3) Use step/play controls\n4) Export or Record/Replay."));
}

void MainWindow::refreshTimeline()
{
    if (m_timelineList == nullptr)
    {
        return;
    }

    m_timelineList->clear();

    const auto &events = m_engine.recordingEvents();
    for (const auto &event : events)
    {
        const QString action = translateText(event.action);
        const QString text = QString("[%1 ms] %2 (frame %3)")
                                 .arg(event.timestampMs)
                                 .arg(action)
                                 .arg(event.frameIndex);
        auto *item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, event.frameIndex);
        m_timelineList->addItem(item);
    }

    if (m_timelineList->count() > 0)
    {
        m_timelineList->scrollToBottom();
    }
}

void MainWindow::onTimelineEventSelected(QListWidgetItem *item)
{
    if (item == nullptr)
    {
        return;
    }

    const int frameIndex = item->data(Qt::UserRole).toInt();
    m_engine.jumpToFrame(frameIndex);
    refreshView();
    statusBar()->showMessage(QString("%1 %2").arg(t("status.jump", "Jumped to frame"), QString::number(frameIndex)), 2000);
}
