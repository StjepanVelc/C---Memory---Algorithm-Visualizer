#pragma once

#include <QString>
#include <QStringList>
#include <vector>

enum class Domain
{
    Memory,
    Algorithms,
    Concurrency
};

enum class Scenario
{
    LinearSearch,
    BinarySearch,
    BubbleSort,
    SelectionSort,
    InsertionSort,
    StackMemory,
    HeapMemory,
    StackDs,
    QueueDs,
    LinkedList,
    NewDelete,
    UniquePtr,
    CopyMove,
    ThreadMutex,
    RaceCondition
};

struct MemoryCell
{
    QString name;
    QString value;
    QString address;
    bool highlighted = false;
    bool freed = false;
};

struct PointerLink
{
    int stackIndex = -1;
    int heapIndex = -1;
    QString label;
};

struct ThreadState
{
    QString name;
    QString state;
    bool active = false;
};

struct SimulationFrame
{
    QString title;
    QString description;
    QStringList rows;
    int currentStep = 0;
    int totalSteps = 0;
    int comparisons = 0;
    int swaps = 0;
    std::vector<int> arrayValues;
    std::vector<int> highlightedIndices;
    std::vector<MemoryCell> stackCells;
    std::vector<MemoryCell> heapCells;
    std::vector<PointerLink> pointerLinks;
    std::vector<int> stackValues;
    std::vector<int> queueValues;
    int queueFront = -1;
    int queueBack = -1;
    std::vector<int> linkedListValues;
    std::vector<QString> linkedListAddresses;
    int linkedListHighlight = -1;
    std::vector<ThreadState> threadStates;
    QString mutexOwner;
};

using FrameSequence = std::vector<SimulationFrame>;
