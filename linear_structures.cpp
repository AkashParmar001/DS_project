#include "linear_structures.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// ═══════════════════════════════════════════════════════════════
//  Utility
// ═══════════════════════════════════════════════════════════════
static void printLine(char c = '=', int w = 50) {
    for (int i = 0; i < w; i++) cout << c;
    cout << "\n";
}

// ═══════════════════════════════════════════════════════════════
//  ARRAY STACK
// ═══════════════════════════════════════════════════════════════
ArrayStack::ArrayStack() : top(-1) {}

bool ArrayStack::isEmpty() const { return top == -1; }
bool ArrayStack::isFull()  const { return top == STACK_CAPACITY - 1; }

bool ArrayStack::push(const string& val) {
    if (isFull()) { cout << "[STACK] Stack is full.\n"; return false; }
    data[++top] = val;
    return true;
}

bool ArrayStack::pop(string& val) {
    if (isEmpty()) { cout << "[STACK] Stack is empty.\n"; return false; }
    val = data[top--];
    return true;
}

bool ArrayStack::peek(string& val) const {
    if (isEmpty()) { cout << "[STACK] Stack is empty.\n"; return false; }
    val = data[top];
    return true;
}

void ArrayStack::display() const {
    if (isEmpty()) { cout << "  (stack is empty)\n"; return; }
    cout << "  TOP\n";
    for (int i = top; i >= 0; i--)
        cout << "  [" << i << "] " << data[i] << "\n";
    cout << "  BOTTOM\n";
}

void ArrayStack::reverseDisplay() const {
    if (isEmpty()) { cout << "  (nothing to reverse)\n"; return; }
    cout << "  Reversed order: ";
    for (int i = 0; i <= top; i++) {
        if (i > 0) cout << " -> ";
        cout << data[i];
    }
    cout << "\n";
}

// ═══════════════════════════════════════════════════════════════
//  SIMPLE QUEUE
// ═══════════════════════════════════════════════════════════════
SimpleQueue::SimpleQueue() : front(0), rear(-1), size(0) {}

bool SimpleQueue::isEmpty() const { return size == 0; }
bool SimpleQueue::isFull()  const { return size == QUEUE_CAPACITY; }

bool SimpleQueue::enqueue(const string& val) {
    if (isFull()) { cout << "[QUEUE] Queue is full.\n"; return false; }
    rear = (rear + 1) % QUEUE_CAPACITY;
    data[rear] = val;
    size++;
    return true;
}

bool SimpleQueue::dequeue(string& val) {
    if (isEmpty()) { cout << "[QUEUE] Queue is empty.\n"; return false; }
    val = data[front];
    front = (front + 1) % QUEUE_CAPACITY;
    size--;
    return true;
}

void SimpleQueue::display() const {
    if (isEmpty()) { cout << "  (queue is empty)\n"; return; }
    cout << "  FRONT -> ";
    for (int i = 0, idx = front; i < size; i++, idx = (idx + 1) % QUEUE_CAPACITY) {
        cout << "[" << data[idx] << "]";
        if (i < size - 1) cout << " -> ";
    }
    cout << " <- REAR\n";
}

// ═══════════════════════════════════════════════════════════════
//  CIRCULAR QUEUE
// ═══════════════════════════════════════════════════════════════
CircularQueue::CircularQueue() : front(0), rear(-1), size(0) {}

bool CircularQueue::isEmpty() const { return size == 0; }
bool CircularQueue::isFull()  const { return size == QUEUE_CAPACITY; }

bool CircularQueue::enqueue(const string& val) {
    if (isFull()) { cout << "[CQUEUE] Circular queue is full.\n"; return false; }
    rear = (rear + 1) % QUEUE_CAPACITY;
    data[rear] = val;
    size++;
    return true;
}

bool CircularQueue::dequeue(string& val) {
    if (isEmpty()) { cout << "[CQUEUE] Circular queue is empty.\n"; return false; }
    val = data[front];
    front = (front + 1) % QUEUE_CAPACITY;
    size--;
    return true;
}

