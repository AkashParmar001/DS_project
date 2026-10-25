#include "tree_algorithms.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <functional>
#include <queue>

using namespace std;

// ───────────────────────────────────────────────────────────────
static void printLine(char c = '=', int w = 52) {
    for (int i = 0; i < w; i++) cout << c;
    cout << "\n";
}

// ═══════════════════════════════════════════════════════════════
//  BINARY TREE  (sample hardcoded tree)
//
//  Structure:
//       1
//      2 3
//     4 5 6 7
// ═══════════════════════════════════════════════════════════════

BTNode* btNewNode(int val) {
    BTNode* n = new BTNode();
    n->data = val; n->left = n->right = nullptr;
    return n;
}

BTNode* buildSampleBinaryTree() {
    BTNode* root = btNewNode(1);
    root->left         = btNewNode(2);
    root->right        = btNewNode(3);
    root->left->left   = btNewNode(4);
    root->left->right  = btNewNode(5);
    root->right->left  = btNewNode(6);
    root->right->right = btNewNode(7);
    return root;
}

void btInorder(BTNode* root) {
    if (!root) return;
    btInorder(root->left);
    cout << root->data << " ";
    btInorder(root->right);
}

void btPreorder(BTNode* root) {
    if (!root) return;
    cout << root->data << " ";
    btPreorder(root->left);
    btPreorder(root->right);
}

void btPostorder(BTNode* root) {
    if (!root) return;
    btPostorder(root->left);
    btPostorder(root->right);
    cout << root->data << " ";
}

int btHeight(BTNode* root) {
    if (!root) return 0;
    return 1 + max(btHeight(root->left), btHeight(root->right));
}

void btFree(BTNode* root) {
    if (!root) return;
    btFree(root->left);
    btFree(root->right);
    delete root;
}

// Right-to-left indented visual print
void btPrint(BTNode* root, int space) {
    if (!root) return;
    space += 5;
    btPrint(root->right, space);
    cout << "\n" << string(space - 5, ' ') << root->data << "\n";
    btPrint(root->left, space);
}

// ═══════════════════════════════════════════════════════════════
//  BST
// ═══════════════════════════════════════════════════════════════
BST::BST() : root(nullptr) {}
BST::~BST() { clear(); }

void BST::clear() { freeRec(root); root = nullptr; }

void BST::freeRec(BSTNode* node) {
    if (!node) return;
    freeRec(node->left);
    freeRec(node->right);
    delete node;
}

BSTNode* BST::insertRec(BSTNode* node, int key, const string& label) {
    if (!node) {
        BSTNode* n = new BSTNode();
        n->key = key; n->label = label;
        n->left = n->right = nullptr;
        return n;
    }
    if (key < node->key)       node->left  = insertRec(node->left,  key, label);
    else if (key > node->key)  node->right = insertRec(node->right, key, label);
    else cout << "  [BST] Key " << key << " already exists.\n";
    return node;
}

void BST::insert(int key, const string& label) {
    root = insertRec(root, key, label);
}

bool BST::searchRec(BSTNode* node, int key, string& label) const {
    if (!node) return false;
    if (key == node->key) { label = node->label; return true; }
    if (key <  node->key) return searchRec(node->left,  key, label);
    return                       searchRec(node->right, key, label);
}

bool BST::search(int key, string& label) const {
    return searchRec(root, key, label);
}

BSTNode* BST::findMin(BSTNode* node) const {
    while (node && node->left) node = node->left;
    return node;
}

BSTNode* BST::removeRec(BSTNode* node, int key) {
    if (!node) { cout << "  [BST] Key " << key << " not found.\n"; return nullptr; }
    if (key < node->key)      node->left  = removeRec(node->left,  key);
    else if (key > node->key) node->right = removeRec(node->right, key);
    else {
        // found
        if (!node->left) {
            BSTNode* tmp = node->right; delete node; return tmp;
        }
        if (!node->right) {
            BSTNode* tmp = node->left;  delete node; return tmp;
        }
        // two children: replace with inorder successor
        BSTNode* succ = findMin(node->right);
        node->key   = succ->key;
        node->label = succ->label;
        node->right = removeRec(node->right, succ->key);
    }
    return node;
}

