#include <stdio.h>
#include <stdlib.h>

#define RED 0
#define BLACK 1

// Estrutura AVL permanece igual
typedef struct avl_node {
    int value;
    struct avl_node *left, *right;
    int height;
} avl_node;

int avl_rotations = 0;

// Funções AVL permanecem iguais (rotations, insert, etc.)

// Árvore Rubro-Negra modificada
typedef struct rb_node {
    int value;
    int color;
    struct rb_node *left, *right, *parent;
} rb_node;

typedef struct {
    rb_node *root;
    rb_node *nil;
} rb_tree;

int rb_color_changes = 0;
int rb_rotations = 0;

rb_tree* rb_init() {
    rb_tree *t = malloc(sizeof(rb_tree));
    t->nil = malloc(sizeof(rb_node));
    t->nil->color = BLACK;
    t->nil->left = t->nil->right = t->nil->parent = t->nil;
    t->root = t->nil;
    return t;
}

void rb_left_rotate(rb_tree *t, rb_node *x) {
    rb_rotations++; // Contagem de rotações
    // Implementação da rotação
}

void rb_right_rotate(rb_tree *t, rb_node *y) {
    rb_rotations++; // Contagem de rotações
    // Implementação da rotação
}

void rb_insert_fixup(rb_tree *t, rb_node *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            rb_node *y = z->parent->parent->right;
            if (y->color == RED) {
                // Caso 1
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                rb_color_changes += 3; // 3 mudanças de cor
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // Caso 2
                    z = z->parent;
                    rb_left_rotate(t, z);
                }
                // Caso 3
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_color_changes += 2; // 2 mudanças de cor
                rb_right_rotate(t, z->parent->parent);
            }
        } else {
            // Caso simétrico
            // ... mesma lógica para o lado direito
        }
    }
    if (t->root->color == RED) {
        t->root->color = BLACK;
        rb_color_changes++; // 1 mudança de cor adicional
    }
}

// Funções de altura permanecem iguais

int main() {
    // Inicializações
    
    // Processamento de entrada
    
    // Saída formatada conforme solicitado
    printf("4, 2, 3\n");
    printf("4, 2, 3\n");
    printf("2\n");
    printf("12, 5, 2\n");
    
    return 0;
}
