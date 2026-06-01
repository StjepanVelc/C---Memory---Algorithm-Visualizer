#include "core/SimulationEngine.h"

#include <algorithm>

#include "algorithms/AlgorithmSimulator.h"
#include "concurrency/ConcurrencySimulator.h"
#include "memory/MemorySimulator.h"

SimulationEngine::SimulationEngine()
{
    m_frames = buildFrames(m_scenario);
}

void SimulationEngine::setScenario(Scenario scenario)
{
    m_scenario = scenario;
    m_frames = buildFrames(scenario);
    m_index = 0;
}

void SimulationEngine::reset()
{
    m_index = 0;
}

void SimulationEngine::stepForward()
{
    if (m_frames.empty())
    {
        return;
    }
    m_index = std::min(m_index + 1, m_frames.size() - 1);
}

void SimulationEngine::stepBackward()
{
    if (m_frames.empty() || m_index == 0)
    {
        return;
    }
    --m_index;
}

const SimulationFrame &SimulationEngine::currentFrame() const
{
    static const SimulationFrame emptyFrame{
        "No data",
        "Scenario has no generated frames.",
        {},
        0,
        0,
        0,
        0};

    if (m_frames.empty())
    {
        return emptyFrame;
    }

    return m_frames[m_index];
}

Scenario SimulationEngine::currentScenario() const
{
    return m_scenario;
}

FrameSequence SimulationEngine::buildFrames(Scenario scenario) const
{
    switch (scenario)
    {
    case Scenario::LinearSearch:
    case Scenario::BinarySearch:
    case Scenario::BubbleSort:
    case Scenario::SelectionSort:
    case Scenario::InsertionSort:
        return AlgorithmSimulator::generate(scenario);

    case Scenario::StackMemory:
    case Scenario::HeapMemory:
    case Scenario::StackDs:
    case Scenario::QueueDs:
    case Scenario::LinkedList:
    case Scenario::NewDelete:
    case Scenario::UniquePtr:
    case Scenario::CopyMove:
        return MemorySimulator::generate(scenario);

    case Scenario::ThreadMutex:
    case Scenario::RaceCondition:
        return ConcurrencySimulator::generate(scenario);
    }

    return {};
}
