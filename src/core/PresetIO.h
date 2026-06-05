#pragma once

#include <QJsonObject>

#include "core/SimulationTypes.h"

QJsonObject buildPresetJson(Scenario scenario, const UserScenarioConfig &config);
bool parsePresetJson(const QJsonObject &json, Scenario &scenario, UserScenarioConfig &config);
