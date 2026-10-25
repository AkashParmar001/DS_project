#include "metro_graph.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <queue>
#include <stack>
#include <climits>
#include <algorithm>

using namespace std;

// ═══════════════════════════════════════════════════════════════
//  Helpers
// ═══════════════════════════════════════════════════════════════

AdjNode* MetroGraph::newNode(int id, int weight) {
    AdjNode* n = new AdjNode();
    n->stationId = id;
    n->weight    = weight;
    n->next      = nullptr;
    return n;
}

void MetroGraph::freeAdjList(int index) {
    AdjNode* cur = adjList[index];
    while (cur) {
        AdjNode* tmp = cur->next;
        delete cur;
        cur = tmp;
    }
    adjList[index] = nullptr;
}

void MetroGraph::rebuildSparse() {
    sparseCount = 0;
    for (int i = 0; i < stationCount; i++) {
        if (!stations[i].active) continue;
        for (int j = 0; j < stationCount; j++) {
            if (adjMatrix[i][j] != 0) {
                sparse[sparseCount++] = { i, j, adjMatrix[i][j] };
            }
        }
    }
}

// ═══════════════════════════════════════════════════════════════
//  Constructor / Destructor
// ═══════════════════════════════════════════════════════════════

MetroGraph::MetroGraph() {
    stationCount = 0;
    sparseCount  = 0;
    for (int i = 0; i < MAX_STATIONS; i++) {
        adjList[i] = nullptr;
        for (int j = 0; j < MAX_STATIONS; j++) {
            adjMatrix[i][j]    = 0;
            weightMatrix[i][j] = 0;
        }
    }
}

MetroGraph::~MetroGraph() {
    for (int i = 0; i < MAX_STATIONS; i++)
        freeAdjList(i);
}

// ═══════════════════════════════════════════════════════════════
//  Sample Network  (10 stations, undirected)
// ═══════════════════════════════════════════════════════════════
//
//  ID  Name          Zone
//   0  Central        A
//   1  Museum         A
//   2  University     B
//   3  Market         B
//   4  City Hall      A
//   5  Airport        C
//   6  Terminal       C
//   7  Harbor         C
//   8  Stadium        B
//   9  Riverside      C
//
//  Edges (undirected, weight = distance in km):
//  Central   – Museum      (2)
//  Central   – Market      (4)
//  Central   – City Hall   (3)
//  Museum    – University  (3)
//  University– Market      (2)
//  Market    – Airport     (6)
//  Market    – Stadium     (5)
//  City Hall – Airport     (7)
//  City Hall – Harbor      (4)
//  Airport   – Terminal    (3)
//  Terminal  – Harbor      (2)
//  Harbor    – Riverside   (4)
//  Stadium   – Riverside   (3)
// ═══════════════════════════════════════════════════════════════

void MetroGraph::loadSampleNetwork() {
    // clear first
    for (int i = 0; i < MAX_STATIONS; i++) {
        freeAdjList(i);
        for (int j = 0; j < MAX_STATIONS; j++) {
            adjMatrix[i][j]    = 0;
            weightMatrix[i][j] = 0;
        }
    }
    stationCount = 0;

    // add stations
    addStation(0, "Central",    "A");
    addStation(1, "Museum",     "A");
    addStation(2, "University", "B");
    addStation(3, "Market",     "B");
    addStation(4, "City Hall",  "A");
    addStation(5, "Airport",    "C");
    addStation(6, "Terminal",   "C");
    addStation(7, "Harbor",     "C");
    addStation(8, "Stadium",    "B");
    addStation(9, "Riverside",  "C");

    // add edges
    addEdge(0, 1, 2);
    addEdge(0, 3, 4);
    addEdge(0, 4, 3);
    addEdge(1, 2, 3);
    addEdge(2, 3, 2);
    addEdge(3, 5, 6);
    addEdge(3, 8, 5);
    addEdge(4, 5, 7);
    addEdge(4, 7, 4);
    addEdge(5, 6, 3);
    addEdge(6, 7, 2);
    addEdge(7, 9, 4);
    addEdge(8, 9, 3);

    rebuildSparse();
    cout << "\n[INFO] Sample network loaded: " << stationCount << " stations.\n";
}

