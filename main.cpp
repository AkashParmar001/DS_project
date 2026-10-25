// ═══════════════════════════════════════════════════════════════
//  METRO TRAIN ROUTE & STATION MANAGEMENT SYSTEM
//  Data Structures Course Project
// ═══════════════════════════════════════════════════════════════
//  Modules:
//    metro_graph       — Graph, BFS, DFS, MST (Units 1 & 5)
//    linear_structures — Stacks, Queues, Linked Lists (Units 2 & 3)
//    expression_parser — Expression conversion, Tower of Hanoi (Unit 2)
//    tree_algorithms   — Binary/BST/AVL/B-tree/B+ tree (Unit 4)
//    sorting_searching — Sorts, search, hashing (Unit 6)
// ═══════════════════════════════════════════════════════════════

#include <iostream>
#include <string>
#include <limits>

#include "metro_graph.h"
#include "linear_structures.h"
#include "expression_parser.h"
#include "tree_algorithms.h"
#include "sorting_searching.h"

using namespace std;

// ───────────────────────────────────────────────────────────────
//  Helpers
// ───────────────────────────────────────────────────────────────
static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static void printBanner() {
    cout << "\n";
    cout << "  =================================================\n";
    cout << "       METRO TRAIN ROUTE MANAGEMENT SYSTEM\n";
    cout << "         Data Structures Course Project\n";
    cout << "  =================================================\n";
}

static void printMainMenu() {
    cout << "\n  --- MAIN MENU ---\n";
    cout << "  1. Stations        — add / delete / search / display\n";
    cout << "  2. Routes          — find route (BFS), explore (DFS)\n";
    cout << "  3. Passenger       — queue & route history\n";
    cout << "  4. Display Network — adjacency list / matrix / sparse\n";
    cout << "  5. DS Lab          — linked lists, stacks, trees, sorting, hashing, MST\n";
    cout << "  0. Exit\n";
    cout << "  Enter choice: ";
}

