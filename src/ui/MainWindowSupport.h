#pragma once

#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QList>

#include <vector>

#include "core/SimulationTypes.h"

namespace ui::mainwindow
{
    struct ScenarioItem
    {
        QString label;
        Scenario scenario;
    };

    QList<ScenarioItem> algorithmItems();
    QList<ScenarioItem> memoryItems();
    QList<ScenarioItem> dataStructureItems();
    QList<ScenarioItem> concurrencyItems();

    QString modeNameForTab(int index);
    QJsonArray intVectorToJson(const std::vector<int> &values);
    QString joinInts(const std::vector<int> &values);

    QDir resolveProjectRootFromApp();
    QString i18nPathFor(const QString &langCode);
    QJsonObject loadI18n(const QString &langCode);
} // namespace ui::mainwindow