// ═══════════════════════════════════════════════════════════════
//  Station CRUD
// ═══════════════════════════════════════════════════════════════

bool MetroGraph::addStation(int id, const string& name, const string& zone) {
    // check duplicate ID
    if (findStationById(id) != -1) {
        cout << "[ERROR] Station ID " << id << " already exists.\n";
        return false;
    }
    if (stationCount >= MAX_STATIONS) {
        cout << "[ERROR] Station limit reached.\n";
        return false;
    }
    stations[stationCount] = { id, name, zone, true };
    stationCount++;
    return true;
}

bool MetroGraph::deleteStation(int id) {
    int idx = findStationById(id);
    if (idx == -1) {
        cout << "[ERROR] Station ID " << id << " not found.\n";
        return false;
    }
    stations[idx].active = false;

    // remove all edges in adjacency list that involve this station
    freeAdjList(idx);

    // remove edges from other stations' lists that point to this station
    for (int i = 0; i < stationCount; i++) {
        if (i == idx || !stations[i].active) continue;
        AdjNode* prev = nullptr;
        AdjNode* cur  = adjList[i];
        while (cur) {
            if (cur->stationId == id) {
                if (prev) prev->next = cur->next;
                else       adjList[i] = cur->next;
                delete cur;
                break;
            }
            prev = cur;
            cur  = cur->next;
        }
    }

    // zero out matrix rows/cols
    for (int j = 0; j < stationCount; j++) {
        adjMatrix[idx][j]    = adjMatrix[j][idx]    = 0;
        weightMatrix[idx][j] = weightMatrix[j][idx] = 0;
    }

    rebuildSparse();
    cout << "[INFO] Station \"" << stations[idx].name << "\" deleted.\n";
    return true;
}

// Returns the index in stations[] of matching name (case-insensitive), or -1
int MetroGraph::findStation(const string& name) const {
    string lower = name;
    for (char& c : lower) c = (char)tolower(c);
    for (int i = 0; i < stationCount; i++) {
        if (!stations[i].active) continue;
        string sname = stations[i].name;
        for (char& c : sname) c = (char)tolower(c);
        if (sname == lower) return i;
    }
    return -1;
}

// Returns the index in stations[] of matching ID, or -1
int MetroGraph::findStationById(int id) const {
    for (int i = 0; i < stationCount; i++) {
        if (stations[i].active && stations[i].id == id) return i;
    }
    return -1;
}

void MetroGraph::displayAllStations() const {
    cout << "\n+----+------------------+------+-----------------------------+\n";
    cout << "| ID | Name             | Zone | Connections                 |\n";
    cout << "+----+------------------+------+-----------------------------+\n";
    for (int i = 0; i < stationCount; i++) {
        if (!stations[i].active) continue;
        cout << "| " << setw(2) << stations[i].id
             << " | " << setw(16) << left << stations[i].name
             << " | " << setw(4) << stations[i].zone
             << " | ";
        AdjNode* cur = adjList[i];
        bool first = true;
        while (cur) {
            int ni = findStationById(cur->stationId);
            if (ni != -1) {
                if (!first) cout << ", ";
                cout << stations[ni].name;
                first = false;
            }
            cur = cur->next;
        }
        cout << right << "\n";
    }
    cout << "+----+------------------+------+-----------------------------+\n";
    cout << "Total active stations: " << stationCount << "\n";
}

