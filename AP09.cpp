#include <bits/stdc++.h>
using namespace std;

struct Node {
    vector<int> keys;
    vector<Node*> children;
    bool leaf;
    Node(bool leaf, int order): leaf(leaf) {
        keys.reserve(order-1);
        children.reserve(order);
    }
};

struct BTree {
    int order;
    Node* root;

    BTree(int order): order(order), root(new Node(true, order)) {}

    void insert(int k) {
        if(contains(k)) return;
        if((int)root->keys.size() == order-1) {
            Node* s = new Node(false, order);
            s->children.push_back(root);
            splitChild(s, 0);
            root = s;
        }
        insertNonFull(root, k);
    }

    bool contains(int k) { return search(root, k) != nullptr; }

    Node* search(Node* x, int k) {
        int i = 0;
        while(i < (int)x->keys.size() && k > x->keys[i]) i++;
        if(i < (int)x->keys.size() && k == x->keys[i]) return x;
        return x->leaf ? nullptr : search(x->children[i], k);
    }

    void insertNonFull(Node* x, int k) {
        int i = x->keys.size()-1;
        if(x->leaf) {
            x->keys.push_back(k);
            while(i >= 0 && x->keys[i] > k) {
                x->keys[i+1] = x->keys[i]; 
                i--;
            }
            x->keys[i+1] = k;
        } else {
            while(i >= 0 && x->keys[i] > k) i--;
            i++;
            if((int)x->children[i]->keys.size() == order-1) {
                splitChild(x, i);
                if(k > x->keys[i]) i++;
            }
            insertNonFull(x->children[i], k);
        }
    }

    void splitChild(Node* x, int i) {
        Node* y = x->children[i];
        Node* z = new Node(y->leaf, order);
        int t = (order-1)/2; // Median index (corrected)
        
        // Move the keys after t from y to z
        for(int j = t+1; j < y->keys.size(); j++) 
            z->keys.push_back(y->keys[j]);
        y->keys.resize(t);
        
        // Move the children after t from y to z if not leaf
        if(!y->leaf) {
            for(int j = t+1; j < y->children.size(); j++)
                z->children.push_back(y->children[j]);
            y->children.resize(t+1);
        }
        
        // Insert the median key into x
        x->keys.insert(x->keys.begin()+i, y->keys[t]);
        // Insert z as a new child of x
        x->children.insert(x->children.begin()+i+1, z);
    }
};

int main() {
    int order;
    cin >> order;
    BTree tree(order);
    unordered_set<int> seen;
    int value;
    while(cin >> value && value >= 0) {
        if(!seen.count(value)) {
            seen.insert(value);
            tree.insert(value);
        }
    }
    int searchVal;
    cin >> searchVal;
    cout << tree.root->keys.size() << "\n";
    Node* found = tree.search(tree.root, searchVal);
    if(!found) {
        cout << "Valor nao encontrado\n";
    } else {
        cout << found->keys.size() << "\n";
        for(int k: found->keys) cout << k << " ";
        cout << "\n";
    }
    return 0;
}
