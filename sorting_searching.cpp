#include "sorting_searching.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

// ───────────────────────────────────────────────────────────────
static void printLine(char c = '=', int w = 52) {
    for (int i = 0; i < w; i++) cout << c;
    cout << "\n";
}

// ═══════════════════════════════════════════════════════════════
//  DATASET
// ═══════════════════════════════════════════════════════════════

void ssLoadSampleData(SSRecord arr[], int& n) {
    SSRecord data[] = {
        { 5, "Airport",    "C" },
        { 3, "Market",     "B" },
        { 8, "Stadium",    "B" },
        { 1, "Museum",     "A" },
        { 7, "Harbor",     "C" },
        { 0, "Central",    "A" },
        { 9, "Riverside",  "C" },
        { 4, "City Hall",  "A" },
        { 6, "Terminal",   "C" },
        { 2, "University", "B" }
    };
    n = 10;
    for (int i = 0; i < n; i++) arr[i] = data[i];
}

void ssPrint(const SSRecord arr[], int n, const string& label) {
    if (!label.empty()) cout << "\n  " << label << "\n";
    cout << "  +----+------------------+------+\n";
    cout << "  | ID | Name             | Zone |\n";
    cout << "  +----+------------------+------+\n";
    for (int i = 0; i < n; i++) {
        cout << "  | " << setw(2) << arr[i].id
             << " | " << setw(16) << left << arr[i].name
             << " | " << setw(4) << arr[i].zone << " |\n" << right;
    }
    cout << "  +----+------------------+------+\n";
}

// ═══════════════════════════════════════════════════════════════
//  BUBBLE SORT  —  sort by ID, ascending
//  O(n^2) average and worst case
// ═══════════════════════════════════════════════════════════════
void bubbleSort(SSRecord arr[], int n) {
    int passes = 0, swaps = 0;
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j].id > arr[j + 1].id) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
                swaps++;
            }
        }
        passes++;
        if (!swapped) break;   // early exit if already sorted
    }
    cout << "  [Bubble Sort] Passes: " << passes
         << "  Swaps: " << swaps
         << "  Complexity: O(n^2)\n";
}

// ═══════════════════════════════════════════════════════════════
//  INSERTION SORT  —  sort by ID, ascending
//  O(n^2) worst, O(n) best (nearly sorted)
// ═══════════════════════════════════════════════════════════════
void insertionSort(SSRecord arr[], int n) {
    int shifts = 0;
    for (int i = 1; i < n; i++) {
        SSRecord key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].id > key.id) {
            arr[j + 1] = arr[j];
            j--;
            shifts++;
        }
        arr[j + 1] = key;
    }
    cout << "  [Insertion Sort] Shifts: " << shifts
         << "  Complexity: O(n^2)\n";
}

// ═══════════════════════════════════════════════════════════════
//  SELECTION SORT  —  sort by ID, ascending
//  O(n^2) always
// ═══════════════════════════════════════════════════════════════
void selectionSort(SSRecord arr[], int n) {
    int comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (arr[j].id < arr[minIdx].id) minIdx = j;
        }
        if (minIdx != i) swap(arr[i], arr[minIdx]);
    }
    cout << "  [Selection Sort] Comparisons: " << comparisons
         << "  Complexity: O(n^2)\n";
}

void selectionSortByName(SSRecord arr[], int n) {
    int comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (arr[j].name < arr[minIdx].name) minIdx = j;
        }
        if (minIdx != i) swap(arr[i], arr[minIdx]);
    }
    cout << "  [Selection Sort by Name] Comparisons: " << comparisons
         << "  Complexity: O(n^2)\n";
}

// ═══════════════════════════════════════════════════════════════
//  QUICK SORT  —  sort by ID, ascending
//  O(n log n) average, O(n^2) worst
// ═══════════════════════════════════════════════════════════════
int partition(SSRecord arr[], int lo, int hi) {
    int pivot = arr[hi].id;
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (arr[j].id <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[hi]);
    return i + 1;
}

void quickSort(SSRecord arr[], int lo, int hi) {
    if (lo < hi) {
        int pi = partition(arr, lo, hi);
        quickSort(arr, lo, pi - 1);
        quickSort(arr, pi + 1, hi);
    }
}