void CircularQueue::display() const {
    if (isEmpty()) { cout << "  (circular queue is empty)\n"; return; }
    cout << "  [Circular Queue | Capacity: " << QUEUE_CAPACITY
         << " | Used: " << size << "]\n  ";
    for (int i = 0, idx = front; i < size; i++, idx = (idx + 1) % QUEUE_CAPACITY) {
        cout << "[" << data[idx] << "]";
        if (i < size - 1) cout << " -> ";
    }
    cout << " (wraps around)\n";
}

// ═══════════════════════════════════════════════════════════════
//  PRIORITY QUEUE
// ═══════════════════════════════════════════════════════════════
PriorityQueue::PriorityQueue() : size(0) {}

bool PriorityQueue::isEmpty() const { return size == 0; }
bool PriorityQueue::isFull()  const { return size == QUEUE_CAPACITY; }

bool PriorityQueue::enqueue(const string& passenger, int priority) {
    if (isFull()) { cout << "[PQ] Priority queue is full.\n"; return false; }
    data[size++] = { passenger, priority };
    return true;
}

bool PriorityQueue::dequeue(PQEntry& out) {
    if (isEmpty()) { cout << "[PQ] Priority queue is empty.\n"; return false; }
    int maxIdx = 0;
    for (int i = 1; i < size; i++)
        if (data[i].priority > data[maxIdx].priority) maxIdx = i;
    out = data[maxIdx];
    // shift left
    for (int i = maxIdx; i < size - 1; i++) data[i] = data[i + 1];
    size--;
    return true;
}

void PriorityQueue::display() const {
    if (isEmpty()) { cout << "  (priority queue is empty)\n"; return; }
    const char* labels[] = { "", "Normal", "Senior/Disabled", "Emergency" };
    cout << "  Passenger              Priority\n";
    cout << "  ----------------------------------\n";
    for (int i = 0; i < size; i++) {
        int p = data[i].priority;
        string label = (p >= 1 && p <= 3) ? labels[p] : "Unknown";
        cout << "  " << setw(22) << left << data[i].passenger
             << " " << p << " (" << label << ")\n";
    }
    cout << right;
}

// ═══════════════════════════════════════════════════════════════
//  DEQUE
// ═══════════════════════════════════════════════════════════════
Deque::Deque() : front(DEQUE_CAPACITY / 2), rear(DEQUE_CAPACITY / 2 - 1), size(0) {}

bool Deque::isEmpty() const { return size == 0; }
bool Deque::isFull()  const { return size == DEQUE_CAPACITY; }

bool Deque::pushFront(const string& val) {
    if (isFull()) { cout << "[DEQUE] Deque is full.\n"; return false; }
    front = (front - 1 + DEQUE_CAPACITY) % DEQUE_CAPACITY;
    data[front] = val;
    size++;
    return true;
}

bool Deque::pushBack(const string& val) {
    if (isFull()) { cout << "[DEQUE] Deque is full.\n"; return false; }
    rear = (rear + 1) % DEQUE_CAPACITY;
    data[rear] = val;
    size++;
    return true;
}

bool Deque::popFront(string& val) {
    if (isEmpty()) { cout << "[DEQUE] Deque is empty.\n"; return false; }
    val = data[front];
    front = (front + 1) % DEQUE_CAPACITY;
    size--;
    return true;
}

bool Deque::popBack(string& val) {
    if (isEmpty()) { cout << "[DEQUE] Deque is empty.\n"; return false; }
    val = data[rear];
    rear = (rear - 1 + DEQUE_CAPACITY) % DEQUE_CAPACITY;
    size--;
    return true;
}

void Deque::display() const {
    if (isEmpty()) { cout << "  (deque is empty)\n"; return; }
    cout << "  FRONT <-> ";
    for (int i = 0, idx = front; i < size; i++, idx = (idx + 1) % DEQUE_CAPACITY) {
        cout << "[" << data[idx] << "]";
        if (i < size - 1) cout << " <-> ";
    }
    cout << " <-> REAR\n";
}

