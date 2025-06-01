#include <iostream>
#include <algorithm>
using namespace std;

// Contadores globais
int avl_rotations = 0;
int rb_rotations = 0;
int rb_color_changes = 0;

// ========== ÁRVORE AVL ==========
struct AVLNode {
    int data;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(int val) : data(val), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;
    
    int getHeight(AVLNode* node) {
        return node ? node->height : 0;
    }
    
    int getBalance(AVLNode* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
    void updateHeight(AVLNode* node) {
        if (node) {
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }
    
    AVLNode* rotateRight(AVLNode* y) {
        avl_rotations++;
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        updateHeight(y);
        updateHeight(x);
        
        return x;
    }
    
    AVLNode* rotateLeft(AVLNode* x) {
        avl_rotations++;
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        updateHeight(x);
        updateHeight(y);
        
        return y;
    }
    
    AVLNode* insert(AVLNode* node, int key) {
        if (!node) return new AVLNode(key);
        
        if (key < node->data)
            node->left = insert(node->left, key);
        else if (key > node->data)
            node->right = insert(node->right, key);
        else
            return node;
        
        updateHeight(node);
        
        int balance = getBalance(node);
        
        // Left Left Case
        if (balance > 1 && key < node->left->data)
            return rotateRight(node);
        
        // Right Right Case
        if (balance < -1 && key > node->right->data)
            return rotateLeft(node);
        
        // Left Right Case
        if (balance > 1 && key > node->left->data) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        
        // Right Left Case
        if (balance < -1 && key < node->right->data) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }
    
    int calculateHeight(AVLNode* node) {
        if (!node) return 0;
        return max(calculateHeight(node->left), calculateHeight(node->right)) + 1;
    }
    
public:
    AVLTree() : root(nullptr) {}
    
    void insert(int key) {
        root = insert(root, key);
    }
    
    void getHeights(int& total, int& left, int& right) {
        if (!root) {
            total = left = right = 0;
            return;
        }
        total = calculateHeight(root);
        left = root->left ? calculateHeight(root->left) : 0;
        right = root->right ? calculateHeight(root->right) : 0;
    }
};

// ========== ÁRVORE VERMELHO-PRETA ==========
enum Color { RED, BLACK };

struct RBNode {
    int data;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
    
    RBNode(int val) : data(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBTree {
private:
    RBNode* root;
    RBNode* NIL;
    
    void rotateLeft(RBNode* x) {
        rb_rotations++;
        RBNode* y = x->right;
        x->right = y->left;
        
        if (y->left != NIL)
            y->left->parent = x;
        
        y->parent = x->parent;
        
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        
        y->left = x;
        x->parent = y;
    }
    
    void rotateRight(RBNode* x) {
        rb_rotations++;
        RBNode* y = x->left;
        x->left = y->right;
        
        if (y->right != NIL)
            y->right->parent = x;
        
        y->parent = x->parent;
        
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        
        y->right = x;
        x->parent = y;
    }
    
    void insertFixup(RBNode* z) {
        while (z->parent && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                RBNode* y = z->parent->parent->right;
                if (y && y->color == RED) {
                    // Caso 1: Tio é vermelho
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    rb_color_changes += 3;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        // Caso 2: z é filho direito
                        z = z->parent;
                        rotateLeft(z);
                    }
                    // Caso 3: z é filho esquerdo
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rb_color_changes += 2;
                    rotateRight(z->parent->parent);
                }
            } else {
                RBNode* y = z->parent->parent->left;
                if (y && y->color == RED) {
                    // Caso 1: Tio é vermelho
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    rb_color_changes += 3;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        // Caso 2: z é filho esquerdo
                        z = z->parent;
                        rotateRight(z);
                    }
                    // Caso 3: z é filho direito
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rb_color_changes += 2;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        if (root->color == RED) {
            root->color = BLACK;
            rb_color_changes++;
        }
    }
    
    int calculateHeight(RBNode* node) {
        if (node == NIL || node == nullptr) return 0;
        return max(calculateHeight(node->left), calculateHeight(node->right)) + 1;
    }
    
    int calculateBlackHeight(RBNode* node) {
        if (node == NIL || node == nullptr) return 0;
        int leftBH = calculateBlackHeight(node->left);
        int rightBH = calculateBlackHeight(node->right);
        int maxBH = max(leftBH, rightBH);
        return maxBH + (node->color == BLACK ? 1 : 0);
    }
    
public:
    RBTree() {
        NIL = new RBNode(0);
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = nullptr;
        root = NIL;
    }
    
    void insert(int key) {
        RBNode* z = new RBNode(key);
        z->left = z->right = NIL;
        
        RBNode* y = nullptr;
        RBNode* x = root;
        
        while (x != NIL) {
            y = x;
            if (z->data < x->data)
                x = x->left;
            else
                x = x->right;
        }
        
        z->parent = y;
        if (y == nullptr) {
            root = z;
        } else if (z->data < y->data) {
            y->left = z;
        } else {
            y->right = z;
        }
        
        z->left = NIL;
        z->right = NIL;
        z->color = RED;
        
        insertFixup(z);
    }
    
    void getHeights(int& total, int& left, int& right) {
        if (root == NIL) {
            total = left = right = 0;
            return;
        }
        total = calculateHeight(root);
        left = (root->left != NIL) ? calculateHeight(root->left) : 0;
        right = (root->right != NIL) ? calculateHeight(root->right) : 0;
    }
    
    int getBlackHeight() {
        if (root == NIL) return 0;
        return calculateBlackHeight(root);
    }
};

int main() {
    AVLTree avl;
    RBTree rb;
    
    int num;
    while (cin >> num && num >= 0) {
        avl.insert(num);
        rb.insert(num);
    }
    
    int avl_total, avl_left, avl_right;
    int rb_total, rb_left, rb_right;
    
    avl.getHeights(avl_total, avl_left, avl_right);
    rb.getHeights(rb_total, rb_left, rb_right);
    
    int black_height = rb.getBlackHeight();
    
    cout << avl_total << ", " << avl_left << ", " << avl_right << endl;
    cout << rb_total << ", " << rb_left << ", " << rb_right << endl;
    cout << black_height << endl;
    cout << rb_color_changes << ", " << rb_rotations << ", " << avl_rotations << endl;
    
    return 0;
}
