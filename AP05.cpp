#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Estrutura do nó da árvore AVL
struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
    
    Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

// Função para calcular a altura de um nó
int height(Node* node) {
    return node ? node->height : 0;
}

// Função para atualizar a altura de um nó
void updateHeight(Node* node) {
    if (node) {
        node->height = 1 + max(height(node->left), height(node->right));
    }
}

// Função para calcular o fator de balanceamento
int balanceFactor(Node* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// Rotação à direita
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    updateHeight(y);
    updateHeight(x);
    
    return x;
}

// Rotação à esquerda
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    updateHeight(x);
    updateHeight(y);
    
    return y;
}

// Balancear a árvore após inserção/remoção
Node* balance(Node* node) {
    if (!node) return nullptr;
    
    updateHeight(node);
    int bf = balanceFactor(node);
    
    // Caso esquerda-esquerda
    if (bf > 1 && balanceFactor(node->left) >= 0) {
        return rightRotate(node);
    }
    
    // Caso esquerda-direita
    if (bf > 1 && balanceFactor(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    // Caso direita-direita
    if (bf < -1 && balanceFactor(node->right) <= 0) {
        return leftRotate(node);
    }
    
    // Caso direita-esquerda
    if (bf < -1 && balanceFactor(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node;
}

// Inserir um nó na árvore AVL
Node* insert(Node* node, int key) {
    if (!node) return new Node(key);
    
    if (key < node->key) {
        node->left = insert(node->left, key);
    } else if (key > node->key) {
        node->right = insert(node->right, key);
    } else {
        return node; // Chaves duplicadas não são permitidas
    }
    
    return balance(node);
}

// Encontrar o nó com o valor mínimo (usado na remoção)
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

// Remover um nó da árvore AVL
Node* remove(Node* root, int key) {
    if (!root) return root;
    
    if (key < root->key) {
        root->left = remove(root->left, key);
    } else if (key > root->key) {
        root->right = remove(root->right, key);
    } else {
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            
            if (!temp) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp;
            }
            
            delete temp;
        } else {
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = remove(root->right, temp->key);
        }
    }
    
    if (!root) return root;
    
    return balance(root);
}

// Buscar um valor na árvore
bool search(Node* root, int key) {
    if (!root) return false;
    
    if (key == root->key) {
        return true;
    } else if (key < root->key) {
        return search(root->left, key);
    } else {
        return search(root->right, key);
    }
}

// Função para coletar nós dentro de uma faixa de valores
void rangeSearch(Node* root, int low, int high, vector<int>& result) {
    if (!root) return;
    
    if (root->key >= low && root->key <= high) {
        result.push_back(root->key);
    }
    
    if (root->key > low) {
        rangeSearch(root->left, low, high, result);
    }
    
    if (root->key < high) {
        rangeSearch(root->right, low, high, result);
    }
}

// Função para imprimir alturas (raiz, esquerda, direita)
void printHeights(Node* root) {
    if (!root) {
        cout << "ARVORE VAZIA" << endl;
        return;
    }
    
    cout << root->height - 1 << ", " 
         << height(root->left) << ", " 
         << height(root->right) << endl;
}

// Função para imprimir alturas de um nó específico
void printNodeHeights(Node* root, int key) {
    if (!root) return;
    
    if (root->key == key) {
        cout << root->height - 1 << ", " 
             << height(root->left) << ", " 
             << height(root->right) << endl;
    } else if (key < root->key) {
        printNodeHeights(root->left, key);
    } else {
        printNodeHeights(root->right, key);
    }
}

// Liberar memória da árvore
void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    Node* root = nullptr;
    vector<int> initialValues;
    vector<int> searchValues;
    int low, high;
    
    // Ler primeira linha (valores iniciais)
    int num;
    while (cin >> num && num >= 0) {
        initialValues.push_back(num);
    }
    
    // Construir árvore inicial
    for (int val : initialValues) {
        root = insert(root, val);
    }
    
    // Imprimir alturas após primeira linha
    printHeights(root);
    
    // Ler segunda linha (valores para pesquisa/remoção)
    while (cin >> num && num >= 0) {
        searchValues.push_back(num);
    }
    
    // Processar segunda linha
    for (int val : searchValues) {
        if (search(root, val)) {
            root = remove(root, val);
        } else {
            root = insert(root, val);
        }
    }
    
    // Imprimir alturas após segunda linha
    printHeights(root);
    
    // Ler terceira linha (faixa de valores)
    cin >> low >> high;
    
    // Buscar valores na faixa
    vector<int> rangeResult;
    rangeSearch(root, low, high, rangeResult);
    sort(rangeResult.begin(), rangeResult.end());
    
    // Imprimir resultados da faixa
    if (rangeResult.empty()) {
        cout << "NADA A EXIBIR" << endl;
    } else {
        for (size_t i = 0; i < rangeResult.size(); ++i) {
            if (i > 0) cout << ", ";
            cout << rangeResult[i];
        }
        cout << endl;
        
        // Imprimir alturas dos nós encontrados
        for (int val : rangeResult) {
            printNodeHeights(root, val);
        }
    }
    
    // Liberar memória
    freeTree(root);
    
    return 0;
}
