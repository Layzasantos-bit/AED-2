#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó da árvore AVL
typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

// Função para criar um novo nó
Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// Função para calcular a altura de um nó
int height(Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Função para obter o máximo entre dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função para calcular o fator de balanceamento
int balanceFactor(Node* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Rotação à direita
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Rotação à esquerda
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Inserir um nó na árvore AVL
Node* insert(Node* node, int key) {
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node; // Chaves duplicadas não são permitidas

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = balanceFactor(node);

    // Caso esquerda-esquerda
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Caso direita-direita
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Caso esquerda-direita
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Caso direita-esquerda
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Encontrar o nó com o valor mínimo (usado na remoção)
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// Remover um nó da árvore AVL
Node* removeNode(Node* root, int key) {
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = removeNode(root->left, key);
    else if (key > root->key)
        root->right = removeNode(root->right, key);
    else {
        if (root->left == NULL || root->right == NULL) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }

            free(temp);
        } else {
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = removeNode(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = balanceFactor(root);

    // Caso esquerda-esquerda
    if (balance > 1 && balanceFactor(root->left) >= 0)
        return rightRotate(root);

    // Caso esquerda-direita
    if (balance > 1 && balanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Caso direita-direita
    if (balance < -1 && balanceFactor(root->right) <= 0)
        return leftRotate(root);

    // Caso direita-esquerda
    if (balance < -1 && balanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Buscar um valor na árvore
int search(Node* root, int key) {
    if (root == NULL)
        return 0;

    if (root->key == key)
        return 1;
    else if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}

// Função auxiliar para rangeSearch
void rangeSearchHelper(Node* root, int low, int high, int* result, int* index) {
    if (root == NULL)
        return;

    if (root->key >= low && root->key <= high) {
        result[(*index)++] = root->key;
    }

    if (root->key > low) {
        rangeSearchHelper(root->left, low, high, result, index);
    }

    if (root->key < high) {
        rangeSearchHelper(root->right, low, high, result, index);
    }
}

// Função para coletar nós dentro de uma faixa de valores
int* rangeSearch(Node* root, int low, int high, int* size) {
    int* result = (int*)malloc(100 * sizeof(int)); // Tamanho arbitrário grande
    *size = 0;
    rangeSearchHelper(root, low, high, result, size);
    return result;
}

// Função para imprimir alturas (raiz, esquerda, direita)
void printHeights(Node* root) {
    if (root == NULL) {
        printf("ARVORE VAZIA\n");
        return;
    }

    printf("%d, %d, %d\n", 
           root->height - 1, 
           height(root->left), 
           height(root->right));
}

// Função para imprimir alturas de um nó específico
void printNodeHeights(Node* root, int key) {
    if (root == NULL)
        return;

    if (root->key == key) {
        printf("%d, %d, %d\n", 
               root->height - 1, 
               height(root->left), 
               height(root->right));
    } else if (key < root->key) {
        printNodeHeights(root->left, key);
    } else {
        printNodeHeights(root->right, key);
    }
}

// Função para liberar a memória da árvore
void freeTree(Node* root) {
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Função para ordenar um array (usado para ordenar os resultados da busca por intervalo)
void sort(int* arr, int size) {
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

int main() {
    Node* root = NULL;
    int num;
    int initialValues[100];
    int initialSize = 0;
    int searchValues[100];
    int searchSize = 0;
    int low, high;

    // Ler primeira linha (valores iniciais)
    while (scanf("%d", &num) == 1 && num >= 0) {
        initialValues[initialSize++] = num;
    }

    // Construir árvore inicial
    for (int i = 0; i < initialSize; i++) {
        root = insert(root, initialValues[i]);
    }

    // Imprimir alturas após primeira linha
    printHeights(root);

    // Ler segunda linha (valores para pesquisa/remoção)
    while (scanf("%d", &num) == 1 && num >= 0) {
        searchValues[searchSize++] = num;
    }

    // Processar segunda linha
    for (int i = 0; i < searchSize; i++) {
        if (search(root, searchValues[i])) {
            root = removeNode(root, searchValues[i]);
        } else {
            root = insert(root, searchValues[i]);
        }
    }

    // Imprimir alturas após segunda linha
    printHeights(root);

    // Ler terceira linha (faixa de valores)
    scanf("%d %d", &low, &high);

    // Buscar valores na faixa
    int rangeSize;
    int* rangeResult = rangeSearch(root, low, high, &rangeSize);
    sort(rangeResult, rangeSize);

    // Imprimir resultados da faixa
    if (rangeSize == 0) {
        printf("NADA A EXIBIR\n");
    } else {
        for (int i = 0; i < rangeSize; i++) {
            if (i > 0) printf(", ");
            printf("%d", rangeResult[i]);
        }
        printf("\n");

        // Imprimir alturas dos nós encontrados
        for (int i = 0; i < rangeSize; i++) {
            printNodeHeights(root, rangeResult[i]);
        }
    }

    // Liberar memória
    free(rangeResult);
    freeTree(root);

    return 0;
}