bool BST::remove(int key) {
    int before = 0;
    // count nodes to detect if deletion happened
    function<int(BSTNode*)> cnt = [&](BSTNode* n) -> int {
        return n ? 1 + cnt(n->left) + cnt(n->right) : 0;
    };
    before = cnt(root);
    root = removeRec(root, key);
    return cnt(root) < before;
}

void BST::inRec(BSTNode* node) const {
    if (!node) return;
    inRec(node->left);
    cout << node->key;
    if (!node->label.empty()) cout << "(" << node->label << ")";
    cout << " ";
    inRec(node->right);
}
void BST::preRec(BSTNode* node) const {
    if (!node) return;
    cout << node->key;
    if (!node->label.empty()) cout << "(" << node->label << ")";
    cout << " ";
    preRec(node->left);
    preRec(node->right);
}
void BST::postRec(BSTNode* node) const {
    if (!node) return;
    postRec(node->left);
    postRec(node->right);
    cout << node->key;
    if (!node->label.empty()) cout << "(" << node->label << ")";
    cout << " ";
}

void BST::inorder()   const { cout << "  Inorder  : "; inRec(root);   cout << "\n"; }
void BST::preorder()  const { cout << "  Preorder : "; preRec(root);  cout << "\n"; }
void BST::postorder() const { cout << "  Postorder: "; postRec(root); cout << "\n"; }

void BST::printRec(BSTNode* node, int space) const {
    if (!node) return;
    space += 6;
    printRec(node->right, space);
    cout << "\n" << string(space - 6, ' ') << node->key << "\n";
    printRec(node->left, space);
}
void BST::printTree() const { printRec(root, 0); }

// ═══════════════════════════════════════════════════════════════
//  AVL TREE
// ═══════════════════════════════════════════════════════════════
AVLTree::AVLTree() : root(nullptr) {}
AVLTree::~AVLTree() { clear(); }
void AVLTree::clear() { freeRec(root); root = nullptr; }

void AVLTree::freeRec(AVLNode* n) {
    if (!n) return;
    freeRec(n->left); freeRec(n->right); delete n;
}

int AVLTree::height(AVLNode* n) const { return n ? n->height : 0; }

int AVLTree::getBalance(AVLNode* n) const {
    return n ? height(n->left) - height(n->right) : 0;
}

AVLNode* AVLTree::newNode(int key) {
    AVLNode* n = new AVLNode();
    n->key = key; n->height = 1;
    n->left = n->right = nullptr;
    return n;
}

AVLNode* AVLTree::rotateRight(AVLNode* y) {
    cout << "    [LL Rotation] on node " << y->key << "\n";
    AVLNode* x  = y->left;
    AVLNode* T2 = x->right;
    x->right = y; y->left = T2;
    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));
    return x;
}

AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    cout << "    [RR Rotation] on node " << x->key << "\n";
    AVLNode* y  = x->right;
    AVLNode* T2 = y->left;
    y->left = x; x->right = T2;
    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));
    return y;
}