// ═══════════════════════════════════════════════════════════════
//  MERGE SORT  —  sort by ID (and by name variant)
//  O(n log n) always
// ═══════════════════════════════════════════════════════════════
void merge(SSRecord arr[], int lo, int mid, int hi) {
    int n1 = mid - lo + 1, n2 = hi - mid;
    SSRecord L[SS_MAX], R[SS_MAX];
    for (int i = 0; i < n1; i++) L[i] = arr[lo + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = lo;
    while (i < n1 && j < n2) {
        if (L[i].id <= R[j].id) arr[k++] = L[i++];
        else                     arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(SSRecord arr[], int lo, int hi) {
    if (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        mergeSort(arr, lo, mid);
        mergeSort(arr, mid + 1, hi);
        merge(arr, lo, mid, hi);
    }
}

void mergeByName(SSRecord arr[], int lo, int mid, int hi) {
    int n1 = mid - lo + 1, n2 = hi - mid;
    SSRecord L[SS_MAX], R[SS_MAX];
    for (int i = 0; i < n1; i++) L[i] = arr[lo + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = lo;
    while (i < n1 && j < n2) {
        if (L[i].name <= R[j].name) arr[k++] = L[i++];
        else                         arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSortByName(SSRecord arr[], int lo, int hi) {
    if (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        mergeSortByName(arr, lo, mid);
        mergeSortByName(arr, mid + 1, hi);
        mergeByName(arr, lo, mid, hi);
    }
}

// ═══════════════════════════════════════════════════════════════
//  SEARCHING
// ═══════════════════════════════════════════════════════════════

// Linear search — O(n)
int linearSearchByName(const SSRecord arr[], int n, const string& name) {
    string lower = name;
    for (char& c : lower) c = (char)tolower(c);
    int steps = 0;
    for (int i = 0; i < n; i++) {
        steps++;
        string sn = arr[i].name;
        for (char& c : sn) c = (char)tolower(c);
        if (sn == lower) {
            cout << "  [Linear Search] Found at index " << i
                 << " after " << steps << " comparisons. Complexity: O(n)\n";
            return i;
        }
    }
    cout << "  [Linear Search] Not found after " << steps
         << " comparisons. Complexity: O(n)\n";
    return -1;
}

// Binary search — array must be sorted by ID — O(log n)
int binarySearchById(const SSRecord arr[], int n, int id) {
    int lo = 0, hi = n - 1, steps = 0;
    while (lo <= hi) {
        steps++;
        int mid = lo + (hi - lo) / 2;
        cout << "    [Step " << steps << "] lo=" << lo
             << " mid=" << mid << " hi=" << hi
             << " arr[mid].id=" << arr[mid].id << "\n";
        if (arr[mid].id == id) {
            cout << "  [Binary Search] Found at index " << mid
                 << " in " << steps << " steps. Complexity: O(log n)\n";
            return mid;
        }
        if (arr[mid].id < id) lo = mid + 1;
        else                   hi = mid - 1;
    }
    cout << "  [Binary Search] Not found in " << steps
         << " steps. Complexity: O(log n)\n";
    return -1;
}

// ═══════════════════════════════════════════════════════════════
//  HASH FUNCTION  — division method
// ═══════════════════════════════════════════════════════════════
int hashFunc(int key, int tableSize) {
    return key % tableSize;
}

// ═══════════════════════════════════════════════════════════════
//  LINEAR PROBING HASH TABLE
// ═══════════════════════════════════════════════════════════════
LPHashTable::LPHashTable() : count(0) {
    for (int i = 0; i < HT_SIZE; i++) {
        occupied[i] = false;
        deleted [i] = false;
    }
}

bool LPHashTable::insert(const SSRecord& rec) {
    if (count >= HT_SIZE) {
        cout << "  [LP] Table is full.\n";
        return false;
    }
    int h = hashFunc(rec.id);
    int probe = 0;
    while (probe < HT_SIZE) {
        int idx = (h + probe) % HT_SIZE;
        if (!occupied[idx] || deleted[idx]) {
            table[idx]    = rec;
            occupied[idx] = true;
            deleted[idx]  = false;
            count++;
            if (probe > 0)
                cout << "  [LP] Collision on " << rec.id
                     << " — probed " << probe << " step(s), placed at slot " << idx << "\n";
            return true;
        }
        probe++;
    }
    return false;
}

bool LPHashTable::search(int id, SSRecord& out) const {
    int h = hashFunc(id);
    for (int probe = 0; probe < HT_SIZE; probe++) {
        int idx = (h + probe) % HT_SIZE;
        if (!occupied[idx] && !deleted[idx]) return false;
        if (occupied[idx] && !deleted[idx] && table[idx].id == id) {
            out = table[idx];
            return true;
        }
    }
    return false;
}

bool LPHashTable::remove(int id) {
    int h = hashFunc(id);
    for (int probe = 0; probe < HT_SIZE; probe++) {
        int idx = (h + probe) % HT_SIZE;
        if (!occupied[idx] && !deleted[idx]) return false;
        if (occupied[idx] && !deleted[idx] && table[idx].id == id) {
            deleted[idx] = true;   // tombstone
            count--;
            return true;
        }
    }
    return false;
}

void LPHashTable::display() const {
    cout << "\n  Linear Probing Hash Table (size=" << HT_SIZE << ")\n";
    cout << "  Slot  | ID   | Name             | Status\n";
    cout << "  ------+------+------------------+---------\n";
    for (int i = 0; i < HT_SIZE; i++) {
        cout << "  " << setw(4) << i << "  | ";
        if (!occupied[i])       cout << setw(4) << "-" << "   | " << setw(16) << left << "-" << " | empty\n";
        else if (deleted[i])    cout << setw(4) << "-" << "   | " << setw(16) << left << "-" << " | deleted\n";
        else                    cout << setw(4) << table[i].id << "   | "
                                     << setw(16) << left << table[i].name << " | occupied\n";
        cout << right;
    }
    cout << "  Load factor: " << count << "/" << HT_SIZE << " = "
         << fixed << setprecision(2) << (double)count / HT_SIZE << "\n";
}

void LPHashTable::showStats() const {
    int collisions = 0;
    for (int i = 0; i < HT_SIZE; i++) {
        if (!occupied[i] || deleted[i]) continue;
        int h = hashFunc(table[i].id);
        if (h != i) collisions++;
    }
    cout << "  Entries     : " << count << "\n";
    cout << "  Collisions  : " << collisions << "\n";
    cout << "  Load factor : " << fixed << setprecision(2) << (double)count / HT_SIZE << "\n";
}

// ═══════════════════════════════════════════════════════════════
//  SEPARATE CHAINING HASH TABLE
// ═══════════════════════════════════════════════════════════════
SCHashTable::SCHashTable() : count(0) {
    for (int i = 0; i < HT_SIZE; i++) buckets[i] = nullptr;
}

SCHashTable::~SCHashTable() { clear(); }

void SCHashTable::clear() {
    for (int i = 0; i < HT_SIZE; i++) {
        SCNode* cur = buckets[i];
        while (cur) { SCNode* tmp = cur->next; delete cur; cur = tmp; }
        buckets[i] = nullptr;
    }
    count = 0;
}

bool SCHashTable::insert(const SSRecord& rec) {
    int h = hashFunc(rec.id);
    // check duplicate
    SCNode* cur = buckets[h];
    while (cur) { if (cur->data.id == rec.id) { cout << "  [SC] Duplicate ID " << rec.id << "\n"; return false; } cur = cur->next; }
    SCNode* n = new SCNode{ rec, buckets[h] };
    buckets[h] = n;
    count++;
    if (n->next != nullptr)
        cout << "  [SC] Collision on ID " << rec.id
             << " — chained at slot " << h << "\n";
    return true;
}

bool SCHashTable::search(int id, SSRecord& out) const {
    int h = hashFunc(id);
    SCNode* cur = buckets[h];
    while (cur) {
        if (cur->data.id == id) { out = cur->data; return true; }
        cur = cur->next;
    }
    return false;
}

bool SCHashTable::remove(int id) {
    int h = hashFunc(id);
    SCNode* prev = nullptr, *cur = buckets[h];
    while (cur) {
        if (cur->data.id == id) {
            if (prev) prev->next = cur->next;
            else       buckets[h] = cur->next;
            delete cur; count--;
            return true;
        }
        prev = cur; cur = cur->next;
    }
    return false;
}

void SCHashTable::display() const {
    cout << "\n  Separate Chaining Hash Table (size=" << HT_SIZE << ")\n";
    for (int i = 0; i < HT_SIZE; i++) {
        cout << "  [" << setw(2) << i << "] -> ";
        SCNode* cur = buckets[i];
        if (!cur) { cout << "empty\n"; continue; }
        while (cur) {
            cout << "{" << cur->data.id << ":" << cur->data.name << "}";
            if (cur->next) cout << " -> ";
            cur = cur->next;
        }
        cout << "\n";
    }
    cout << "  Entries: " << count << "\n";
}

void SCHashTable::showStats() const {
    int maxChain = 0, chains = 0, total = 0;
    for (int i = 0; i < HT_SIZE; i++) {
        int len = 0;
        for (SCNode* c = buckets[i]; c; c = c->next) len++;
        if (len > 0) chains++;
        if (len > maxChain) maxChain = len;
        total += len;
    }
    cout << "  Entries       : " << count << "\n";
    cout << "  Non-empty slots: " << chains << "\n";
    cout << "  Max chain len : " << maxChain << "\n";
    cout << "  Load factor   : " << fixed << setprecision(2) << (double)count / HT_SIZE << "\n";
}

// ═══════════════════════════════════════════════════════════════
//  SYMBOL TABLE DEMO  (chaining-based, station ID → record)
// ═══════════════════════════════════════════════════════════════
void symbolTableDemo() {
    SCHashTable st;
    SSRecord data[SS_MAX]; int n = 0;
    ssLoadSampleData(data, n);

    printLine();
    cout << "  SYMBOL TABLE  (Station ID -> Record)\n";
    printLine();
    cout << "  Inserting all 10 station records...\n";
    for (int i = 0; i < n; i++) st.insert(data[i]);
    st.display();
    st.showStats();

    cout << "\n  Lookup station ID 5: ";
    SSRecord r;
    if (st.search(5, r)) cout << r.name << " (Zone " << r.zone << ")\n";

    cout << "  Lookup station ID 99: ";
    if (!st.search(99, r)) cout << "Not found.\n";

    cout << "\nPress Enter to continue...";
    cin.ignore(); cin.get();
}

// ═══════════════════════════════════════════════════════════════
//  SORTING MENU
// ═══════════════════════════════════════════════════════════════
void sortingMenu() {
    int choice;
    do {
        printLine();
        cout << "  SORTING ALGORITHMS\n";
        printLine();
        cout << " 1. Bubble Sort    (by ID)\n";
        cout << " 2. Insertion Sort (by ID)\n";
        cout << " 3. Selection Sort (by ID)\n";
        cout << " 4. Selection Sort (by Name — alphabetical)\n";
        cout << " 5. Quick Sort     (by ID)\n";
        cout << " 6. Merge Sort     (by ID)\n";
        cout << " 7. Merge Sort     (by Name)\n";
        cout << " 8. Compare all algorithms on same dataset\n";
        cout << " 0. Back\n";
        printLine();
        cout << "Choice: ";
        cin >> choice;

        SSRecord arr[SS_MAX]; int n = 0;

        switch (choice) {
        case 1:
            ssLoadSampleData(arr, n);
            ssPrint(arr, n, "Before (unsorted):");
            bubbleSort(arr, n);
            ssPrint(arr, n, "After Bubble Sort:");
            break;
        case 2:
            ssLoadSampleData(arr, n);
            ssPrint(arr, n, "Before:");
            insertionSort(arr, n);
            ssPrint(arr, n, "After Insertion Sort:");
            break;
        case 3:
            ssLoadSampleData(arr, n);
            ssPrint(arr, n, "Before:");
            selectionSort(arr, n);
            ssPrint(arr, n, "After Selection Sort:");
            break;
        case 4:
            ssLoadSampleData(arr, n);
            ssPrint(arr, n, "Before:");
            selectionSortByName(arr, n);
            ssPrint(arr, n, "After Selection Sort by Name:");
            break;
        case 5:
            ssLoadSampleData(arr, n);
            ssPrint(arr, n, "Before:");
            quickSort(arr, 0, n - 1);
            cout << "  [Quick Sort] Complexity: O(n log n) average\n";
            ssPrint(arr, n, "After Quick Sort:");
            break;
        case 6:
            ssLoadSampleData(arr, n);
            ssPrint(arr, n, "Before:");
            mergeSort(arr, 0, n - 1);
            cout << "  [Merge Sort] Complexity: O(n log n)\n";
            ssPrint(arr, n, "After Merge Sort by ID:");
            break;
        case 7:
            ssLoadSampleData(arr, n);
            ssPrint(arr, n, "Before:");
            mergeSortByName(arr, 0, n - 1);
            cout << "  [Merge Sort by Name] Complexity: O(n log n)\n";
            ssPrint(arr, n, "After Merge Sort by Name:");
            break;
        case 8: {
            // Run all 5 on separate copies of same data
            SSRecord a1[SS_MAX], a2[SS_MAX], a3[SS_MAX], a4[SS_MAX], a5[SS_MAX];
            int n1, n2, n3, n4, n5;
            ssLoadSampleData(a1, n1); ssLoadSampleData(a2, n2);
            ssLoadSampleData(a3, n3); ssLoadSampleData(a4, n4);
            ssLoadSampleData(a5, n5);
            printLine('-');
            cout << "  Same 10-station dataset, sorted by ID:\n";
            printLine('-');
            bubbleSort(a1, n1);
            insertionSort(a2, n2);
            selectionSort(a3, n3);
            quickSort(a4, 0, n4-1); cout << "  [Quick Sort]  Complexity: O(n log n) average\n";
            mergeSort(a5, 0, n5-1); cout << "  [Merge Sort]  Complexity: O(n log n)\n";
            printLine('-');
            cout << "  All produce the same sorted output.\n";
            ssPrint(a5, n5, "Sorted result:");
            break;
        }
        case 0: break;
        default: cout << "[ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

// ═══════════════════════════════════════════════════════════════
//  SEARCHING MENU
// ═══════════════════════════════════════════════════════════════
void searchingMenu() {
    SSRecord arr[SS_MAX]; int n = 0;
    ssLoadSampleData(arr, n);

    int choice;
    do {
        printLine();
        cout << "  SEARCHING ALGORITHMS\n";
        printLine();
        cout << " 1. Linear Search  (by name, unsorted array)\n";
        cout << " 2. Binary Search  (by ID, sorts array first)\n";
        cout << " 0. Back\n";
        printLine();
        cout << "Choice: ";
        cin >> choice;

        string name; int id;

        switch (choice) {
        case 1:
            ssPrint(arr, n, "Current array (unsorted):");
            cout << "\n  Enter station name to search: ";
            cin.ignore(); getline(cin, name);
            {
                int idx = linearSearchByName(arr, n, name);
                if (idx != -1)
                    cout << "  Found: ID=" << arr[idx].id
                         << " Name=" << arr[idx].name
                         << " Zone=" << arr[idx].zone << "\n";
                else
                    cout << "  \"" << name << "\" not found.\n";
            }
            break;
        case 2: {
            // Sort by ID first
            SSRecord sorted[SS_MAX]; int sn = n;
            for (int i = 0; i < n; i++) sorted[i] = arr[i];
            mergeSort(sorted, 0, sn - 1);
            ssPrint(sorted, sn, "Sorted by ID (binary search requires sorted input):");
            cout << "\n  Enter station ID to search: ";
            cin >> id;
            int idx = binarySearchById(sorted, sn, id);
            if (idx != -1)
                cout << "  Found: ID=" << sorted[idx].id
                     << " Name=" << sorted[idx].name
                     << " Zone=" << sorted[idx].zone << "\n";
            else
                cout << "  ID " << id << " not found.\n";
            break;
        }
        case 0: break;
        default: cout << "[ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

// ═══════════════════════════════════════════════════════════════
//  HASHING MENU
// ═══════════════════════════════════════════════════════════════
void hashingMenu() {
    LPHashTable lp;
    SCHashTable sc;

    SSRecord data[SS_MAX]; int n = 0;
    ssLoadSampleData(data, n);

    // pre-load both tables
    cout << "\n  [INFO] Pre-loading 10 records into both tables...\n";
    for (int i = 0; i < n; i++) { lp.insert(data[i]); sc.insert(data[i]); }

    int choice;
    do {
        printLine();
        cout << "  HASH TABLE OPERATIONS\n";
        printLine();
        cout << "  Hash function: h(key) = key % " << HT_SIZE << " (division method)\n";
        printLine('-');
        cout << " 1. Display Linear Probing table\n";
        cout << " 2. Display Separate Chaining table\n";
        cout << " 3. Insert into both tables\n";
        cout << " 4. Search in both tables\n";
        cout << " 5. Delete from both tables\n";
        cout << " 6. Show statistics (load factor, collisions)\n";
        cout << " 7. Symbol Table Demo (station ID -> record)\n";
        cout << " 8. Show hash function values for all station IDs\n";
        cout << " 0. Back\n";
        printLine();
        cout << "Choice: ";
        cin >> choice;

        int id; string name, zone;
        SSRecord r;

        switch (choice) {
        case 1: lp.display(); break;
        case 2: sc.display(); break;
        case 3:
            cout << "ID: ";   cin >> id;
            cout << "Name: "; cin.ignore(); getline(cin, name);
            cout << "Zone: "; getline(cin, zone);
            {
                SSRecord rec = { id, name, zone };
                cout << "\n  Linear Probing:\n";  lp.insert(rec);
                cout << "  Separate Chaining:\n"; sc.insert(rec);
            }
            break;
        case 4:
            cout << "Search ID: "; cin >> id;
            cout << "\n  Linear Probing: ";
            if (lp.search(id, r)) cout << "Found — " << r.name << " (Zone " << r.zone << ")\n";
            else cout << "Not found.\n";
            cout << "  Separate Chaining: ";
            if (sc.search(id, r)) cout << "Found — " << r.name << " (Zone " << r.zone << ")\n";
            else cout << "Not found.\n";
            break;
        case 5:
            cout << "Delete ID: "; cin >> id;
            cout << "\n  Linear Probing: "  << (lp.remove(id) ? "Deleted.\n" : "Not found.\n");
            cout << "  Separate Chaining: " << (sc.remove(id) ? "Deleted.\n" : "Not found.\n");
            break;
        case 6:
            cout << "\n  --- Linear Probing Stats ---\n";   lp.showStats();
            cout << "\n  --- Separate Chaining Stats ---\n"; sc.showStats();
            cout << "\n  Complexity: Insert/Search/Delete O(1) average, O(n) worst\n";
            break;
        case 7: symbolTableDemo(); break;
        case 8:
            cout << "\n  Hash values (key % " << HT_SIZE << "):\n";
            cout << "  ID  -> Slot\n";
            cout << "  ----------\n";
            for (int i = 0; i < 10; i++)
                cout << "  " << setw(2) << i << "  ->  " << hashFunc(i) << "\n";
            break;
        case 0: break;
        default: cout << "[ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

// ═══════════════════════════════════════════════════════════════
//  TOP-LEVEL SORTING & SEARCHING MENU
// ═══════════════════════════════════════════════════════════════
void sortingSearchingMenu() {
    int choice;
    do {
        printLine();
        cout << "  SORTING, SEARCHING & HASHING\n";
        printLine();
        cout << " 1. Sorting Algorithms\n";
        cout << " 2. Searching Algorithms\n";
        cout << " 3. Hash Table Operations\n";
        cout << " 0. Back\n";
        printLine();
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: sortingMenu();   break;
        case 2: searchingMenu(); break;
        case 3: hashingMenu();   break;
        case 0: break;
        default: cout << "[ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}
