#include "ui/MainWindow.h"

#include <QComboBox>
#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QStringList>
#include <QUrl>

#include "core/I18nKey.h"
#include "ui/MainWindowSupport.h"
#include "ui/VisualizationCanvas.h"

namespace mw = ui::mainwindow;

void MainWindow::onExportSnapshot()
{
    const auto &frame = m_engine.currentFrame();

    QDir rootDir = mw::resolveProjectRootFromApp();
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
    root.insert("mode", mw::modeNameForTab(m_tabs->currentIndex()));
    root.insert("scenario", m_scenarioSelector->currentText());
    root.insert("arrayValues", mw::intVectorToJson(frame.arrayValues));
    root.insert("stackValues", mw::intVectorToJson(frame.stackValues));
    root.insert("queueValues", mw::intVectorToJson(frame.queueValues));
    root.insert("linkedListValues", mw::intVectorToJson(frame.linkedListValues));
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
                                 t("dialog.exportSuccess", "Export successful"),
                                 QString("%1:\n%2").arg(t("dialog.savedIn", "Saved JSON and screenshot in"), exportDir.path()));
        statusBar()->showMessage(QString("%1 %2").arg(t("status.exported", "Exported JSON + PNG to"), exportDir.path()), 4000);
    }
    else
    {
        QMessageBox::warning(this,
                             t("dialog.exportPartial", "Export partial"),
                             QString("Screenshot: %1\nJSON: %2")
                                 .arg(imageSaved ? "OK" : "FAILED")
                                 .arg(jsonSaved ? "OK" : "FAILED"));
        statusBar()->showMessage(t("status.exportFailed", "Export failed or partial."), 4000);
    }
}

void MainWindow::onOpenExportsFolder()
{
    QDir rootDir = mw::resolveProjectRootFromApp();
    if (!rootDir.exists("exports"))
    {
        rootDir.mkdir("exports");
    }

    const QString exportsPath = rootDir.filePath("exports");
    const bool opened = QDesktopServices::openUrl(QUrl::fromLocalFile(exportsPath));
    if (!opened)
    {
        QMessageBox::warning(this, t("dialog.openFailed", "Open folder failed"), QString("%1:\n%2").arg(t("dialog.couldNotOpen", "Could not open"), exportsPath));
        statusBar()->showMessage(t("status.openExportsFailed", "Opening exports folder failed."), 3000);
        return;
    }
    statusBar()->showMessage(QString("%1: %2").arg(t("status.openExportsOk", "Opened exports folder"), exportsPath), 3000);
}

void MainWindow::applyLanguage(const QString &langCode)
{
    m_langCode = langCode;
    m_i18n = mw::loadI18n(langCode);

    m_playPauseButton->setText(t("play", "Play"));
    m_exportButton->setText(t("export", "Export JSON + PNG"));
    m_openExportsButton->setText(t("openExports", "Open Exports Folder"));
    m_recordButton->setText(m_engine.isRecording() ? t("recordStop", "Stop Recording") : t("record", "Record Session"));
    m_replayButton->setText(t("replay", "Replay Session"));
    m_applyInputButton->setText(t("applyInput", "Apply Input"));
    m_savePresetButton->setText(t("savePreset", "Save Preset"));
    m_loadPresetButton->setText(t("loadPreset", "Load Preset"));

    updateTooltipsForMode();
    loadScenariosForCurrentTab();
    refreshView();
}

QString MainWindow::t(const QString &key, const QString &fallback) const
{
    return m_i18n.value(key).toString(fallback);
}

QString MainWindow::translateText(const QString &text) const
{
    if (i18nkey::isKeyed(text))
    {
        const QString key = i18nkey::key(text);
        QString resolved = t(key, key);
        const QStringList args = i18nkey::args(text);
        for (const auto &arg : args)
        {
            resolved = resolved.arg(arg);
        }
        return resolved;
    }

    return text;
}

SimulationFrame MainWindow::localizedFrame(const SimulationFrame &frame) const
{
    SimulationFrame localized = frame;
    localized.title = translateText(frame.title);
    localized.description = translateText(frame.description);

    QStringList rows;
    rows.reserve(frame.rows.size());
    for (const auto &row : frame.rows)
    {
        rows << translateText(row);
    }
    localized.rows = rows;

    for (auto &thread : localized.threadStates)
    {
        thread.name = translateText(thread.name);
        thread.state = translateText(thread.state);
    }

    return localized;
}