void MetroGraph::searchStation() const {
    cout << "\nEnter station name or ID to search: ";
    string input;
    cin.ignore();
    getline(cin, input);

    // try numeric ID first
    bool isNum = true;
    for (char c : input) if (!isdigit(c)) { isNum = false; break; }

    int idx = -1;
    if (isNum && !input.empty()) {
        idx = findStationById(stoi(input));
    } else {
        idx = findStation(input);
    }

    if (idx == -1) {
        cout << "[NOT FOUND] No station matching \"" << input << "\".\n";
        return;
    }

    const Station& s = stations[idx];
    cout << "\n  ID   : " << s.id
         << "\n  Name : " << s.name
         << "\n  Zone : " << s.zone
         << "\n  Connections: ";
    AdjNode* cur = adjList[idx];
    bool first = true;
    while (cur) {
        int ni = findStationById(cur->stationId);
        if (ni != -1) {
            if (!first) cout << " -> ";
            cout << stations[ni].name << " (" << cur->weight << " km)";
            first = false;
        }
        cur = cur->next;
    }
    if (first) cout << "(none)";
    cout << "\n";
}

// ═══════════════════════════════════════════════════════════════
//  Edge operations
// ═══════════════════════════════════════════════════════════════

bool MetroGraph::addEdge(int fromId, int toId, int weight) {
    int fi = findStationById(fromId);
    int ti = findStationById(toId);
    if (fi == -1 || ti == -1) {
        cout << "[ERROR] One or both station IDs not found.\n";
        return false;
    }
    // undirected: add both directions
    // adjacency list
    AdjNode* n1 = newNode(toId,   weight);
    n1->next    = adjList[fi];
    adjList[fi] = n1;

    AdjNode* n2 = newNode(fromId, weight);
    n2->next    = adjList[ti];
    adjList[ti] = n2;

    // adjacency matrix (unweighted: mark 1)
    adjMatrix[fi][ti] = 1;
    adjMatrix[ti][fi] = 1;

    // weighted matrix
    weightMatrix[fi][ti] = weight;
    weightMatrix[ti][fi] = weight;

    rebuildSparse();
    return true;
}

bool MetroGraph::removeEdge(int fromId, int toId) {
    int fi = findStationById(fromId);
    int ti = findStationById(toId);
    if (fi == -1 || ti == -1) {
        cout << "[ERROR] One or both station IDs not found.\n";
        return false;
    }

    // helper lambda to remove from adj list
    auto removeFromList = [&](int src, int dst) {
        AdjNode* prev = nullptr;
        AdjNode* cur  = adjList[src];
        while (cur) {
            if (cur->stationId == dst) {
                if (prev) prev->next = cur->next;
                else       adjList[src] = cur->next;
                delete cur;
                return;
            }
            prev = cur;
            cur  = cur->next;
        }
    };

    removeFromList(fi, toId);
    removeFromList(ti, fromId);

    adjMatrix[fi][ti] = adjMatrix[ti][fi] = 0;
    weightMatrix[fi][ti] = weightMatrix[ti][fi] = 0;
    rebuildSparse();
    return true;
}

// ═══════════════════════════════════════════════════════════════
//  Display functions
// ═══════════════════════════════════════════════════════════════

void MetroGraph::displayAdjacencyList() const {
    cout << "\n--- Adjacency List ---\n";
    for (int i = 0; i < stationCount; i++) {
        if (!stations[i].active) continue;
        cout << "  [" << stations[i].id << "] " << setw(12) << left << stations[i].name << " : ";
        AdjNode* cur = adjList[i];
        if (!cur) { cout << "(no connections)\n"; continue; }
        while (cur) {
            int ni = findStationById(cur->stationId);
            if (ni != -1)
                cout << stations[ni].name << "(" << cur->weight << ") ";
            cur = cur->next;
        }
        cout << right << "\n";
    }
    cout << "  Complexity: O(V + E) space\n";
}

void MetroGraph::displayAdjacencyMatrix() const {
    cout << "\n--- Adjacency Matrix (Unweighted: 0/1) ---\n";
    // header
    cout << "     ";
    for (int i = 0; i < stationCount; i++) {
        if (!stations[i].active) continue;
        cout << setw(4) << stations[i].id;
    }
    cout << "\n";
    for (int i = 0; i < stationCount; i++) {
        if (!stations[i].active) continue;
        cout << setw(3) << stations[i].id << "  ";
        for (int j = 0; j < stationCount; j++) {
            if (!stations[j].active) continue;
            cout << setw(4) << adjMatrix[i][j];
        }
        cout << "\n";
    }
    cout << "  Complexity: O(V^2) space\n";
}