// ═══════════════════════════════════════════════════════════════
//  SINGLY LINKED LIST
// ═══════════════════════════════════════════════════════════════
SinglyList::SinglyList() : head(nullptr), size(0) {}

SinglyList::~SinglyList() { clear(); }

void SinglyList::clear() {
    SLLNode* cur = head;
    while (cur) { SLLNode* tmp = cur->next; delete cur; cur = tmp; }
    head = nullptr; size = 0;
}

void SinglyList::insertFront(const string& val) {
    SLLNode* n = new SLLNode{ val, head };
    head = n; size++;
}

void SinglyList::insertBack(const string& val) {
    SLLNode* n = new SLLNode{ val, nullptr };
    if (!head) { head = n; }
    else {
        SLLNode* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = n;
    }
    size++;
}

void SinglyList::insertAt(int pos, const string& val) {
    if (pos <= 0) { insertFront(val); return; }
    if (pos >= size) { insertBack(val); return; }
    SLLNode* cur = head;
    for (int i = 0; i < pos - 1; i++) cur = cur->next;
    SLLNode* n = new SLLNode{ val, cur->next };
    cur->next = n;
    size++;
}

bool SinglyList::deleteFront(string& val) {
    if (!head) { cout << "[SLL] List is empty.\n"; return false; }
    val = head->data;
    SLLNode* tmp = head;
    head = head->next;
    delete tmp; size--;
    return true;
}

bool SinglyList::deleteBack(string& val) {
    if (!head) { cout << "[SLL] List is empty.\n"; return false; }
    if (!head->next) { val = head->data; delete head; head = nullptr; size--; return true; }
    SLLNode* cur = head;
    while (cur->next->next) cur = cur->next;
    val = cur->next->data;
    delete cur->next;
    cur->next = nullptr; size--;
    return true;
}

bool SinglyList::deleteAt(int pos, string& val) {
    if (!head) { cout << "[SLL] List is empty.\n"; return false; }
    if (pos <= 0) return deleteFront(val);
    if (pos >= size - 1) return deleteBack(val);
    SLLNode* cur = head;
    for (int i = 0; i < pos - 1; i++) cur = cur->next;
    SLLNode* target = cur->next;
    val = target->data;
    cur->next = target->next;
    delete target; size--;
    return true;
}

int SinglyList::search(const string& val) const {
    SLLNode* cur = head; int pos = 0;
    while (cur) {
        if (cur->data == val) return pos;
        cur = cur->next; pos++;
    }
    return -1;
}

void SinglyList::display() const {
    if (!head) { cout << "  (empty)\n"; return; }
    cout << "  HEAD -> ";
    SLLNode* cur = head;
    while (cur) {
        cout << "[" << cur->data << "]";
        if (cur->next) cout << " -> ";
        cur = cur->next;
    }
    cout << " -> NULL  (size=" << size << ")\n";
}

void SinglyList::reverse() {
    SLLNode* prev = nullptr, *cur = head, *nxt = nullptr;
    while (cur) { nxt = cur->next; cur->next = prev; prev = cur; cur = nxt; }
    head = prev;
}

// ═══════════════════════════════════════════════════════════════
//  SINGLY CIRCULAR LINKED LIST
// ═══════════════════════════════════════════════════════════════
CircularSLL::CircularSLL() : tail(nullptr), size(0) {}

CircularSLL::~CircularSLL() { clear(); }

void CircularSLL::clear() {
    if (!tail) return;
    SLLNode* head = tail->next;
    SLLNode* cur = head;
    tail->next = nullptr;  // break the circle
    while (cur) { SLLNode* tmp = cur->next; delete cur; cur = tmp; }
    tail = nullptr; size = 0;
}

void CircularSLL::insertFront(const string& val) {
    SLLNode* n = new SLLNode{ val, nullptr };
    if (!tail) { n->next = n; tail = n; }
    else { n->next = tail->next; tail->next = n; }
    size++;
}

