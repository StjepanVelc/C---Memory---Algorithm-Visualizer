#include "ui/MainWindow.h"

#include <QComboBox>
#include <QCheckBox>
#include <QCoreApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QSlider>
#include <QTabWidget>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

#include "ui/VisualizationCanvas.h"

namespace
{
    struct ScenarioItem
    {
        QString label;
        Scenario scenario;
    };

    QList<ScenarioItem> algorithmItems()
    {
        return {{"Linear Search", Scenario::LinearSearch},
                {"Binary Search", Scenario::BinarySearch},
                {"Bubble Sort", Scenario::BubbleSort},
                {"Selection Sort", Scenario::SelectionSort},
                {"Insertion Sort", Scenario::InsertionSort}};
    }

    QList<ScenarioItem> memoryItems()
    {
        return {{"Stack varijable", Scenario::StackMemory},
                {"Heap objekti", Scenario::HeapMemory},
                {"new/delete", Scenario::NewDelete},
                {"unique_ptr", Scenario::UniquePtr},
                {"copy vs move", Scenario::CopyMove}};
    }

    QList<ScenarioItem> dataStructureItems()
    {
        return {{"Stack (LIFO)", Scenario::StackDs},
                {"Queue (FIFO)", Scenario::QueueDs},
                {"Linked list", Scenario::LinkedList}};
    }

    QList<ScenarioItem> concurrencyItems()
    {
        return {{"Thread + Mutex", Scenario::ThreadMutex},
                {"Race Condition", Scenario::RaceCondition}};
    }

    QString modeNameForTab(int index)
    {
        switch (index)
        {
        case 0:
            return "Algorithms";
        case 1:
            return "Memory";
        case 2:
            return "Concurrency";
        case 3:
            return "Data Structures";
        default:
            return "Unknown";
        }
    }

    QJsonArray intVectorToJson(const std::vector<int> &values)
    {
        QJsonArray array;
        for (int value : values)
        {
            array.append(value);
        }
        return array;
    }

    QDir resolveProjectRootFromApp()
    {
        QDir rootDir(QCoreApplication::applicationDirPath());
        for (int i = 0; i < 3; ++i)
        {
            rootDir.cdUp();
        }
        if (!rootDir.exists())
        {
            rootDir = QDir::current();
        }
        return rootDir;
    }
} // namespace

MainWindow::MainWindow()
{
    m_playbackTimer.setInterval(450);

    buildUi();
    loadScenariosForCurrentTab();
    refreshView();

    connect(&m_playbackTimer, &QTimer::timeout, this, &MainWindow::onPlaybackTick);
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
    m_playPauseButton->setText(m_isPlaying ? "Pause" : "Play");

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
    m_speedLabel->setText(QString("Speed: %1 ms/step").arg(value));
}

void MainWindow::onExportSnapshot()
{
    const auto &frame = m_engine.currentFrame();

    QDir rootDir = resolveProjectRootFromApp();

    if (!rootDir.exists("exports"))
    {
        rootDir.mkdir("exports");
    }

    QDir exportDir(rootDir.filePath("exports"));

    const QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString scenarioName = m_scenarioSelector->currentText();
    scenarioName.replace(' ', '_');
    scenarioName.replace('/', '_');
    const QString baseName = QString("%1_%2").arg(scenarioName, timestamp);

    const QString imagePath = exportDir.filePath(baseName + ".png");
    const QString jsonPath = exportDir.filePath(baseName + ".json");

    const bool imageSaved = m_canvas->grab().save(imagePath);

    QJsonObject root;
    root.insert("title", frame.title);
    root.insert("description", frame.description);
    root.insert("step", frame.currentStep);
    root.insert("totalSteps", frame.totalSteps);
    root.insert("comparisons", frame.comparisons);
    root.insert("swaps", frame.swaps);
    root.insert("mode", modeNameForTab(m_tabs->currentIndex()));
    root.insert("scenario", m_scenarioSelector->currentText());
    root.insert("arrayValues", intVectorToJson(frame.arrayValues));
    root.insert("stackValues", intVectorToJson(frame.stackValues));
    root.insert("queueValues", intVectorToJson(frame.queueValues));
    root.insert("linkedListValues", intVectorToJson(frame.linkedListValues));
    root.insert("timestamp", QDateTime::currentDateTime().toString(Qt::ISODate));
    root.insert("screenshot", imagePath);

    QJsonArray eventRows;
    for (const auto &row : frame.rows)
    {
        eventRows.append(row);
    }
    root.insert("rows", eventRows);

    QFile file(jsonPath);
    bool jsonSaved = false;
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
        file.close();
        jsonSaved = true;
    }

    if (imageSaved && jsonSaved)
    {
        QMessageBox::information(this,
                                 "Export successful",
                                 QString("Saved JSON and screenshot in:\n%1").arg(exportDir.path()));
    }
    else
    {
        QMessageBox::warning(this,
                             "Export partial",
                             QString("Screenshot: %1\nJSON: %2")
                                 .arg(imageSaved ? "OK" : "FAILED")
                                 .arg(jsonSaved ? "OK" : "FAILED"));
    }
}

void MainWindow::onOpenExportsFolder()
{
    QDir rootDir = resolveProjectRootFromApp();
    if (!rootDir.exists("exports"))
    {
        rootDir.mkdir("exports");
    }

    const QString exportsPath = rootDir.filePath("exports");
    const bool opened = QDesktopServices::openUrl(QUrl::fromLocalFile(exportsPath));
    if (!opened)
    {
        QMessageBox::warning(this, "Open folder failed", QString("Could not open:\n%1").arg(exportsPath));
    }
}

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
}

void MainWindow::refreshView()
{
    const auto &frame = m_engine.currentFrame();

    m_titleLabel->setText(frame.title);
    m_descriptionLabel->setText(frame.description);
    m_canvas->setFrame(frame);

    m_rowsList->clear();
    for (const auto &row : frame.rows)
    {
        m_rowsList->addItem(row);
    }

    m_statsLabel->setText(
        QString("Step %1/%2 | Comparisons: %3 | Swaps: %4")
            .arg(frame.currentStep)
            .arg(frame.totalSteps)
            .arg(frame.comparisons)
            .arg(frame.swaps));
}

void MainWindow::loadScenariosForCurrentTab()
{
    m_scenarioSelector->blockSignals(true);
    m_scenarioSelector->clear();
    m_modeBadgeLabel->setText(QString("Mode: %1").arg(modeNameForTab(m_tabs->currentIndex())));

    QList<ScenarioItem> items;
    switch (m_tabs->currentIndex())
    {
    case 0:
        items = algorithmItems();
        break;
    case 1:
        items = memoryItems();
        break;
    case 2:
        items = concurrencyItems();
        break;
    case 3:
        items = dataStructureItems();
        break;
    default:
        items = algorithmItems();
        break;
    }

    for (const auto &item : items)
    {
        m_scenarioSelector->addItem(item.label, static_cast<int>(item.scenario));
    }

    m_scenarioSelector->setCurrentIndex(0);
    m_scenarioSelector->blockSignals(false);

    if (m_scenarioSelector->count() > 0)
    {
        onScenarioChanged(0);
    }
}
