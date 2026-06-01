#pragma once

#include <vector>

#include "core/SimulationTypes.h"

class AlgorithmSimulator;
class MemorySimulator;
class ConcurrencySimulator;

class SimulationEngine
{
public:
    SimulationEngine();

    void setScenario(Scenario scenario);
    void reset();
    void stepForward();
    void stepBackward();

    [[nodiscard]] const SimulationFrame &currentFrame() const;
    [[nodiscard]] Scenario currentScenario() const;

private:
    FrameSequence buildFrames(Scenario scenario) const;

    Scenario m_scenario = Scenario::LinearSearch;
    FrameSequence m_frames;
    std::size_t m_index = 0;
};