void CircularSLL::insertBack(const string& val) {
    SLLNode* n = new SLLNode{ val, nullptr };
    if (!tail) { n->next = n; tail = n; }
    else { n->next = tail->next; tail->next = n; tail = n; }
    size++;
}

bool CircularSLL::deleteFront(string& val) {
    if (!tail) { cout << "[CSLL] List is empty.\n"; return false; }
    SLLNode* head = tail->next;
    val = head->data;
    if (head == tail) { delete head; tail = nullptr; }
    else { tail->next = head->next; delete head; }
    size--;
    return true;
}

void CircularSLL::display() const {
    if (!tail) { cout << "  (empty)\n"; return; }
    SLLNode* cur = tail->next;  // head
    cout << "  HEAD -> ";
    for (int i = 0; i < size; i++) {
        cout << "[" << cur->data << "]";
        if (i < size - 1) cout << " -> ";
        cur = cur->next;
    }
    cout << " -> (back to HEAD)  circular\n";
}

void CircularSLL::traverse(int steps) const {
    if (!tail) { cout << "  (empty)\n"; return; }
    SLLNode* cur = tail->next;
    cout << "  Circular traverse (" << steps << " steps): ";
    for (int i = 0; i < steps; i++) {
        cout << "[" << cur->data << "]";
        if (i < steps - 1) cout << " -> ";
        cur = cur->next;
    }
    cout << "\n";
}

// ═══════════════════════════════════════════════════════════════
//  DOUBLY LINKED LIST
// ═══════════════════════════════════════════════════════════════
DoublyList::DoublyList() : head(nullptr), tail(nullptr), size(0) {}

DoublyList::~DoublyList() { clear(); }

void DoublyList::clear() {
    DLLNode* cur = head;
    while (cur) { DLLNode* tmp = cur->next; delete cur; cur = tmp; }
    head = tail = nullptr; size = 0;
}

void DoublyList::insertFront(const string& val) {
    DLLNode* n = new DLLNode{ val, nullptr, head };
    if (head) head->prev = n;
    head = n;
    if (!tail) tail = n;
    size++;
}

void DoublyList::insertBack(const string& val) {
    DLLNode* n = new DLLNode{ val, tail, nullptr };
    if (tail) tail->next = n;
    tail = n;
    if (!head) head = n;
    size++;
}

void DoublyList::insertAt(int pos, const string& val) {
    if (pos <= 0) { insertFront(val); return; }
    if (pos >= size) { insertBack(val); return; }
    DLLNode* cur = head;
    for (int i = 0; i < pos - 1; i++) cur = cur->next;
    DLLNode* n = new DLLNode{ val, cur, cur->next };
    if (cur->next) cur->next->prev = n;
    cur->next = n;
    size++;
}

bool DoublyList::deleteFront(string& val) {
    if (!head) { cout << "[DLL] List is empty.\n"; return false; }
    val = head->data;
    DLLNode* tmp = head;
    head = head->next;
    if (head) head->prev = nullptr; else tail = nullptr;
    delete tmp; size--;
    return true;
}

bool DoublyList::deleteBack(string& val) {
    if (!tail) { cout << "[DLL] List is empty.\n"; return false; }
    val = tail->data;
    DLLNode* tmp = tail;
    tail = tail->prev;
    if (tail) tail->next = nullptr; else head = nullptr;
    delete tmp; size--;
    return true;
}

bool DoublyList::deleteAt(int pos, string& val) {
    if (!head) { cout << "[DLL] List is empty.\n"; return false; }
    if (pos <= 0) return deleteFront(val);
    if (pos >= size - 1) return deleteBack(val);
    DLLNode* cur = head;
    for (int i = 0; i < pos; i++) cur = cur->next;
    val = cur->data;
    if (cur->prev) cur->prev->next = cur->next;
    if (cur->next) cur->next->prev = cur->prev;
    delete cur; size--;
    return true;
}

void DoublyList::displayForward() const {
    if (!head) { cout << "  (empty)\n"; return; }
    cout << "  NULL <-> ";
    DLLNode* cur = head;
    while (cur) {
        cout << "[" << cur->data << "]";
        if (cur->next) cout << " <-> ";
        cur = cur->next;
    }
    cout << " <-> NULL  (forward)\n";
}

