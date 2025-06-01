#include <stdio.h>
#include <stdlib.h>

// ------------------------------ AVL ------------------------------
typedef struct avl_node {
    int value;
    struct avl_node *left, *right;
    int height;
} avl_node;

typedef struct {
    avl_node *root;
} avl_tree;

int avl_rotations = 0;

int avl_max(int a, int b) {
    return (a > b) ? a : b;
}

avl_tree* avl_init() {
    avl_tree *t = malloc(sizeof(avl_tree));
    t->root = NULL;
    return t;
}

avl_node* avl_create_node(int value) {
    avl_node *n = malloc(sizeof(avl_node));
    n->value = value;
    n->left = n->right = NULL;
    n->height = 1;
    return n;
}

int avl_height(avl_node *n) {
    return n ? n->height : 0;
}

avl_node* avl_rotate_right(avl_node *y) {
    avl_rotations++;
    avl_node *x = y->left;
    avl_node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;
    x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;

    return x;
}

avl_node* avl_rotate_left(avl_node *x) {
    avl_rotations++;
    avl_node *y = x->right;
    avl_node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;
    y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;

    return y;
}

avl_node* avl_insert(avl_node *node, int value) {
    if (!node) return avl_create_node(value);

    if (value < node->value)
        node->left = avl_insert(node->left, value);
    else if (value > node->value)
        node->right = avl_insert(node->right, value);
    else
        return node;

    node->height = 1 + avl_max(avl_height(node->left), avl_height(node->right));

    int balance = avl_height(node->left) - avl_height(node->right);

    if (balance > 1) {
        if (value < node->left->value)
            return avl_rotate_right(node);
        else {
            node->left = avl_rotate_left(node->left);
            return avl_rotate_right(node);
        }
    }

    if (balance < -1) {
        if (value > node->right->value)
            return avl_rotate_left(node);
        else {
            node->right = avl_rotate_right(node->right);
            return avl_rotate_left(node);
        }
    }

    return node;
}

// ------------------------------ RB Tree ------------------------------
typedef enum { RED, BLACK } Color;

typedef struct rb_node {
    int value;
    Color color;
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

rb_node* rb_create_node(int value, rb_tree *t) {
    rb_node *n = malloc(sizeof(rb_node));
    n->value = value;
    n->color = RED;
    n->left = n->right = n->parent = t->nil;
    return n;
}

void rb_left_rotate(rb_tree *t, rb_node *x) {
    rb_rotations++;
    rb_node *y = x->right;
    x->right = y->left;
    
    if (y->left != t->nil)
        y->left->parent = x;
    
    y->parent = x->parent;
    
    if (x->parent == t->nil)
        t->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    
    y->left = x;
    x->parent = y;
}

void rb_right_rotate(rb_tree *t, rb_node *y) {
    rb_rotations++;
    rb_node *x = y->left;
    y->left = x->right;
    
    if (x->right != t->nil)
        x->right->parent = y;
    
    x->parent = y->parent;
    
    if (y->parent == t->nil)
        t->root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;
    
    x->right = y;
    y->parent = x;
}

void rb_insert_fixup(rb_tree *t, rb_node *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            rb_node *y = z->parent->parent->right;
            
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                rb_color_changes += 3;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rb_left_rotate(t, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_color_changes += 2;
                rb_right_rotate(t, z->parent->parent);
            }
        } else {
            // Caso simétrico
            rb_node *y = z->parent->parent->left;
            
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                rb_color_changes += 3;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rb_right_rotate(t, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_color_changes += 2;
                rb_left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = BLACK;
    rb_color_changes++;
}

void rb_insert(rb_tree *t, int value) {
    rb_node *z = rb_create_node(value, t);
    rb_node *y = t->nil;
    rb_node *x = t->root;
    
    while (x != t->nil) {
        y = x;
        if (z->value < x->value)
            x = x->left;
        else
            x = x->right;
    }
    
    z->parent = y;
    
    if (y == t->nil)
        t->root = z;
    else if (z->value < y->value)
        y->left = z;
    else
        y->right = z;
    
    rb_insert_fixup(t, z);
}

int rb_height(rb_node *n, rb_node *nil) {
    if (n == nil) return -1;
    int left = rb_height(n->left, nil);
    int right = rb_height(n->right, nil);
    return (left > right ? left : right) + 1;
}

int rb_black_height(rb_node *n, rb_node *nil) {
    if (n == nil) return 0;
    int left = rb_black_height(n->left, nil);
    int right = rb_black_height(n->right, nil);
    return (n->color == BLACK ? 1 : 0) + (left > right ? left : right);
}

// ------------------------------ Main ------------------------------
int main() {
    avl_tree *avl = avl_init();
    rb_tree *rb = rb_init();
    
    int value;
    while (scanf("%d", &value) == 1 && value >= 0) {
        avl->root = avl_insert(avl->root, value);
        rb_insert(rb, value);
    }
    
    // AVL metrics
    int avl_h = avl_height(avl->root);
    int avl_lh = avl->root ? avl_height(avl->root->left) : 0;
    int avl_rh = avl->root ? avl_height(avl->root->right) : 0;
    
    // RB metrics
    int rb_h = rb_height(rb->root, rb->nil);
    int rb_lh = rb->root != rb->nil ? rb_height(rb->root->left, rb->nil) : 0;
    int rb_rh = rb->root != rb->nil ? rb_height(rb->root->right, rb->nil) : 0;
    int rb_bh = rb_black_height(rb->root, rb->nil);
    
    printf("%d, %d, %d\n", avl_h, avl_lh, avl_rh);
    printf("%d, %d, %d\n", rb_h, rb_lh, rb_rh);
    printf("%d\n", rb_bh);
    printf("%d, %d, %d\n", rb_color_changes, rb_rotations, avl_rotations);
    
    return 0;
}
