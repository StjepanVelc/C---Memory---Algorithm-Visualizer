#pragma once

#include <map>
#include <vector>

#include <QJsonObject>
#include <QMainWindow>
#include <QTimer>

#include "core/SimulationEngine.h"

class QComboBox;
class QLabel;
class QListWidget;
class QPushButton;
class QCheckBox;
class QLineEdit;
class QSlider;
class QTabWidget;
class QListWidgetItem;
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
    void onApplyCustomInput();
    void onSavePreset();
    void onLoadPreset();
    void onRecordSession();
    void onReplaySession();
    void onReplayTick();
    void onLanguageChanged(int index);
    void onTimelineEventSelected(QListWidgetItem *item);

private:
    void buildUi();
    void refreshView();
    void loadScenariosForCurrentTab();
    void updateTooltipsForMode();
    void updateInputGuidanceForScenario();
    void maybeShowTutorialOverlay();
    UserScenarioConfig parseUserConfig() const;
    void applyLanguage(const QString &langCode);
    void refreshTimeline();
    QString t(const QString &key, const QString &fallback) const;
    QString translateText(const QString &text) const;
    SimulationFrame localizedFrame(const SimulationFrame &frame) const;

    QTabWidget *m_tabs = nullptr;
    QComboBox *m_scenarioSelector = nullptr;
    QLabel *m_titleLabel = nullptr;
    QLabel *m_descriptionLabel = nullptr;
    QLabel *m_modeBadgeLabel = nullptr;
    VisualizationCanvas *m_canvas = nullptr;
    QListWidget *m_rowsList = nullptr;
    QListWidget *m_timelineList = nullptr;
    QLabel *m_statsLabel = nullptr;
    QPushButton *m_playPauseButton = nullptr;
    QLabel *m_speedLabel = nullptr;
    QSlider *m_speedSlider = nullptr;
    QCheckBox *m_pointerAnimationToggle = nullptr;
    QPushButton *m_exportButton = nullptr;
    QPushButton *m_openExportsButton = nullptr;
    QLineEdit *m_customValuesInput = nullptr;
    QLineEdit *m_targetInput = nullptr;
    QLineEdit *m_insertIndexInput = nullptr;
    QLineEdit *m_insertValueInput = nullptr;
    QLineEdit *m_deleteIndexInput = nullptr;
    QLabel *m_inputGuidanceLabel = nullptr;
    QPushButton *m_applyInputButton = nullptr;
    QPushButton *m_savePresetButton = nullptr;
    QPushButton *m_loadPresetButton = nullptr;
    QPushButton *m_recordButton = nullptr;
    QPushButton *m_replayButton = nullptr;
    QComboBox *m_languageSelector = nullptr;

    QTimer m_replayTimer;
    std::vector<RecordingEvent> m_replayEvents;
    int m_replayIndex = 0;
    QString m_langCode = "hr";
    QJsonObject m_i18n;
    std::map<QString, bool> m_tutorialShown;

    SimulationEngine m_engine;
    QTimer m_playbackTimer;
    bool m_isPlaying = false;
};
