#include "algorithms/AlgorithmSimulator.h"

#include <algorithm>
#include <queue>

#include "core/I18nKey.h"

namespace
{
    SimulationFrame makeFrame(const QString &title,
                              const QString &description,
                              const QStringList &rows,
                              int step,
                              int total,
                              int comparisons,
                              int swaps,
                              const std::vector<int> &arrayValues,
                              const std::vector<int> &highlightedIndices)
    {
        SimulationFrame frame{title, description, rows, step, total, comparisons, swaps};
        frame.arrayValues = arrayValues;
        frame.highlightedIndices = highlightedIndices;
        return frame;
    }

    std::vector<int> fallbackValues(const UserScenarioConfig &config, std::vector<int> defaults)
    {
        if (!config.values.empty())
        {
            return config.values;
        }
        return defaults;
    }

    QString k(const QString &key, const QStringList &args = {})
    {
        return i18nkey::make(key, args);
    }
} // namespace

FrameSequence AlgorithmSimulator::generate(Scenario scenario, const UserScenarioConfig &config)
{
    switch (scenario)
    {
    case Scenario::LinearSearch:
        return linearSearch(config);
    case Scenario::BinarySearch:
        return binarySearch(config);
    case Scenario::BubbleSort:
        return bubbleSort(config);
    case Scenario::SelectionSort:
        return selectionSort(config);
    case Scenario::InsertionSort:
        return insertionSort(config);
    case Scenario::GraphBfs:
        return graphBfs(config);
    default:
        return {};
    }
}

FrameSequence AlgorithmSimulator::linearSearch(const UserScenarioConfig &config)
{
    const std::vector<int> values = fallbackValues(config, {7, 2, 11, 5, 19, 13});
    const int target = config.target;
    if (values.empty())
    {
        return {makeFrame(k("alg.linear.title"), k("alg.noData"), {k("alg.inputRequired")}, 1, 1, 0, 0, {}, {})};
    }

    const int resolvedTarget = target == 0 ? values.back() : target;
    FrameSequence frames;
    int comparisons = 0;

    for (std::size_t i = 0; i < values.size(); ++i)
    {
        ++comparisons;
        const bool found = values[i] == resolvedTarget;
        QStringList rows;
        rows << k("alg.linear.arraySize", {QString::number(values.size())})
             << k("alg.linear.target", {QString::number(resolvedTarget)})
             << k("alg.linear.comparing", {QString::number(i), QString::number(values[i])})
             << (found ? k("alg.resultFound") : k("alg.resultPending"));

        frames.push_back(makeFrame(k("alg.linear.title"),
                                   k("alg.linear.desc"),
                                   rows,
                                   static_cast<int>(i + 1),
                                   static_cast<int>(values.size()),
                                   comparisons,
                                   0,
                                   values,
                                   {static_cast<int>(i)}));
        if (found)
        {
            break;
        }
    }

    return frames;
}

FrameSequence AlgorithmSimulator::binarySearch(const UserScenarioConfig &config)
{
    std::vector<int> values = fallbackValues(config, {2, 4, 6, 8, 10, 12, 14, 16});
    if (values.empty())
    {
        return {makeFrame(k("alg.binary.title"), k("alg.noData"), {k("alg.inputRequired")}, 1, 1, 0, 0, {}, {})};
    }
    std::sort(values.begin(), values.end());

    const int target = config.target == 0 ? values[values.size() / 2] : config.target;
    FrameSequence frames;

    int left = 0;
    int right = static_cast<int>(values.size()) - 1;
    int comparisons = 0;
    int step = 0;

    while (left <= right)
    {
        ++step;
        const int mid = left + (right - left) / 2;
        ++comparisons;

        QStringList rows;
        rows << k("alg.binary.sorted")
             << k("alg.binary.bounds", {QString::number(left), QString::number(right), QString::number(mid), QString::number(values[mid])});

        if (values[mid] == target)
        {
            rows << k("alg.resultFound");
            frames.push_back(makeFrame(k("alg.binary.title"),
                                       k("alg.binary.desc"),
                                       rows,
                                       step,
                                       4,
                                       comparisons,
                                       0,
                                       values,
                                       {mid}));
            break;
        }

        if (values[mid] < target)
        {
            rows << k("alg.binary.actionLeft");
            left = mid + 1;
        }
        else
        {
            rows << k("alg.binary.actionRight");
            right = mid - 1;
        }

        frames.push_back(makeFrame(k("alg.binary.title"),
                                   k("alg.binary.desc"),
                                   rows,
                                   step,
                                   4,
                                   comparisons,
                                   0,
                                   values,
                                   {mid, left, right}));
    }

    return frames;
}

FrameSequence AlgorithmSimulator::bubbleSort(const UserScenarioConfig &config)
{
    std::vector<int> values = fallbackValues(config, {9, 3, 7, 1, 5});
    if (values.size() < 2)
    {
        return {makeFrame(k("alg.bubble.title"), k("alg.needTwo"), {k("alg.needMoreValues")}, 1, 1, 0, 0, values, {})};
    }

    FrameSequence frames;
    int comparisons = 0;
    int swaps = 0;
    int step = 0;

    for (std::size_t i = 0; i < values.size(); ++i)
    {
        for (std::size_t j = 0; j + 1 < values.size() - i; ++j)
        {
            ++comparisons;
            if (values[j] > values[j + 1])
            {
                std::swap(values[j], values[j + 1]);
                ++swaps;
            }
            ++step;

            QStringList rows;
            rows << k("alg.common.length", {QString::number(values.size())})
                 << k("alg.bubble.compare", {QString::number(j), QString::number(j + 1)})
                 << k("alg.bubble.pass", {QString::number(i)});

            frames.push_back(makeFrame(k("alg.bubble.title"),
                                       k("alg.bubble.desc"),
                                       rows,
                                       step,
                                       static_cast<int>(values.size() * values.size()),
                                       comparisons,
                                       swaps,
                                       values,
                                       {static_cast<int>(j), static_cast<int>(j + 1)}));
        }
    }

    return frames;
}

