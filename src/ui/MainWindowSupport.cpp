#include "ui/MainWindowSupport.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QStringList>

#include "core/I18nKey.h"

namespace ui::mainwindow
{
    QString keyText(const QString &key)
    {
        return i18nkey::make(key);
    }

    QList<ScenarioItem> algorithmItems()
    {
        return {{keyText("alg.linear.title"), Scenario::LinearSearch},
                {keyText("alg.binary.title"), Scenario::BinarySearch},
                {keyText("alg.bubble.title"), Scenario::BubbleSort},
                {keyText("alg.selection.title"), Scenario::SelectionSort},
                {keyText("alg.insertion.title"), Scenario::InsertionSort},
                {keyText("alg.graphBfs.title"), Scenario::GraphBfs}};
    }

    QList<ScenarioItem> memoryItems()
    {
        return {{keyText("mem.stackVars.title"), Scenario::StackMemory},
                {keyText("mem.heapObjects.title"), Scenario::HeapMemory},
                {keyText("mem.newDelete.title"), Scenario::NewDelete},
                {keyText("mem.uniquePtr.title"), Scenario::UniquePtr},
                {keyText("mem.copyMove.title"), Scenario::CopyMove}};
    }

    QList<ScenarioItem> dataStructureItems()
    {
        return {{keyText("mem.stackDs.title"), Scenario::StackDs},
                {keyText("mem.queueDs.title"), Scenario::QueueDs},
                {keyText("mem.linkedList.title"), Scenario::LinkedList}};
    }

    QList<ScenarioItem> concurrencyItems()
    {
        return {{keyText("con.threadMutex.title"), Scenario::ThreadMutex},
                {keyText("con.race.title"), Scenario::RaceCondition}};
    }

    QString modeNameForTab(int index)
    {
        switch (index)
        {
        case 0:
            return keyText("mode.algorithms");
        case 1:
            return keyText("mode.memory");
        case 2:
            return keyText("mode.concurrency");
        case 3:
            return keyText("mode.dataStructures");
        default:
            return keyText("mode.unknown");
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
