#include "ui/MainWindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QSlider>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "ui/MainWindowSupport.h"
#include "ui/VisualizationCanvas.h"

namespace mw = ui::mainwindow;

void MainWindow::buildUi()
{
    setWindowTitle("C++ Memory & Algorithm Visualizer");
    setStyleSheet(
        "QMainWindow { background-color: #0b1220; color: #e2e8f0; }"
        "QLabel { color: #cbd5e1; }"
        "QTabBar::tab { background: #1e293b; color: #cbd5e1; padding: 8px 16px; border-radius: 6px; margin-right: 4px; }"
        "QTabBar::tab:selected { background: #0ea5e9; color: #0b1220; font-weight: 700; }"
        "QComboBox, QListWidget { background: #111827; color: #e5e7eb; border: 1px solid #334155; border-radius: 6px; padding: 6px; }"
        "QComboBox QAbstractItemView { background: #0f172a; color: #f8fafc; border: 1px solid #334155; selection-background-color: #0ea5e9; selection-color: #06202f; }"
        "QPushButton { background: #1d4ed8; color: white; border: none; border-radius: 6px; padding: 8px 12px; }"
        "QPushButton:hover { background: #2563eb; }"
        "QCheckBox { color: #cbd5e1; }");

    statusBar()->showMessage("Ready", 1500);

    auto *central = new QWidget(this);
    auto *root = new QHBoxLayout(central);

    auto *leftColumn = new QVBoxLayout();
    m_tabs = new QTabWidget(this);
    m_tabs->addTab(new QWidget(this), "Algorithms");
    m_tabs->addTab(new QWidget(this), "Memory");
    m_tabs->addTab(new QWidget(this), "Concurrency");
    m_tabs->addTab(new QWidget(this), "Data Structures");

    m_scenarioSelector = new QComboBox(this);
    m_titleLabel = new QLabel(this);
    m_titleLabel->setStyleSheet("font-size: 22px; font-weight: 700;");

    m_descriptionLabel = new QLabel(this);
    m_descriptionLabel->setWordWrap(true);
    m_modeBadgeLabel = new QLabel(this);
    m_modeBadgeLabel->setStyleSheet("background:#0ea5e9;color:#082f49;padding:4px 10px;border-radius:10px;font-weight:700;");

    m_canvas = new VisualizationCanvas(this);
    m_rowsList = new QListWidget(this);
    m_rowsList->setMaximumHeight(180);
    m_statsLabel = new QLabel(this);

    leftColumn->addWidget(m_tabs);
    leftColumn->addWidget(new QLabel("Scenario:", this));
    leftColumn->addWidget(m_scenarioSelector);
    leftColumn->addWidget(m_modeBadgeLabel);
    leftColumn->addWidget(m_titleLabel);
    leftColumn->addWidget(m_descriptionLabel);
    leftColumn->addWidget(m_canvas, 2);
    leftColumn->addWidget(m_rowsList, 1);
    leftColumn->addWidget(m_statsLabel);

    auto *inputRow = new QHBoxLayout();
    m_customValuesInput = new QLineEdit(this);
    m_customValuesInput->setPlaceholderText("values: 7,2,11,5");
    m_targetInput = new QLineEdit(this);
    m_targetInput->setPlaceholderText("target");
    m_targetInput->setMaximumWidth(90);
    m_insertIndexInput = new QLineEdit(this);
    m_insertIndexInput->setPlaceholderText("ins idx");
    m_insertIndexInput->setMaximumWidth(90);
    m_insertValueInput = new QLineEdit(this);
    m_insertValueInput->setPlaceholderText("ins val");
    m_insertValueInput->setMaximumWidth(90);
    m_deleteIndexInput = new QLineEdit(this);
    m_deleteIndexInput->setPlaceholderText("del idx");
    m_deleteIndexInput->setMaximumWidth(90);
    m_applyInputButton = new QPushButton("Apply Input", this);

    inputRow->addWidget(m_customValuesInput, 2);
    inputRow->addWidget(m_targetInput);
    inputRow->addWidget(m_insertIndexInput);
    inputRow->addWidget(m_insertValueInput);
    inputRow->addWidget(m_deleteIndexInput);
    inputRow->addWidget(m_applyInputButton);
    leftColumn->addLayout(inputRow);

    m_inputGuidanceLabel = new QLabel(this);
    m_inputGuidanceLabel->setStyleSheet("color:#93c5fd;font-size:12px;");
    leftColumn->addWidget(m_inputGuidanceLabel);

    auto *rightColumn = new QVBoxLayout();
    auto *backButton = new QPushButton("Step Back", this);
    auto *nextButton = new QPushButton("Step Forward", this);
    auto *resetButton = new QPushButton("Reset", this);
    m_playPauseButton = new QPushButton("Play", this);
    m_speedLabel = new QLabel("Speed: 450 ms/step", this);
    m_speedSlider = new QSlider(Qt::Horizontal, this);
    m_speedSlider->setRange(120, 1500);
    m_speedSlider->setValue(450);
    m_pointerAnimationToggle = new QCheckBox("Animate pointers", this);
    m_pointerAnimationToggle->setChecked(true);
    m_exportButton = new QPushButton("Export JSON + PNG", this);
    m_openExportsButton = new QPushButton("Open Exports Folder", this);
    m_savePresetButton = new QPushButton("Save Preset", this);
    m_loadPresetButton = new QPushButton("Load Preset", this);
    m_recordButton = new QPushButton("Record Session", this);
    m_replayButton = new QPushButton("Replay Session", this);
    m_languageSelector = new QComboBox(this);
    m_languageSelector->addItem("HR", "hr");
    m_languageSelector->addItem("EN", "en");
    m_timelineList = new QListWidget(this);
    m_timelineList->setMinimumHeight(140);

    rightColumn->addWidget(new QLabel("Controls", this));
    rightColumn->addWidget(m_playPauseButton);
    rightColumn->addWidget(backButton);
    rightColumn->addWidget(nextButton);
    rightColumn->addWidget(resetButton);
    rightColumn->addWidget(m_speedLabel);
    rightColumn->addWidget(m_speedSlider);
    rightColumn->addWidget(m_pointerAnimationToggle);
    rightColumn->addWidget(m_exportButton);
    rightColumn->addWidget(m_openExportsButton);
    rightColumn->addWidget(m_savePresetButton);
    rightColumn->addWidget(m_loadPresetButton);
    rightColumn->addWidget(m_recordButton);
    rightColumn->addWidget(m_replayButton);
    rightColumn->addWidget(new QLabel("Timeline", this));
    rightColumn->addWidget(m_timelineList);
    rightColumn->addWidget(new QLabel("Language", this));
    rightColumn->addWidget(m_languageSelector);
    rightColumn->addStretch();

    root->addLayout(leftColumn, 5);
    root->addLayout(rightColumn, 2);

    setCentralWidget(central);

    connect(m_tabs, &QTabWidget::currentChanged, this, [this](int)
            {
                loadScenariosForCurrentTab();
                refreshView(); });
    connect(m_scenarioSelector,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::onScenarioChanged);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onStepBackward);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::onStepForward);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onReset);
    connect(m_playPauseButton, &QPushButton::clicked, this, &MainWindow::onPlayPause);
    connect(m_speedSlider, &QSlider::valueChanged, this, &MainWindow::onSpeedChanged);
    connect(m_pointerAnimationToggle, &QCheckBox::toggled, m_canvas, &VisualizationCanvas::setPointerAnimationEnabled);
    connect(m_exportButton, &QPushButton::clicked, this, &MainWindow::onExportSnapshot);
    connect(m_openExportsButton, &QPushButton::clicked, this, &MainWindow::onOpenExportsFolder);
    connect(m_applyInputButton, &QPushButton::clicked, this, &MainWindow::onApplyCustomInput);
    connect(m_savePresetButton, &QPushButton::clicked, this, &MainWindow::onSavePreset);
    connect(m_loadPresetButton, &QPushButton::clicked, this, &MainWindow::onLoadPreset);
    connect(m_recordButton, &QPushButton::clicked, this, &MainWindow::onRecordSession);
    connect(m_replayButton, &QPushButton::clicked, this, &MainWindow::onReplaySession);
    connect(m_languageSelector,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::onLanguageChanged);
    connect(m_timelineList, &QListWidget::itemClicked, this, &MainWindow::onTimelineEventSelected);
}

