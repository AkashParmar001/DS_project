/*
 * Metro Train Route Management System
 * Data Structures — Second Year Computer Engineering
 *
 * Concepts demonstrated:
 *   Array        — station name storage
 *   Graph        — adjacency matrix for metro connections
 *   Queue        — BFS traversal + passenger FIFO queue
 *   Stack        — recent route history
 *   Linked list  — passenger queue implemented as singly linked list
 *   Linear search, Bubble sort
 */

#include <iostream>
#include <string>
#include <climits>
using namespace std;

// ─────────────────────────────────────────────────────────────
//  CONSTANTS & GLOBAL NETWORK
// ─────────────────────────────────────────────────────────────

const int MAX_STATIONS = 10;
const int INF          = INT_MAX;

// Station names stored in a plain array (demonstrates Array DS)
int   stationCount = 10;
string stationName[MAX_STATIONS] = {
    "Central",      // 0
    "Museum",       // 1
    "University",   // 2
    "Market",       // 3
    "City Hall",    // 4
    "Airport",      // 5
    "Terminal",     // 6
    "Harbor",       // 7
    "Stadium",      // 8
    "Riverside"     // 9
};

// Adjacency matrix — graph[i][j] = distance in km, 0 = no direct link
// (demonstrates Graph / Adjacency Matrix DS)
int graph[MAX_STATIONS][MAX_STATIONS];

// Initialise the predefined metro network
void buildNetwork() {
    // zero out first
    for (int i = 0; i < MAX_STATIONS; i++)
        for (int j = 0; j < MAX_STATIONS; j++)
            graph[i][j] = 0;

    // helper: add undirected edge
    auto addEdge = [](int u, int v, int w) {
        graph[u][v] = w;
        graph[v][u] = w;
    };

    addEdge(0, 1, 2);   // Central  — Museum       2 km
    addEdge(0, 3, 4);   // Central  — Market       4 km
    addEdge(0, 4, 3);   // Central  — City Hall    3 km
    addEdge(1, 2, 3);   // Museum   — University   3 km
    addEdge(2, 3, 2);   // University — Market     2 km
    addEdge(3, 5, 6);   // Market   — Airport      6 km
    addEdge(3, 8, 5);   // Market   — Stadium      5 km
    addEdge(4, 5, 7);   // City Hall — Airport     7 km
    addEdge(4, 7, 4);   // City Hall — Harbor      4 km
    addEdge(5, 6, 3);   // Airport  — Terminal     3 km
    addEdge(6, 7, 2);   // Terminal — Harbor       2 km
    addEdge(7, 9, 4);   // Harbor   — Riverside    4 km
    addEdge(8, 9, 3);   // Stadium  — Riverside    3 km
}

// ─────────────────────────────────────────────────────────────
//  UTILITY
// ─────────────────────────────────────────────────────────────

void printLine(char c = '=', int w = 45) {
    for (int i = 0; i < w; i++) cout << c;
    cout << '\n';
}

// Case-insensitive string compare
bool strEqCI(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++)
        if (tolower(a[i]) != tolower(b[i])) return false;
    return true;
}

// Return station index by name, -1 if not found
int stationIndex(const string& name) {
    for (int i = 0; i < stationCount; i++)
        if (strEqCI(stationName[i], name)) return i;
    return -1;
}

// ─────────────────────────────────────────────────────────────
//  MENU OPTION 1 — Display all stations (Array)
// ─────────────────────────────────────────────────────────────

void displayStations() {
    printLine();
    cout << "  ALL METRO STATIONS\n";
    printLine();
    for (int i = 0; i < stationCount; i++)
        cout << "  [" << i << "] " << stationName[i] << '\n';
    printLine();
    cout << "  Total: " << stationCount << " stations\n";
    // DS note: iterating through a fixed-size array — O(n) time, O(1) extra space
}

// ─────────────────────────────────────────────────────────────
//  MENU OPTION 2 — Search station (Linear Search)
// ─────────────────────────────────────────────────────────────

