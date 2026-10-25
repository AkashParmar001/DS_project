#ifndef TREE_ALGORITHMS_H
#define TREE_ALGORITHMS_H

#include <string>

// ═══════════════════════════════════════════════════════════════
//  BINARY TREE  (generic sample tree, manual construction)
// ═══════════════════════════════════════════════════════════════
struct BTNode {
    int     data;
    BTNode* left;
    BTNode* right;
};

BTNode* btNewNode(int val);
BTNode* buildSampleBinaryTree();          // returns a hardcoded 7-node tree
void    btInorder   (BTNode* root);
void    btPreorder  (BTNode* root);
void    btPostorder (BTNode* root);
int     btHeight    (BTNode* root);
void    btFree      (BTNode* root);
void    btPrint     (BTNode* root, int space = 0); // visual 2-D print

// ═══════════════════════════════════════════════════════════════
//  BINARY SEARCH TREE
// ═══════════════════════════════════════════════════════════════
struct BSTNode {
    int      key;
    std::string label;   // station name or description
    BSTNode* left;
    BSTNode* right;
};

struct BST {
    BSTNode* root;

    BST();
    ~BST();

    void     insert(int key, const std::string& label = "");
    bool     search(int key, std::string& label) const;
    bool     remove(int key);
    void     inorder()   const;
    void     preorder()  const;
    void     postorder() const;
    void     printTree() const;           // visual 2-D print
    BSTNode* findMin(BSTNode* node) const;
    void     clear();

private:
    BSTNode* insertRec (BSTNode* node, int key, const std::string& label);
    BSTNode* removeRec (BSTNode* node, int key);
    bool     searchRec (BSTNode* node, int key, std::string& label) const;
    void     inRec     (BSTNode* node) const;
    void     preRec    (BSTNode* node) const;
    void     postRec   (BSTNode* node) const;
    void     printRec  (BSTNode* node, int space) const;
    void     freeRec   (BSTNode* node);
};

// ═══════════════════════════════════════════════════════════════
//  AVL TREE
// ═══════════════════════════════════════════════════════════════
struct AVLNode {
    int      key;
    int      height;
    AVLNode* left;
    AVLNode* right;
};

struct AVLTree {
    AVLNode* root;

    AVLTree();
    ~AVLTree();

    void insert(int key);
    bool remove(int key);
    bool search(int key) const;
    void inorder()   const;
    void printTree() const;
    void clear();

private:
    int      height   (AVLNode* n) const;
    int      getBalance(AVLNode* n) const;
    AVLNode* newNode  (int key);
    AVLNode* rotateRight (AVLNode* y);
    AVLNode* rotateLeft  (AVLNode* x);
    AVLNode* insertRec   (AVLNode* node, int key);
    AVLNode* removeRec   (AVLNode* node, int key);
    AVLNode* minNode     (AVLNode* node) const;
    bool     searchRec   (AVLNode* node, int key) const;
    void     inRec       (AVLNode* node) const;
    void     printRec    (AVLNode* node, int space) const;
    void     freeRec     (AVLNode* node);
};

// ═══════════════════════════════════════════════════════════════
//  B-TREE  (order / degree t = 2  →  min 1 key, max 3 keys)
// ═══════════════════════════════════════════════════════════════
static const int BT_T = 2;                      // minimum degree
static const int BT_MAX_KEYS = 2 * BT_T - 1;   // 3
static const int BT_MAX_CH   = 2 * BT_T;        // 4

struct BTreeNode {
    int        keys[BT_MAX_KEYS];
    BTreeNode* children[BT_MAX_CH];
    int        n;        // current number of keys
    bool       leaf;
};

struct BTree {
    BTreeNode* root;

    BTree();
    ~BTree();

    void insert (int key);
    bool search (int key) const;
    void display() const;   // level-order print
    void clear();

private:
    BTreeNode* allocNode(bool leaf);
    void       splitChild(BTreeNode* parent, int i, BTreeNode* child);
    void       insertNonFull(BTreeNode* node, int key);
    bool       searchRec(BTreeNode* node, int key) const;
    void       displayRec(BTreeNode* node, int level) const;
    void       freeRec(BTreeNode* node);
};

// ═══════════════════════════════════════════════════════════════
//  B+ TREE  (simplified, order t = 2, leaf chain)
// ═══════════════════════════════════════════════════════════════
struct BPlusNode {
    int         keys[BT_MAX_KEYS];
    BPlusNode*  children[BT_MAX_CH];
    BPlusNode*  next;      // leaf chain pointer
    int         n;
    bool        leaf;
};

struct BPlusTree {
    BPlusNode* root;

    BPlusTree();
    ~BPlusTree();

    void insert (int key);
    bool search (int key) const;
    void displayLeaves() const;    // print all leaves in order
    void display()       const;    // show internal + leaf levels
    void clear();

private:
    BPlusNode* allocNode(bool leaf);
    void       splitChild(BPlusNode* parent, int i, BPlusNode* child);
    void       insertNonFull(BPlusNode* node, int key);
    bool       searchRec(BPlusNode* node, int key) const;
    void       displayRec(BPlusNode* node, int level) const;
    void       freeRec(BPlusNode* node);
};

// ═══════════════════════════════════════════════════════════════
//  GENERAL TREE  →  Binary Tree  (left-child / right-sibling)
// ═══════════════════════════════════════════════════════════════
struct GenNode {
    int      id;
    std::string name;
    GenNode* firstChild;
    GenNode* nextSibling;
    // left-child/right-sibling representation shares the same pointers
};

GenNode* genNewNode(int id, const std::string& name);
void     buildSampleGeneralTree(GenNode*& root);
void     genTreePrint(GenNode* root, int depth = 0);
// Convert by reinterpreting firstChild/nextSibling as left/right
void     lcrsInorder(GenNode* root);   // treat as binary via LC-RS
void     genFree(GenNode* root);

// ═══════════════════════════════════════════════════════════════
//  Menu entry points
// ═══════════════════════════════════════════════════════════════
void binaryTreeMenu();
void bstMenu();
void avlMenu();
void btreeBplusMenu();
void generalTreeMenu();
void treeAlgorithmsMenu();   // top-level tree menu

#endif // TREE_ALGORITHMS_H