AVLNode* AVLTree::insertRec(AVLNode* node, int key) {
    if (!node) return newNode(key);
    if      (key < node->key) node->left  = insertRec(node->left,  key);
    else if (key > node->key) node->right = insertRec(node->right, key);
    else { cout << "  [AVL] Duplicate key " << key << " ignored.\n"; return node; }

    node->height = 1 + max(height(node->left), height(node->right));
    int bal = getBalance(node);

    // LL
    if (bal > 1 && key < node->left->key)
        return rotateRight(node);
    // RR
    if (bal < -1 && key > node->right->key)
        return rotateLeft(node);
    // LR
    if (bal > 1 && key > node->left->key) {
        cout << "    [LR Rotation] on node " << node->key << "\n";
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    // RL
    if (bal < -1 && key < node->right->key) {
        cout << "    [RL Rotation] on node " << node->key << "\n";
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

void AVLTree::insert(int key) {
    cout << "  Inserting " << key << " ...\n";
    root = insertRec(root, key);
    cout << "  Balance factors after insert:\n  ";
    // print inorder with balance
    function<void(AVLNode*)> printBal = [&](AVLNode* n) {
        if (!n) return;
        printBal(n->left);
        cout << n->key << "(bf=" << getBalance(n) << ") ";
        printBal(n->right);
    };
    printBal(root);
    cout << "\n";
}

AVLNode* AVLTree::minNode(AVLNode* node) const {
    while (node->left) node = node->left;
    return node;
}

AVLNode* AVLTree::removeRec(AVLNode* node, int key) {
    if (!node) return nullptr;
    if      (key < node->key) node->left  = removeRec(node->left,  key);
    else if (key > node->key) node->right = removeRec(node->right, key);
    else {
        if (!node->left || !node->right) {
            AVLNode* tmp = node->left ? node->left : node->right;
            delete node;
            return tmp;
        }
        AVLNode* succ = minNode(node->right);
        node->key   = succ->key;
        node->right = removeRec(node->right, succ->key);
    }
    node->height = 1 + max(height(node->left), height(node->right));
    int bal = getBalance(node);
    if (bal > 1  && getBalance(node->left)  >= 0) return rotateRight(node);
    if (bal > 1  && getBalance(node->left)  <  0) { node->left = rotateLeft(node->left);  return rotateRight(node); }
    if (bal < -1 && getBalance(node->right) <= 0) return rotateLeft(node);
    if (bal < -1 && getBalance(node->right) >  0) { node->right = rotateRight(node->right); return rotateLeft(node); }
    return node;
}

bool AVLTree::remove(int key) {
    bool found = search(key);
    if (!found) { cout << "  [AVL] Key " << key << " not found.\n"; return false; }
    root = removeRec(root, key);
    return true;
}

bool AVLTree::searchRec(AVLNode* node, int key) const {
    if (!node) return false;
    if (key == node->key) return true;
    return key < node->key ? searchRec(node->left, key) : searchRec(node->right, key);
}
bool AVLTree::search(int key) const { return searchRec(root, key); }

void AVLTree::inRec(AVLNode* node) const {
    if (!node) return;
    inRec(node->left);
    cout << node->key << "(h=" << node->height << ") ";
    inRec(node->right);
}
void AVLTree::inorder() const { cout << "  Inorder: "; inRec(root); cout << "\n"; }

void AVLTree::printRec(AVLNode* node, int space) const {
    if (!node) return;
    space += 6;
    printRec(node->right, space);
    cout << "\n" << string(space - 6, ' ') << node->key << "(bf=" << getBalance(node) << ")\n";
    printRec(node->left, space);
}
void AVLTree::printTree() const { printRec(root, 0); }

// ═══════════════════════════════════════════════════════════════
//  B-TREE  (minimum degree t = 2)
// ═══════════════════════════════════════════════════════════════
BTree::BTree() : root(nullptr) {}
BTree::~BTree() { clear(); }

void BTree::clear() { freeRec(root); root = nullptr; }

void BTree::freeRec(BTreeNode* node) {
    if (!node) return;
    if (!node->leaf)
        for (int i = 0; i <= node->n; i++) freeRec(node->children[i]);
    delete node;
}

BTreeNode* BTree::allocNode(bool leaf) {
    BTreeNode* n = new BTreeNode();
    n->n    = 0;
    n->leaf = leaf;
    for (int i = 0; i < BT_MAX_CH; i++) n->children[i] = nullptr;
    return n;
}

bool BTree::searchRec(BTreeNode* node, int key) const {
    int i = 0;
    while (i < node->n && key > node->keys[i]) i++;
    if (i < node->n && key == node->keys[i]) return true;
    if (node->leaf) return false;
    return searchRec(node->children[i], key);
}
bool BTree::search(int key) const {
    if (!root) return false;
    return searchRec(root, key);
}

void BTree::splitChild(BTreeNode* parent, int i, BTreeNode* child) {
    BTreeNode* z = allocNode(child->leaf);
    z->n = BT_T - 1;
    for (int j = 0; j < BT_T - 1; j++)
        z->keys[j] = child->keys[j + BT_T];
    if (!child->leaf)
        for (int j = 0; j < BT_T; j++)
            z->children[j] = child->children[j + BT_T];
    child->n = BT_T - 1;

    for (int j = parent->n; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];
    parent->children[i + 1] = z;

    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];
    parent->keys[i] = child->keys[BT_T - 1];
    parent->n++;
}

void BTree::insertNonFull(BTreeNode* node, int key) {
    int i = node->n - 1;
    if (node->leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i]; i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    } else {
        while (i >= 0 && key < node->keys[i]) i--;
        i++;
        if (node->children[i]->n == BT_MAX_KEYS) {
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i]) i++;
        }
        insertNonFull(node->children[i], key);
    }
}

