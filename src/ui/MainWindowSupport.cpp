#include "ui/MainWindowSupport.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QStringList>

namespace ui::mainwindow
{
    QList<ScenarioItem> algorithmItems()
    {
        return {{"Linear Search", Scenario::LinearSearch},
                {"Binary Search", Scenario::BinarySearch},
                {"Bubble Sort", Scenario::BubbleSort},
                {"Selection Sort", Scenario::SelectionSort},
                {"Insertion Sort", Scenario::InsertionSort},
                {"Graph BFS", Scenario::GraphBfs}};
    }

    QList<ScenarioItem> memoryItems()
    {
        return {{"Stack Variables", Scenario::StackMemory},
                {"Heap Objects", Scenario::HeapMemory},
                {"new/delete", Scenario::NewDelete},
                {"unique_ptr", Scenario::UniquePtr},
                {"copy vs move", Scenario::CopyMove}};
    }

    QList<ScenarioItem> dataStructureItems()
    {
        return {{"Stack (LIFO)", Scenario::StackDs},
                {"Queue (FIFO)", Scenario::QueueDs},
                {"Linked List", Scenario::LinkedList}};
    }

    QList<ScenarioItem> concurrencyItems()
    {
        return {{"Thread + Mutex", Scenario::ThreadMutex},
                {"Race Condition", Scenario::RaceCondition}};
    }

    QString modeNameForTab(int index)
    {
        switch (index)
        {
        case 0:
            return "Algorithms";
        case 1:
            return "Memory";
        case 2:
            return "Concurrency";
        case 3:
            return "Data Structures";
        default:
            return "Unknown";
        }
    }

    QJsonArray intVectorToJson(const std::vector<int> &values)
    {
        QJsonArray array;
        for (int value : values)
        {
            array.append(value);
        }
        return array;
    }

    QString joinInts(const std::vector<int> &values)
    {
        QStringList parts;
        for (int value : values)
        {
            parts << QString::number(value);
        }
        return parts.join(", ");
    }

    QDir resolveProjectRootFromApp()
    {
        QDir rootDir(QCoreApplication::applicationDirPath());
        for (int i = 0; i < 3; ++i)
        {
            rootDir.cdUp();
        }
        if (!rootDir.exists())
        {
            rootDir = QDir::current();
        }
        return rootDir;
    }

    QString i18nPathFor(const QString &langCode)
    {
        QDir root = resolveProjectRootFromApp();
        return root.filePath(QString("resources/i18n/%1.json").arg(langCode));
    }

    QJsonObject loadI18n(const QString &langCode)
    {
        QFile file(i18nPathFor(langCode));
        if (!file.open(QIODevice::ReadOnly))
        {
            return {};
        }
        return QJsonDocument::fromJson(file.readAll()).object();
    }
} // namespace ui::mainwindow
