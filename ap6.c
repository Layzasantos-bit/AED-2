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

void liberar_memoria_impl(node *n) {
    if (!n) return;
    liberar_memoria_impl(n->esq);
    liberar_memoria_impl(n->dir);
    free(n);
}

void liberar_memoria(tree *t) {
    liberar_memoria_impl(t->raiz);
    free(t);
}

int altura(const node *n) {
    if (!n) return 0;
    int ae = altura(n->esq);
    int ad = altura(n->dir);
    return (ae > ad ? ae : ad) + 1;
}

int fator_bal(const node *n) {
    if (!n) return 0;
    int ae = n->esq ? altura(n->esq) : 0;
    int ad = n->dir ? altura(n->dir) : 0;
    return ae - ad;
}

int rotacoes_avl = 0;

node *rotacionar_esq_esq(node *n) {
    rotacoes_avl++;
    node *e = n->esq;
    n->esq = e->dir;
    e->dir = n;
    return e;
}

node *rotacionar_esq_dir(node *n) {
    rotacoes_avl++;
    node *e = n->esq;
    node *d = e->dir;
    n->esq = d->dir;
    e->dir = d->esq;
    d->esq = e;
    d->dir = n;
    return d;
}

node *rotacionar_dir_esq(node *n) {
    rotacoes_avl++;
    node *d = n->dir;
    node *e = d->esq;
    n->dir = e->esq;
    d->esq = e->dir;
    e->dir = d;
    e->esq = n;
    return e;
}

node *rotacionar_dir_dir(node *n) {
    rotacoes_avl++;
    node *d = n->dir;
    n->dir = d->esq;
    d->esq = n;
    return d;
}

node *balancear_node_mod(node *n) {
    if (!n) return NULL;
    n->esq = balancear_node_mod(n->esq);
    n->dir = balancear_node_mod(n->dir);
    int f = fator_bal(n);
    if (f >= 2) {
        if (fator_bal(n->esq) <= -1)
            return rotacionar_esq_dir(n);
        else
            return rotacionar_esq_esq(n);
    } else if (f <= -2) {
        if (fator_bal(n->dir) >= 1)
            return rotacionar_dir_esq(n);
        else
            return rotacionar_dir_dir(n);
    }
    return n;
}

void balancear_tree_mod(tree *t) {
    t->raiz = balancear_node_mod(t->raiz);
}

void inserir_mod(tree *t, int valor) {
    node *novo = init_node(valor);
    if (!t->raiz) {
        t->raiz = novo;
        return;
    }
    node *atual = t->raiz, *pai = NULL;
    while (atual) {
        pai = atual;
        if (valor < atual->valor)
            atual = atual->esq;
        else if (valor > atual->valor)
            atual = atual->dir;
        else
            return;
    }
    if (valor < pai->valor) pai->esq = novo;
    else pai->dir = novo;
    balancear_tree_mod(t);
}

int altura_subarvore(node *r) {
    return r ? altura(r) : 0;
}

// ------------------------------ AVP ------------------------------
typedef struct Node {
    int key, color; // 0=vermelho, 1=preto
    struct Node *left, *right, *parent;
} Node;

typedef struct {
    Node *root;
} Tree;

int rotacoes_avp = 0, mudancas_cor_avp = 0;

Node *make_t_nill() {
    Node *n = calloc(1, sizeof(Node));
    n->color = 1;
    n->key = 1000;
    n->left = n->right = n->parent = n;
    return n;
}

Tree *make_tree(Node *t_nill) {
    Tree *t = calloc(1, sizeof(Tree));
    t->root = t_nill;
    return t;
}

int altura_no(Node *r, Node *t_nill) {
    if (r == t_nill) return -1;
    int ae = altura_no(r->left, t_nill);
    int ad = altura_no(r->right, t_nill);
    return (ae > ad ? ae : ad) + 1;
}