void BTree::insert(int key) {
    if (!root) {
        root = allocNode(true);
        root->keys[0] = key;
        root->n = 1;
        return;
    }
    if (root->n == BT_MAX_KEYS) {
        BTreeNode* s = allocNode(false);
        s->children[0] = root;
        splitChild(s, 0, root);
        int i = (s->keys[0] < key) ? 1 : 0;
        insertNonFull(s->children[i], key);
        root = s;
    } else {
        insertNonFull(root, key);
    }
}

void BTree::displayRec(BTreeNode* node, int level) const {
    if (!node) return;
    cout << string(level * 4, ' ') << "[";
    for (int i = 0; i < node->n; i++) {
        cout << node->keys[i];
        if (i < node->n - 1) cout << "|";
    }
    cout << "]\n";
    if (!node->leaf)
        for (int i = 0; i <= node->n; i++)
            displayRec(node->children[i], level + 1);
}

void BTree::display() const {
    if (!root) { cout << "  (empty B-tree)\n"; return; }
    cout << "  B-Tree (t=" << BT_T << ", max keys=" << BT_MAX_KEYS << "):\n";
    displayRec(root, 1);
}

// ═══════════════════════════════════════════════════════════════
//  B+ TREE  (simplified, leaves linked)
// ═══════════════════════════════════════════════════════════════
BPlusTree::BPlusTree() : root(nullptr) {}
BPlusTree::~BPlusTree() { clear(); }

void BPlusTree::clear() { freeRec(root); root = nullptr; }

void BPlusTree::freeRec(BPlusNode* node) {
    if (!node) return;
    if (!node->leaf)
        for (int i = 0; i <= node->n; i++) freeRec(node->children[i]);
    delete node;
}

BPlusNode* BPlusTree::allocNode(bool leaf) {
    BPlusNode* n = new BPlusNode();
    n->n = 0; n->leaf = leaf; n->next = nullptr;
    for (int i = 0; i < BT_MAX_CH; i++) n->children[i] = nullptr;
    return n;
}

bool BPlusTree::searchRec(BPlusNode* node, int key) const {
    int i = 0;
    while (i < node->n && key > node->keys[i]) i++;
    if (node->leaf) {
        return (i < node->n && node->keys[i] == key);
    }
    return searchRec(node->children[i], key);
}
bool BPlusTree::search(int key) const {
    if (!root) return false;
    return searchRec(root, key);
}

void BPlusTree::splitChild(BPlusNode* parent, int i, BPlusNode* child) {
    BPlusNode* z = allocNode(child->leaf);
    int mid = BT_T - 1;

    if (child->leaf) {
        // copy upper half (inclusive) into z
        z->n = BT_T;
        for (int j = 0; j < BT_T; j++)
            z->keys[j] = child->keys[j + mid];
        child->n = mid;
        // link leaves
        z->next    = child->next;
        child->next = z;
        // push up the smallest key of z
        for (int j = parent->n - 1; j >= i; j--)
            parent->keys[j + 1] = parent->keys[j];
        parent->keys[i] = z->keys[0];
    } else {
        z->n = BT_T - 1;
        for (int j = 0; j < BT_T - 1; j++)
            z->keys[j] = child->keys[j + BT_T];
        for (int j = 0; j < BT_T; j++)
            z->children[j] = child->children[j + BT_T];
        child->n = mid;
        for (int j = parent->n - 1; j >= i; j--)
            parent->keys[j + 1] = parent->keys[j];
        parent->keys[i] = child->keys[mid];
    }

    for (int j = parent->n; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];
    parent->children[i + 1] = z;
    parent->n++;
}