void MetroGraph::displayWeightedMatrix() const {
    cout << "\n--- Weighted Adjacency Matrix (km) ---\n";
    cout << "     ";
    for (int i = 0; i < stationCount; i++) {
        if (!stations[i].active) continue;
        cout << setw(5) << stations[i].id;
    }
    cout << "\n";
    for (int i = 0; i < stationCount; i++) {
        if (!stations[i].active) continue;
        cout << setw(3) << stations[i].id << "  ";
        for (int j = 0; j < stationCount; j++) {
            if (!stations[j].active) continue;
            cout << setw(5) << weightMatrix[i][j];
        }
        cout << "\n";
    }
}

void MetroGraph::displaySparseMatrix() const {
    cout << "\n--- Sparse Matrix Representation ---\n";
    cout << "  (stores only non-zero entries of the adjacency matrix)\n";
    cout << "  Total cells : " << stationCount << " x " << stationCount
         << " = " << stationCount * stationCount << "\n";
    cout << "  Non-zero    : " << sparseCount << "\n";
    cout << "  Saved space : "
         << (stationCount * stationCount - sparseCount) << " zero entries skipped\n\n";
    cout << "  Row  Col  Value\n";
    cout << "  ---------------\n";
    for (int k = 0; k < sparseCount; k++) {
        cout << "  " << setw(3) << sparse[k].row
             << "  " << setw(3) << sparse[k].col
             << "  " << setw(5) << sparse[k].value << "\n";
    }
    cout << "\n  Complexity: O(non-zero entries) space vs O(V^2) for full matrix\n";
}

void MetroGraph::displayMemoryLayout() const {
    cout << "\n--- 2D Array Memory Layout (Row-major vs Column-major) ---\n";
    int rows = stationCount, cols = stationCount;
    cout << "  For a " << rows << " x " << cols << " matrix A:\n\n";
    cout << "  ROW-MAJOR (C/C++ default):\n";
    cout << "    A[i][j] is at offset = i * " << cols << " + j\n";
    cout << "    Example: A[2][3] -> offset = 2*" << cols << "+3 = "
         << 2*cols+3 << "\n\n";
    cout << "  COLUMN-MAJOR (Fortran / MATLAB):\n";
    cout << "    A[i][j] is at offset = j * " << rows << " + i\n";
    cout << "    Example: A[2][3] -> offset = 3*" << rows << "+2 = "
         << 3*rows+2 << "\n\n";
    cout << "  In C++, 2D arrays are stored row-by-row in contiguous memory.\n";
    cout << "  Accessing row by row is cache-friendly (O(1) per element).\n";
}

// ═══════════════════════════════════════════════════════════════
//  BFS  — fewest-stop route  (Unit 5)
// ═══════════════════════════════════════════════════════════════

