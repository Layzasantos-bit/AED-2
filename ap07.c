// Árvore Rubro-Negra com remoção e cálculo de alturas
#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

typedef struct Node {
    int key;
    int color;
    struct Node *left, *right, *parent;
} Node;

Node *NIL;

// Cria um novo nó
Node* createNode(int key) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->color = RED;
    node->left = node->right = node->parent = NIL;
    return node;
}

// Rotação à esquerda
void leftRotate(Node **root, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NIL) *root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Rotação à direita
void rightRotate(Node **root, Node *x) {
    Node *y = x->left;
    x->left = y->right;
    if (y->right != NIL) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == NIL) *root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// Corrige inserção
void fixInsert(Node **root, Node *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

// Insere na árvore
void insert(Node **root, int key) {
    Node *z = createNode(key);
    Node *y = NIL, *x = *root;
    while (x != NIL) {
        y = x;
        if (z->key < x->key) x = x->left;
        else x = x->right;
    }
    z->parent = y;
    if (y == NIL) *root = z;
    else if (z->key < y->key) y->left = z;
    else y->right = z;
    z->left = z->right = NIL;
    z->color = RED;
    fixInsert(root, z);
}

// Altura da árvore
int height(Node *node) {
    if (node == NIL) return -1;
    int l = height(node->left);
    int r = height(node->right);
    return (l > r ? l : r) + 1;
}

// Altura rubro
int redHeight(Node *node) {
    if (node == NIL) return 0;
    int h = redHeight(node->left);
    int r = redHeight(node->right);
    int add = (node->color == RED ? 1 : 0);
    return (h > r ? h : r) + add;
}

// Busca nó
Node* search(Node *root, int key) {
    if (root == NIL || root->key == key) return root;
    if (key < root->key) return search(root->left, key);
    else return search(root->right, key);
}

// Inicializa NIL e raiz
void initialize(Node **root) {
    NIL = (Node*)malloc(sizeof(Node));
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL->parent = NULL;
    *root = NIL;
}

// Liberação da árvore
void freeTree(Node *root) {
    if (root == NIL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Node *root;
    initialize(&root);
    // A continuação incluirá leitura e comandos
    return 0;
}