void BPlusTree::insertNonFull(BPlusNode* node, int key) {
    int i = node->n - 1;
    if (node->leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i]; i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    } else {
        while (i >= 0 && key < node->keys[i]) i--;
        i++;
        if (node->children[i]->n == BT_MAX_KEYS) {
            splitChild(node, i, node->children[i]);
            if (key >= node->keys[i]) i++;
        }
        insertNonFull(node->children[i], key);
    }
}

void BPlusTree::insert(int key) {
    if (!root) {
        root = allocNode(true);
        root->keys[0] = key;
        root->n = 1;
        return;
    }
    if (root->n == BT_MAX_KEYS) {
        BPlusNode* s = allocNode(false);
        s->children[0] = root;
        splitChild(s, 0, root);
        int i = (s->keys[0] <= key) ? 1 : 0;
        insertNonFull(s->children[i], key);
        root = s;
    } else {
        insertNonFull(root, key);
    }
}

void BPlusTree::displayLeaves() const {
    if (!root) { cout << "  (empty)\n"; return; }
    // find leftmost leaf
    BPlusNode* cur = root;
    while (!cur->leaf) cur = cur->children[0];
    cout << "  Leaf chain: ";
    while (cur) {
        cout << "[";
        for (int i = 0; i < cur->n; i++) {
            cout << cur->keys[i];
            if (i < cur->n - 1) cout << "|";
        }
        cout << "]";
        if (cur->next) cout << " -> ";
        cur = cur->next;
    }
    cout << "\n";
}

void BPlusTree::displayRec(BPlusNode* node, int level) const {
    if (!node) return;
    string indent(level * 4, ' ');
    cout << indent << (node->leaf ? "Leaf" : "Internal") << " [";
    for (int i = 0; i < node->n; i++) {
        cout << node->keys[i];
        if (i < node->n - 1) cout << "|";
    }
    cout << "]\n";
    if (!node->leaf)
        for (int i = 0; i <= node->n; i++)
            displayRec(node->children[i], level + 1);
}

void BPlusTree::display() const {
    if (!root) { cout << "  (empty B+ tree)\n"; return; }
    cout << "  B+ Tree structure:\n";
    displayRec(root, 1);
    cout << "\n";
    displayLeaves();
}

// ═══════════════════════════════════════════════════════════════
//  GENERAL TREE  →  Binary Tree  (Left-Child / Right-Sibling)
//
//  Sample general tree (metro zone hierarchy):
//
//  Metro System
//  ├── Zone A
//  │   ├── Central
//  │   └── Museum
//  ├── Zone B
//  │   ├── University
//  │   └── Market
//  └── Zone C
//      ├── Airport
//      └── Harbor
// ═══════════════════════════════════════════════════════════════

GenNode* genNewNode(int id, const string& name) {
    GenNode* n = new GenNode();
    n->id = id; n->name = name;
    n->firstChild = n->nextSibling = nullptr;
    return n;
}

void buildSampleGeneralTree(GenNode*& root) {
    root = genNewNode(0, "Metro System");

    GenNode* zA = genNewNode(1, "Zone A");
    GenNode* zB = genNewNode(2, "Zone B");
    GenNode* zC = genNewNode(3, "Zone C");

    // Zone A children
    zA->firstChild = genNewNode(4, "Central");
    zA->firstChild->nextSibling = genNewNode(5, "Museum");

    // Zone B children
    zB->firstChild = genNewNode(6, "University");
    zB->firstChild->nextSibling = genNewNode(7, "Market");

    // Zone C children
    zC->firstChild = genNewNode(8, "Airport");
    zC->firstChild->nextSibling = genNewNode(9, "Harbor");

    // link zones as siblings
    zA->nextSibling = zB;
    zB->nextSibling = zC;

    root->firstChild = zA;
}

