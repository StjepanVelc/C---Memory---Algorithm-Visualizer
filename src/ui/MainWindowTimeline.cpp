#include "ui/MainWindow.h"

#include <QComboBox>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QTabWidget>

#include "ui/MainWindowSupport.h"

namespace mw = ui::mainwindow;

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
