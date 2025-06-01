/*
 * AED2 2025 (1S) - AP06 - ABBS BALANCEADAS
 * Comparação entre Árvores AVL e Vermelho-Pretas
 * Implementação em C++
 */

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

// Cores para Red-Black Tree
enum Color { RED, BLACK };

// Estrutura do nó para AVL
struct AVLNode {
    int data;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(int value) : data(value), left(nullptr), right(nullptr), height(1) {}
};

// Estrutura do nó para Red-Black Tree
struct RBNode {
    int data;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
    Color color;
    
    RBNode(int value) : data(value), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
};

// Classe para Árvore AVL
class AVLTree {
private:
    AVLNode* root;
    int rotations;
    
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
        rotations++;
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        updateHeight(y);
        updateHeight(x);
        
        return x;
    }
    
    AVLNode* rotateLeft(AVLNode* x) {
        rotations++;
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
        
        if (key < node->data) {
            node->left = insert(node->left, key);
        } else if (key > node->data) {
            node->right = insert(node->right, key);
        } else {
            return node; // Não inserir duplicatas
        }
        
        updateHeight(node);
        
        int balance = getBalance(node);
        
        // Rotação à direita
        if (balance > 1 && key < node->left->data) {
            return rotateRight(node);
        }
        
        // Rotação à esquerda
        if (balance < -1 && key > node->right->data) {
            return rotateLeft(node);
        }
        
        // Rotação esquerda-direita
        if (balance > 1 && key > node->left->data) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        
        // Rotação direita-esquerda
        if (balance < -1 && key < node->right->data) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }
    
    AVLNode* search(AVLNode* node, int key) {
        if (!node || node->data == key) return node;
        if (key < node->data) return search(node->left, key);
        return search(node->right, key);
    }
    
    int calculateHeight(AVLNode* node) {
        if (!node) return -1;
        return max(calculateHeight(node->left), calculateHeight(node->right)) + 1;
    }
    
    void destroyTree(AVLNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
    
public:
    AVLTree() : root(nullptr), rotations(0) {}
    
    ~AVLTree() {
        destroyTree(root);
    }
    
    void insert(int key) {
        root = insert(root, key);
    }
    
    bool search(int key) {
        return search(root, key) != nullptr;
    }
    
    int getRotations() {
        return rotations;
    }
    
    int getRootHeight() {
        return calculateHeight(root);
    }
    
    int getLeftSubtreeHeight() {
        return root ? calculateHeight(root->left) : -1;
    }
    
    int getRightSubtreeHeight() {
        return root ? calculateHeight(root->right) : -1;
    }
};

// Classe para Red-Black Tree
class RBTree {
private:
    RBNode* root;
    int colorChanges;
    int rotations;
    
    void rotateLeft(RBNode* x) {
        rotations++;
        RBNode* y = x->right;
        x->right = y->left;
        
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        
        if (!x->parent) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        
        y->left = x;
        x->parent = y;
    }
    
    void rotateRight(RBNode* y) {
        rotations++;
        RBNode* x = y->left;
        y->left = x->right;
        
        if (x->right) x->right->parent = y;
        x->parent = y->parent;
        
        if (!y->parent) {
            root = x;
        } else if (y == y->parent->right) {
            y->parent->right = x;
        } else {
            y->parent->left = x;
        }
        
        x->right = y;
        y->parent = x;
    }
    
    void fixInsert(RBNode* node) {
        while (node != root && node->parent->color == RED) {
            if (node->parent == node->parent->parent->left) {
                RBNode* uncle = node->parent->parent->right;
                
                if (uncle && uncle->color == RED) {
                    // Caso 1: Tio é vermelho
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    colorChanges += 3;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        // Caso 2: Tio é preto e node é filho direito
                        node = node->parent;
                        rotateLeft(node);
                    }
                    // Caso 3: Tio é preto e node é filho esquerdo
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    colorChanges += 2;
                    rotateRight(node->parent->parent);
                }
            } else {
                RBNode* uncle = node->parent->parent->left;
                
                if (uncle && uncle->color == RED) {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    colorChanges += 3;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rotateRight(node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    colorChanges += 2;
                    rotateLeft(node->parent->parent);
                }
            }
        }
        
        if (root->color == RED) {
            root->color = BLACK;
            colorChanges++;
        }
    }
    
    RBNode* search(RBNode* node, int key) {
        if (!node || node->data == key) return node;
        if (key < node->data) return search(node->left, key);
        return search(node->right, key);
    }
    
    int calculateHeight(RBNode* node) {
        if (!node) return -1;
        return max(calculateHeight(node->left), calculateHeight(node->right)) + 1;
    }
    
    int calculateBlackHeight(RBNode* node) {
        if (!node) return 0;
        
        int leftBlackHeight = calculateBlackHeight(node->left);
        int rightBlackHeight = calculateBlackHeight(node->right);
        
        int blackHeight = max(leftBlackHeight, rightBlackHeight);
        
        if (node->color == BLACK) {
            blackHeight++;
        }
        
        return blackHeight;
    }
    
    void destroyTree(RBNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
    
public:
    RBTree() : root(nullptr), colorChanges(0), rotations(0) {}
    
    ~RBTree() {
        destroyTree(root);
    }
    
    void insert(int key) {
        RBNode* newNode = new RBNode(key);
        RBNode* parent = nullptr;
        RBNode* current = root;
        
        while (current) {
            parent = current;
            if (key < current->data) {
                current = current->left;
            } else if (key > current->data) {
                current = current->right;
            } else {
                delete newNode;
                return; // Não inserir duplicatas
            }
        }
        
        newNode->parent = parent;
        
        if (!parent) {
            root = newNode;
        } else if (key < parent->data) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
        
        if (!newNode->parent) {
            newNode->color = BLACK;
            return;
        }
        
        if (!newNode->parent->parent) {
            return;
        }
        
        fixInsert(newNode);
    }
    
    bool search(int key) {
        return search(root, key) != nullptr;
    }
    
    int getColorChanges() {
        return colorChanges;
    }
    
    int getRotations() {
        return rotations;
    }
    
    int getRootHeight() {
        return calculateHeight(root);
    }
    
    int getLeftSubtreeHeight() {
        return root ? calculateHeight(root->left) : -1;
    }
    
    int getRightSubtreeHeight() {
        return root ? calculateHeight(root->right) : -1;
    }
    
    int getBlackHeight() {
        return calculateBlackHeight(root);
    }
};

int main() {
    AVLTree avlTree;
    RBTree rbTree;
    
    int value;
    vector<int> values;
    
    // Leitura da entrada
    while (cin >> value && value >= 0) {
        values.push_back(value);
    }
    
    // Inserção nas árvores
    for (int val : values) {
        avlTree.insert(val);
        rbTree.insert(val);
    }
    
    // Saída conforme especificação
    cout << avlTree.getRootHeight() << ", " 
         << avlTree.getLeftSubtreeHeight() << ", " 
         << avlTree.getRightSubtreeHeight() << endl;
    
    cout << rbTree.getRootHeight() << ", " 
         << rbTree.getLeftSubtreeHeight() << ", " 
         << rbTree.getRightSubtreeHeight() << endl;
    
    cout << rbTree.getBlackHeight() << endl;
    
    cout << rbTree.getColorChanges() << ", " 
         << rbTree.getRotations() << ", " 
         << avlTree.getRotations() << endl;
    
    return 0;
}