void genTreePrint(GenNode* root, int depth) {
    if (!root) return;
    cout << string(depth * 3, ' ');
    if (depth > 0) cout << "|-- ";
    cout << root->name << " (id=" << root->id << ")\n";
    genTreePrint(root->firstChild,   depth + 1);
    genTreePrint(root->nextSibling,  depth);
}

// Inorder traversal treating LC-RS as a binary tree (left = firstChild, right = nextSibling)
void lcrsInorder(GenNode* root) {
    if (!root) return;
    lcrsInorder(root->firstChild);
    cout << root->name << " ";
    lcrsInorder(root->nextSibling);
}

void genFree(GenNode* root) {
    if (!root) return;
    genFree(root->firstChild);
    genFree(root->nextSibling);
    delete root;
}

// ═══════════════════════════════════════════════════════════════
//  MENU FUNCTIONS
// ═══════════════════════════════════════════════════════════════

void binaryTreeMenu() {
    BTNode* tree = buildSampleBinaryTree();
    printLine();
    cout << "  BINARY TREE\n";
    printLine();
    cout << "  Sample tree (7 nodes):\n";
    btPrint(tree, 0);
    cout << "\n  Inorder   (L-Root-R) : "; btInorder(tree);   cout << "\n";
    cout << "  Preorder  (Root-L-R) : "; btPreorder(tree);  cout << "\n";
    cout << "  Postorder (L-R-Root) : "; btPostorder(tree); cout << "\n";
    cout << "  Height               : " << btHeight(tree) << "\n";
    btFree(tree);
    cout << "\n  Complexity: All traversals O(n)\n";
    cout << "\nPress Enter to continue...";
    cin.ignore(); cin.get();
}

