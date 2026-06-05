#pragma once

#include "core/SimulationTypes.h"

class AlgorithmSimulator
{
public:
    static FrameSequence generate(Scenario scenario, const UserScenarioConfig &config);

private:
    static FrameSequence linearSearch(const UserScenarioConfig &config);
    static FrameSequence binarySearch(const UserScenarioConfig &config);
    static FrameSequence bubbleSort(const UserScenarioConfig &config);
    static FrameSequence selectionSort(const UserScenarioConfig &config);
    static FrameSequence insertionSort(const UserScenarioConfig &config);
    static FrameSequence graphBfs(const UserScenarioConfig &config);
};
