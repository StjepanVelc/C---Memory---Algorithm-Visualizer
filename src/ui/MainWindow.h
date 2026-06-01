#pragma once

#include <QMainWindow>
#include <QTimer>

#include "core/SimulationEngine.h"

class QComboBox;
class QLabel;
class QListWidget;
class QPushButton;
class QCheckBox;
class QSlider;
class QTabWidget;
class VisualizationCanvas;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void onScenarioChanged(int index);
    void onStepForward();
    void onStepBackward();
    void onReset();
    void onPlayPause();
    void onPlaybackTick();
    void onSpeedChanged(int value);
    void onExportSnapshot();
    void onOpenExportsFolder();

private:
    void buildUi();
    void refreshView();
    void loadScenariosForCurrentTab();

    QTabWidget *m_tabs = nullptr;
    QComboBox *m_scenarioSelector = nullptr;
    QLabel *m_titleLabel = nullptr;
    QLabel *m_descriptionLabel = nullptr;
    QLabel *m_modeBadgeLabel = nullptr;
    VisualizationCanvas *m_canvas = nullptr;
    QListWidget *m_rowsList = nullptr;
    QLabel *m_statsLabel = nullptr;
    QPushButton *m_playPauseButton = nullptr;
    QLabel *m_speedLabel = nullptr;
    QSlider *m_speedSlider = nullptr;
    QCheckBox *m_pointerAnimationToggle = nullptr;
    QPushButton *m_exportButton = nullptr;
    QPushButton *m_openExportsButton = nullptr;

    SimulationEngine m_engine;
    QTimer m_playbackTimer;
    bool m_isPlaying = false;
};