void searchStation() {
    cout << "\n  Enter station name to search: ";
    string name;
    cin.ignore();
    getline(cin, name);

    // Linear search — O(n), works on unsorted data
    int comparisons = 0;
    int found = -1;
    for (int i = 0; i < stationCount; i++) {
        comparisons++;
        if (strEqCI(stationName[i], name)) { found = i; break; }
    }

    if (found != -1)
        cout << "  Found: [" << found << "] " << stationName[found]
             << "  (after " << comparisons << " comparison"
             << (comparisons > 1 ? "s" : "") << ")\n";
    else
        cout << "  Station \"" << name << "\" not found."
             << "  (" << comparisons << " comparisons made)\n";
    // DS note: Linear search — O(n) worst case
}

// ─────────────────────────────────────────────────────────────
//  MENU OPTION 3 — Display connections (Graph / Adjacency Matrix)
// ─────────────────────────────────────────────────────────────

void displayConnections() {
    printLine();
    cout << "  METRO NETWORK — CONNECTIONS\n";
    printLine();
    for (int i = 0; i < stationCount; i++) {
        cout << "  " << stationName[i] << " -> ";
        bool any = false;
        for (int j = 0; j < stationCount; j++) {
            if (graph[i][j] > 0) {
                if (any) cout << ", ";
                cout << stationName[j] << " (" << graph[i][j] << " km)";
                any = true;
            }
        }
        if (!any) cout << "(no connections)";
        cout << '\n';
    }
    printLine('-');
    cout << "  Adjacency matrix size: " << stationCount
         << " x " << stationCount << " = "
         << stationCount * stationCount << " cells\n";
    // DS note: adjacency matrix — O(V^2) space
}

// ─────────────────────────────────────────────────────────────
//  MENU OPTION 4 — Find route with BFS (Queue + Graph)
// ─────────────────────────────────────────────────────────────

void findRoute() {
    cout << "\n  Enter source station     : ";
    string src, dst;
    cin.ignore();
    getline(cin, src);
    cout << "  Enter destination station: ";
    getline(cin, dst);

    int s = stationIndex(src);
    int d = stationIndex(dst);

    if (s == -1) { cout << "  Source \"" << src << "\" not found.\n"; return; }
    if (d == -1) { cout << "  Destination \"" << dst << "\" not found.\n"; return; }
    if (s == d)  { cout << "  Already at " << stationName[s] << ".\n"; return; }

    // BFS using a plain array as queue (demonstrates Queue DS)
    int  bfsQueue[MAX_STATIONS];
    int  head = 0, tail = 0;
    bool visited[MAX_STATIONS] = {};
    int  parent[MAX_STATIONS];
    for (int i = 0; i < MAX_STATIONS; i++) parent[i] = -1;

    visited[s]     = true;
    bfsQueue[tail++] = s;          // enqueue

    bool found = false;
    while (head < tail) {          // while queue is not empty
        int cur = bfsQueue[head++]; // dequeue
        if (cur == d) { found = true; break; }
        for (int nb = 0; nb < stationCount; nb++) {
            if (graph[cur][nb] > 0 && !visited[nb]) {
                visited[nb]      = true;
                parent[nb]       = cur;
                bfsQueue[tail++] = nb; // enqueue
            }
        }
    }

    if (!found) {
        cout << "  No route found between "
             << stationName[s] << " and " << stationName[d] << ".\n";
        return;
    }

    // Reconstruct path using a small stack (demonstrates Stack DS)
    int pathStack[MAX_STATIONS];
    int top = 0;
    for (int cur = d; cur != -1; cur = parent[cur])
        pathStack[top++] = cur;

    int totalDist = 0;
    int stops     = top - 1;

    printLine();
    cout << "  ROUTE: " << stationName[s]
         << "  ->  " << stationName[d] << "\n";
    printLine('-');
    cout << "  ";
    for (int i = top - 1; i >= 0; i--) {
        cout << stationName[pathStack[i]];
        if (i > 0) {
            int u = pathStack[i], v = pathStack[i - 1];
            totalDist += graph[u][v];
            cout << "  ->  ";
        }
    }
    cout << "\n";
    printLine('-');
    cout << "  Stops   : " << stops      << "\n";
    cout << "  Distance: " << totalDist  << " km\n";
    cout << "  BFS guarantees the fewest stops (unweighted).\n";
    // DS note: BFS — O(V + E) time, uses queue for level-by-level expansion
}