void DoublyList::displayBackward() const {
    if (!tail) { cout << "  (empty)\n"; return; }
    cout << "  NULL <-> ";
    DLLNode* cur = tail;
    while (cur) {
        cout << "[" << cur->data << "]";
        if (cur->prev) cout << " <-> ";
        cur = cur->prev;
    }
    cout << " <-> NULL  (backward)\n";
}

// ═══════════════════════════════════════════════════════════════
//  DOUBLY CIRCULAR LINKED LIST
// ═══════════════════════════════════════════════════════════════
CircularDLL::CircularDLL() : head(nullptr), size(0) {}

CircularDLL::~CircularDLL() { clear(); }

void CircularDLL::clear() {
    if (!head) return;
    DLLNode* cur = head->next;
    while (cur != head) { DLLNode* tmp = cur->next; delete cur; cur = tmp; }
    delete head; head = nullptr; size = 0;
}

void CircularDLL::insertFront(const string& val) {
    DLLNode* n = new DLLNode{ val, nullptr, nullptr };
    if (!head) { n->next = n; n->prev = n; head = n; }
    else {
        DLLNode* tail = head->prev;
        n->next = head; n->prev = tail;
        head->prev = n; tail->next = n;
        head = n;
    }
    size++;
}

void CircularDLL::insertBack(const string& val) {
    DLLNode* n = new DLLNode{ val, nullptr, nullptr };
    if (!head) { n->next = n; n->prev = n; head = n; }
    else {
        DLLNode* tail = head->prev;
        n->next = head; n->prev = tail;
        tail->next = n; head->prev = n;
    }
    size++;
}

bool CircularDLL::deleteFront(string& val) {
    if (!head) { cout << "[CDLL] List is empty.\n"; return false; }
    val = head->data;
    if (head->next == head) { delete head; head = nullptr; }
    else {
        DLLNode* tail = head->prev;
        DLLNode* tmp  = head;
        head = head->next;
        head->prev = tail;
        tail->next = head;
        delete tmp;
    }
    size--;
    return true;
}

bool CircularDLL::deleteBack(string& val) {
    if (!head) { cout << "[CDLL] List is empty.\n"; return false; }
    DLLNode* tail = head->prev;
    val = tail->data;
    if (tail == head) { delete head; head = nullptr; }
    else {
        DLLNode* newTail = tail->prev;
        newTail->next = head;
        head->prev = newTail;
        delete tail;
    }
    size--;
    return true;
}

void CircularDLL::displayForward() const {
    if (!head) { cout << "  (empty)\n"; return; }
    DLLNode* cur = head;
    cout << "  (circular) -> ";
    for (int i = 0; i < size; i++) {
        cout << "[" << cur->data << "]";
        if (i < size - 1) cout << " <-> ";
        cur = cur->next;
    }
    cout << " -> (back to HEAD)\n";
}

void CircularDLL::displayBackward() const {
    if (!head) { cout << "  (empty)\n"; return; }
    DLLNode* tail = head->prev;
    DLLNode* cur  = tail;
    cout << "  (circular) <- ";
    for (int i = 0; i < size; i++) {
        cout << "[" << cur->data << "]";
        if (i < size - 1) cout << " <-> ";
        cur = cur->prev;
    }
    cout << " <- (back to TAIL)\n";
}

// ═══════════════════════════════════════════════════════════════
//  LINKED STACK
// ═══════════════════════════════════════════════════════════════
LinkedStack::LinkedStack() : top(nullptr), size(0) {}

LinkedStack::~LinkedStack() {
    string tmp;
    while (!isEmpty()) pop(tmp);
}

bool LinkedStack::isEmpty() const { return top == nullptr; }

void LinkedStack::push(const string& val) {
    SLLNode* n = new SLLNode{ val, top };
    top = n; size++;
}

bool LinkedStack::pop(string& val) {
    if (isEmpty()) { cout << "[LSTACK] Stack is empty.\n"; return false; }
    val = top->data;
    SLLNode* tmp = top;
    top = top->next;
    delete tmp; size--;
    return true;
}

