#include <stdio.h>
#include <stdlib.h>

// ------------------------------ AVL ------------------------------
typedef struct node {
    int valor;
    struct node *esq, *dir;
} node;

typedef struct {
    node *raiz;
} tree;

int rotacoes_avl = 0;

tree *init_tree() {
    tree *t = malloc(sizeof(tree));
    t->raiz = NULL;
    return t;
}

node *init_node(int valor) {
    node *n = malloc(sizeof(node));
    n->valor = valor;
    n->esq = n->dir = NULL;
    return n;
}

int altura(node *n) {
    if (!n) return 0;
    int ae = altura(n->esq);
    int ad = altura(n->dir);
    return (ae > ad ? ae : ad) + 1;
}

node *rot_esq(node *y) {
    rotacoes_avl++;
    node *x = y->dir;
    y->dir = x->esq;
    x->esq = y;
    return x;
}

node *rot_dir(node *y) {
    rotacoes_avl++;
    node *x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    return x;
}

node *inserir_avl(node *n, int valor) {
    if (!n) return init_node(valor);
    if (valor < n->valor)
        n->esq = inserir_avl(n->esq, valor);
    else if (valor > n->valor)
        n->dir = inserir_avl(n->dir, valor);
    else
        return n;
    
    int fb = altura(n->esq) - altura(n->dir);
    if (fb > 1) {
        if (valor < n->esq->valor)
            return rot_dir(n);
        else {
            n->esq = rot_esq(n->esq);
            return rot_dir(n);
        }
    } else if (fb < -1) {
        if (valor > n->dir->valor)
            return rot_esq(n);
        else {
            n->dir = rot_dir(n->dir);
            return rot_esq(n);
        }
    }
    return n;
}

void inserir_mod(tree *t, int valor) {
    t->raiz = inserir_avl(t->raiz, valor);
}

int altura_sub(node *r) {
    return r ? altura(r) : 0;
}

void liberar_avl(node *n) {
    if (!n) return;
    liberar_avl(n->esq);
    liberar_avl(n->dir);
    free(n);
}

void liberar_memoria(tree *t) {
    liberar_avl(t->raiz);
    free(t);
}

// ------------------------------ AVP ------------------------------
typedef struct Node {
    int key, color; // 0=red, 1=black
    struct Node *left, *right, *parent;
} Node;

typedef struct {
    Node *root;
    Node *nil;
} Tree;

int rotacoes_avp = 0, mudancas_cor_avp = 0;

Node *make_nil() {
    Node *n = calloc(1, sizeof(Node));
    n->color = 1;
    n->key = 0;
    n->left = n->right = n->parent = NULL;
    return n;
}

Tree *make_tree() {
    Tree *t = calloc(1, sizeof(Tree));
    t->nil = make_nil();
    t->root = t->nil;
    return t;
}

void left_rotate(Tree *t, Node *x) {
    rotacoes_avp++;
    Node *y = x->right;
    x->right = y->left;
    if (y->left != t->nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == t->nil) t->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void right_rotate(Tree *t, Node *x) {
    rotacoes_avp++;
    Node *y = x->left;
    x->left = y->right;
    if (y->right != t->nil) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == t->nil) t->root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void rb_fixup(Tree *t, Node *z) {
    while (z->parent->color == 0) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if (y->color == 0) {
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;
                mudancas_cor_avp += 3;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(t, z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                mudancas_cor_avp += 2;
                right_rotate(t, z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if (y->color == 0) {
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;
                mudancas_cor_avp += 3;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(t, z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                mudancas_cor_avp += 2;
                left_rotate(t, z->parent->parent);
            }
        }
    }
    if (t->root->color == 0) {
        t->root->color = 1;
        mudancas_cor_avp++;
    }
}

void rb_insert(Tree *t, int key) {
    Node *z = calloc(1, sizeof(Node));
    z->key = key;
    z->color = 0;
    mudancas_cor_avp++;
    z->left = z->right = t->nil;
    
    Node *y = t->nil;
    Node *x = t->root;
    
    while (x != t->nil) {
        y = x;
        if (key < x->key) x = x->left;
        else x = x->right;
    }
    
    z->parent = y;
    if (y == t->nil) t->root = z;
    else if (key < y->key) y->left = z;
    else y->right = z;
    
    rb_fixup(t, z);
}

int altura_no(Node *n, Node *nil) {
    if (n == nil) return 0;
    int ae = altura_no(n->left, nil);
    int ad = altura_no(n->right, nil);
    return (ae > ad ? ae : ad) + 1;
}

int altura_negra(Node *n, Node *nil) {
    if (n == nil) return 0;
    int e = altura_negra(n->left, nil);
    int d = altura_negra(n->right, nil);
    return (e > d ? e : d) + (n->color == 1);
}

void liberar_avp(Node *n, Node *nil) {
    if (n == nil) return;
    liberar_avp(n->left, nil);
    liberar_avp(n->right, nil);
    free(n);
}

void liberar_memoria_avp(Tree *t) {
    liberar_avp(t->root, t->nil);
    free(t->nil);
    free(t);
}

// ------------------------------ Main ------------------------------
int main() {
    tree *avl = init_tree();
    Tree *avp = make_tree();

    int x;
    while (scanf("%d", &x) == 1 && x >= 0) {
        inserir_mod(avl, x);
        rb_insert(avp, x);
    }

    int ha = altura(avl->raiz);
    int hae = avl->raiz ? altura(avl->raiz->esq) : 0;
    int had = avl->raiz ? altura(avl->raiz->dir) : 0;
    
    int hr = altura_no(avp->root, avp->nil);
    int hre = avp->root != avp->nil ? altura_no(avp->root->left, avp->nil) : 0;
    int hrd = avp->root != avp->nil ? altura_no(avp->root->right, avp->nil) : 0;
    int hnegra = altura_negra(avp->root, avp->nil);

    printf("%d, %d, %d\n", ha, hae, had);
    printf("%d, %d, %d\n", hr, hre, hrd);
    printf("%d\n", hnegra);
    printf("%d, %d, %d\n", mudancas_cor_avp, rotacoes_avp, rotacoes_avl);

    liberar_memoria(avl);
    liberar_memoria_avp(avp);
    return 0;
}
