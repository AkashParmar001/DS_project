# Metro Train Route Management System
### Data Structures — Second Year Computer Engineering

---

## How to run

**Prerequisites:** Docker Desktop must be running.

```bash
# Build the image (run once, or after any code change)
docker build -t metro-ds .

# Run interactively
docker run -it --rm metro-ds
```

That's it. No compiler, no dependencies, nothing else to install.

---

## Menu

```
=============================================
   METRO ROUTE MANAGEMENT SYSTEM
=============================================
  1. Display all stations
  2. Search for a station
  3. Display station connections
  4. Find a route (BFS)
  5. Add a connection
  6. Passenger queue
  7. Sort stations alphabetically
  8. View route history
  0. Exit
```

---

## Sample outputs

### Option 1 — Display all stations
```
  [0] Central       [1] Museum        [2] University
  [3] Market        [4] City Hall      [5] Airport
  [6] Terminal      [7] Harbor         [8] Stadium
  [9] Riverside
  Total: 10 stations
```

### Option 2 — Search (linear search)
```
  Enter station name to search: airport
  Found: [5] Airport  (after 6 comparisons)
```

### Option 4 — Find route (BFS)
```
  Enter source station     : Central
  Enter destination station: Riverside

  ROUTE: Central  ->  Riverside
  -----------------------------------------
  Central  ->  Market  ->  Stadium  ->  Riverside
  -----------------------------------------
  Stops   : 3
  Distance: 12 km
```

### Option 6 — Passenger queue
```
  1. Passenger boards (enqueue)
  > Passenger name: Alice
  Boarded : Alice

  2. Train departs — next passenger (dequeue)
  Departed: Alice
```

### Option 7 — Sort (bubble sort)
```
  STATIONS — ALPHABETICAL ORDER (Bubble Sort)
  1. Airport    2. Central    3. City Hall
  4. Harbor     5. Market     6. Museum
  7. Riverside  8. Stadium    9. Terminal
  10. University
  Passes: 7   Swaps: 14
```

---

## Data structures used

| Structure | Where used |
|---|---|
| Array | Station name storage (`stationName[]`) |
| Adjacency matrix | Metro network (`graph[][]`) |
| Queue (array-based) | BFS traversal in `findRoute()` |
| Stack (array-based) | Path reconstruction in BFS; route history |
| Singly linked list | Passenger FIFO queue (`LinkedQueue`) |
| Linear search | `searchStation()` — O(n) |
| Bubble sort | `sortStations()` — O(n²) worst, O(n) best |

---

## Viva quick-reference

**Q: Why adjacency matrix and not adjacency list?**
With 10 stations the matrix is only 100 cells. It gives O(1) edge lookup which is useful for BFS. An adjacency list would save memory on a sparse graph with hundreds of stations.

**Q: Why does BFS give the fewest stops?**
BFS visits nodes level by level — all stations one hop away are visited before any two-hop stations. The first time it reaches the destination it has taken the shortest hop-count path.

**Q: What is the time complexity of BFS?**
O(V + E) — each vertex and each edge is processed at most once.

**Q: Why use a linked list for the passenger queue instead of an array?**
A linked list grows dynamically with no wasted capacity and O(1) enqueue/dequeue. An array queue needs a fixed size or a resize operation.

**Q: What is bubble sort's best case?**
O(n) — when the array is already sorted, the early-exit flag detects no swaps in the first pass and stops immediately.

**Q: What is linear search's worst case?**
O(n) — the target is the last element or not present at all.