bool LinkedStack::peek(string& val) const {
    if (isEmpty()) { cout << "[LSTACK] Stack is empty.\n"; return false; }
    val = top->data; return true;
}

void LinkedStack::display() const {
    if (isEmpty()) { cout << "  (stack empty)\n"; return; }
    cout << "  TOP -> ";
    SLLNode* cur = top;
    while (cur) {
        cout << "[" << cur->data << "]";
        if (cur->next) cout << " -> ";
        cur = cur->next;
    }
    cout << " -> NULL\n";
}

// ═══════════════════════════════════════════════════════════════
//  LINKED QUEUE
// ═══════════════════════════════════════════════════════════════
LinkedQueue::LinkedQueue() : front(nullptr), rear(nullptr), size(0) {}

LinkedQueue::~LinkedQueue() {
    string tmp;
    while (!isEmpty()) dequeue(tmp);
}

bool LinkedQueue::isEmpty() const { return front == nullptr; }

void LinkedQueue::enqueue(const string& val) {
    SLLNode* n = new SLLNode{ val, nullptr };
    if (!rear) { front = rear = n; }
    else { rear->next = n; rear = n; }
    size++;
}

bool LinkedQueue::dequeue(string& val) {
    if (isEmpty()) { cout << "[LQUEUE] Queue is empty.\n"; return false; }
    val = front->data;
    SLLNode* tmp = front;
    front = front->next;
    if (!front) rear = nullptr;
    delete tmp; size--;
    return true;
}

void LinkedQueue::display() const {
    if (isEmpty()) { cout << "  (queue empty)\n"; return; }
    cout << "  FRONT -> ";
    SLLNode* cur = front;
    while (cur) {
        cout << "[" << cur->data << "]";
        if (cur->next) cout << " -> ";
        cur = cur->next;
    }
    cout << " <- REAR\n";
}

