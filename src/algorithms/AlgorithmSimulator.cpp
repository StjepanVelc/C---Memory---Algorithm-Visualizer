#include "algorithms/AlgorithmSimulator.h"

#include <algorithm>

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
} // namespace

FrameSequence AlgorithmSimulator::generate(Scenario scenario)
{
    switch (scenario)
    {
    case Scenario::LinearSearch:
        return linearSearch();
    case Scenario::BinarySearch:
        return binarySearch();
    case Scenario::BubbleSort:
        return bubbleSort();
    case Scenario::SelectionSort:
        return selectionSort();
    case Scenario::InsertionSort:
        return insertionSort();
    default:
        return {};
    }
}

FrameSequence AlgorithmSimulator::linearSearch()
{
    const std::vector<int> values{7, 2, 11, 5, 19, 13};
    const int target = 19;
    FrameSequence frames;
    int comparisons = 0;

    for (std::size_t i = 0; i < values.size(); ++i)
    {
        ++comparisons;
        const bool found = values[i] == target;
        QStringList rows;
        rows << "Array: [7, 2, 11, 5, 19, 13]"
             << QString("Target: %1").arg(target)
             << QString("Comparing index %1 (value %2)").arg(i).arg(values[i])
             << (found ? "Result: FOUND" : "Result: not yet");

        frames.push_back(makeFrame("Linear Search",
                                   "Sekvencijalno provjerava svaki element.",
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

FrameSequence AlgorithmSimulator::binarySearch()
{
    const std::vector<int> values{2, 4, 6, 8, 10, 12, 14, 16};
    const int target = 14;
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
        rows << "Sorted array: [2, 4, 6, 8, 10, 12, 14, 16]"
             << QString("left=%1 right=%2 mid=%3 value[mid]=%4")
                    .arg(left)
                    .arg(right)
                    .arg(mid)
                    .arg(values[mid]);

        if (values[mid] == target)
        {
            rows << "Result: FOUND";
            frames.push_back(makeFrame("Binary Search",
                                       "Prepolovljava prostor pretrage.",
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
            rows << "Action: left = mid + 1";
            left = mid + 1;
        }
        else
        {
            rows << "Action: right = mid - 1";
            right = mid - 1;
        }

        frames.push_back(makeFrame("Binary Search",
                                   "Prepolovljava prostor pretrage.",
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

FrameSequence AlgorithmSimulator::bubbleSort()
{
    std::vector<int> values{9, 3, 7, 1, 5};
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
            rows << QString("Current: [%1, %2, %3, %4, %5]")
                        .arg(values[0])
                        .arg(values[1])
                        .arg(values[2])
                        .arg(values[3])
                        .arg(values[4])
                 << QString("Compare indices %1 and %2").arg(j).arg(j + 1)
                 << QString("Pass=%1").arg(i);

            frames.push_back(makeFrame("Bubble Sort",
                                       "Mijenja susjedne elemente ako su u krivom redoslijedu.",
                                       rows,
                                       step,
                                       10,
                                       comparisons,
                                       swaps,
                                       values,
                                       {static_cast<int>(j), static_cast<int>(j + 1)}));
        }
    }

    return frames;
}

FrameSequence AlgorithmSimulator::selectionSort()
{
    std::vector<int> values{8, 4, 6, 2, 9};
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
            rows << QString("Current: [%1, %2, %3, %4, %5]")
                        .arg(values[0])
                        .arg(values[1])
                        .arg(values[2])
                        .arg(values[3])
                        .arg(values[4])
                 << QString("i=%1 j=%2 minIdx=%3").arg(i).arg(j).arg(minIdx);

            frames.push_back(makeFrame("Selection Sort",
                                       "Trazi minimum u nesortiranom dijelu i stavlja ga na pocetak.",
                                       rows,
                                       step,
                                       10,
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

FrameSequence AlgorithmSimulator::insertionSort()
{
    std::vector<int> values{10, 3, 8, 1, 4};
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
            rows << QString("Current: [%1, %2, %3, %4, %5]")
                        .arg(values[0])
                        .arg(values[1])
                        .arg(values[2])
                        .arg(values[3])
                        .arg(values[4])
                 << QString("Shifting for key=%1").arg(key)
                 << QString("j=%1").arg(j);

            frames.push_back(makeFrame("Insertion Sort",
                                       "Umetanje elementa na pravo mjesto u sortiranom prefiksu.",
                                       rows,
                                       step,
                                       10,
                                       comparisons,
                                       0,
                                       values,
                                       {j + 1, static_cast<int>(i)}));
        }
        values[static_cast<std::size_t>(j + 1)] = key;
    }

    return frames;
}
