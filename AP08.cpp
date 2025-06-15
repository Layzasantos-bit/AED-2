#include <iostream>
#include <vector>
using namespace std;

#define R -2
#define V -1

int f1(int k, int m) {
    return k % m;
}

int f2(int k, int m) {
    return 1 + (k % (m - 1));
}

int h(int k, int i, int m) {
    return (f1(k, m) + i * f2(k, m)) % m;
}

void insere(vector<int>& t, int m, int v) {
    for (int i = 0; i < m; i++) {
        int p = h(v, i, m);
        if (t[p] == V || t[p] == R) {
            t[p] = v;
            return;
        }
    }
}

vector<int> busca(const vector<int>& t, int m, int v) {
    vector<int> pos;
    for (int i = 0; i < m; i++) {
        int p = h(v, i, m);
        if (t[p] == V) break;
        if (t[p] == v) pos.push_back(p);
    }
    return pos;
}

void remove(vector<int>& t, int m, int v) {
    vector<int> pos = busca(t, m, v);
    for (int p : pos) {
        t[p] = R;
    }
}

void imprime(const vector<int>& t) {
    for (int i = 0; i < t.size(); i++) {
        if (t[i] == V) cout << "\\";
        else if (t[i] == R) cout << "D";
        else cout << t[i];
        if (i < t.size() - 1) cout << " ";
    }
    cout << endl;
}

int main() {
    int m;
    cin >> m;

    vector<int> t(m, V);

    int v;
    while (cin >> v && v != -1) {
        insere(t, m, v);
    }

    int k;
    cin >> k;

    vector<int> pos = busca(t, m, k);
    if (pos.empty()) {
        cout << "Valor nao encontrado" << endl;
    } else {
        remove(t, m, k);
    }

    imprime(t);

    return 0;
}