void MetroGraph::bfsRoute() const {
    cout << "\nEnter source station name: ";
    string src, dst;
    cin.ignore();
    getline(cin, src);
    cout << "Enter destination station name: ";
    getline(cin, dst);

    int si = findStation(src);
    int di = findStation(dst);

    if (si == -1) { cout << "[ERROR] Source station \"" << src << "\" not found.\n"; return; }
    if (di == -1) { cout << "[ERROR] Destination station \"" << dst << "\" not found.\n"; return; }
    if (si == di) { cout << "[INFO] You are already at " << stations[si].name << "!\n"; return; }

    // BFS using std::queue
    // visited[], parent[] indexed by stationCount position
    bool visited[MAX_STATIONS]  = { false };
    int  parent[MAX_STATIONS];
    fill(parent, parent + MAX_STATIONS, -1);

    queue<int> q;
    visited[si] = true;
    q.push(si);
    bool found = false;

    while (!q.empty()) {
        int cur = q.front(); q.pop();
        if (cur == di) { found = true; break; }
        AdjNode* node = adjList[cur];
        while (node) {
            int ni = findStationById(node->stationId);
            if (ni != -1 && !visited[ni]) {
                visited[ni] = true;
                parent[ni]  = cur;
                q.push(ni);
            }
            node = node->next;
        }
    }

    if (!found) {
        cout << "\n[RESULT] No route exists between \""
             << stations[si].name << "\" and \"" << stations[di].name << "\".\n";
        return;
    }

    // Reconstruct path
    stack<int> path;
    for (int cur = di; cur != -1; cur = parent[cur])
        path.push(cur);

    cout << "\n=========================================\n";
    cout << "  BFS Route: " << stations[si].name << " -> " << stations[di].name << "\n";
    cout << "=========================================\n  ";
    int stops = 0;
    int prev  = -1;
    int totalDist = 0;
    while (!path.empty()) {
        int cur = path.top(); path.pop();
        if (prev != -1) {
            cout << " -> ";
            totalDist += weightMatrix[prev][cur];
            stops++;
        }
        cout << stations[cur].name;
        prev = cur;
    }
    cout << "\n\n  Stops      : " << stops;
    cout << "\n  Distance   : " << totalDist << " km";
    cout << "\n  Complexity : O(V + E)\n";
    cout << "=========================================\n";
}

// ═══════════════════════════════════════════════════════════════
//  DFS  — full network exploration  (Unit 5)
// ═══════════════════════════════════════════════════════════════

void MetroGraph::dfsExplore() const {
    cout << "\nEnter starting station name: ";
    string src;
    cin.ignore();
    getline(cin, src);

    int si = findStation(src);
    if (si == -1) {
        cout << "[ERROR] Station \"" << src << "\" not found.\n";
        return;
    }

    bool visited[MAX_STATIONS] = { false };
    stack<int> stk;
    stk.push(si);

    cout << "\n=========================================\n";
    cout << "  DFS Traversal from: " << stations[si].name << "\n";
    cout << "=========================================\n  ";

    int count = 0;
    while (!stk.empty()) {
        int cur = stk.top(); stk.pop();
        if (visited[cur]) continue;
        visited[cur] = true;
        if (count > 0) cout << " -> ";
        cout << stations[cur].name;
        count++;

        // push neighbours (reversed so left-most is visited first)
        // collect into temp array first
        int nb[MAX_ADJ]; int nc = 0;
        AdjNode* node = adjList[cur];
        while (node) {
            int ni = findStationById(node->stationId);
            if (ni != -1 && !visited[ni]) nb[nc++] = ni;
            node = node->next;
        }
        for (int k = nc - 1; k >= 0; k--)
            stk.push(nb[k]);
    }
    cout << "\n\n  Stations visited : " << count;
    cout << "\n  Complexity       : O(V + E)\n";
    cout << "=========================================\n";
}

// ═══════════════════════════════════════════════════════════════
//  Prim's MST  (Unit 5)
// ═══════════════════════════════════════════════════════════════

