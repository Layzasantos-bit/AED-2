// AP06 - Comparação entre Árvores AVL e Vermelho-Preto (AVP)
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

// Cria nó AVL
node *init_node(int v) {
    node *n = malloc(sizeof(node));
    n->valor = v;
    n->esq = n->dir = NULL;
    return n;
}

// Altura em nós (leaf → 0)
int alturaAVL(node *n) {
    if (!n) return 0;
    int l = alturaAVL(n->esq);
    int r = alturaAVL(n->dir);
    return (l > r ? l : r) + 1;
}

// Rotações AVL
node *rotDir(node *y) {
    rotacoes_avl++;
    node *x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    return x;
}
node *rotEsq(node *y) {
    rotacoes_avl++;
    node *x = y->dir;
    y->dir = x->esq;
    x->esq = y;
    return x;
}

// Insere e balanceia AVL
node *insAVL(node *n, int v) {
    if (!n) return init_node(v);
    if (v < n->valor) n->esq = insAVL(n->esq, v);
    else if (v > n->valor) n->dir = insAVL(n->dir, v);
    int fb = alturaAVL(n->esq) - alturaAVL(n->dir);
    if (fb > 1) {
        if (v < n->esq->valor) return rotDir(n);
        else { n->esq = rotEsq(n->esq); return rotDir(n); }
    }
    if (fb < -1) {
        if (v > n->dir->valor) return rotEsq(n);
        else { n->dir = rotDir(n->dir); return rotEsq(n); }
    }
    return n;
}

void insereAVL(tree *t, int v) {
    t->raiz = insAVL(t->raiz, v);
}

// ------------------------------ AVP (Red-Black) ------------------------------
typedef struct Node {
    int key, color; // 0=red, 1=black
    struct Node *l, *r, *p;
} Node;

typedef struct {
    Node *root;
} Tree;

int rotacoes_avp = 0, mudancas_cor_avp = 0;
Node *NIL;

// Cria sentinel NIL
typedef void (*VB)();
void defineRB() {
    NIL = calloc(1, sizeof(Node));
    NIL->color = 1;
    NIL->l = NIL->r = NIL->p = NIL;
}

// Rotações RBT
void left_rotate(Node *x, Tree *t) {
    rotacoes_avp++;
    Node *y = x->r;
    x->r = y->l;
    if (y->l != NIL) y->l->p = x;
    y->p = x->p;
    if (x->p == NIL) t->root = y;
    else if (x == x->p->l) x->p->l = y;
    else x->p->r = y;
    y->l = x;
    x->p = y;
}
void right_rotate(Node *x, Tree *t) {
    rotacoes_avp++;
    Node *y = x->l;
    x->l = y->r;
    if (y->r != NIL) y->r->p = x;
    y->p = x->p;
    if (x->p == NIL) t->root = y;
    else if (x == x->p->r) x->p->r = y;
    else x->p->l = y;
    y->r = x;
    x->p = y;
}

// Fixup RBT
void rb_fixup(Node *z, Tree *T) {
    while (z->p->color == 0) {
        Node *y;
        if (z->p == z->p->p->l) {
            y = z->p->p->r;
            if (y->color == 0) {
                z->p->color = y->color = 1;
                z->p->p->color = 0;
                mudancas_cor_avp += 3;
                z = z->p->p;
            } else {
                if (z == z->p->r) { z = z->p; left_rotate(z, T); }
                z->p->color = 1;
                z->p->p->color = 0;
                mudancas_cor_avp += 2;
                right_rotate(z->p->p, T);
            }
        } else {
            y = z->p->p->l;
            if (y->color == 0) {
                z->p->color = y->color = 1;
                z->p->p->color = 0;
                mudancas_cor_avp += 3;
                z = z->p->p;
            } else {
                if (z == z->p->l) { z = z->p; right_rotate(z, T); }
                z->p->color = 1;
                z->p->p->color = 0;
                mudancas_cor_avp += 2;
                left_rotate(z->p->p, T);
            }
        }
    }
    if (T->root->color == 0) { T->root->color = 1; }
}

// Insere RBT
void rb_insert(Tree *T, int key) {
    Node *z = calloc(1, sizeof(Node));
    z->key = key; z->color = 0;
    z->l = z->r = z->p = NIL;
    Node *y = NIL, *x = T->root;
    while (x != NIL) { y = x; x = (key < x->key ? x->l : x->r); }
    z->p = y;
    if (y == NIL) T->root = z;
    else if (key < y->key) y->l = z;
    else y->r = z;
    rb_fixup(z, T);
}

// Altura em nós da RBT (leaf → 0)
int alturaR(Node *n) {
    if (n == NIL) return 0;
    int l = alturaR(n->l);
    int r = alturaR(n->r);
    return (l > r ? l : r) + 1;
}

// Altura negra
int alturaNeg(Node *n) {
    if (n == NIL) return 0;
    int l = alturaNeg(n->l);
    int r = alturaNeg(n->r);
    return (l > r ? l : r) + (n->color == 1);
}

int main() {
    tree *avl = malloc(sizeof(tree)); avl->raiz = NULL;
    defineRB(); Tree *avp = malloc(sizeof(Tree)); avp->root = NIL;

    int x;
    while (scanf("%d", &x) && x >= 0) {
        insereAVL(avl, x);
        rb_insert(avp, x);
    }

    int ha = alturaAVL(avl->raiz) - 1;
    int hae = alturaAVL(avl->raiz->esq) - 1;
    int had = alturaAVL(avl->raiz->dir) - 1;

    int hr = alturaR(avp->root) - 1;
    int hre = alturaR(avp->root->l) - 1;
    int hrd = alturaR(avp->root->r) - 1;

    int hn = alturaNeg(avp->root);

    printf("%d, %d, %d\n", ha, hae, had);
    printf("%d, %d, %d\n", hr, hre, hrd);
    printf("%d\n", hn);
    printf("%d, %d, %d\n", mudancas_cor_avp, rotacoes_avp, rotacoes_avl);
    return 0;
}
