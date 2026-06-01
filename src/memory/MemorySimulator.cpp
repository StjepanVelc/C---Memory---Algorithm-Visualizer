#include "memory/MemorySimulator.h"

namespace
{
    SimulationFrame makeMemoryFrame(const QString &title,
                                    const QString &desc,
                                    const QStringList &rows,
                                    int step,
                                    int total,
                                    const std::vector<MemoryCell> &stackCells,
                                    const std::vector<MemoryCell> &heapCells,
                                    const std::vector<PointerLink> &pointerLinks)
    {
        SimulationFrame frame{title, desc, rows, step, total, 0, 0};
        frame.stackCells = stackCells;
        frame.heapCells = heapCells;
        frame.pointerLinks = pointerLinks;
        return frame;
    }
} // namespace

FrameSequence MemorySimulator::generate(Scenario scenario)
{
    switch (scenario)
    {
    case Scenario::StackMemory:
        return {
            makeMemoryFrame("Stack Variables",
                            "Lokalne varijable su u stack frame-u funkcije.",
                            {"Function main() frame",
                             "int x = 42  @ 0x0012FF70",
                             "double y = 3.14 @ 0x0012FF78"},
                            1,
                            3,
                            {{"x", "42", "0x0012FF70", true, false}, {"y", "3.14", "0x0012FF78", false, false}},
                            {},
                            {}),
            makeMemoryFrame("Stack Variables",
                            "Novi scope dodaje varijable iznad postojecih.",
                            {"main() frame",
                             "int x = 42  @ 0x0012FF70",
                             "block: int z = 5 @ 0x0012FF68"},
                            2,
                            3,
                            {{"x", "42", "0x0012FF70", false, false}, {"y", "3.14", "0x0012FF78", false, false}, {"z", "5", "0x0012FF68", true, false}},
                            {},
                            {}),
            makeMemoryFrame("Stack Variables",
                            "Izlazak iz scope-a uklanja z sa stacka.",
                            {"main() frame",
                             "int x = 42  @ 0x0012FF70",
                             "z destroyed"},
                            3,
                            3,
                            {{"x", "42", "0x0012FF70", false, false}, {"y", "3.14", "0x0012FF78", false, false}, {"z", "destroyed", "0x0012FF68", false, true}},
                            {},
                            {})};

    case Scenario::HeapMemory:
        return {
            makeMemoryFrame("Heap Objects",
                            "Objekti na heapu zive dok ih eksplicitno ne oslobodis.",
                            {"MyObj* p = new MyObj",
                             "p @ stack: 0x0012FF70 -> 0x00AF1000",
                             "Heap object MyObj @ 0x00AF1000"},
                            1,
                            2,
                            {{"p", "0x00AF1000", "0x0012FF70", true, false}},
                            {{"MyObj", "{...}", "0x00AF1000", true, false}},
                            {{0, 0, "p"}}),
            makeMemoryFrame("Heap Objects",
                            "delete oslobadja heap memoriju.",
                            {"delete p",
                             "Heap object @ 0x00AF1000 FREED",
                             "p should become nullptr"},
                            2,
                            2,
                            {{"p", "nullptr", "0x0012FF70", true, false}},
                            {{"MyObj", "freed", "0x00AF1000", false, true}},
                            {})};

    case Scenario::StackDs:
    {
        SimulationFrame step1 = makeMemoryFrame("Stack (LIFO)",
                                                "Push operacije dodaju element na vrh stoga.",
                                                {"push(10)", "push(20)", "top = 20"},
                                                1,
                                                3,
                                                {},
                                                {},
                                                {});
        step1.stackValues = {10, 20};

        SimulationFrame step2 = makeMemoryFrame("Stack (LIFO)",
                                                "Jos jedan push pomice top na novi element.",
                                                {"push(30)", "top = 30"},
                                                2,
                                                3,
                                                {},
                                                {},
                                                {});
        step2.stackValues = {10, 20, 30};

        SimulationFrame step3 = makeMemoryFrame("Stack (LIFO)",
                                                "Pop uklanja zadnji ubaceni element.",
                                                {"pop() returns 30", "top = 20"},
                                                3,
                                                3,
                                                {},
                                                {},
                                                {});
        step3.stackValues = {10, 20};

        return {step1, step2, step3};
    }

    case Scenario::QueueDs:
    {
        SimulationFrame step1 = makeMemoryFrame("Queue (FIFO)",
                                                "Enqueue dodaje element na kraj reda.",
                                                {"enqueue(5)", "enqueue(8)", "front = 5"},
                                                1,
                                                3,
                                                {},
                                                {},
                                                {});
        step1.queueValues = {5, 8};
        step1.queueFront = 0;
        step1.queueBack = 1;

        SimulationFrame step2 = makeMemoryFrame("Queue (FIFO)",
                                                "Novi enqueue ide na back poziciju.",
                                                {"enqueue(13)", "back = 13"},
                                                2,
                                                3,
                                                {},
                                                {},
                                                {});
        step2.queueValues = {5, 8, 13};
        step2.queueFront = 0;
        step2.queueBack = 2;

        SimulationFrame step3 = makeMemoryFrame("Queue (FIFO)",
                                                "Dequeue uklanja element s fronta.",
                                                {"dequeue() returns 5", "front = 8"},
                                                3,
                                                3,
                                                {},
                                                {},
                                                {});
        step3.queueValues = {8, 13};
        step3.queueFront = 0;
        step3.queueBack = 1;

        return {step1, step2, step3};
    }

    case Scenario::LinkedList:
    {
        SimulationFrame step1 = makeMemoryFrame("Linked List",
                                                "Pocetno stanje liste i head pointera.",
                                                {"Node(10) -> Node(20) -> Node(30)",
                                                 "head = 0x00D01000"},
                                                1,
                                                5,
                                                {{"head", "0x00D01000", "0x0012FA10", true, false}},
                                                {},
                                                {});
        step1.linkedListValues = {10, 20, 30};
        step1.linkedListAddresses = {"0x00D01000", "0x00D01030", "0x00D01060"};
        step1.linkedListHighlight = 0;

        SimulationFrame step2 = makeMemoryFrame("Linked List",
                                                "Traverse ide na sljedeci cvor preko next pointera.",
                                                {"curr = curr->next", "curr now points to value 20"},
                                                2,
                                                5,
                                                {{"head", "0x00D01000", "0x0012FA10", false, false},
                                                 {"curr", "0x00D01030", "0x0012FA18", true, false}},
                                                {},
                                                {});
        step2.linkedListValues = {10, 20, 30};
        step2.linkedListAddresses = {"0x00D01000", "0x00D01030", "0x00D01060"};
        step2.linkedListHighlight = 1;

        SimulationFrame step3 = makeMemoryFrame("Linked List",
                                                "Insert iza cvora 20 ubacuje novi cvor 25.",
                                                {"newNode->next = curr->next", "curr->next = newNode"},
                                                3,
                                                5,
                                                {{"head", "0x00D01000", "0x0012FA10", false, false},
                                                 {"curr", "0x00D01030", "0x0012FA18", true, false},
                                                 {"newNode", "0x00D01045", "0x0012FA28", true, false}},
                                                {},
                                                {});
        step3.linkedListValues = {10, 20, 25, 30};
        step3.linkedListAddresses = {"0x00D01000", "0x00D01030", "0x00D01045", "0x00D01060"};
        step3.linkedListHighlight = 2;

        SimulationFrame step4 = makeMemoryFrame("Linked List",
                                                "Delete cvora 25 prespaja listu i oslobadja cvor.",
                                                {"curr->next = node25->next", "delete node25"},
                                                4,
                                                5,
                                                {{"head", "0x00D01000", "0x0012FA10", false, false},
                                                 {"curr", "0x00D01030", "0x0012FA18", true, false}},
                                                {},
                                                {});
        step4.linkedListValues = {10, 20, 30};
        step4.linkedListAddresses = {"0x00D01000", "0x00D01030", "0x00D01060"};
        step4.linkedListHighlight = 1;

        SimulationFrame step5 = makeMemoryFrame("Linked List",
                                                "Insert na rep dodaje novi cvor i veze ga s prethodnim.",
                                                {"tail->next = new Node(40)", "Node(40) appended"},
                                                5,
                                                5,
                                                {{"head", "0x00D01000", "0x0012FA10", false, false},
                                                 {"tail", "0x00D01090", "0x0012FA20", true, false}},
                                                {},
                                                {});
        step5.linkedListValues = {10, 20, 30, 40};
        step5.linkedListAddresses = {"0x00D01000", "0x00D01030", "0x00D01060", "0x00D01090"};
        step5.linkedListHighlight = 3;

        return {step1, step2, step3, step4, step5};
    }

    case Scenario::NewDelete:
        return {
            makeMemoryFrame("new/delete",
                            "new alocira, delete oslobadja.",
                            {"int* p = new int(10)", "p -> 0x00BC2000", "*p = 10"},
                            1,
                            3,
                            {{"p", "0x00BC2000", "0x0012FE20", true, false}},
                            {{"int", "10", "0x00BC2000", true, false}},
                            {{0, 0, "p"}}),
            makeMemoryFrame("new/delete",
                            "Nakon delete, dereference je UB.",
                            {"delete p", "0x00BC2000 FREED", "*p is invalid (dangling)"},
                            2,
                            3,
                            {{"p", "dangling", "0x0012FE20", true, false}},
                            {{"int", "freed", "0x00BC2000", false, true}},
                            {{0, 0, "dangling"}}),
            makeMemoryFrame("new/delete",
                            "Sigurnije: p = nullptr nakon delete.",
                            {"p = nullptr", "Dangling risk removed"},
                            3,
                            3,
                            {{"p", "nullptr", "0x0012FE20", true, false}},
                            {{"int", "freed", "0x00BC2000", false, true}},
                            {})};

    case Scenario::UniquePtr:
        return {
            makeMemoryFrame("unique_ptr",
                            "unique_ptr ima jednog vlasnika.",
                            {"std::unique_ptr<Node> a(new Node)", "a owns 0x00CD1000"},
                            1,
                            3,
                            {{"a", "0x00CD1000", "0x0012FD90", true, false}},
                            {{"Node", "{value:7}", "0x00CD1000", true, false}},
                            {{0, 0, "owner"}}),
            makeMemoryFrame("unique_ptr",
                            "Ownership se prenosi move operacijom.",
                            {"auto b = std::move(a)", "a == nullptr", "b owns 0x00CD1000"},
                            2,
                            3,
                            {{"a", "nullptr", "0x0012FD90", false, false}, {"b", "0x00CD1000", "0x0012FD88", true, false}},
                            {{"Node", "{value:7}", "0x00CD1000", true, false}},
                            {{1, 0, "owner"}}),
            makeMemoryFrame("unique_ptr",
                            "Pri izlasku iz scope-a automatski delete.",
                            {"~unique_ptr for b", "Node @ 0x00CD1000 FREED"},
                            3,
                            3,
                            {{"b", "destroyed", "0x0012FD88", false, true}},
                            {{"Node", "freed", "0x00CD1000", false, true}},
                            {})};

    case Scenario::CopyMove:
        return {
            makeMemoryFrame("Copy vs Move",
                            "Copy pravi novu kopiju resursa.",
                            {"Buffer a(size=1024)", "Buffer b = a (copy)", "two allocations"},
                            1,
                            3,
                            {{"a", "0x00DA1000", "0x0012FC80", false, false}, {"b", "0x00DB1000", "0x0012FC78", true, false}},
                            {{"BufferData", "1024 bytes", "0x00DA1000", false, false}, {"BufferData", "1024 bytes", "0x00DB1000", true, false}},
                            {{0, 0, "a"}, {1, 1, "b"}}),
            makeMemoryFrame("Copy vs Move",
                            "Move prenosi resurs bez duplog alociranja.",
                            {"Buffer c = std::move(a)", "a in moved-from state", "c owns buffer"},
                            2,
                            3,
                            {{"a", "moved-from", "0x0012FC80", false, true}, {"c", "0x00DA1000", "0x0012FC70", true, false}},
                            {{"BufferData", "1024 bytes", "0x00DA1000", true, false}},
                            {{1, 0, "c"}}),
            makeMemoryFrame("Copy vs Move",
                            "Move je tipicno jeftiniji od copy-a.",
                            {"copies=1", "moves=1", "allocations reduced"},
                            3,
                            3,
                            {{"stats", "copy=1 move=1", "", true, false}},
                            {{"allocations", "reduced", "", true, false}},
                            {})};

    default:
        return {};
    }
}
