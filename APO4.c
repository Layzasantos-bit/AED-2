#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALAVRAS 100
#define MAX_TAMANHO 21

void ordenacao_contagem(char palavras[][MAX_TAMANHO], int n, int posicao, int ordem[26], int C[27]) {
    char auxiliar[MAX_PALAVRAS][MAX_TAMANHO];
    int contagem[27] = {0};

    for (int i = 0; i < n; i++) {
        char caractere = palavras[i][posicao];
        int indice = 0;
        if (caractere == ' ') {
            indice = 0;
        } else {
            for (int j = 0; j < 26; j++) {
                if (ordem[j] == caractere - 'a') {
                    indice = j + 1;
                    break;
                }
            }
        }
        contagem[indice]++;
    }

    for (int i = 1; i < 27; i++) {
        contagem[i] += contagem[i - 1];
    }

    for (int i = 0; i < 27; i++) {
        C[i] = contagem[i];
    }

    for (int i = n - 1; i >= 0; i--) {
        char caractere = palavras[i][posicao];
        int indice = 0;
        if (caractere == ' ') {
            indice = 0;
        } else {
            for (int j = 0; j < 26; j++) {
                if (ordem[j] == caractere - 'a') {
                    indice = j + 1;
                    break;
                }
            }
        }
        strcpy(auxiliar[contagem[indice] - 1], palavras[i]);
        contagem[indice]--;
    }

    for (int i = 0; i < n; i++) {
        strcpy(palavras[i], auxiliar[i]);
    }
}

void ordenacao_radix(char palavras[][MAX_TAMANHO], int n, int d, int ordem[26]) {
    for (int i = d - 1; i >= 0; i--) {
        int C[27] = {0};
        ordenacao_contagem(palavras, n, i, ordem, C);
        for (int j = 0; j < 27; j++) {
            printf("%d", C[j]);
            if (j < 26) printf(" ");
        }
        printf("\n");
    }
}

int main() {
    int quantidade;
    scanf("%d", &quantidade);
    char chave[27];
    scanf("%s", chave);
    int ordem[26];
    for (int i = 0; i < 26; i++) {
        ordem[i] = chave[i] - 'a';
    }

    char palavras[MAX_PALAVRAS][MAX_TAMANHO];
    int tamanho_maximo = 0;
    for (int i = 0; i < quantidade; i++) {
        scanf("%s", palavras[i]);
        int tamanho = strlen(palavras[i]);
        for (int j = 0; j < tamanho; j++) {
            palavras[i][j] = tolower(palavras[i][j]);
        }
        if (tamanho > tamanho_maximo) {
            tamanho_maximo = tamanho;
        }
    }

    for (int i = 0; i < quantidade; i++) {
        int tamanho = strlen(palavras[i]);
        for (int j = tamanho; j < tamanho_maximo; j++) {
            palavras[i][j] = ' ';
        }
        palavras[i][tamanho_maximo] = '\0';
    }

    for (int i = 0; i < quantidade; i++) {
        printf("%s.\n", palavras[i]);
    }

    printf("%d\n", tamanho_maximo);

    ordenacao_radix(palavras, quantidade, tamanho_maximo, ordem);

    for (int i = 0; i < quantidade; i++) {
        printf("%s\n", palavras[i]);
    }

    return 0;
}