// ─────────────────────────────────────────────────────────────
//  MENU OPTION 5 — Add a connection (Graph edge insertion)
// ─────────────────────────────────────────────────────────────

void addConnection() {
    cout << "\n  Available stations:\n";
    for (int i = 0; i < stationCount; i++)
        cout << "    [" << i << "] " << stationName[i] << "\n";

    cout << "\n  Enter station A name: ";
    string a, b;
    int dist;
    cin.ignore();
    getline(cin, a);
    cout << "  Enter station B name: ";
    getline(cin, b);
    cout << "  Distance in km      : ";
    cin >> dist;

    int ia = stationIndex(a);
    int ib = stationIndex(b);

    if (ia == -1) { cout << "  Station \"" << a << "\" not found.\n"; return; }
    if (ib == -1) { cout << "  Station \"" << b << "\" not found.\n"; return; }
    if (ia == ib) { cout << "  Cannot connect a station to itself.\n"; return; }
    if (dist <= 0){ cout << "  Distance must be greater than 0.\n"; return; }

    graph[ia][ib] = dist;
    graph[ib][ia] = dist;
    cout << "  Connected: " << stationName[ia]
         << " <-> " << stationName[ib]
         << " (" << dist << " km)\n";
    // DS note: O(1) update to adjacency matrix
}

// ─────────────────────────────────────────────────────────────
//  MENU OPTION 6 — Passenger queue (Singly Linked List FIFO)
//
//  Implements a queue using a singly linked list so it
//  demonstrates both Queue and Linked List DS.
// ─────────────────────────────────────────────────────────────

struct Node {
    string name;
    Node*  next;
};

struct LinkedQueue {
    Node* front = nullptr;
    Node* rear  = nullptr;
    int   size  = 0;

    void enqueue(const string& name) {
        Node* n = new Node{name, nullptr};
        if (!rear) { front = rear = n; }
        else       { rear->next = n; rear = n; }
        size++;
        cout << "  Boarded : " << name << "\n";
    }

    void dequeue() {
        if (!front) { cout << "  Platform is empty.\n"; return; }
        cout << "  Departed: " << front->name << "\n";
        Node* tmp = front;
        front = front->next;
        if (!front) rear = nullptr;
        delete tmp;
        size--;
    }

    void display() const {
        if (!front) { cout << "  (no passengers waiting)\n"; return; }
        cout << "  Queue (front -> rear): ";
        Node* cur = front;
        while (cur) {
            cout << "[" << cur->name << "]";
            if (cur->next) cout << " -> ";
            cur = cur->next;
        }
        cout << "\n  Total waiting: " << size << "\n";
        // DS note: linked list traversal — O(n)
    }

    ~LinkedQueue() {
        while (front) {
            Node* tmp = front;
            front = front->next;
            delete tmp;
        }
    }
};

void passengerMenu(LinkedQueue& q) {
    int choice;
    do {
        printLine('-');
        cout << "  PASSENGER QUEUE\n";
        printLine('-');
        cout << "  1. Passenger boards (enqueue)\n";
        cout << "  2. Train departs — next passenger (dequeue)\n";
        cout << "  3. View queue\n";
        cout << "  0. Back\n";
        printLine('-');
        cout << "  Choice: ";
        cin >> choice;

        string name;
        switch (choice) {
        case 1:
            cout << "  Passenger name: ";
            cin.ignore(); getline(cin, name);
            q.enqueue(name);
            break;
        case 2: q.dequeue();  break;
        case 3: q.display();  break;
        case 0: break;
        default: cout << "  Invalid choice.\n";
        }
    } while (choice != 0);
}