FrameSequence AlgorithmSimulator::selectionSort(const UserScenarioConfig &config)
{
    std::vector<int> values = fallbackValues(config, {8, 4, 6, 2, 9});
    if (values.size() < 2)
    {
        return {makeFrame(k("alg.selection.title"), k("alg.needTwo"), {k("alg.needMoreValues")}, 1, 1, 0, 0, values, {})};
    }

    FrameSequence frames;
    int comparisons = 0;
    int swaps = 0;
    int step = 0;

    for (std::size_t i = 0; i < values.size(); ++i)
    {
        std::size_t minIdx = i;
        for (std::size_t j = i + 1; j < values.size(); ++j)
        {
            ++comparisons;
            if (values[j] < values[minIdx])
            {
                minIdx = j;
            }
            ++step;

            QStringList rows;
            rows << k("alg.common.length", {QString::number(values.size())})
                 << k("alg.selection.indices", {QString::number(i), QString::number(j), QString::number(minIdx)});

            frames.push_back(makeFrame(k("alg.selection.title"),
                                       k("alg.selection.desc"),
                                       rows,
                                       step,
                                       static_cast<int>(values.size() * values.size()),
                                       comparisons,
                                       swaps,
                                       values,
                                       {static_cast<int>(i), static_cast<int>(j), static_cast<int>(minIdx)}));
        }
        if (minIdx != i)
        {
            std::swap(values[i], values[minIdx]);
            ++swaps;
        }
    }

    return frames;
}

FrameSequence AlgorithmSimulator::insertionSort(const UserScenarioConfig &config)
{
    std::vector<int> values = fallbackValues(config, {10, 3, 8, 1, 4});
    if (values.size() < 2)
    {
        return {makeFrame(k("alg.insertion.title"), k("alg.needTwo"), {k("alg.needMoreValues")}, 1, 1, 0, 0, values, {})};
    }

    FrameSequence frames;
    int comparisons = 0;
    int step = 0;

    for (std::size_t i = 1; i < values.size(); ++i)
    {
        int key = values[i];
        int j = static_cast<int>(i) - 1;

        while (j >= 0 && values[static_cast<std::size_t>(j)] > key)
        {
            ++comparisons;
            values[static_cast<std::size_t>(j + 1)] = values[static_cast<std::size_t>(j)];
            --j;
            ++step;

            QStringList rows;
            rows << k("alg.common.length", {QString::number(values.size())})
                 << k("alg.insertion.shifting", {QString::number(key)})
                 << k("alg.insertion.j", {QString::number(j)});

            frames.push_back(makeFrame(k("alg.insertion.title"),
                                       k("alg.insertion.desc"),
                                       rows,
                                       step,
                                       static_cast<int>(values.size() * values.size()),
                                       comparisons,
                                       0,
                                       values,
                                       {j + 1, static_cast<int>(i)}));
        }
        values[static_cast<std::size_t>(j + 1)] = key;
    }

    return frames;
}

FrameSequence AlgorithmSimulator::graphBfs(const UserScenarioConfig &config)
{
    Q_UNUSED(config);

    const std::vector<GraphNodeView> nodes = {
        {"0", 160, 110, false, false},
        {"1", 290, 60, false, false},
        {"2", 290, 160, false, false},
        {"3", 430, 110, false, false},
        {"4", 560, 60, false, false},
        {"5", 560, 160, false, false}};
    const std::vector<GraphEdgeView> edges = {
        {0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 4}, {3, 5}};

    FrameSequence frames;
    std::queue<int> q;
    std::vector<bool> visited(nodes.size(), false);
    q.push(0);
    visited[0] = true;

    int step = 0;
    while (!q.empty())
    {
        const int current = q.front();
        q.pop();
        ++step;

        SimulationFrame frame = makeFrame(k("alg.graphBfs.title"),
                                          k("alg.graphBfs.desc"),
                                          {k("alg.graphBfs.visit", {QString::number(current)})},
                                          step,
                                          static_cast<int>(nodes.size()),
                                          step,
                                          0,
                                          {},
                                          {});
        frame.graphNodes = nodes;
        frame.graphEdges = edges;
        for (std::size_t i = 0; i < frame.graphNodes.size(); ++i)
        {
            frame.graphNodes[i].visited = visited[i];
        }
        frame.graphNodes[static_cast<std::size_t>(current)].active = true;
        frame.graphNodes[static_cast<std::size_t>(current)].visited = true;
        frames.push_back(frame);

        for (const auto &e : edges)
        {
            if (e.from == current && !visited[static_cast<std::size_t>(e.to)])
            {
                visited[static_cast<std::size_t>(e.to)] = true;
                q.push(e.to);
            }
        }
    }

    return frames;
}
