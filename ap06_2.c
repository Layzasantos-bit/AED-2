// Inclusões necessárias
#include <stdio.h>
#include <stdlib.h>
#include "avl.h"
#include "tree_red_black.h"

// Contadores globais
int rotacoes_avl = 0;
int rotacoes_avp = 0;
int mudancas_cor_avp = 0;

// Wrappers com contadores para AVL
node *rotacionar_esq_esq_conta(node *node_ptr) {
    rotacoes_avl++;
    return rotacionar_esq_esq(node_ptr);
}

node *rotationar_esq_dir_conta(node *node_ptr) {
    rotacoes_avl++;
    return rotationar_esq_dir(node_ptr);
}

node *rotacionar_dir_dir_conta(node *node_ptr) {
    rotacoes_avl++;
    return rotacionar_dir_dir(node_ptr);
}

node *rotacionar_dir_esq_conta(node *node_ptr) {
    rotacoes_avl++;
    return rotacionar_dir_esq(node_ptr);
}

// Substitui chamadas originais
node *balancear_node_mod(node *const node_ptr) {
    if (node_ptr->esq) node_ptr->esq = balancear_node_mod(node_ptr->esq);
    if (node_ptr->dir) node_ptr->dir = balancear_node_mod(node_ptr->dir);

    int fator = fator_bal(node_ptr);

    if (fator >= 2) {
        if (fator_bal(node_ptr->esq) <= -1)
            return rotationar_esq_dir_conta(node_ptr);
        else
            return rotacionar_esq_esq_conta(node_ptr);
    } else if (fator <= -2) {
        if (fator_bal(node_ptr->dir) >= 1)
            return rotacionar_dir_esq_conta(node_ptr);
        else
            return rotacionar_dir_dir_conta(node_ptr);
    }
    return node_ptr;
}

void balancear_tree_mod(tree *const tree_ptr) {
    node *nova_raiz = balancear_node_mod(tree_ptr->raiz);
    if (nova_raiz != tree_ptr->raiz)
        tree_ptr->raiz = nova_raiz;
}

void inserir_mod(tree *tree_ptr, int valor) {
    node *novo_node_ptr = NULL, *next_ptr = tree_ptr->raiz, *last_ptr = NULL;

    while (next_ptr != NULL) {
        last_ptr = next_ptr;
        if (valor < next_ptr->valor) next_ptr = next_ptr->esq;
        else if (valor > next_ptr->valor) next_ptr = next_ptr->dir;
        else return; // valor repetido
    }

    novo_node_ptr = init_node(valor);
    if (!last_ptr) tree_ptr->raiz = novo_node_ptr;
    else if (valor < last_ptr->valor) last_ptr->esq = novo_node_ptr;
    else last_ptr->dir = novo_node_ptr;

    balancear_tree_mod(tree_ptr);
}

// Altura da subárvore esquerda e direita da raiz
int altura_subarvore(node *raiz) {
    if (!raiz) return 0;
    return altura(raiz);
}

// Altura negra de uma AVP
int altura_negra(Node *no, Node *t_nill) {
    if (no == t_nill) return 0;
    int esq = altura_negra(no->left, t_nill);
    int dir = altura_negra(no->right, t_nill);
    int max_alt = esq > dir ? esq : dir;
    return max_alt + (no->color == 1 ? 1 : 0);
}

// Wrappers de rotação AVP com contador
int left_rotate_conta(Node *no, Tree *t) {
    rotacoes_avp++;
    return left_rotate(no, t);
}

int right_rotate_conta(Node *no, Tree *t) {
    rotacoes_avp++;
    return right_rotate(no, t);
}

// Altera função de ajuste para contar mudanças de cor e rotações na AVP
int RB_insert_fixup_conta(Node *no, Tree *t) {
    Node *tio;
    while (no->parent->color == 0) {
        if (no->parent == no->parent->parent->left) {
            tio = no->parent->parent->right;
            if (tio->color == 0) {
                no->parent->color = 1;
                tio->color = 1;
                no->parent->parent->color = 0;
                mudancas_cor_avp += 3;
                no = no->parent->parent;
            } else {
                if (no == no->parent->right) {
                    no = no->parent;
                    left_rotate_conta(no, t);
                }
                no->parent->color = 1;
                no->parent->parent->color = 0;
                mudancas_cor_avp += 2;
                right_rotate_conta(no->parent->parent, t);
            }
        } else {
            tio = no->parent->parent->left;
            if (tio->color == 0) {
                no->parent->color = 1;
                tio->color = 1;
                no->parent->parent->color = 0;
                mudancas_cor_avp += 3;
                no = no->parent->parent;
            } else {
                if (no == no->parent->left) {
                    no = no->parent;
                    right_rotate_conta(no, t);
                }
                no->parent->color = 1;
                no->parent->parent->color = 0;
                mudancas_cor_avp += 2;
                left_rotate_conta(no->parent->parent, t);
            }
        }
    }
    if (t->root->color == 0) {
        t->root->color = 1;
        mudancas_cor_avp++;
    }
    return 1;
}

int insert_node_conta(Tree *t, int info, Node *t_nill) {
    Node *aux = t->root, *ant = t->root;
    while (aux != t_nill) {
        ant = aux;
        aux = (info < aux->key) ? aux->left : aux->right;
    }

    Node *new_node = (Node *)calloc(1, sizeof(Node));
    new_node->key = info;
    new_node->color = 0;
    new_node->left = t_nill;
    new_node->right = t_nill;

    if (ant == t_nill) {
        t->root = new_node;
        new_node->parent = t_nill;
    } else {
        new_node->parent = ant;
        if (info < ant->key) ant->left = new_node;
        else ant->right = new_node;
    }

    return RB_insert_fixup_conta(new_node, t);
}

int main() {
    tree *avl = init_tree();
    Node *t_nill = make_t_nill();
    Tree *avp = make_tree(t_nill);

    int x;
    while (scanf("%d", &x) && x >= 0) {
        inserir_mod(avl, x);
        insert_node_conta(avp, x, t_nill);
    }

    int h_avl = altura(avl->raiz);
    int h_avl_e = altura_subarvore(avl->raiz->esq);
    int h_avl_d = altura_subarvore(avl->raiz->dir);

    int h_avp = altura_no(avp->root, t_nill);
    int h_avp_e = altura_no(avp->root->left, t_nill);
    int h_avp_d = altura_no(avp->root->right, t_nill);

    int hnegra = altura_negra(avp->root, t_nill);

    printf("%d, %d, %d\n", h_avl, h_avl_e, h_avl_d);
    printf("%d, %d, %d\n", h_avp, h_avp_e, h_avp_d);
    printf("%d\n", hnegra);
    printf("%d, %d, %d\n", mudancas_cor_avp, rotacoes_avp, rotacoes_avl);

    liberar_memoria(avl);
    return 0;
}