static void stationsMenu(MetroGraph& metro) {
    int choice;
    do {
        cout << "\n  --- Stations ---\n";
        cout << "  1. Display All Stations\n";
        cout << "  2. Search Station\n";
        cout << "  3. Add Station\n";
        cout << "  4. Delete Station\n";
        cout << "  5. Add Connection (Edge)\n";
        cout << "  0. Back\n";
        cout << "  Choice: ";
        cin >> choice;
        switch (choice) {
        case 1:
            metro.displayAllStations();
            break;

        case 2:
            metro.searchStation();
            break;

        case 3: {
            int id;
            string name, zone;
            cout << "\n  New Station ID   : "; cin >> id;
            cout << "  New Station Name : "; cin.ignore(); getline(cin, name);
            cout << "  Zone (A/B/C)     : "; getline(cin, zone);
            if (metro.addStation(id, name, zone))
                cout << "  [OK] Station \"" << name << "\" added.\n";
            break;
        }

        case 4: {
            int id;
            cout << "\n  Station ID to delete: "; cin >> id;
            metro.deleteStation(id);
            break;
        }

        case 5: {
            int fromId, toId, weight;
            cout << "\n  From Station ID : "; cin >> fromId;
            cout << "  To   Station ID : "; cin >> toId;
            cout << "  Weight (km)     : "; cin >> weight;
            if (metro.addEdge(fromId, toId, weight))
                cout << "  [OK] Connection added.\n";
            break;
        }

        case 0: break;
        default: cout << "  [ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

static void routesMenu(MetroGraph& metro, ArrayStack& routeHistory) {
    int choice;
    do {
        cout << "\n  --- Routes ---\n";
        cout << "  1. Find Route — BFS (fewest stops)\n";
        cout << "  2. Explore Network — DFS\n";
        cout << "  0. Back\n";
        cout << "  Choice: ";
        cin >> choice;
        switch (choice) {
        case 1: {
            // BFS — push result onto route history stack
            // bfsRoute() handles its own cin.ignore() before getline calls,
            // so the buffer is clean when we return here.
            metro.bfsRoute();
            string rec;
            cout << "\n  Save this route to history? (y/n): ";
            getline(cin, rec);
            if (rec == "y" || rec == "Y") {
                cout << "  Label for history (e.g. Central->Airport): ";
                getline(cin, rec);
                routeHistory.push(rec);
                cout << "  [HISTORY] Saved.\n";
            }
            break;
        }

        case 2:
            metro.dfsExplore();
            break;

        case 0: break;
        default: cout << "  [ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

static void passengerMenu(ArrayStack& routeHistory) {
    int choice;
    do {
        cout << "\n  --- Passenger ---\n";
        cout << "  1. Passenger Queue Management\n";
        cout << "  2. Route History (Stack)\n";
        cout << "  0. Back\n";
        cout << "  Choice: ";
        cin >> choice;
        switch (choice) {
        case 1: passengerQueueMenu();          break;
        case 2: routeHistoryMenu(routeHistory); break;
        case 0: break;
        default: cout << "  [ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

static void mstMenu(MetroGraph& metro);
static void dsAdvancedMenu(MetroGraph& metro);

static void dsLabMenu(MetroGraph& metro) {
    int choice;
    do {
        cout << "\n  --- DS Lab ---\n";
        cout << "  1. Linked List Demonstrations\n";
        cout << "  2. Expression Calculator & Stack Demos\n";
        cout << "  3. Tree Operations (BST / AVL / B-Tree / B+)\n";
        cout << "  4. Sorting & Searching\n";
        cout << "  5. Hash Table Operations\n";
        cout << "  6. Minimum Spanning Tree (Prim's & Kruskal's)\n";
        cout << "  7. Advanced Demonstrations (Units 1-6)\n";
        cout << "  0. Back\n";
        cout << "  Choice: ";
        cin >> choice;
        switch (choice) {
        case 1: linkedListDemoMenu();          break;
        case 2: expressionCalculatorMenu();    break;
        case 3: treeAlgorithmsMenu();          break;
        case 4: sortingSearchingMenu();        break;
        case 5: hashingMenu();                 break;
        case 6: mstMenu(metro);                break;
        case 7: dsAdvancedMenu(metro);         break;
        case 0: break;
        default: cout << "  [ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

// ───────────────────────────────────────────────────────────────
//  Network Display Sub-menu
// ───────────────────────────────────────────────────────────────
static void networkDisplayMenu(MetroGraph& metro) {
    int choice;
    do {
        cout << "\n  --- Display Metro Network ---\n";
        cout << "  1. Adjacency List\n";
        cout << "  2. Adjacency Matrix (unweighted)\n";
        cout << "  3. Weighted Matrix (km)\n";
        cout << "  4. Sparse Matrix Representation\n";
        cout << "  5. Memory Layout Demo (row-major / column-major)\n";
        cout << "  0. Back\n";
        cout << "  Choice: ";
        cin >> choice;
        switch (choice) {
        case 1: metro.displayAdjacencyList();   break;
        case 2: metro.displayAdjacencyMatrix(); break;
        case 3: metro.displayWeightedMatrix();  break;
        case 4: metro.displaySparseMatrix();    break;
        case 5: metro.displayMemoryLayout();    break;
        case 0: break;
        default: cout << "  [ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

// ───────────────────────────────────────────────────────────────
//  MST Sub-menu
// ───────────────────────────────────────────────────────────────
static void mstMenu(MetroGraph& metro) {
    int choice;
    do {
        cout << "\n  --- Minimum Spanning Tree ---\n";
        cout << "  1. Prim's Algorithm\n";
        cout << "  2. Kruskal's Algorithm\n";
        cout << "  3. Display Weighted Network\n";
        cout << "  0. Back\n";
        cout << "  Choice: ";
        cin >> choice;
        switch (choice) {
        case 1: metro.primsMST();           break;
        case 2: metro.kruskalsMST();        break;
        case 3: metro.displayWeightedMatrix(); break;
        case 0: break;
        default: cout << "  [ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

// ───────────────────────────────────────────────────────────────
//  DS Lab — Advanced Demonstrations sub-menu
//  Covers concepts that don't belong in the metro core:
//    Sparse matrix, memory layout, Tower of Hanoi,
//    General tree conversion, Directed/weighted graph notes
// ───────────────────────────────────────────────────────────────
static void dsAdvancedMenu(MetroGraph& metro) {
    int choice;
    do {
        cout << "\n";
        cout << "  =================================================\n";
        cout << "         DS LAB — ADVANCED DEMONSTRATIONS\n";
        cout << "  =================================================\n";
        cout << "   Unit 1\n";
        cout << "   1.  Sparse Matrix Representation\n";
        cout << "   2.  2D Array Memory Layout (Row-major / Col-major)\n";
        cout << "  -------------------------------------------------\n";
        cout << "   Unit 2\n";
        cout << "   3.  Tower of Hanoi (Recursive)\n";
        cout << "  -------------------------------------------------\n";
        cout << "   Unit 4\n";
        cout << "   4.  General Tree -> Binary Tree (LC-RS)\n";
        cout << "  -------------------------------------------------\n";
        cout << "   Unit 5\n";
        cout << "   5.  Directed Graph Concept\n";
        cout << "   6.  Weighted Graph Concept\n";
        cout << "   7.  Cyclic vs Acyclic Graph Notes\n";
        cout << "   8.  Multigraph Concept\n";
        cout << "  -------------------------------------------------\n";
        cout << "   Unit 6\n";
        cout << "   9.  Complexity Summary (all algorithms)\n";
        cout << "  =================================================\n";
        cout << "   0.  Back\n";
        cout << "  =================================================\n";
        cout << "  Choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            metro.displaySparseMatrix();
            break;

        case 2:
            metro.displayMemoryLayout();
            break;

        case 3: {
            int n;
            cout << "\n  Number of disks (1-10): ";
            cin >> n;
            if (n < 1 || n > 10) { cout << "  [ERROR] Enter 1-10.\n"; break; }
            int moves = 0;
            cout << "\n  Tower of Hanoi — " << n << " disk(s)\n";
            cout << "  Pegs: A (source)  B (auxiliary)  C (destination)\n";
            cout << "  " << string(48, '-') << "\n";
            hanoi(n, 'A', 'C', 'B', moves);
            cout << "  " << string(48, '-') << "\n";
            cout << "  Total moves : " << moves << "\n";
            cout << "  Formula     : 2^" << n << " - 1 = " << ((1 << n) - 1) << "\n";
            cout << "  Complexity  : O(2^n)\n";
            break;
        }

        case 4:
            generalTreeMenu();
            break;

        case 5:
            cout << "\n  --- Directed Graph ---\n";
            cout << "  A directed graph (digraph) has edges with direction.\n";
            cout << "  Edge A -> B means you can travel from A to B,\n";
            cout << "  but NOT from B to A unless a separate edge exists.\n\n";
            cout << "  Example: one-way metro lines.\n";
            cout << "  The metro network loaded here is UNDIRECTED\n";
            cout << "  (all connections are bidirectional).\n";
            cout << "  Complexity: Same as undirected — O(V + E) for BFS/DFS.\n";
            break;

        case 6:
            cout << "\n  --- Weighted Graph ---\n";
            cout << "  Each edge carries a numeric weight (e.g., distance in km).\n";
            cout << "  The metro network uses weights for MST algorithms.\n";
            cout << "  Weighted shortest path needs Dijkstra's O((V+E) log V).\n";
            cout << "  Unweighted shortest path (fewest stops): BFS O(V+E).\n";
            metro.displayWeightedMatrix();
            break;

        case 7:
            cout << "\n  --- Cyclic vs Acyclic Graph ---\n";
            cout << "  Cyclic   : Contains at least one cycle (closed loop).\n";
            cout << "             Example: Central -> Market -> Airport ->\n";
            cout << "             City Hall -> Central forms a cycle.\n";
            cout << "  Acyclic  : No cycles. Trees are acyclic connected graphs.\n";
            cout << "  DAG      : Directed Acyclic Graph — used in scheduling.\n";
            cout << "\n  The metro network is CYCLIC (multiple loops exist).\n";
            cout << "  MST removes edges to produce a SPANNING TREE (acyclic).\n";
            break;

        case 8:
            cout << "\n  --- Multigraph ---\n";
            cout << "  A multigraph allows MORE THAN ONE edge between the\n";
            cout << "  same pair of vertices.\n";
            cout << "  Example: Two metro lines (Line 1 and Line 2) both\n";
            cout << "  connect Central to Market — two parallel edges.\n";
            cout << "  Standard adjacency matrix cannot represent multigraphs.\n";
            cout << "  Adjacency list can: store multiple entries per pair.\n";
            cout << "  This project uses a simple graph (at most 1 edge per pair).\n";
            break;

        case 9:
            cout << "\n";
            cout << "  =====================================================\n";
            cout << "   COMPLEXITY SUMMARY\n";
            cout << "  =====================================================\n";
            cout << "   Operation                  Time       Space\n";
            cout << "  -----------------------------------------------------\n";
            cout << "   BFS / DFS                  O(V+E)     O(V)\n";
            cout << "   Bubble Sort                O(n^2)     O(1)\n";
            cout << "   Insertion Sort             O(n^2)     O(1)\n";
            cout << "   Selection Sort             O(n^2)     O(1)\n";
            cout << "   Quick Sort (avg)           O(n log n) O(log n)\n";
            cout << "   Merge Sort                 O(n log n) O(n)\n";
            cout << "   Linear Search              O(n)       O(1)\n";
            cout << "   Binary Search              O(log n)   O(1)\n";
            cout << "   Hash Insert/Search (avg)   O(1)       O(n)\n";
            cout << "   Hash Insert/Search (worst) O(n)       O(n)\n";
            cout << "   BST Search (avg)           O(log n)   O(n)\n";
            cout << "   BST Search (worst)         O(n)       O(n)\n";
            cout << "   AVL Search                 O(log n)   O(n)\n";
            cout << "   B-Tree Search              O(log n)   O(n)\n";
            cout << "   Prim's MST (matrix)        O(V^2)     O(V)\n";
            cout << "   Kruskal's MST              O(E log E) O(V)\n";
            cout << "   Tower of Hanoi             O(2^n)     O(n)\n";
            cout << "   Infix -> Postfix            O(n)       O(n)\n";
            cout << "  =====================================================\n";
            break;

        case 0: break;
        default: cout << "  [ERROR] Invalid choice.\n";
        }

        if (choice != 0 && choice != 4) {
            cout << "\n  Press Enter to continue...";
            clearInput();
            cin.get();
        }

    } while (choice != 0);
}

// ═══════════════════════════════════════════════════════════════
//  MAIN
// ═══════════════════════════════════════════════════════════════
int main() {
    printBanner();

    // ── Initialise core objects ──────────────────────────────
    MetroGraph metro;
    metro.loadSampleNetwork();

    ArrayStack routeHistory;   // shared route history stack

    cout << "\n  Press Enter to open the main menu...";
    cin.get();

    int choice;
    do {
        printMainMenu();
        cin >> choice;

        switch (choice) {

        case 1: stationsMenu(metro);                    break;
        case 2: routesMenu(metro, routeHistory);        break;
        case 3: passengerMenu(routeHistory);            break;
        case 4: networkDisplayMenu(metro);              break;
        case 5: dsLabMenu(metro);                       break;

        case 0:
            cout << "\n  Exiting Metro Train Route Management System.\n";
            cout << "  Thank you!\n\n";
            break;

        default:
            cout << "  [ERROR] Invalid choice. Enter 0-5.\n";
        }

    } while (choice != 0);

    return 0;
}