// ═══════════════════════════════════════════════════════════════
//  PASSENGER QUEUE MENU
// ═══════════════════════════════════════════════════════════════
void passengerQueueMenu() {
    SimpleQueue   sq;
    CircularQueue cq;
    PriorityQueue pq;
    Deque         dq;

    int choice;
    do {
        printLine();
        cout << "   PASSENGER QUEUE MANAGEMENT\n";
        printLine();
        cout << " 1. Simple Queue   — Enqueue passenger\n";
        cout << " 2. Simple Queue   — Dequeue passenger\n";
        cout << " 3. Simple Queue   — Display\n";
        cout << " 4. Circular Queue — Enqueue passenger\n";
        cout << " 5. Circular Queue — Dequeue passenger\n";
        cout << " 6. Circular Queue — Display\n";
        cout << " 7. Priority Queue — Add passenger (with priority)\n";
        cout << " 8. Priority Queue — Serve next passenger\n";
        cout << " 9. Priority Queue — Display\n";
        cout << "10. Deque          — Push front\n";
        cout << "11. Deque          — Push back\n";
        cout << "12. Deque          — Pop front\n";
        cout << "13. Deque          — Pop back\n";
        cout << "14. Deque          — Display\n";
        cout << " 0. Back\n";
        printLine();
        cout << "Choice: ";
        cin >> choice;

        string name, val;
        PQEntry entry;
        int pri;

        switch (choice) {
        case 1:
            cout << "Passenger name: "; cin.ignore(); getline(cin, name);
            sq.enqueue(name);
            cout << "[OK] " << name << " added to simple queue.\n";
            break;
        case 2:
            if (sq.dequeue(val)) cout << "[SERVED] " << val << "\n";
            break;
        case 3: sq.display(); break;
        case 4:
            cout << "Passenger name: "; cin.ignore(); getline(cin, name);
            cq.enqueue(name);
            cout << "[OK] " << name << " added to circular queue.\n";
            break;
        case 5:
            if (cq.dequeue(val)) cout << "[SERVED] " << val << "\n";
            break;
        case 6: cq.display(); break;
        case 7:
            cout << "Passenger name: "; cin.ignore(); getline(cin, name);
            cout << "Priority (1=Normal, 2=Senior, 3=Emergency): ";
            cin >> pri;
            pq.enqueue(name, pri);
            cout << "[OK] " << name << " added with priority " << pri << ".\n";
            break;
        case 8:
            if (pq.dequeue(entry))
                cout << "[SERVED] " << entry.passenger
                     << " (priority " << entry.priority << ")\n";
            break;
        case 9: pq.display(); break;
        case 10:
            cout << "Value: "; cin.ignore(); getline(cin, name);
            dq.pushFront(name); break;
        case 11:
            cout << "Value: "; cin.ignore(); getline(cin, name);
            dq.pushBack(name); break;
        case 12:
            if (dq.popFront(val)) cout << "[POPPED FRONT] " << val << "\n";
            break;
        case 13:
            if (dq.popBack(val)) cout << "[POPPED BACK] " << val << "\n";
            break;
        case 14: dq.display(); break;
        case 0: break;
        default: cout << "[ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

// ═══════════════════════════════════════════════════════════════
//  ROUTE HISTORY STACK MENU
// ═══════════════════════════════════════════════════════════════
void routeHistoryMenu(ArrayStack& history) {
    int choice;
    do {
        printLine();
        cout << "   ROUTE HISTORY (Stack)\n";
        printLine();
        cout << " 1. View history (top = most recent)\n";
        cout << " 2. Pop last search (undo)\n";
        cout << " 3. Peek last search\n";
        cout << " 4. Push a manual entry\n";
        cout << " 5. Display in reverse (oldest first)\n";
        cout << " 6. Linked-list Stack demo\n";
        cout << " 0. Back\n";
        printLine();
        cout << "Choice: ";
        cin >> choice;

        string val;
        switch (choice) {
        case 1: history.display(); break;
        case 2:
            if (history.pop(val)) cout << "[POPPED] " << val << "\n";
            break;
        case 3:
            if (history.peek(val)) cout << "[TOP] " << val << "\n";
            break;
        case 4:
            cout << "Enter entry: "; cin.ignore(); getline(cin, val);
            history.push(val);
            cout << "[PUSHED] " << val << "\n";
            break;
        case 5: history.reverseDisplay(); break;
        case 6: {
            // small linked-stack demo
            LinkedStack ls;
            cout << "\n--- Linked Stack Demo ---\n";
            string items[] = { "Central->Museum", "Museum->University", "Market->Airport" };
            for (auto& s : items) { ls.push(s); cout << "  Pushed: " << s << "\n"; }
            ls.display();
            ls.pop(val); cout << "  Popped: " << val << "\n";
            ls.display();
            break;
        }
        case 0: break;
        default: cout << "[ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

// ═══════════════════════════════════════════════════════════════
//  LINKED LIST DEMO MENU
// ═══════════════════════════════════════════════════════════════
void linkedListDemoMenu() {
    SinglyList   sll;
    CircularSLL  csll;
    DoublyList   dll;
    CircularDLL  cdll;
    LinkedQueue  lq;

    // pre-populate with metro station names
    string seeds[] = { "Central", "Museum", "University", "Market", "City Hall" };
    for (auto& s : seeds) {
        sll.insertBack(s);
        csll.insertBack(s);
        dll.insertBack(s);
        cdll.insertBack(s);
        lq.enqueue(s);
    }

    int choice;
    do {
        printLine();
        cout << "   LINKED LIST DEMONSTRATIONS\n";
        printLine();
        cout << " -- Singly Linked List --\n";
        cout << "  1. Display\n";
        cout << "  2. Insert at front\n";
        cout << "  3. Insert at back\n";
        cout << "  4. Insert at position\n";
        cout << "  5. Delete front\n";
        cout << "  6. Delete back\n";
        cout << "  7. Delete at position\n";
        cout << "  8. Search\n";
        cout << "  9. Reverse\n";
        cout << " -- Circular Singly --\n";
        cout << " 10. Display\n";
        cout << " 11. Insert front\n";
        cout << " 12. Insert back\n";
        cout << " 13. Delete front\n";
        cout << " 14. Simulate circular metro line (traverse 8 steps)\n";
        cout << " -- Doubly Linked List --\n";
        cout << " 15. Display forward\n";
        cout << " 16. Display backward\n";
        cout << " 17. Insert front\n";
        cout << " 18. Insert back\n";
        cout << " 19. Delete front\n";
        cout << " 20. Delete back\n";
        cout << " -- Doubly Circular --\n";
        cout << " 21. Display forward\n";
        cout << " 22. Display backward\n";
        cout << " 23. Insert front\n";
        cout << " 24. Delete back\n";
        cout << " -- Queue using Linked List --\n";
        cout << " 25. Enqueue\n";
        cout << " 26. Dequeue\n";
        cout << " 27. Display\n";
        cout << "  0. Back\n";
        printLine();
        cout << "Choice: ";
        cin >> choice;

        string val;
        int pos;

        switch (choice) {
        case 1: sll.display(); break;
        case 2: cout << "Value: "; cin.ignore(); getline(cin, val); sll.insertFront(val); sll.display(); break;
        case 3: cout << "Value: "; cin.ignore(); getline(cin, val); sll.insertBack(val);  sll.display(); break;
        case 4:
            cout << "Position (0-indexed): "; cin >> pos;
            cout << "Value: "; cin.ignore(); getline(cin, val);
            sll.insertAt(pos, val); sll.display(); break;
        case 5: if (sll.deleteFront(val)) cout << "[DELETED] " << val << "\n"; sll.display(); break;
        case 6: if (sll.deleteBack(val))  cout << "[DELETED] " << val << "\n"; sll.display(); break;
        case 7:
            cout << "Position (0-indexed): "; cin >> pos;
            if (sll.deleteAt(pos, val)) cout << "[DELETED] " << val << "\n";
            sll.display(); break;
        case 8:
            cout << "Search value: "; cin.ignore(); getline(cin, val);
            pos = sll.search(val);
            if (pos == -1) cout << "  Not found.\n";
            else cout << "  Found at position " << pos << "\n";
            break;
        case 9: sll.reverse(); cout << "  Reversed.\n"; sll.display(); break;
        case 10: csll.display(); break;
        case 11: cout << "Value: "; cin.ignore(); getline(cin, val); csll.insertFront(val); csll.display(); break;
        case 12: cout << "Value: "; cin.ignore(); getline(cin, val); csll.insertBack(val);  csll.display(); break;
        case 13: if (csll.deleteFront(val)) cout << "[DELETED] " << val << "\n"; csll.display(); break;
        case 14: csll.traverse(8); break;
        case 15: dll.displayForward(); break;
        case 16: dll.displayBackward(); break;
        case 17: cout << "Value: "; cin.ignore(); getline(cin, val); dll.insertFront(val); dll.displayForward(); break;
        case 18: cout << "Value: "; cin.ignore(); getline(cin, val); dll.insertBack(val);  dll.displayForward(); break;
        case 19: if (dll.deleteFront(val)) cout << "[DELETED] " << val << "\n"; dll.displayForward(); break;
        case 20: if (dll.deleteBack(val))  cout << "[DELETED] " << val << "\n"; dll.displayForward(); break;
        case 21: cdll.displayForward(); break;
        case 22: cdll.displayBackward(); break;
        case 23: cout << "Value: "; cin.ignore(); getline(cin, val); cdll.insertFront(val); cdll.displayForward(); break;
        case 24: if (cdll.deleteBack(val)) cout << "[DELETED] " << val << "\n"; cdll.displayForward(); break;
        case 25: cout << "Value: "; cin.ignore(); getline(cin, val); lq.enqueue(val); lq.display(); break;
        case 26: if (lq.dequeue(val)) cout << "[DEQUEUED] " << val << "\n"; lq.display(); break;
        case 27: lq.display(); break;
        case 0: break;
        default: cout << "[ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}