void MainWindow::refreshView()
{
    const SimulationFrame frame = localizedFrame(m_engine.currentFrame());

    m_titleLabel->setText(frame.title);
    m_descriptionLabel->setText(frame.description);
    m_canvas->setFrame(frame);

    m_rowsList->clear();
    for (const auto &row : frame.rows)
    {
        m_rowsList->addItem(row);
    }

    m_statsLabel->setText(
        QString("%1 %2/%3 | %4: %5 | %6: %7")
            .arg(t("label.step", "Step"))
            .arg(frame.currentStep)
            .arg(frame.totalSteps)
            .arg(t("label.comparisons", "Comparisons"))
            .arg(frame.comparisons)
            .arg(t("label.swaps", "Swaps"))
            .arg(frame.swaps));

    refreshTimeline();
}

void MainWindow::loadScenariosForCurrentTab()
{
    m_scenarioSelector->blockSignals(true);
    m_scenarioSelector->clear();
    m_modeBadgeLabel->setText(QString("%1: %2").arg(t("label.mode", "Mode"), translateText(mw::modeNameForTab(m_tabs->currentIndex()))));
    updateTooltipsForMode();
    maybeShowTutorialOverlay();

    QList<mw::ScenarioItem> items;
    switch (m_tabs->currentIndex())
    {
    case 0:
        items = mw::algorithmItems();
        break;
    case 1:
        items = mw::memoryItems();
        break;
    case 2:
        items = mw::concurrencyItems();
        break;
    case 3:
        items = mw::dataStructureItems();
        break;
    default:
        items = mw::algorithmItems();
        break;
    }

    for (const auto &item : items)
    {
        m_scenarioSelector->addItem(translateText(item.label), static_cast<int>(item.scenario));
    }

    m_scenarioSelector->setCurrentIndex(0);
    m_scenarioSelector->blockSignals(false);

    if (m_scenarioSelector->count() > 0)
    {
        onScenarioChanged(0);
    }
}
