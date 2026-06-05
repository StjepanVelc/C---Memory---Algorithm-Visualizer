#pragma once

#include <QElapsedTimer>

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
    void setUserConfig(const UserScenarioConfig &config);
    void reset();
    void stepForward();
    void stepBackward();
    void jumpToFrame(int index);

    void beginRecording();
    void endRecording();
    [[nodiscard]] bool isRecording() const;

    [[nodiscard]] const SimulationFrame &currentFrame() const;
    [[nodiscard]] Scenario currentScenario() const;
    [[nodiscard]] int currentFrameIndex() const;
    [[nodiscard]] int frameCount() const;
    [[nodiscard]] const std::vector<RecordingEvent> &recordingEvents() const;
    [[nodiscard]] const UserScenarioConfig &userConfig() const;

private:
    void pushRecordingEvent(const QString &action);
    FrameSequence buildFrames(Scenario scenario) const;

    Scenario m_scenario = Scenario::LinearSearch;
    UserScenarioConfig m_userConfig;
    FrameSequence m_frames;
    std::size_t m_index = 0;
    bool m_isRecording = false;
    QElapsedTimer m_recordingTimer;
    std::vector<RecordingEvent> m_recordingEvents;
};
