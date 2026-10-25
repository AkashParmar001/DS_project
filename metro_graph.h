#ifndef METRO_GRAPH_H
#define METRO_GRAPH_H

#include <string>

// ─────────────────────────────────────────────
//  Constants
// ─────────────────────────────────────────────
static const int MAX_STATIONS  = 50;   // max stations in the system
static const int MAX_ADJ       = 20;   // max direct neighbours per station

// ─────────────────────────────────────────────
//  Station record
// ─────────────────────────────────────────────
struct Station {
    int         id;
    std::string name;
    std::string zone;
    bool        active;     // false = deleted (soft delete)
};

// ─────────────────────────────────────────────
//  Adjacency-list node  (linked list of neighbours)
// ─────────────────────────────────────────────
struct AdjNode {
    int      stationId;   // neighbour station ID
    int      weight;      // edge weight (km); 1 for unweighted use
    AdjNode* next;
};

// ─────────────────────────────────────────────
//  Sparse-matrix entry  (non-zero entries only)
// ─────────────────────────────────────────────
struct SparseEntry {
    int row, col, value;
};

// ─────────────────────────────────────────────
//  MetroGraph class
// ─────────────────────────────────────────────
class MetroGraph {
public:
    // ── data ──────────────────────────────────
    Station   stations[MAX_STATIONS];
    int       stationCount;

    // adjacency list heads (one per station slot)
    AdjNode*  adjList[MAX_STATIONS];

    // adjacency matrix (unweighted: 0/1)
    int       adjMatrix[MAX_STATIONS][MAX_STATIONS];

    // weighted matrix (for MST demonstrations)
    int       weightMatrix[MAX_STATIONS][MAX_STATIONS];

    // sparse-matrix store
    SparseEntry sparse[MAX_STATIONS * MAX_ADJ];
    int          sparseCount;

    // ── lifecycle ─────────────────────────────
    MetroGraph();
    ~MetroGraph();

    void loadSampleNetwork();   // pre-load the 10-station demo network

    // ── station CRUD ──────────────────────────
    bool addStation(int id, const std::string& name, const std::string& zone);
    bool deleteStation(int id);
    int  findStation(const std::string& name) const;  // returns index (-1 if not found)
    int  findStationById(int id) const;               // returns index (-1 if not found)
    void displayAllStations() const;
    void searchStation() const;

    // ── edge operations ───────────────────────
    bool addEdge(int fromId, int toId, int weight = 1);
    bool removeEdge(int fromId, int toId);

    // ── display ───────────────────────────────
    void displayAdjacencyList()   const;
    void displayAdjacencyMatrix() const;
    void displayWeightedMatrix()  const;
    void displaySparseMatrix()    const;
    void displayMemoryLayout()    const;   // row-major / column-major demo

    // ── traversals (implemented in Phase 2) ───
    void bfsRoute()   const;   // prompts for source & destination
    void dfsExplore() const;   // prompts for start station

    // ── MST (implemented in Phase 2) ──────────
    void primsMST()    const;
    void kruskalsMST() const;

private:
    // helpers
    void   rebuildSparse();
    void   freeAdjList(int index);
    AdjNode* newNode(int id, int weight);
};

#endif // METRO_GRAPH_H
