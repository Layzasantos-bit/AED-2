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

// Inicializa árvore AVL
tree *init_tree() {
    tree *t = malloc(sizeof(tree));
    t->raiz = NULL;
    return t;
}

// Cria novo nó AVL
node *init_node(int valor) {
    node *n = malloc(sizeof(node));
    n->valor = valor;
    n->esq = n->dir = NULL;
    return n;
}

// Altura em arestas (leaf → 0)
int altura(node *n) {
    if (!n) return -1;
    int ae = altura(n->esq);
    int ad = altura(n->dir);
    return (ae > ad ? ae : ad) + 1;
}

// Rotações clássicas AVL
node *rot_dir(node *y) {
    rotacoes_avl++;
    node *x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    return x;
}

node *rot_esq(node *y) {
    rotacoes_avl++;
    node *x = y->dir;
    y->dir = x->esq;
    x->esq = y;
    return x;
}

// Insere e balanceia AVL
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

// Altura de subárvore AVL em arestas
int altura_sub(node *r) {
    return r ? altura(r) : -1;
}

// Libera memória AVL
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

// ------------------------------ AVP (Red-Black) ------------------------------
typedef struct Node {
    int key, color; // 0=red, 1=black
    struct Node *left, *right, *parent;
} Node;

typedef struct {
    Node *root;
} Tree;

int rotacoes_avp = 0;
int mudancas_cor_avp = 0;

// Nó nil (sentinela)
Node *make_nil() {
    Node *n = calloc(1, sizeof(Node));
    n->color = 1;
    n->left = n->right = n->parent = n;
    return n;
}

Tree *make_tree(Node *nil) {
    Tree *t = calloc(1, sizeof(Tree));
    t->root = nil;
    return t;
}

// Rotações Red-Black
void left_rotate(Node *x, Tree *t, Node *nil) {
    rotacoes_avp++;
    Node *y = x->right;
    x->right = y->left;
    if (y->left != nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nil) t->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void right_rotate(Node *x, Tree *t, Node *nil) {
    rotacoes_avp++;
    Node *y = x->left;
    x->left = y->right;
    if (y->right != nil) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nil) t->root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// Fixup após inserção
void rb_fixup(Node *z, Tree *t, Node *nil) {
    while (z->parent->color == 0) {
        Node *y;
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
                    left_rotate(z, t, nil);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                mudancas_cor_avp += 2;
                right_rotate(z->parent->parent, t, nil);
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
                    right_rotate(z, t, nil);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                mudancas_cor_avp += 2;
                left_rotate(z->parent->parent, t, nil);
            }
        }
    }
    if (t->root->color == 0) {
        t->root->color = 1;
        mudancas_cor_avp++;
    }
}

// Insere nó e conta mudança inicial
void rb_insert(Tree *t, int key, Node *nil) {
    Node *y = nil, *x = t->root;
    while (x != nil) {
        y = x;
        x = (key < x->key) ? x->left : x->right;
    }
    Node *z = calloc(1, sizeof(Node));
    z->key = key;
    z->color = 0;
    mudancas_cor_avp++; // coloração inicial
    z->left = z->right = nil;
    z->parent = y;
    if (y == nil) t->root = z;
    else if (key < y->key) y->left = z;
    else y->right = z;
    rb_fixup(z, t, nil);
}

// Altura em arestas para AVP (nil → -1)
int altura_no(Node *n, Node *nil) {
    if (n == nil) return -1;
    int ae = altura_no(n->left, nil);
    int ad = altura_no(n->right, nil);
    return (ae > ad ? ae : ad) + 1;
}

// Altura negra em nós pretos
int altura_negra(Node *n, Node *nil) {
    if (n == nil) return 0;
    int e = altura_negra(n->left, nil);
    int d = altura_negra(n->right, nil);
    return (e > d ? e : d) + (n->color == 1);
}

// Libera memória AVP
void liberar_avp(Node *n, Node *nil) {
    if (n == nil) return;
    liberar_avp(n->left, nil);
    liberar_avp(n->right, nil);
    free(n);
}

void liberar_memoria_avp(Tree *t, Node *nil) {
    liberar_avp(t->root, nil);
    free(nil);
    free(t);
}

// ------------------------------ Main ------------------------------
int main() {
    tree *avl = init_tree();
    Node *nil = make_nil();
    Tree *avp = make_tree(nil);

    int x;
    while (scanf("%d", &x) && x >= 0) {
        inserir_mod(avl, x);
        rb_insert(avp, x, nil);
    }

    int ha  = altura(avl->raiz);
    int hae = altura_sub(avl->raiz->esq);
    int had = altura_sub(avl->raiz->dir);
    int hr  = altura_no(avp->root, nil);
    int hre = altura_no(avp->root->left, nil);
    int hrd = altura_no(avp->root->right, nil);
    int hn  = altura_negra(avp->root, nil);

    printf("%d, %d, %d\n", ha,  hae, had);
    printf("%d, %d, %d\n", hr,  hre, hrd);
    printf("%d\n", hn);
    printf("%d, %d, %d\n", mudancas_cor_avp, rotacoes_avp, rotacoes_avl);

    liberar_memoria(avl);
    liberar_memoria_avp(avp, nil);
    return 0;
}