// ─────────────────────────────────────────────────────────────
//  MENU OPTION 7 — Sort stations (Bubble Sort)
// ─────────────────────────────────────────────────────────────

void sortStations() {
    // Work on a copy so the original index mapping stays intact
    string sorted[MAX_STATIONS];
    for (int i = 0; i < stationCount; i++) sorted[i] = stationName[i];

    int swaps = 0, passes = 0;

    // Bubble sort — O(n^2) worst case
    for (int i = 0; i < stationCount - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < stationCount - 1 - i; j++) {
            if (sorted[j] > sorted[j + 1]) {
                swap(sorted[j], sorted[j + 1]);
                swapped = true;
                swaps++;
            }
        }
        passes++;
        if (!swapped) break;   // already sorted — early exit
    }

    printLine();
    cout << "  STATIONS — ALPHABETICAL ORDER (Bubble Sort)\n";
    printLine();
    for (int i = 0; i < stationCount; i++)
        cout << "  " << (i + 1) << ". " << sorted[i] << "\n";
    printLine('-');
    cout << "  Passes: " << passes << "   Swaps: " << swaps << "\n";
    cout << "  Bubble sort — O(n^2) worst case, O(n) best case (early exit)\n";
}

// ─────────────────────────────────────────────────────────────
//  ROUTE HISTORY — simple array-based stack
//  (demonstrates Stack DS alongside BFS)
// ─────────────────────────────────────────────────────────────

const int HISTORY_CAP = 20;
string historyStack[HISTORY_CAP];
int    historyTop = 0;

void pushHistory(const string& entry) {
    if (historyTop < HISTORY_CAP)
        historyStack[historyTop++] = entry;
}

void showHistory() {
    if (historyTop == 0) { cout << "  No route history yet.\n"; return; }
    printLine('-');
    cout << "  RECENT ROUTE HISTORY (most recent first)\n";
    printLine('-');
    for (int i = historyTop - 1; i >= 0; i--)
        cout << "  " << (historyTop - i) << ". " << historyStack[i] << "\n";
    // DS note: stack — last-in, first-out, O(1) push/pop
}

// ─────────────────────────────────────────────────────────────
//  MAIN MENU
// ─────────────────────────────────────────────────────────────

void printMenu() {
    printLine();
    cout << "   METRO ROUTE MANAGEMENT SYSTEM\n";
    printLine();
    cout << "  1. Display all stations\n";
    cout << "  2. Search for a station\n";
    cout << "  3. Display station connections\n";
    cout << "  4. Find a route (BFS)\n";
    cout << "  5. Add a connection\n";
    cout << "  6. Passenger queue\n";
    cout << "  7. Sort stations alphabetically\n";
    cout << "  8. View route history\n";
    cout << "  0. Exit\n";
    printLine();
    cout << "  Enter choice: ";
}

int main() {
    buildNetwork();
    LinkedQueue passengers;

    cout << "\n  Metro network loaded — " << stationCount << " stations ready.\n";

    int choice;
    do {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
        }

        cout << "\n";
        switch (choice) {
        case 1: displayStations();           break;
        case 2: searchStation();             break;
        case 3: displayConnections();        break;
        case 4: {
            // capture a summary for history after BFS returns
            // We re-read src/dst from inside findRoute(), so store
            // a placeholder here and let the user see the result above.
            findRoute();
            cout << "\n  Save to history? (y/n): ";
            char yn; cin >> yn;
            if (yn == 'y' || yn == 'Y') {
                cout << "  Label (e.g. Central->Airport): ";
                string label;
                cin.ignore(); getline(cin, label);
                pushHistory(label);
                cout << "  Saved.\n";
            }
            break;
        }
        case 5: addConnection();             break;
        case 6: passengerMenu(passengers);   break;
        case 7: sortStations();              break;
        case 8: showHistory();               break;
        case 0:
            cout << "  Goodbye!\n\n";
            break;
        default:
            cout << "  Invalid choice. Enter 0-8.\n";
        }
        cout << "\n";
    } while (choice != 0);

    return 0;
}
