#include "ui/MainWindow.h"

#include <QPushButton>
#include <QStatusBar>

void MainWindow::onRecordSession()
{
    if (!m_engine.isRecording())
    {
        m_engine.beginRecording();
        m_recordButton->setText(t("recordStop", "Stop Recording"));
        statusBar()->showMessage(t("status.recordingStarted", "Session recording started."), 3000);
    }
    else
    {
        m_engine.endRecording();
        m_recordButton->setText(t("record", "Record Session"));
        statusBar()->showMessage(QString("%1 (%2).")
                                     .arg(t("status.recordingStopped", "Recording stopped"))
                                     .arg(m_engine.recordingEvents().size()),
                                 3000);
    }
    refreshTimeline();
}

void MainWindow::onReplaySession()
{
    m_replayEvents = m_engine.recordingEvents();
    if (m_replayEvents.empty())
    {
        statusBar()->showMessage(t("status.noReplay", "No recording available for replay."), 3000);
        return;
    }

    m_replayIndex = 0;
    m_replayTimer.start();
    statusBar()->showMessage(t("status.replayStarted", "Replay started."), 3000);
}

void MainWindow::onReplayTick()
{
    if (m_replayIndex >= static_cast<int>(m_replayEvents.size()))
    {
        m_replayTimer.stop();
        statusBar()->showMessage(t("status.replayFinished", "Replay finished."), 3000);
        return;
    }

    const auto &event = m_replayEvents[static_cast<std::size_t>(m_replayIndex)];
    m_engine.jumpToFrame(event.frameIndex);
    refreshView();
    ++m_replayIndex;
}
