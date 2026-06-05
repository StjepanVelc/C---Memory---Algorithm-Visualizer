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
    pushRecordingEvent("set-scenario");
}

void SimulationEngine::setUserConfig(const UserScenarioConfig &config)
{
    m_userConfig = config;
    m_frames = buildFrames(m_scenario);
    m_index = 0;
    pushRecordingEvent("set-config");
}

void SimulationEngine::reset()
{
    m_index = 0;
    pushRecordingEvent("reset");
}

void SimulationEngine::stepForward()
{
    if (m_frames.empty())
    {
        return;
    }
    m_index = std::min(m_index + 1, m_frames.size() - 1);
    pushRecordingEvent("step-forward");
}

void SimulationEngine::stepBackward()
{
    if (m_frames.empty() || m_index == 0)
    {
        return;
    }
    --m_index;
    pushRecordingEvent("step-backward");
}

void SimulationEngine::jumpToFrame(int index)
{
    if (m_frames.empty())
    {
        return;
    }

    if (index < 0)
    {
        m_index = 0;
    }
    else
    {
        m_index = std::min(static_cast<std::size_t>(index), m_frames.size() - 1);
    }
    pushRecordingEvent("jump-frame");
}

void SimulationEngine::beginRecording()
{
    m_recordingEvents.clear();
    m_recordingTimer.restart();
    m_isRecording = true;
    pushRecordingEvent("recording-start");
}

void SimulationEngine::endRecording()
{
    pushRecordingEvent("recording-stop");
    m_isRecording = false;
}

bool SimulationEngine::isRecording() const
{
    return m_isRecording;
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

int SimulationEngine::currentFrameIndex() const
{
    return static_cast<int>(m_index);
}

int SimulationEngine::frameCount() const
{
    return static_cast<int>(m_frames.size());
}

const std::vector<RecordingEvent> &SimulationEngine::recordingEvents() const
{
    return m_recordingEvents;
}

const UserScenarioConfig &SimulationEngine::userConfig() const
{
    return m_userConfig;
}

void SimulationEngine::pushRecordingEvent(const QString &action)
{
    if (!m_isRecording)
    {
        return;
    }

    m_recordingEvents.push_back(
        RecordingEvent{static_cast<int>(m_index), action, m_recordingTimer.elapsed()});
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
    case Scenario::GraphBfs:
        return AlgorithmSimulator::generate(scenario, m_userConfig);

    case Scenario::StackMemory:
    case Scenario::HeapMemory:
    case Scenario::StackDs:
    case Scenario::QueueDs:
    case Scenario::LinkedList:
    case Scenario::NewDelete:
    case Scenario::UniquePtr:
    case Scenario::CopyMove:
        return MemorySimulator::generate(scenario, m_userConfig);

    case Scenario::ThreadMutex:
    case Scenario::RaceCondition:
        return ConcurrencySimulator::generate(scenario, m_userConfig);
    }

    return {};
}
