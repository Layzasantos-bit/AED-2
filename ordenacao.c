#include <stdio.h>

void selection_sort(int v[], int n, int *comp, int *trocas) {
    *comp = 0;
    *trocas = 0;

    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            (*comp)++;
            if (v[j] < v[min]) {
                min = j;
            }
        }
        if (min != i) {
            int aux = v[i];
            v[i] = v[min];
            v[min] = aux;
            (*trocas)++;
        }
    }
}

void bubble_sort(int v[], int n, int *comp, int *trocas) {
    *comp = 0;
    *trocas = 0;
    int trocou;

    for (int i = 0; i < n - 1; i++) {
        trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            (*comp)++;
            if (v[j] > v[j + 1]) {
                int aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;
                (*trocas)++;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
}

void insertion_sort(int v[], int n, int *comp, int *mov) {
    *comp = 0;
    *mov = 0;

    for (int j = 1; j < n; j++) {
        int chave = v[j];
        int i = j - 1;

        while (i >= 0) {
            (*comp)++;
            if (v[i] > chave) {
                v[i + 1] = v[i];
                (*mov)++;
                i--;
            } else {
                break;
            }
        }

        if (i + 1 != j) {
            v[i + 1] = chave;
            (*mov)++;
        }
    }
}

int main() {
    int n;
    scanf("%d", &n);
    int original[1000];
    for (int i = 0; i < n; i++) {
        scanf("%d", &original[i]);
    }

    int v1[1000], v2[1000], v3[1000];
    for (int i = 0; i < n; i++) {
        v1[i] = v2[i] = v3[i] = original[i];
    }

    int c1, t1, c2, t2, c3, t3;

    selection_sort(v1, n, &c1, &t1);
    bubble_sort(v2, n, &c2, &t2);
    insertion_sort(v3, n, &c3, &t3);

    printf("%d %d\n", c1, t1);
    printf("%d %d\n", c2, t2);
    printf("%d %d\n", c3, t3);

    return 0;
}
