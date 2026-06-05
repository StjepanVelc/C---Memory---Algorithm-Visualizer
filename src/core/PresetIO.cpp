#include "core/PresetIO.h"

#include <QJsonArray>

namespace
{
    std::vector<int> readValues(const QJsonArray &array)
    {
        std::vector<int> values;
        values.reserve(static_cast<std::size_t>(array.size()));
        for (const auto &value : array)
        {
            values.push_back(value.toInt());
        }
        return values;
    }
} // namespace

QJsonObject buildPresetJson(Scenario scenario, const UserScenarioConfig &config)
{
    QJsonObject json;
    json.insert("version", 1);
    json.insert("scenario", static_cast<int>(scenario));

    QJsonArray values;
    for (int value : config.values)
    {
        values.append(value);
    }

    json.insert("values", values);
    json.insert("target", config.target);
    json.insert("insertIndex", config.insertIndex);
    json.insert("insertValue", config.insertValue);
    json.insert("deleteIndex", config.deleteIndex);
    return json;
}

bool parsePresetJson(const QJsonObject &json, Scenario &scenario, UserScenarioConfig &config)
{
    if (!json.contains("scenario"))
    {
        return false;
    }

    scenario = static_cast<Scenario>(json.value("scenario").toInt());
    config.values = readValues(json.value("values").toArray());
    config.target = json.value("target").toInt(0);
    config.insertIndex = json.value("insertIndex").toInt(0);
    config.insertValue = json.value("insertValue").toInt(0);
    config.deleteIndex = json.value("deleteIndex").toInt(0);
    return true;
}
