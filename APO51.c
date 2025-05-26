#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct AVLNode {
    int key;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

// Protótipos de funções
AVLNode* createAVLNode(int key);
int getHeight(AVLNode* node);
int max(int a, int b);
int calculateBalanceFactor(AVLNode* node);
AVLNode* rotateRight(AVLNode* y);
AVLNode* rotateLeft(AVLNode* x);
AVLNode* insertAVLNode(AVLNode* node, int key);
AVLNode* findMinValueNode(AVLNode* node);
AVLNode* deleteAVLNode(AVLNode* root, int key);
bool searchAVL(AVLNode* root, int key);
void rangeSearch(AVLNode* root, int low, int high, int* results, int* count);
void printInorder(AVLNode* root);
void printPreorder(AVLNode* root);
void printPostorder(AVLNode* root);
int countAVLNodes(AVLNode* root);
void freeAVLTree(AVLNode* root);
void sortArray(int* arr, int size);
void printTreeState(AVLNode* root);
void printNodeInfo(AVLNode* root, int key);

// Implementação das funções

AVLNode* createAVLNode(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    if (!node) {
        fprintf(stderr, "Erro na alocação de memória\n");
        exit(EXIT_FAILURE);
    }
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

int getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int calculateBalanceFactor(AVLNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

AVLNode* insertAVLNode(AVLNode* node, int key) {
    if (!node) return createAVLNode(key);

    if (key < node->key) {
        node->left = insertAVLNode(node->left, key);
    } else if (key > node->key) {
        node->right = insertAVLNode(node->right, key);
    } else {
        return node; // Chaves duplicadas não são permitidas
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = calculateBalanceFactor(node);

    // Casos de rotação
    if (balance > 1 && key < node->left->key)
        return rotateRight(node);
    
    if (balance < -1 && key > node->right->key)
        return rotateLeft(node);
    
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

AVLNode* findMinValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

AVLNode* deleteAVLNode(AVLNode* root, int key) {
    if (!root) return root;

    if (key < root->key) {
        root->left = deleteAVLNode(root->left, key);
    } else if (key > root->key) {
        root->right = deleteAVLNode(root->right, key);
    } else {
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            
            if (!temp) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            
            free(temp);
        } else {
            AVLNode* temp = findMinValueNode(root->right);
            root->key = temp->key;
            root->right = deleteAVLNode(root->right, temp->key);
        }
    }

    if (!root) return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = calculateBalanceFactor(root);

    // Balanceamento após remoção
    if (balance > 1 && calculateBalanceFactor(root->left) >= 0)
        return rotateRight(root);
    
    if (balance > 1 && calculateBalanceFactor(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    
    if (balance < -1 && calculateBalanceFactor(root->right) <= 0)
        return rotateLeft(root);
    
    if (balance < -1 && calculateBalanceFactor(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

bool searchAVL(AVLNode* root, int key) {
    if (!root) return false;
    
    if (key == root->key) return true;
    if (key < root->key) return searchAVL(root->left, key);
    return searchAVL(root->right, key);
}

void rangeSearch(AVLNode* root, int low, int high, int* results, int* count) {
    if (!root) return;
    
    if (root->key >= low && root->key <= high) {
        results[(*count)++] = root->key;
    }
    
    if (root->key > low) {
        rangeSearch(root->left, low, high, results, count);
    }
    
    if (root->key < high) {
        rangeSearch(root->right, low, high, results, count);
    }
}

void printInorder(AVLNode* root) {
    if (root) {
        printInorder(root->left);
        printf("%d ", root->key);
        printInorder(root->right);
    }
}

void printPreorder(AVLNode* root) {
    if (root) {
        printf("%d ", root->key);
        printPreorder(root->left);
        printPreorder(root->right);
    }
}

void printPostorder(AVLNode* root) {
    if (root) {
        printPostorder(root->left);
        printPostorder(root->right);
        printf("%d ", root->key);
    }
}

int countAVLNodes(AVLNode* root) {
    return root ? 1 + countAVLNodes(root->left) + countAVLNodes(root->right) : 0;
}

void freeAVLTree(AVLNode* root) {
    if (root) {
        freeAVLTree(root->left);
        freeAVLTree(root->right);
        free(root);
    }
}

void sortArray(int* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void printTreeState(AVLNode* root) {
    if (!root) {
        printf("ARVORE VAZIA\n");
        return;
    }
    
    printf("%d, %d, %d\n", 
           root->height - 1, 
           getHeight(root->left), 
           getHeight(root->right));
}

void printNodeInfo(AVLNode* root, int key) {
    if (!root) return;
    
    if (key == root->key) {
        printf("%d, %d, %d\n", 
               root->height - 1, 
               getHeight(root->left), 
               getHeight(root->right));
    } else if (key < root->key) {
        printNodeInfo(root->left, key);
    } else {
        printNodeInfo(root->right, key);
    }
}

int main() {
    AVLNode* root = NULL;
    int input, count;
    int initialValues[1000];
    int searchValues[1000];
    int range[2];
    
    // Leitura dos valores iniciais
    count = 0;
    while (scanf("%d", &input) == 1 && input >= 0) {
        initialValues[count++] = input;
    }
    
    // Construção da árvore inicial
    for (int i = 0; i < count; i++) {
        root = insertAVLNode(root, initialValues[i]);
    }
    
    // Saída após primeira fase
    printTreeState(root);
    
    // Leitura dos valores para busca/remoção
    count = 0;
    while (scanf("%d", &input) == 1 && input >= 0) {
        searchValues[count++] = input;
    }
    
    // Processamento da segunda fase
    for (int i = 0; i < count; i++) {
        if (searchAVL(root, searchValues[i])) {
            root = deleteAVLNode(root, searchValues[i]);
        } else {
            root = insertAVLNode(root, searchValues[i]);
        }
    }
    
    // Saída após segunda fase
    printTreeState(root);
    
    // Leitura do intervalo
    if (scanf("%d %d", &range[0], &range[1]) != 2) {
        fprintf(stderr, "Erro na leitura do intervalo\n");
        freeAVLTree(root);
        return EXIT_FAILURE;
    }
    
    // Busca no intervalo
    int results[1000];
    int resultCount = 0;
    rangeSearch(root, range[0], range[1], results, &resultCount);
    
    // Saída dos resultados
    if (resultCount == 0) {
        printf("NADA A EXIBIR\n");
    } else {
        sortArray(results, resultCount);
        for (int i = 0; i < resultCount; i++) {
            if (i > 0) printf(", ");
            printf("%d", results[i]);
        }
        printf("\n");
        
        for (int i = 0; i < resultCount; i++) {
            printNodeInfo(root, results[i]);
        }
    }
    
    // Liberação de memória
    freeAVLTree(root);
    
    return EXIT_SUCCESS;
}