void MetroGraph::primsMST() const {
    int n = stationCount;
    if (n == 0) { cout << "[ERROR] No stations.\n"; return; }

    // collect active indices
    int active[MAX_STATIONS]; int ac = 0;
    for (int i = 0; i < n; i++)
        if (stations[i].active) active[ac++] = i;

    int  key[MAX_STATIONS];    // min edge weight to include vertex
    int  parent[MAX_STATIONS]; // MST parent
    bool inMST[MAX_STATIONS];

    fill(key,    key    + MAX_STATIONS, INT_MAX);
    fill(parent, parent + MAX_STATIONS, -1);
    fill(inMST,  inMST  + MAX_STATIONS, false);

    key[active[0]] = 0;  // start from first active station

    for (int iter = 0; iter < ac; iter++) {
        // pick min key not yet in MST
        int u = -1;
        for (int i = 0; i < ac; i++) {
            int v = active[i];
            if (!inMST[v] && (u == -1 || key[v] < key[u])) u = v;
        }
        if (u == -1) break;
        inMST[u] = true;

        // update neighbours
        for (int i = 0; i < ac; i++) {
            int v = active[i];
            int w = weightMatrix[u][v];
            if (!inMST[v] && w > 0 && w < key[v]) {
                key[v]    = w;
                parent[v] = u;
            }
        }
    }

    cout << "\n=========================================\n";
    cout << "  Prim's Minimum Spanning Tree\n";
    cout << "=========================================\n";
    cout << "  Edge                          Weight\n";
    cout << "  --------------------------------\n";
    int total = 0;
    for (int i = 0; i < ac; i++) {
        int v = active[i];
        if (parent[v] != -1) {
            cout << "  " << setw(12) << left << stations[parent[v]].name
                 << " -- " << setw(12) << stations[v].name
                 << right << setw(4) << weightMatrix[parent[v]][v] << " km\n";
            total += weightMatrix[parent[v]][v];
        }
    }
    cout << "  --------------------------------\n";
    cout << "  Total MST cost : " << total << " km\n";
    cout << "  Complexity     : O(V^2)\n";
    cout << "=========================================\n";
}

// ═══════════════════════════════════════════════════════════════
//  Kruskal's MST  — Union-Find  (Unit 5)
// ═══════════════════════════════════════════════════════════════

struct Edge { int u, v, w; };

static int ufFind(int parent[], int x) {
    if (parent[x] != x) parent[x] = ufFind(parent, parent[x]);
    return parent[x];
}
static void ufUnion(int parent[], int rank[], int x, int y) {
    int rx = ufFind(parent, x), ry = ufFind(parent, y);
    if (rank[rx] < rank[ry]) swap(rx, ry);
    parent[ry] = rx;
    if (rank[rx] == rank[ry]) rank[rx]++;
}

void MetroGraph::kruskalsMST() const {
    int n = stationCount;
    if (n == 0) { cout << "[ERROR] No stations.\n"; return; }

    // build edge list (avoid duplicates: only i < j)
    Edge edges[MAX_STATIONS * MAX_ADJ];
    int  ec = 0;
    for (int i = 0; i < n; i++) {
        if (!stations[i].active) continue;
        for (int j = i + 1; j < n; j++) {
            if (!stations[j].active) continue;
            if (weightMatrix[i][j] > 0) {
                edges[ec++] = { i, j, weightMatrix[i][j] };
            }
        }
    }

    // sort edges by weight (selection sort — keeps this file self-contained)
    for (int i = 0; i < ec - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < ec; j++)
            if (edges[j].w < edges[minIdx].w) minIdx = j;
        if (minIdx != i) swap(edges[i], edges[minIdx]);
    }

    int ufParent[MAX_STATIONS], ufRank[MAX_STATIONS];
    for (int i = 0; i < n; i++) { ufParent[i] = i; ufRank[i] = 0; }

    cout << "\n=========================================\n";
    cout << "  Kruskal's Minimum Spanning Tree\n";
    cout << "=========================================\n";
    cout << "  Edge                          Weight\n";
    cout << "  --------------------------------\n";

    int total = 0, edgesUsed = 0;
    for (int k = 0; k < ec; k++) {
        int u = edges[k].u, v = edges[k].v;
        if (ufFind(ufParent, u) != ufFind(ufParent, v)) {
            ufUnion(ufParent, ufRank, u, v);
            cout << "  " << setw(12) << left << stations[u].name
                 << " -- " << setw(12) << stations[v].name
                 << right << setw(4) << edges[k].w << " km\n";
            total += edges[k].w;
            edgesUsed++;
        }
    }
    cout << "  --------------------------------\n";
    cout << "  Total MST cost : " << total << " km\n";
    cout << "  Edges used     : " << edgesUsed << "\n";
    cout << "  Complexity     : O(E log E)\n";
    cout << "=========================================\n";
}
