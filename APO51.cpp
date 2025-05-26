#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;

    Node(int k) : key(k), left(nullptr), right(nullptr), height(0) {}
};

int getHeight(Node* node) {
    return node ? node->height : -1;
}

int getBalance(Node* node) {
    return getHeight(node->right) - getHeight(node->left);
}

void updateHeight(Node* node) {
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* B = x->right;

    x->right = y;
    y->left = B;

    updateHeight(y);
    updateHeight(x);
    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* B = y->left;

    y->left = x;
    x->right = B;

    updateHeight(x);
    updateHeight(y);
    return y;
}

Node* balance(Node* node) {
    updateHeight(node);
    int bf = getBalance(node);

    if (bf == -2) {
        if (getBalance(node->left) > 0)
            node->left = rotateLeft(node->left);
        return rotateRight(node);
    } else if (bf == 2) {
        if (getBalance(node->right) < 0)
            node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

Node* insert(Node* root, int key) {
    if (!root) return new Node(key);
    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    return balance(root);
}

Node* findMin(Node* node) {
    return node->left ? findMin(node->left) : node;
}

Node* removeMin(Node* node) {
    if (!node->left) return node->right;
    node->left = removeMin(node->left);
    return balance(node);
}

Node* remove(Node* root, int key) {
    if (!root) return nullptr;
    if (key < root->key)
        root->left = remove(root->left, key);
    else if (key > root->key)
        root->right = remove(root->right, key);
    else {
        Node* l = root->left;
        Node* r = root->right;
        delete root;
        if (!r) return l;
        Node* min = findMin(r);
        min->right = removeMin(r);
        min->left = l;
        return balance(min);
    }
    return balance(root);
}

bool search(Node* root, int key) {
    if (!root) return false;
    if (key == root->key) return true;
    return key < root->key ? search(root->left, key) : search(root->right, key);
}

void findInRange(Node* root, int low, int high, vector<int>& result) {
    if (!root) return;
    if (root->key >= low) findInRange(root->left, low, high, result);
    if (root->key >= low && root->key <= high) result.push_back(root->key);
    if (root->key <= high) findInRange(root->right, low, high, result);
}

Node* findNode(Node* root, int key) {
    if (!root || root->key == key) return root;
    return key < root->key ? findNode(root->left, key) : findNode(root->right, key);
}

void printHeights(Node* node) {
    if (!node) return;
    cout << getHeight(node) << ", " << getHeight(node->left) << ", " << getHeight(node->right) << endl;
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    Node* root = nullptr;
    int x;

    // Entrada 1
    while (cin >> x && x >= 0)
        root = insert(root, x);

    printHeights(root);

    // Entrada 2
    while (cin >> x && x >= 0) {
        if (search(root, x))
            root = remove(root, x);
        else
            root = insert(root, x);
    }

    if (!root)
        cout << "ARVORE VAZIA" << endl;
    else
        printHeights(root);

    // Entrada 3
    int a, b;
    cin >> a >> b;

    vector<int> faixa;
    findInRange(root, a, b, faixa);

    if (faixa.empty()) {
        cout << "NADA A EXIBIR" << endl;
    } else {
        sort(faixa.begin(), faixa.end());
        for (size_t i = 0; i < faixa.size(); ++i) {
            cout << faixa[i];
            if (i != faixa.size() - 1) cout << ", ";
        }
        cout << endl;
        for (int v : faixa) {
            Node* n = findNode(root, v);
            cout << getHeight(n) << ", " << getHeight(n->left) << ", " << getHeight(n->right) << endl;
        }
    }

    freeTree(root);
    return 0;
}
