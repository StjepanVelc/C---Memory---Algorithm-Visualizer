#include "ui/MainWindow.h"

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QStatusBar>

#include "ui/MainWindowSupport.h"

MainWindow::MainWindow()
{
    m_playbackTimer.setInterval(450);
    m_replayTimer.setInterval(250);

    buildUi();
    applyLanguage(m_langCode);

    connect(&m_playbackTimer, &QTimer::timeout, this, &MainWindow::onPlaybackTick);
    connect(&m_replayTimer, &QTimer::timeout, this, &MainWindow::onReplayTick);
}

void MainWindow::onScenarioChanged(int index)
{
    if (index < 0)
    {
        return;
    }

    const QVariant selectedData = m_scenarioSelector->itemData(index);
    if (!selectedData.isValid())
    {
        return;
    }

    const auto scenario = static_cast<Scenario>(selectedData.toInt());
    m_engine.setScenario(scenario);
    updateInputGuidanceForScenario();
    if (m_isPlaying)
    {
        onPlayPause();
    }
    refreshView();
}

void MainWindow::onStepForward()
{
    m_engine.stepForward();
    refreshView();
}

void MainWindow::onStepBackward()
{
    m_engine.stepBackward();
    refreshView();
}

void MainWindow::onReset()
{
    m_engine.reset();
    refreshView();
}

void MainWindow::onPlayPause()
{
    m_isPlaying = !m_isPlaying;
    m_playPauseButton->setText(m_isPlaying ? t("pause", "Pause") : t("play", "Play"));

    if (m_isPlaying)
    {
        m_playbackTimer.start();
    }
    else
    {
        m_playbackTimer.stop();
    }
}

void MainWindow::onPlaybackTick()
{
    const auto &before = m_engine.currentFrame();
    m_engine.stepForward();
    const auto &after = m_engine.currentFrame();

    refreshView();

    if (before.currentStep == after.currentStep || after.currentStep >= after.totalSteps)
    {
        if (m_isPlaying)
        {
            onPlayPause();
        }
    }
}

void MainWindow::onSpeedChanged(int value)
{
    m_playbackTimer.setInterval(value);
    m_speedLabel->setText(QString("%1: %2 ms/step").arg(t("speed", "Speed")).arg(value));
}

void MainWindow::onLanguageChanged(int index)
{
    const QString langCode = m_languageSelector->itemData(index).toString();
    applyLanguage(langCode);
    statusBar()->showMessage(QString("%1 %2").arg(t("language.switched", "Language switched to")).arg(langCode.toUpper()), 2500);
    refreshView();
}
