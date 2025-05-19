#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void ordenacaoContagem(vector<string>& palavras, int posicao, const string& chave, vector<int>& vetorC) {
    int n = palavras.size();
    vector<string> auxiliar(n);
    vector<int> contagem(27, 0);

    for (int i = 0; i < n; i++) {
        char caractere = (posicao < palavras[i].size()) ? palavras[i][posicao] : ' ';
        int indice = 0;
        
        if (caractere == ' ') {
            indice = 0;
        } else {
            size_t encontrado = chave.find(caractere);
            if (encontrado != string::npos) {
                indice = encontrado + 1;
            }
        }
        contagem[indice]++;
    }

    for (int i = 1; i < 27; i++) {
        contagem[i] += contagem[i-1];
    }

    vetorC = contagem;

    for (int i = n-1; i >= 0; i--) {
        char caractere = (posicao < palavras[i].size()) ? palavras[i][posicao] : ' ';
        int indice = 0;
        
        if (caractere == ' ') {
            indice = 0;
        } else {
            size_t encontrado = chave.find(caractere);
            if (encontrado != string::npos) {
                indice = encontrado + 1;
            }
        }
        auxiliar[contagem[indice]-1] = palavras[i];
        contagem[indice]--;
    }

    palavras = auxiliar;
}

void ordenacaoRadix(vector<string>& palavras, int d, const string& chave) {
    vector<vector<int>> vetoresC(d, vector<int>(27));

    for (int i = d-1; i >= 0; i--) {
        ordenacaoContagem(palavras, i, chave, vetoresC[i]);
    }

    for (const auto& vetor : vetoresC) {
        for (int j = 0; j < 27; j++) {
            cout << vetor[j];
            if (j < 26) cout << " ";
        }
        cout << endl;
    }
}

int main() {
    int quantidade;
    cin >> quantidade;
    cin.ignore();

    string chave;
    getline(cin, chave);

    vector<string> palavras(quantidade);
    for (int i = 0; i < quantidade; i++) {
        cin >> palavras[i];
        transform(palavras[i].begin(), palavras[i].end(), palavras[i].begin(), ::tolower);
    }

    int tamanhoMaximo = 0;
    for (const auto& palavra : palavras) {
        tamanhoMaximo = max(tamanhoMaximo, (int)palavra.size());
    }

    for (auto& palavra : palavras) {
        palavra.resize(tamanhoMaximo, ' ');
    }

    for (const auto& palavra : palavras) {
        cout << palavra << "." << endl;
    }

    cout << tamanhoMaximo << endl;

    ordenacaoRadix(palavras, tamanhoMaximo, chave);

    for (const auto& palavra : palavras) {
        cout << palavra << endl;
    }

    return 0;
}