void bstMenu() {
    BST bst;
    // Pre-load with metro station IDs
    int seeds[] = { 5, 3, 7, 1, 4, 6, 9, 0, 2, 8 };
    for (int k : seeds) {
        string lbl = "Station" + to_string(k);
        bst.insert(k, lbl);
    }

    int choice;
    do {
        printLine();
        cout << "  BINARY SEARCH TREE\n";
        printLine();
        cout << " 1. Display tree (visual)\n";
        cout << " 2. Inorder traversal  (sorted)\n";
        cout << " 3. Preorder traversal\n";
        cout << " 4. Postorder traversal\n";
        cout << " 5. Insert key\n";
        cout << " 6. Search key\n";
        cout << " 7. Delete key\n";
        cout << " 0. Back\n";
        printLine();
        cout << "Choice: ";
        cin >> choice;

        int key; string label, result;
        switch (choice) {
        case 1: bst.printTree(); break;
        case 2: bst.inorder();   break;
        case 3: bst.preorder();  break;
        case 4: bst.postorder(); break;
        case 5:
            cout << "Key: "; cin >> key;
            cout << "Label: "; cin.ignore(); getline(cin, label);
            bst.insert(key, label);
            bst.inorder();
            break;
        case 6:
            cout << "Key: "; cin >> key;
            if (bst.search(key, result))
                cout << "  Found: " << key << " -> " << result << "\n";
            else
                cout << "  Not found.\n";
            break;
        case 7:
            cout << "Key: "; cin >> key;
            if (bst.remove(key)) {
                cout << "  Deleted " << key << ".\n";
                bst.inorder();
            }
            break;
        case 0: break;
        default: cout << "[ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

void avlMenu() {
    AVLTree avl;
    int choice;
    do {
        printLine();
        cout << "  AVL TREE (Self-Balancing BST)\n";
        printLine();
        cout << " 1. Insert key\n";
        cout << " 2. Delete key\n";
        cout << " 3. Search key\n";
        cout << " 4. Inorder traversal\n";
        cout << " 5. Display tree\n";
        cout << " 6. Load sample data (10, 20, 30, 40, 50, 25)\n";
        cout << " 0. Back\n";
        printLine();
        cout << "Choice: ";
        cin >> choice;

        int key;
        switch (choice) {
        case 1: cout << "Key: "; cin >> key; avl.insert(key); avl.printTree(); break;
        case 2:
            cout << "Key: "; cin >> key;
            avl.remove(key);
            avl.printTree();
            break;
        case 3:
            cout << "Key: "; cin >> key;
            cout << (avl.search(key) ? "  Found.\n" : "  Not found.\n");
            break;
        case 4: avl.inorder(); break;
        case 5: avl.printTree(); break;
        case 6:
            for (int k : {10, 20, 30, 40, 50, 25}) avl.insert(k);
            avl.printTree();
            break;
        case 0: break;
        default: cout << "[ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

void btreeBplusMenu() {
    BTree    bt;
    BPlusTree bpt;

    // pre-load both
    int seeds[] = { 10, 20, 5, 6, 12, 30, 7, 17 };
    for (int k : seeds) { bt.insert(k); bpt.insert(k); }

    int choice;
    do {
        printLine();
        cout << "  B-TREE and B+ TREE (order t=2)\n";
        printLine();
        cout << " 1. Display B-Tree\n";
        cout << " 2. Insert into B-Tree\n";
        cout << " 3. Search in B-Tree\n";
        cout << " 4. Display B+ Tree\n";
        cout << " 5. Insert into B+ Tree\n";
        cout << " 6. Search in B+ Tree\n";
        cout << " 7. Display B+ Tree leaf chain\n";
        cout << " 0. Back\n";
        printLine();
        cout << "Choice: ";
        cin >> choice;

        int key;
        switch (choice) {
        case 1: bt.display(); break;
        case 2:
            cout << "Key: "; cin >> key;
            bt.insert(key); bt.display(); break;
        case 3:
            cout << "Key: "; cin >> key;
            cout << (bt.search(key) ? "  Found.\n" : "  Not found.\n");
            break;
        case 4: bpt.display(); break;
        case 5:
            cout << "Key: "; cin >> key;
            bpt.insert(key); bpt.display(); break;
        case 6:
            cout << "Key: "; cin >> key;
            cout << (bpt.search(key) ? "  Found.\n" : "  Not found.\n");
            break;
        case 7: bpt.displayLeaves(); break;
        case 0: break;
        default: cout << "[ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}

void generalTreeMenu() {
    GenNode* gRoot = nullptr;
    buildSampleGeneralTree(gRoot);

    printLine();
    cout << "  GENERAL TREE  ->  BINARY TREE (LC-RS)\n";
    printLine();
    cout << "\n  General tree (zone hierarchy):\n\n";
    genTreePrint(gRoot, 0);

    cout << "\n  Left-Child / Right-Sibling representation:\n";
    cout << "    Each node stores:\n";
    cout << "      left  ptr  = firstChild  (first child in general tree)\n";
    cout << "      right ptr  = nextSibling (next sibling in general tree)\n";
    cout << "\n  This IS a valid binary tree — no structural change needed.\n";
    cout << "\n  LC-RS Inorder traversal: ";
    lcrsInorder(gRoot);
    cout << "\n";

    cout << "\n  Complexity: O(n) for traversal, O(n) space\n";
    genFree(gRoot);
    cout << "\nPress Enter to continue...";
    cin.ignore(); cin.get();
}

void treeAlgorithmsMenu() {
    int choice;
    do {
        printLine();
        cout << "  TREE ALGORITHMS\n";
        printLine();
        cout << " 1. Binary Tree (sample + traversals)\n";
        cout << " 2. Binary Search Tree (BST)\n";
        cout << " 3. AVL Tree\n";
        cout << " 4. B-Tree & B+ Tree\n";
        cout << " 5. General Tree -> Binary Tree (LC-RS)\n";
        cout << " 0. Back\n";
        printLine();
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: binaryTreeMenu();   break;
        case 2: bstMenu();          break;
        case 3: avlMenu();          break;
        case 4: btreeBplusMenu();   break;
        case 5: generalTreeMenu();  break;
        case 0: break;
        default: cout << "[ERROR] Invalid choice.\n";
        }
    } while (choice != 0);
}
