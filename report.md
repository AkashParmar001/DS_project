# Metro Train Route Management System

**Project Report — Data Structures & Algorithms**
*Second Year Computer Engineering*

---

## 1. Abstract

The **Metro Train Route Management System** is a console-based C++ application that models a city metro network and, at the same time, serves as a working demonstration of every major data-structures unit of the syllabus. A real metro network is simulated as a **undirected, weighted graph**: stations are vertices and rail connections are edges carrying distances in kilometres. On top of this core, the system integrates stacks, queues, linked lists, trees, sorting/searching techniques and hashing, all exposed through a simple five-option menu (Stations, Routes, Passenger, Display Network, DS Lab).

The application is organised into five independent modules plus a menu driver, which shows how a single problem domain can exercise **all** the classical data structures — not as isolated textbook programs, but as components of one working system.

---

## 2. Objectives

1. To represent a metro network as an **undirected weighted graph** and support basic operations (add/delete stations, add/remove connections).
2. To implement **graph traversal algorithms** — BFS for the route with the fewest stops and DFS for network exploration.
3. To implement **Minimum Spanning Tree** algorithms (Prim's and Kruskal's) that compute the minimum track length connecting all stations.
4. To apply **linear data structures** (stacks, queues, varied linked lists) to realistic metro scenarios: route history, passenger boarding queues.
5. To implement **expression conversion/evaluation using stacks** and demonstrate recursion through the Tower of Hanoi.
6. To implement and compare **tree structures** — binary tree, BST, AVL, B-tree and B+ tree.
7. To implement and compare **sorting, searching and hashing** techniques on a station-record dataset.

---

## 3. System Architecture

### 3.1 Module organisation

| File | Role | Syllabus units covered |
|---|---|---|
| `main.cpp` | Menu driver — prints the main menu and dispatches to module menus | — |
| `metro_graph.cpp/.h` | Graph core: stations, connections, BFS, DFS, MST | Units 1 & 5 |
| `linear_structures.cpp/.h` | Stacks, queues, deques, priority queues, four linked-list variants | Units 2 & 3 |
| `expression_parser.cpp/.h` | Infix↔postfix↔prefix conversion, evaluation, Tower of Hanoi | Unit 2 |
| `tree_algorithms.cpp/.h` | Binary tree, BST, AVL, B-tree, B+ tree, general-tree conversion | Unit 4 |
| `sorting_searching.cpp/.h` | Sorting algorithms, linear/binary search, hash tables | Unit 6 |

### 3.2 Program flow

```
        ┌──────────────────────────────┐
        │          main.cpp            │
        │   MAIN MENU (5 options)      │
        └──────────────┬───────────────┘
      ┌──────────┬─────┴─────┬──────────────┬───────────┐
      ▼          ▼           ▼              ▼           ▼
  Stations     Routes     Passenger    Display       DS Lab
  (metro_graph)            + linked     adjacency/    (all demo
      │        BFS / DFS   structures   matrices      modules)
      ▼                     arrays       sparse        detailed
  CRUD ops                  Visual 2-D   memory        submenu)
  & edges                   tree prints  layout
```

Each menu level follows the same control pattern: display options, `cin >> choice`, `switch` dispatch, loop until the user selects *Back* (0).

### 3.3 How to build and run

```bash
# Modular build (main.cpp + 5 modules)
g++ -std=c++17 -Wall -Wextra -o metro main.cpp metro_graph.cpp \
    linear_structures.cpp expression_parser.cpp tree_algorithms.cpp \
    sorting_searching.cpp
./metro
```

A one-file demo build (`metro.cpp`, a reduced 8-option version of the same concepts) can be built and run through Docker:

```bash
docker build -t metro-ds .
docker run -it --rm metro-ds
```

---

## 4. Module-wise Working

### 4.1 `metro_graph` — the graph core (Units 1 & 5)

**Data representation.** A station is a record of *id, name, zone, active flag* (deleted stations are soft-deleted by setting `active = false`). The network is stored in **four complementary views**, which is itself a demonstration of Unit 1:

| View | Storage | Purpose |
|---|---|---|
| Adjacency list | `AdjNode*` linked lists — one head per station, each node holds {neighbour Id, weight} | Efficient traversal (BFS/DFS); grows with the number of edges |
| Adjacency matrix (unweighted) | `int adjMatrix[50][50]` with 0/1 entries | O(1) edge existence check |
| Weighted matrix | `int weightMatrix[50][50]` with distance in km | Input to Prim's/Kruskal's MST |
| Sparse matrix | `SparseEntry {row, col, value}` — only non-zero cells | Shows why a 50×50 matrix dominated by zeros wastes memory |

A **2-D array memory-layout demo** (row-major vs column-major address formula `addr = base + (i × ncols + j) × esize`) completes Unit 1 coverage.

**Station operations.**
- `addStation(id, name, zone)` — appends a vertex; both matrices only grow when edges are added later, so a new station costs O(1).
- `deleteStation(id)` — soft-deletes the record and frees that row's adjacency list.
- `searchStation()` — the user may search by **ID (O(1) index lookup)** or by **name (linear search, O(n))** — and the comparison count is printed so the search behaviour is visible.
- `addEdge / removeEdge` — insert or remove nodes in the two adjacency lists (symmetric, since the graph is undirected), then flip the corresponding matrix cells.

**BFS route finding (fewest stops).**
1. Read source and destination, resolve names → indices (case-insensitive).
2. Maintain a queue and a `prev[]` array; enqueue the source, mark visited.
3. Dequeue a station, enqueue all unvisited neighbours — **level by level**.
4. When the destination is dequeued, walk back through `prev[]`, push the path onto a **stack**, pop it to print the route in the correct order.

