#include "ui/MainWindow.h"

#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QLineEdit>
#include <QStatusBar>
#include <QStringList>

#include "core/PresetIO.h"
#include "ui/MainWindowSupport.h"

namespace mw = ui::mainwindow;

UserScenarioConfig MainWindow::parseUserConfig() const
{
    UserScenarioConfig config;

    const QStringList values = m_customValuesInput->text().split(',', Qt::SkipEmptyParts);
    for (const auto &item : values)
    {
        bool ok = false;
        const int parsed = item.trimmed().toInt(&ok);
        if (ok)
        {
            config.values.push_back(parsed);
        }
    }

    bool ok = false;
    config.target = m_targetInput->text().toInt(&ok);
    if (!ok)
    {
        config.target = 0;
    }

    config.insertIndex = m_insertIndexInput->text().toInt(&ok);
    if (!ok)
    {
        config.insertIndex = 1;
    }

    config.insertValue = m_insertValueInput->text().toInt(&ok);
    if (!ok)
    {
        config.insertValue = 25;
    }

    config.deleteIndex = m_deleteIndexInput->text().toInt(&ok);
    if (!ok)
    {
        config.deleteIndex = 1;
    }

    return config;
}

void MainWindow::onApplyCustomInput()
{
    const UserScenarioConfig config = parseUserConfig();
    m_engine.setUserConfig(config);
    statusBar()->showMessage(t("status.customInput", "Custom input applied."), 3000);
    refreshView();
}

void MainWindow::onSavePreset()
{
    const UserScenarioConfig config = parseUserConfig();

    QDir rootDir = mw::resolveProjectRootFromApp();
    if (!rootDir.exists("presets"))
    {
        rootDir.mkdir("presets");
    }

    const QString path = rootDir.filePath("presets/last_preset.json");
    const QJsonObject json = buildPresetJson(m_engine.currentScenario(), config);

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        statusBar()->showMessage(t("status.presetSaveFailed", "Saving preset failed."), 3000);
        return;
    }
    file.write(QJsonDocument(json).toJson(QJsonDocument::Indented));
    file.close();

    statusBar()->showMessage(QString("%1: %2").arg(t("status.presetSaved", "Preset saved"), path), 4000);
}

void MainWindow::onLoadPreset()
{
    QDir rootDir = mw::resolveProjectRootFromApp();
    const QString defaultPath = rootDir.filePath("presets/last_preset.json");
    const QString path = QFileDialog::getOpenFileName(this, t("dialog.loadPreset", "Load preset"), defaultPath, "Preset (*.json)");
    if (path.isEmpty())
    {
        return;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        statusBar()->showMessage(t("status.presetLoadFailed", "Loading preset failed."), 3000);
        return;
    }

    const QJsonObject json = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    UserScenarioConfig config;
    Scenario scenario = Scenario::LinearSearch;
    if (!parsePresetJson(json, scenario, config))
    {
        statusBar()->showMessage(t("status.presetInvalid", "Invalid preset format."), 3000);
        return;
    }

    m_customValuesInput->setText(mw::joinInts(config.values));
    m_targetInput->setText(QString::number(config.target));
    m_insertIndexInput->setText(QString::number(config.insertIndex));
    m_insertValueInput->setText(QString::number(config.insertValue));
    m_deleteIndexInput->setText(QString::number(config.deleteIndex));

    const int scenarioUiIndex = m_scenarioSelector->findData(static_cast<int>(scenario));
    if (scenarioUiIndex >= 0)
    {
        m_scenarioSelector->setCurrentIndex(scenarioUiIndex);
    }

    onApplyCustomInput();
    statusBar()->showMessage(QString("%1: %2").arg(t("status.presetLoaded", "Preset loaded"), path), 4000);
}
