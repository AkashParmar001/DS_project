#ifndef SORTING_SEARCHING_H
#define SORTING_SEARCHING_H

#include <string>

// ─────────────────────────────────────────────────────────────────
//  Station record (lightweight copy for sorting demos)
// ─────────────────────────────────────────────────────────────────
static const int SS_MAX = 20;   // max records in demo dataset

struct SSRecord {
    int         id;
    std::string name;
    std::string zone;
};

// ─────────────────────────────────────────────────────────────────
//  Dataset helpers
// ─────────────────────────────────────────────────────────────────
void ssLoadSampleData(SSRecord arr[], int& n);
void ssPrint(const SSRecord arr[], int n, const std::string& label = "");

// ═══════════════════════════════════════════════════════════════
//  Unit 6 — SORTING  (all operate on SSRecord arrays)
// ═══════════════════════════════════════════════════════════════

// Sorts by ID (ascending)
void bubbleSort   (SSRecord arr[], int n);
void insertionSort(SSRecord arr[], int n);
void selectionSort(SSRecord arr[], int n);   // also has name variant
void quickSort    (SSRecord arr[], int lo, int hi);
void mergeSort    (SSRecord arr[], int lo, int hi);

// Sort by name (alphabetical) — selection sort variant
void selectionSortByName(SSRecord arr[], int n);

// Sort by name — merge sort variant
void mergeSortByName(SSRecord arr[], int lo, int hi);

// Internal helpers for quick/merge
int  partition   (SSRecord arr[], int lo, int hi);
void merge       (SSRecord arr[], int lo, int mid, int hi);
void mergeByName (SSRecord arr[], int lo, int mid, int hi);

// ═══════════════════════════════════════════════════════════════
//  Unit 6 — SEARCHING
// ═══════════════════════════════════════════════════════════════

// Linear search (unsorted) — by name, returns index or -1
int linearSearchByName(const SSRecord arr[], int n, const std::string& name);

// Binary search (sorted by ID) — returns index or -1
int binarySearchById(const SSRecord arr[], int n, int id);

// ═══════════════════════════════════════════════════════════════
//  Unit 6 — HASHING
// ═══════════════════════════════════════════════════════════════
static const int HT_SIZE = 13;    // prime table size

// ── Hash function ─────────────────────────────────────────────
int hashFunc(int key, int tableSize = HT_SIZE);

// ── Linear-probing hash table ─────────────────────────────────
struct LPHashTable {
    SSRecord table[HT_SIZE];
    bool     occupied[HT_SIZE];
    bool     deleted [HT_SIZE];   // tombstone for deletions
    int      count;

    LPHashTable();
    bool insert(const SSRecord& rec);
    bool search(int id, SSRecord& out) const;
    bool remove(int id);
    void display() const;
    void showStats() const;
};

// ── Separate-chaining hash table ─────────────────────────────
struct SCNode {
    SSRecord data;
    SCNode*  next;
};

struct SCHashTable {
    SCNode* buckets[HT_SIZE];
    int     count;

    SCHashTable();
    ~SCHashTable();

    bool insert(const SSRecord& rec);
    bool search(int id, SSRecord& out) const;
    bool remove(int id);
    void display() const;
    void showStats() const;

private:
    void clear();
};

// ── Symbol table (ID → record map, chaining-based) ───────────
// Reuses SCHashTable with a descriptive wrapper menu
void symbolTableDemo();

// ═══════════════════════════════════════════════════════════════
//  Menu entry points
// ═══════════════════════════════════════════════════════════════
void sortingMenu();
void searchingMenu();
void hashingMenu();
void sortingSearchingMenu();   // top-level

#endif // SORTING_SEARCHING_H
