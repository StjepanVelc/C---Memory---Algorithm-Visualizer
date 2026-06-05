#pragma once

#include <QString>
#include <QStringList>
#include <QVector>
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
    GraphBfs,
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

struct GraphNodeView
{
    QString label;
    int x = 0;
    int y = 0;
    bool visited = false;
    bool active = false;
};

struct GraphEdgeView
{
    int from = -1;
    int to = -1;
};

struct UserScenarioConfig
{
    std::vector<int> values;
    int target = 0;
    int insertIndex = 0;
    int insertValue = 0;
    int deleteIndex = 0;
};

struct RecordingEvent
{
    int frameIndex = 0;
    QString action;
    qint64 timestampMs = 0;
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
    std::vector<GraphNodeView> graphNodes;
    std::vector<GraphEdgeView> graphEdges;
    std::vector<ThreadState> threadStates;
    QString mutexOwner;
};

using FrameSequence = std::vector<SimulationFrame>;
