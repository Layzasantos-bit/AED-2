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

int countLeft(Node *raiz) {
    Node *aux = raiz;
    int cont = 0;
    if (aux->left == NIL) return 0;
    while (aux->left != NIL) {
        aux = aux->left;
        cont++;
        if (aux->left == NIL && aux->right != NIL) {
            while (aux->right != NIL) {
                aux = aux->right;
                cont++;
            }
        }
    }
    return cont;
}

int countRight(Node *raiz) {
    Node *aux = raiz;
    int cont = 0;
    if (aux->right == NIL) return 0;
    while (aux->right != NIL) {
        aux = aux->right;
        cont++;
        if (aux->right == NIL && aux->left != NIL) {
            while (aux->left != NIL) {
                aux = aux->left;
                cont++;
            }
        }
    }
    return cont;
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

void transplant(Node **root, Node *u, Node *v) {
    if (u->parent == NIL) *root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}

Node* treeMinimum(Node *x) {
    while (x->left != NIL) x = x->left;
    return x;
}

void fixDelete(Node **root, Node *x) {
    while (x != *root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(root, x->parent);
                x = *root;
            }
        } else {
            Node *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->color = BLACK;
}

void deleteNode(Node **root, Node *z) {
    Node *y = z;
    Node *x;
    int yOriginalColor = y->color;

    if (z->left == NIL) {
        x = z->right;
        transplant(root, z, z->right);
    } else if (z->right == NIL) {
        x = z->left;
        transplant(root, z, z->left);
    } else {
        y = treeMinimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == BLACK) fixDelete(root, x);
    free(z);
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

    // Primeira linha
    while (scanf("%d", &x) && x >= 0) insert(&root, x);
    printf("%d,%d,%d\n", height(root) - 1, countLeft(root), countRight(root));

    // Segunda linha
    while (scanf("%d", &x) && x >= 0) {
        Node *n = search(root, x);
        if (n != NIL) {
            printf("%d,%d,%d\n", height(n) - 1, countLeft(n), countRight(n));
            deleteNode(&root, n);
        } else {
            insert(&root, x);
        }
    }

    // Terceira linha
    if (scanf("%d", &x) == 1) {
        Node *n = search(root, x);
        if (n == NIL) printf("Valor nao encontrado\n");
        else printf("%d\n", redHeight(n));
    }

    freeTree(root);
    free(NIL);
    return 0;
}
