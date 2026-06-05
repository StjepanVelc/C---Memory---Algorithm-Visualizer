#pragma once

#include "core/SimulationTypes.h"

class ConcurrencySimulator
{
public:
    static FrameSequence generate(Scenario scenario, const UserScenarioConfig &config);
};
