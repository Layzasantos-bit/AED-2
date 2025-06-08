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

// Inicializa NIL e árvore
void initialize(Node **root) {
    NIL = (Node*)malloc(sizeof(Node));
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL->parent = NIL;
    *root = NIL;
}

Node* createNode(int key) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->color = RED;
    node->left = NIL;
    node->right = NIL;
    node->parent = NIL;
    return node;
}

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
    fixInsert(root, z);
}

int height(Node *node) {
    if (node == NIL) return 0;
    int l = height(node->left);
    int r = height(node->right);
    return (l > r ? l : r) + 1;
}

int redHeight(Node *node) {
    if (node == NIL) return 0;
    int lh = redHeight(node->left);
    int rh = redHeight(node->right);
    return ((lh > rh ? lh : rh) + (node->color == RED));
}

Node* search(Node *root, int key) {
    while (root != NIL && root->key != key) {
        if (key < root->key) root = root->left;
        else root = root->right;
    }
    return root;
}

void freeTree(Node *root) {
    if (root == NIL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Node *root;
    initialize(&root);
    int x;

    // Primeira linha: inserções iniciais
    while (scanf("%d", &x) && x >= 0) insert(&root, x);

    // Imprime altura da árvore original (número de arestas)
    printf("%d,%d,%d\n", height(root) - 1, height(root->left) - 1, height(root->right) - 1);

    // Segunda linha: busca e remove se encontrar (remoção ainda não implementada)
    while (scanf("%d", &x) && x >= 0) {
        Node *found = search(root, x);
        if (found != NIL) {
            printf("%d,%d,%d\n", height(found) - 1, height(found->left) - 1, height(found->right) - 1);
            // remoção com rebalanceamento aqui
        } else insert(&root, x);
    }

    // Terceira linha: altura rubro
    if (scanf("%d", &x) == 1) {
        Node *n = search(root, x);
        if (n == NIL) printf("Valor nao encontrado\n");
        else printf("%d\n", redHeight(n));
    }

    freeTree(root);
    free(NIL);
    return 0;
}
