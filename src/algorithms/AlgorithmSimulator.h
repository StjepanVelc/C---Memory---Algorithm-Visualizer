#pragma once

#include "core/SimulationTypes.h"

class AlgorithmSimulator
{
public:
    static FrameSequence generate(Scenario scenario);

private:
    static FrameSequence linearSearch();
    static FrameSequence binarySearch();
    static FrameSequence bubbleSort();
    static FrameSequence selectionSort();
    static FrameSequence insertionSort();
};