BFS guarantees the *fewest stops* (not the fewest km — that would need Dijkstra's) because it visits all one-hop stations before any two-hop station. Complexity: **O(V + E)** time, O(V) space. The resulting route can optionally be saved to the route-history stack.

**DFS exploration.** Starting from a chosen station, DFS uses an explicit **stack** (not recursion) and follows one branch as deep as possible before backtracking — printing the order in which the network is explored. Complexity O(V + E). It nicely contrasts with BFS on the same graph.

**Minimum Spanning Tree.** For a connected weighted network of V stations, the MST is the set of **V − 1 edges** with minimum total track length and no cycles.
- *Prim's* — grow one tree from an arbitrary station, always adding the cheapest edge crossing the cut; adjacency-matrix version, **O(V²)**.
- *Kruskal's* — sort all edges by weight and add each edge unless it closes a cycle (cycle check via connected components), **O(E log E)**.
Both functions print step-by-step edge acceptance so the greedy logic is observable.

The same module also hosts conceptual **demonstrations of directed, weighted, cyclic/acyclic and multi graphs** with the metro network as the running example.

### 4.2 `linear_structures` — stacks, queues & linked lists (Units 2 & 3)

Every structure is hand-implemented (no STL adapters) and driven by an interactive demo menu, with metro-themed scenarios:

**Array-based (Unit 2).**

| Structure | Scenario in the app | Key operations | Complexity |
|---|---|---|---|
| `ArrayStack` | **Route history** — each saved BFS route is pushed; backtrack pops the most recent | push, pop, peek, reverse-display (pop until empty) | O(1) each |
| `SimpleQueue` | Fixed-capacity boarding queue | enqueue at rear, dequeue at front, uses front/rear/size indices to avoid shifting | O(1) each |
| `CircularQueue` | Ring-organised queue that reuses freed slots | enqueue/dequeue with wrap-around `% capacity` logic | O(1) each |
| `PriorityQueue` | Queue in which *senior citizens* and *emergency* passengers board before normal ones | insert sorted by priority; highest priority dequeued first | O(n) enqueue, O(1) dequeue |
| `Deque` | Zone-A passengers enter/delete from either end | pushFront/pushBack/popFront/popBack | O(1) each |

**Linked-list-based (Unit 3).** Four list variants are implemented — **singly, singly-circular, doubly, doubly-circular** — with insert front/back/position, delete, search, reverse where applicable. The doubly-circular list's wrap-around display is used to visualise a loop metro line (Riverside ⇢ ... ⇢ Central ⇢ Riverside). Additionally:

- `LinkedStack` — a stack built on a singly linked list (push/pop/peek at the head, O(1), grows dynamically).
- `LinkedQueue` — head/rear pointers, O(1) enqueue/dequeue; this is the structure behind the **Passenger Queue Management** option.

**Viva-relevant design point:** the linked-list queue beats a plain array queue because there is no fixed capacity and no shifting; the array stack beats it on locality and simplicity. Both trade-offs are deliberately visible in the demos.

### 4.3 `expression_parser` — stacks for parsing & recursion (Unit 2)

All operations are built on two primitive stacks (`CharStack`, `DoubleStack`).

- **Parenthesis checker** — push `(`, pop on `)`; end-state must be empty. Typical stack application.
- **Infix → Postfix (shunting-yard)** — operands go straight to output; an operator is pushed only after popping all operators of ≥ precedence from the stack; parentheses are handled explicitly.
- **Infix → Prefix** — reverse the infix, adjust brackets, run the postfix algorithm, reverse the result.
- **Postfix/Prefix evaluation** — scan left→right (right→left for prefix); operands pushed; on each operator pop two numbers, apply, push the result; the final stack element is the answer. A four-operator calculator exposes this in the menu.
- **Tower of Hanoi** — the classic recursive stack: `hanoi(n, A, C, B)` prints every move of the n disks and tracks the move count against the closed form **2ⁿ − 1**; complexity **O(2ⁿ)**.

### 4.4 `tree_algorithms` — hierarchical structures (Unit 4)

All trees include a **visual 2-D print** (sideways indented display) so balance and structure can be seen immediately.

1. **Generic binary tree** — a hardcoded 7-node sample; demonstrates **inorder, preorder, postorder recursive traversals** and height calculation.
2. **Binary Search Tree** — insert (left < node < right), search **O(h)**, delete with all three cases (leaf, one child, two children via in-order successor), plus the three traversals — inorder always prints keys in sorted order.
3. **AVL tree** — BST with per-node `height`; after every insert/remove the **balance factor** is recomputed and single **LL/RR rotations** restore |balance| ≤ 1. Menu allows inserting any key sequence, so degenerate BST vs self-balancing behaviour can be compared side by side. Guarantee: height ≈ 1.44 log₂ n → **O(log n)** search.
4. **B-tree (minimum degree t = 2)** — a multi-way balanced tree: nodes hold up to 3 keys and 4 children; inserts **pre-split** full children on the way down and push a median key into the parent. Level-order display shows keys distributed across levels.
5. **B+ tree (order t = 2)** — B-tree variant where **all data lives in sorted leaves linked as a chain** and internal nodes hold only separator keys; the "display leaves" option walks the chain, explaining why B+ trees are ideal for indexing (fast range scans).
6. **General tree → binary tree (Left-Child/Right-Sibling)** — a general tree (e.g., one zone node with several line children) is converted by reinterpreting `firstChild/nextSibling` as `left/right`, so any binary-tree algorithm applies — the LC-RS representation of Unit 4.

### 4.5 `sorting_searching` — sorting, searching & hashing (Unit 6)

A station-record dataset `SSRecord {id, name, zone}` is loaded, printed before/after every operation, and reused by all three sub-menus.

**Sorting** (each run reports pass/swap counts where meaningful):

| Algorithm | Best | Average | Worst | Stable |
|---|---|---|---|---|
| Bubble (early-exit flag) | O(n) | O(n²) | O(n²) | yes |
| Insertion | O(n) | O(n²) | O(n²) | yes |
| Selection | O(n²) | O(n²) | O(n²) | no |
| Quick (Lomuto partition) | O(n log n) | O(n log n) | O(n²) | no |
| Merge | O(n log n) | O(n log n) | O(n log n) | yes |

Both **by-ID and by-name (alphabetical)** variants are provided (selection/merge by name), which demonstrates comparison-based sorting of records rather than bare integers.

**Searching:** linear search (works on unsorted data, O(n)) vs binary search (needs sorted data, O(log n) — halves the range each step). A complexity-summary screen tabulates these side by side.

**Hashing** — `hashFunc(key) = key % 13` (prime table size) with two collision strategies implemented in full:

- `LPHashTable` — **linear probing**: slot (h + i) mod 13; a `deleted[]` tombstone array keeps probing chains intact after removals.
- `SCHashTable` — **separate chaining**: each bucket is a singly linked list of `SCNode`.

Both expose `showStats()` — occupancy and average/maximum chain or probe lengths — making the collision behaviour measurable, and the chaining variant double-functions as a **symbol table demo** (ID → record lookup).

---

## 5. Complexity Summary

| Operation / Algorithm | Time | Space |
|---|---|---|
| BFS / DFS | O(V + E) | O(V) |
| Prim's MST (matrix) | O(V²) | O(V) |
| Kruskal's MST | O(E log E) | O(V) |
| Stack push/pop/peek (array or linked) | O(1) | O(capacity / O(n)) |
| Queue enqueue/dequeue (array or linked) | O(1) | O(n) |
| Priority queue (sorted insert) | O(n) insert / O(1) delete | O(n) |
| Linked-list insert/delete at front/back | O(1) | O(n); at position O(n) |
| Infix↔postfix / evaluation | O(n) | O(n) |
| Tower of Hanoi | O(2ⁿ) | O(n) |
| BST search (average / worst) | O(log n) / O(n) | O(n) |
| AVL insert/search/delete | O(log n) | O(n) |
| B-tree / B+ search (t = 2) | O(log n) | O(n) |
| Bubble/Insertion/Selection sort | O(n²) | O(1) |
| Quick / Merge sort | O(n log n) avg | O(log n) / O(n) |
| Linear / Binary search | O(n) / O(log n) | O(1) |
| Hash insert/search (average / worst) | O(1) / O(n) | O(n) |

---

## 6. Sample Interaction

The program loads a sample 10-station network (Central, Museum, University, Market, City Hall, Airport, Terminal, Harbor, Stadium, Riverside) in zones A/B/C, then shows the simplified main menu:

```
  --- MAIN MENU ---
  1. Stations        — add / delete / search / display
  2. Routes          — find route (BFS), explore (DFS)
  3. Passenger       — queue & route history
  4. Display Network — adjacency list / matrix / sparse
  5. DS Lab          — linked lists, stacks, trees, sorting, hashing, MST
  0. Exit
```

**Option 1 → 3 — Add a station:**
```
  New Station ID   : 10
  New Station Name : Ring Road
  Zone (A/B/C)     : B
  [OK] Station "Ring Road" added.
```

**Option 2 → 1 — BFS route:**
```
  ROUTE: Central -> Riverside
  Central -> Market -> Stadium -> Riverside
  Stops   : 3
  Save this route to history? (y/n): y
  Label for history (e.g. Central->Airport): Central->Riverside
  [HISTORY] Saved.
```
The saved label is now retrievable from **Option 3 → 2 — Route History (Stack)**, demonstrating LIFO order.

**Option 5 → 3 — AVL insert & rotate:** typing a sorted sequence (e.g. 10, 20, 30) visibly triggers an RR rotation — the picture printed by `printTree()` changes from a right-leaning stick to a balanced tree.

---

## 7. Conclusion

The project shows that data structures are not isolated examples but cooperating parts of a system: the **graph** models the real problem, a **stack** reconstructs BFS paths and stores route history, a **queue** runs the boarding system, **trees** index records, and **hashing** provides constant-time lookups. Implementing every structure from scratch — including four linked-list variants, five sorts, two hash-collision strategies and four tree families — gives complete visibility into complexity trade-offs that library containers would hide.

## 8. Future Scope

- **Dijkstra's algorithm** for the shortest route by *distance*, complementing BFS's shortest by *stops*.
- **File persistence** (load/save the network and history between runs).
- A larger network comparing **adjacency list vs matrix** memory usage in practice.
- Station interchange handling and multi-line transfer routes.
