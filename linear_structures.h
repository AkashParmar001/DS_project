#ifndef LINEAR_STRUCTURES_H
#define LINEAR_STRUCTURES_H

#include <string>

// ─────────────────────────────────────────────────────────────────
//  Constants
// ─────────────────────────────────────────────────────────────────
static const int QUEUE_CAPACITY  = 10;   // fixed-size queues
static const int STACK_CAPACITY  = 50;   // array-based route-history stack
static const int DEQUE_CAPACITY  = 10;

// ═══════════════════════════════════════════════════════════════
//  Unit 2 — ARRAY-BASED STRUCTURES
// ═══════════════════════════════════════════════════════════════

// ── Array Stack  (route history) ────────────────────────────────
struct ArrayStack {
    std::string data[STACK_CAPACITY];
    int top;

    ArrayStack();
    bool isEmpty() const;
    bool isFull()  const;
    bool push(const std::string& val);
    bool pop(std::string& val);
    bool peek(std::string& val) const;
    void display() const;
    void reverseDisplay() const;   // pop-based reversal
};

// ── Simple Queue  (FIFO passenger boarding) ──────────────────────
struct SimpleQueue {
    std::string data[QUEUE_CAPACITY];
    int front, rear, size;

    SimpleQueue();
    bool isEmpty() const;
    bool isFull()  const;
    bool enqueue(const std::string& val);
    bool dequeue(std::string& val);
    void display() const;
};

// ── Circular Queue ────────────────────────────────────────────────
struct CircularQueue {
    std::string data[QUEUE_CAPACITY];
    int front, rear, size;

    CircularQueue();
    bool isEmpty() const;
    bool isFull()  const;
    bool enqueue(const std::string& val);
    bool dequeue(std::string& val);
    void display() const;
};

// ── Priority Queue  (higher number = higher priority) ────────────
struct PQEntry {
    std::string passenger;
    int         priority;   // 1=normal, 2=senior, 3=emergency
};

struct PriorityQueue {
    PQEntry data[QUEUE_CAPACITY];
    int size;

    PriorityQueue();
    bool isEmpty() const;
    bool isFull()  const;
    bool enqueue(const std::string& passenger, int priority);
    bool dequeue(PQEntry& out);   // removes highest-priority entry
    void display() const;
};

// ── Deque  (double-ended queue) ──────────────────────────────────
struct Deque {
    std::string data[DEQUE_CAPACITY];
    int front, rear, size;

    Deque();
    bool isEmpty() const;
    bool isFull()  const;
    bool pushFront(const std::string& val);
    bool pushBack (const std::string& val);
    bool popFront (std::string& val);
    bool popBack  (std::string& val);
    void display() const;
};

// ═══════════════════════════════════════════════════════════════
//  Unit 3 — LINKED LIST NODES
// ═══════════════════════════════════════════════════════════════

// ── Singly linked list node ──────────────────────────────────────
struct SLLNode {
    std::string data;
    SLLNode*    next;
};

// ── Doubly linked list node ──────────────────────────────────────
struct DLLNode {
    std::string data;
    DLLNode*    prev;
    DLLNode*    next;
};

// ═══════════════════════════════════════════════════════════════
//  Unit 3 — SINGLY LINKED LIST
// ═══════════════════════════════════════════════════════════════
struct SinglyList {
    SLLNode* head;
    int      size;

    SinglyList();
    ~SinglyList();

    void insertFront(const std::string& val);
    void insertBack (const std::string& val);
    void insertAt   (int pos, const std::string& val);   // 0-indexed
    bool deleteFront(std::string& val);
    bool deleteBack (std::string& val);
    bool deleteAt   (int pos, std::string& val);
    int  search     (const std::string& val) const;      // returns position or -1
    void display()  const;
    void reverse();
    void clear();
};

// ═══════════════════════════════════════════════════════════════
//  Unit 3 — SINGLY CIRCULAR LINKED LIST
// ═══════════════════════════════════════════════════════════════
struct CircularSLL {
    SLLNode* tail;   // tail->next == head
    int      size;

    CircularSLL();
    ~CircularSLL();

    void insertFront(const std::string& val);
    void insertBack (const std::string& val);
    bool deleteFront(std::string& val);
    void display()  const;          // prints one full cycle
    void traverse(int steps) const; // simulate a circular metro line
    void clear();
};

// ═══════════════════════════════════════════════════════════════
//  Unit 3 — DOUBLY LINKED LIST
// ═══════════════════════════════════════════════════════════════
struct DoublyList {
    DLLNode* head;
    DLLNode* tail;
    int      size;

    DoublyList();
    ~DoublyList();

    void insertFront(const std::string& val);
    void insertBack (const std::string& val);
    void insertAt   (int pos, const std::string& val);
    bool deleteFront(std::string& val);
    bool deleteBack (std::string& val);
    bool deleteAt   (int pos, std::string& val);
    void displayForward()  const;
    void displayBackward() const;
    void clear();
};

// ═══════════════════════════════════════════════════════════════
//  Unit 3 — DOUBLY CIRCULAR LINKED LIST
// ═══════════════════════════════════════════════════════════════
struct CircularDLL {
    DLLNode* head;
    int      size;

    CircularDLL();
    ~CircularDLL();

    void insertFront(const std::string& val);
    void insertBack (const std::string& val);
    bool deleteFront(std::string& val);
    bool deleteBack (std::string& val);
    void displayForward()  const;
    void displayBackward() const;
    void clear();
};

// ═══════════════════════════════════════════════════════════════
//  Unit 3 — STACK using Linked List
// ═══════════════════════════════════════════════════════════════
struct LinkedStack {
    SLLNode* top;
    int      size;

    LinkedStack();
    ~LinkedStack();

    bool isEmpty() const;
    void push(const std::string& val);
    bool pop (std::string& val);
    bool peek(std::string& val) const;
    void display() const;
};

// ═══════════════════════════════════════════════════════════════
//  Unit 3 — QUEUE using Linked List
// ═══════════════════════════════════════════════════════════════
struct LinkedQueue {
    SLLNode* front;
    SLLNode* rear;
    int      size;

    LinkedQueue();
    ~LinkedQueue();

    bool isEmpty() const;
    void enqueue(const std::string& val);
    bool dequeue(std::string& val);
    void display() const;
};

// ═══════════════════════════════════════════════════════════════
//  Menu entry points (called from main.cpp)
// ═══════════════════════════════════════════════════════════════
void passengerQueueMenu();
void routeHistoryMenu(ArrayStack& history);
void linkedListDemoMenu();

#endif // LINEAR_STRUCTURES_H
