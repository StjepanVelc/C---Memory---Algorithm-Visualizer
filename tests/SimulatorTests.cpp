#include <iostream>

#include "algorithms/AlgorithmSimulator.h"
#include "concurrency/ConcurrencySimulator.h"
#include "core/PresetIO.h"
#include "memory/MemorySimulator.h"

int main()
{
    UserScenarioConfig config;
    config.values = {4, 1, 7, 3, 2};
    config.target = 7;
    config.insertIndex = 1;
    config.insertValue = 99;
    config.deleteIndex = 2;

    const auto linear = AlgorithmSimulator::generate(Scenario::LinearSearch, config);
    if (linear.empty())
    {
        std::cerr << "LinearSearch generated no frames\n";
        return 1;
    }

    const auto bfs = AlgorithmSimulator::generate(Scenario::GraphBfs, config);
    if (bfs.empty() || bfs.front().graphNodes.empty())
    {
        std::cerr << "GraphBfs generated invalid graph frames\n";
        return 1;
    }

    const auto linked = MemorySimulator::generate(Scenario::LinkedList, config);
    if (linked.empty() || linked.back().linkedListValues.empty())
    {
        std::cerr << "LinkedList generated invalid frames\n";
        return 1;
    }

    const auto race = ConcurrencySimulator::generate(Scenario::RaceCondition, config);
    if (race.empty() || race.front().threadStates.empty())
    {
        std::cerr << "RaceCondition generated invalid thread frames\n";
        return 1;
    }

    UserScenarioConfig edgeConfig;
    edgeConfig.values = {42};
    edgeConfig.target = 42;
    edgeConfig.insertIndex = 99;
    edgeConfig.insertValue = 123;
    edgeConfig.deleteIndex = 99;

    const auto bubbleEdge = AlgorithmSimulator::generate(Scenario::BubbleSort, edgeConfig);
    if (bubbleEdge.empty() || bubbleEdge.back().arrayValues.size() != 1)
    {
        std::cerr << "BubbleSort edge-case handling failed\n";
        return 1;
    }

    const auto linkedEdge = MemorySimulator::generate(Scenario::LinkedList, edgeConfig);
    if (linkedEdge.empty() || linkedEdge.back().linkedListValues.empty())
    {
        std::cerr << "LinkedList edge-case handling failed\n";
        return 1;
    }

    const QJsonObject preset = buildPresetJson(Scenario::SelectionSort, config);
    Scenario parsedScenario = Scenario::LinearSearch;
    UserScenarioConfig parsedConfig;
    if (!parsePresetJson(preset, parsedScenario, parsedConfig))
    {
        std::cerr << "Preset parse failed\n";
        return 1;
    }

    if (parsedScenario != Scenario::SelectionSort || parsedConfig.values != config.values ||
        parsedConfig.target != config.target || parsedConfig.insertIndex != config.insertIndex ||
        parsedConfig.insertValue != config.insertValue || parsedConfig.deleteIndex != config.deleteIndex)
    {
        std::cerr << "Preset round-trip mismatch\n";
        return 1;
    }

    std::cout << "Simulator tests passed\n";
    return 0;
}
