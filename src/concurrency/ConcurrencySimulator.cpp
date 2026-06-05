#include "concurrency/ConcurrencySimulator.h"

namespace
{
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
            makeConcurrencyFrame("Thread + Mutex",
                                 "Dva threada pristupaju shared counteru.",
                                 {"T1: lock(m)", "T1: counter++", "T2: waiting for mutex"},
                                 1,
                                 3,
                                 {{"T1", "running (in critical section)", true}, {"T2", "waiting for mutex", false}},
                                 "T1"),
            makeConcurrencyFrame("Thread + Mutex",
                                 "Mutex osigurava serializiran pristup.",
                                 {"T1: unlock(m)", "T2: lock(m)", "T2: counter++"},
                                 2,
                                 3,
                                 {{"T1", "ready", false}, {"T2", "running (in critical section)", true}},
                                 "T2"),
            makeConcurrencyFrame("Thread + Mutex",
                                 "Krajnje stanje je konzistentno.",
                                 {"counter == 2", "No race condition"},
                                 3,
                                 3,
                                 {{"T1", "finished", false}, {"T2", "finished", false}},
                                 "none")};

    case Scenario::RaceCondition:
        return {
            makeConcurrencyFrame("Race Condition",
                                 "Bez mutexa inkrement nije atomic.",
                                 {"T1 reads counter=0", "T2 reads counter=0"},
                                 1,
                                 3,
                                 {{"T1", "running", true}, {"T2", "running", true}},
                                 "none"),
            makeConcurrencyFrame("Race Condition",
                                 "Oba threada pisu staru bazu +1.",
                                 {"T1 writes 1", "T2 writes 1", "lost update"},
                                 2,
                                 3,
                                 {{"T1", "writing shared value", true}, {"T2", "writing shared value", true}},
                                 "none"),
            makeConcurrencyFrame("Race Condition",
                                 "Rezultat je nekonzistentan.",
                                 {"Expected 2, got 1", "Need mutex/atomic"},
                                 3,
                                 3,
                                 {{"T1", "finished", false}, {"T2", "finished", false}},
                                 "none")};

    default:
        return {};
    }
}
