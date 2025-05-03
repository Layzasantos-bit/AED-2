#include <stdio.h>

void selection_sort(int vetor[], int n, int *comparacoes, int *trocas) {
    *comparacoes = 0;
    *trocas = 0;

    for (int i = 0; i < n - 1; i++) {
        int min = i;

        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (vetor[j] < vetor[min]) {
                min = j;
            }
        }

        if (min != i) {
            int aux = vetor[i];
            vetor[i] = vetor[min];
            vetor[min] = aux;
            (*trocas)++;
        }
    }
}

int main() {
    int n;
    scanf("%d", &n);
    int vetor[1000];

    for (int i = 0; i < n; i++) {
        scanf("%d", &vetor[i]);
    }

    int comparacoes = 0, trocas = 0;
    selection_sort(vetor, n, &comparacoes, &trocas);

    printf("%d %d\n", comparacoes, trocas);

    return 0;
}
