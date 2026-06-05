#include "concurrency/ConcurrencySimulator.h"

#include "core/I18nKey.h"

namespace
{
    QString k(const QString &key, const QStringList &args = {})
    {
        return i18nkey::make(key, args);
    }

    SimulationFrame makeConcurrencyFrame(const QString &title,
                                         const QString &desc,
                                         const QStringList &rows,
                                         int step,
                                         int total,
                                         const std::vector<ThreadState> &threadStates,
                                         const QString &mutexOwner)
    {
        SimulationFrame frame{title, desc, rows, step, total, 0, 0};
        frame.threadStates = threadStates;
        frame.mutexOwner = mutexOwner;
        return frame;
    }
} // namespace

FrameSequence ConcurrencySimulator::generate(Scenario scenario, const UserScenarioConfig &config)
{
    Q_UNUSED(config);

    switch (scenario)
    {
    case Scenario::ThreadMutex:
        return {
            makeConcurrencyFrame(k("con.threadMutex.title"),
                                 k("con.threadMutex.desc1"),
                                 {k("con.threadMutex.row1"), k("con.threadMutex.row2"), k("con.threadMutex.row3")},
                                 1,
                                 3,
                                 {{k("con.thread.t1"), k("con.state.runningCs"), true}, {k("con.thread.t2"), k("con.state.waitingMutex"), false}},
                                 k("con.thread.t1")),
            makeConcurrencyFrame(k("con.threadMutex.title"),
                                 k("con.threadMutex.desc2"),
                                 {k("con.threadMutex.row4"), k("con.threadMutex.row5"), k("con.threadMutex.row6")},
                                 2,
                                 3,
                                 {{k("con.thread.t1"), k("con.state.ready"), false}, {k("con.thread.t2"), k("con.state.runningCs"), true}},
                                 k("con.thread.t2")),
            makeConcurrencyFrame(k("con.threadMutex.title"),
                                 k("con.threadMutex.desc3"),
                                 {k("con.threadMutex.row7"), k("con.threadMutex.row8")},
                                 3,
                                 3,
                                 {{k("con.thread.t1"), k("con.state.finished"), false}, {k("con.thread.t2"), k("con.state.finished"), false}},
                                 k("con.none"))};

    case Scenario::RaceCondition:
        return {
            makeConcurrencyFrame(k("con.race.title"),
                                 k("con.race.desc1"),
                                 {k("con.race.row1"), k("con.race.row2")},
                                 1,
                                 3,
                                 {{k("con.thread.t1"), k("con.state.running"), true}, {k("con.thread.t2"), k("con.state.running"), true}},
                                 k("con.none")),
            makeConcurrencyFrame(k("con.race.title"),
                                 k("con.race.desc2"),
                                 {k("con.race.row3"), k("con.race.row4"), k("con.race.row5")},
                                 2,
                                 3,
                                 {{k("con.thread.t1"), k("con.state.writingShared"), true}, {k("con.thread.t2"), k("con.state.writingShared"), true}},
                                 k("con.none")),
            makeConcurrencyFrame(k("con.race.title"),
                                 k("con.race.desc3"),
                                 {k("con.race.row6"), k("con.race.row7")},
                                 3,
                                 3,
                                 {{k("con.thread.t1"), k("con.state.finished"), false}, {k("con.thread.t2"), k("con.state.finished"), false}},
                                 k("con.none"))};

    default:
        return {};
    }
}