int altura_negra(Node *n, Node *t_nill) {
    if (n == t_nill) return 0;
    int e = altura_negra(n->left, t_nill);
    int d = altura_negra(n->right, t_nill);
    int max = e > d ? e : d;
    return max + (n->color == 1 ? 1 : 0);
}

int left_rotate(Node *x, Tree *t) {
    rotacoes_avp++;
    Node *y = x->right;
    x->right = y->left;
    if (y->left && y->left->key != 1000) y->left->parent = x;
    y->left = x;
    y->parent = x->parent;
    x->parent = y;
    if (t->root == x) t->root = y;
    else if (y->parent->left == x) y->parent->left = y;
    else y->parent->right = y;
    return 1;
}

int right_rotate(Node *x, Tree *t) {
    rotacoes_avp++;
    Node *y = x->left;
    x->left = y->right;
    if (y->right && y->right->key != 1000) y->right->parent = x;
    y->right = x;
    y->parent = x->parent;
    x->parent = y;
    if (t->root == x) t->root = y;
    else if (y->parent->right == x) y->parent->right = y;
    else y->parent->left = y;
    return 1;
}

int RB_insert_fixup_conta(Node *z, Tree *t) {
    Node *y;
    while (z->parent->color == 0) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->color == 0) {
                z->parent->color = y->color = 1;
                z->parent->parent->color = 0;
                mudancas_cor_avp += 3;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(z, t);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                mudancas_cor_avp += 2;
                right_rotate(z->parent->parent, t);
            }
        } else {
            y = z->parent->parent->left;
            if (y->color == 0) {
                z->parent->color = y->color = 1;
                z->parent->parent->color = 0;
                mudancas_cor_avp += 3;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(z, t);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                mudancas_cor_avp += 2;
                left_rotate(z->parent->parent, t);
            }
        }
    }
    if (t->root->color == 0) {
        t->root->color = 1;
        mudancas_cor_avp++;
    }
    return 1;
}

int insert_node_conta(Tree *t, int key, Node *t_nill) {
    Node *y = t_nill, *x = t->root;
    while (x != t_nill) {
        y = x;
        x = (key < x->key) ? x->left : x->right;
    }
    Node *z = calloc(1, sizeof(Node));
    z->key = key;
    z->color = 0;
    z->left = z->right = t_nill;
    z->parent = y;
    if (y == t_nill) t->root = z;
    else if (key < y->key) y->left = z;
    else y->right = z;
    return RB_insert_fixup_conta(z, t);
}

void liberar_no_avp(Node *n, Node *t_nill) {
    if (n == t_nill) return;
    liberar_no_avp(n->left, t_nill);
    liberar_no_avp(n->right, t_nill);
    free(n);
}

void liberar_memoria_avp(Tree *t, Node *t_nill) {
    liberar_no_avp(t->root, t_nill);
    free(t_nill);
    free(t);
}

// ------------------------------ Main ------------------------------
int main() {
    tree *avl = init_tree();
    Node *t_nill = make_t_nill();
    Tree *avp = make_tree(t_nill);

    int x;
    while (scanf("%d", &x) && x >= 0) {
        inserir_mod(avl, x);
        insert_node_conta(avp, x, t_nill);
    }

    int ha = altura(avl->raiz);
    int hae = altura_subarvore(avl->raiz->esq);
    int had = altura_subarvore(avl->raiz->dir);
    int hr = altura_no(avp->root, t_nill);
    int hre = altura_no(avp->root->left, t_nill);
    int hrd = altura_no(avp->root->right, t_nill);
    int hnegra = altura_negra(avp->root, t_nill);

    printf("%d, %d, %d\n", ha, hae, had);
    printf("%d, %d, %d\n", hr, hre, hrd);
    printf("%d\n", hnegra);
    printf("%d, %d, %d\n", mudancas_cor_avp, rotacoes_avp, rotacoes_avl);

    liberar_memoria(avl);
    liberar_memoria_avp(avp, t_nill);

    return 0;
}
