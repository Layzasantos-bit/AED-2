#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

typedef int tipochave;

typedef struct noAVL {
    tipochave chave;
    struct noAVL *esq, *dir;
    int h;
} noAVL;

typedef struct noRBT {
    tipochave chave;
    struct noRBT *esq, *dir, *pai;
    int cor;
} noRBT;

int max(int a, int b) { return a > b ? a : b; }

int alturaAVL(noAVL *raiz) { return raiz ? raiz->h : -1; }
int rotacoesAVL = 0;

noAVL *rotacaoDireitaAVL(noAVL *k2) {
    noAVL *k1 = k2->esq;
    k2->esq = k1->dir;
    k1->dir = k2;
    k2->h = max(alturaAVL(k2->esq), alturaAVL(k2->dir)) + 1;
    k1->h = max(alturaAVL(k1->esq), alturaAVL(k1->dir)) + 1;
    rotacoesAVL++;
    return k1;
}

noAVL *rotacaoEsquerdaAVL(noAVL *k1) {
    noAVL *k2 = k1->dir;
    k1->dir = k2->esq;
    k2->esq = k1;
    k1->h = max(alturaAVL(k1->esq), alturaAVL(k1->dir)) + 1;
    k2->h = max(alturaAVL(k2->esq), alturaAVL(k2->dir)) + 1;
    rotacoesAVL++;
    return k2;
}

noAVL *rotacaoEsqDirAVL(noAVL *raiz) {
    raiz->esq = rotacaoEsquerdaAVL(raiz->esq);
    return rotacaoDireitaAVL(raiz);
}

noAVL *rotacaoDirEsqAVL(noAVL *raiz) {
    raiz->dir = rotacaoDireitaAVL(raiz->dir);
    return rotacaoEsquerdaAVL(raiz);
}

noAVL *novoNoAVL(tipochave ch) {
    noAVL *no = malloc(sizeof(noAVL));
    no->chave = ch;
    no->esq = no->dir = NULL;
    no->h = 0;
    return no;
}

noAVL *insereAVL(noAVL *raiz, tipochave ch) {
    if (!raiz) return novoNoAVL(ch);
    if (ch < raiz->chave) {
        raiz->esq = insereAVL(raiz->esq, ch);
        if (alturaAVL(raiz->esq) - alturaAVL(raiz->dir) == 2) {
            if (ch < raiz->esq->chave) raiz = rotacaoDireitaAVL(raiz);
            else raiz = rotacaoEsqDirAVL(raiz);
        }
    } else if (ch > raiz->chave) {
        raiz->dir = insereAVL(raiz->dir, ch);
        if (alturaAVL(raiz->dir) - alturaAVL(raiz->esq) == 2) {
            if (ch > raiz->dir->chave) raiz = rotacaoEsquerdaAVL(raiz);
            else raiz = rotacaoDirEsqAVL(raiz);
        }
    }
    raiz->h = max(alturaAVL(raiz->esq), alturaAVL(raiz->dir)) + 1;
    return raiz;
}

int rotacoesRBT = 0, trocasCor = 0;

noRBT *novoNoRBT(tipochave chave) {
    noRBT *no = malloc(sizeof(noRBT));
    no->chave = chave;
    no->esq = no->dir = no->pai = NULL;
    no->cor = RED;
    return no;
}

noRBT *rotacaoEsquerdaRBT(noRBT *raiz, noRBT *x) {
    noRBT *y = x->dir;
    x->dir = y->esq;
    if (y->esq) y->esq->pai = x;
    y->pai = x->pai;
    if (!x->pai) raiz = y;
    else if (x == x->pai->esq) x->pai->esq = y;
    else x->pai->dir = y;
    y->esq = x;
    x->pai = y;
    rotacoesRBT++;
    return raiz;
}

noRBT *rotacaoDireitaRBT(noRBT *raiz, noRBT *x) {
    noRBT *y = x->esq;
    x->esq = y->dir;
    if (y->dir) y->dir->pai = x;
    y->pai = x->pai;
    if (!x->pai) raiz = y;
    else if (x == x->pai->dir) x->pai->dir = y;
    else x->pai->esq = y;
    y->dir = x;
    x->pai = y;
    rotacoesRBT++;
    return raiz;
}

noRBT *corrigeRBT(noRBT *raiz, noRBT *z) {
    while (z->pai && z->pai->cor == RED) {
        if (z->pai == z->pai->pai->esq) {
            noRBT *y = z->pai->pai->dir;
            if (y && y->cor == RED) {
                z->pai->cor = BLACK;
                y->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
                trocasCor += 3;
            } else {
                if (z == z->pai->dir) {
                    z = z->pai;
                    raiz = rotacaoEsquerdaRBT(raiz, z);
                }
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                raiz = rotacaoDireitaRBT(raiz, z->pai->pai);
                trocasCor += 2;
            }
        } else {
            noRBT *y = z->pai->pai->esq;
            if (y && y->cor == RED) {
                z->pai->cor = BLACK;
                y->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
                trocasCor += 3;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    raiz = rotacaoDireitaRBT(raiz, z);
                }
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                raiz = rotacaoEsquerdaRBT(raiz, z->pai->pai);
                trocasCor += 2;
            }
        }
    }
    raiz->cor = BLACK;
    return raiz;
}

noRBT *insereRBT(noRBT *raiz, tipochave chave) {
    noRBT *z = novoNoRBT(chave);
    noRBT *y = NULL;
    noRBT *x = raiz;
    while (x) {
        y = x;
        if (chave < x->chave) x = x->esq;
        else x = x->dir;
    }
    z->pai = y;
    if (!y) raiz = z;
    else if (chave < y->chave) y->esq = z;
    else y->dir = z;
    return corrigeRBT(raiz, z);
}

int alturaRBT(noRBT *raiz) {
    if (!raiz) return -1;
    int ae = alturaRBT(raiz->esq);
    int ad = alturaRBT(raiz->dir);
    return 1 + max(ae, ad);
}

int alturaNegra(noRBT *raiz) {
    if (!raiz) return 0;
    int alt = max(alturaNegra(raiz->esq), alturaNegra(raiz->dir));
    return alt + (raiz->cor == BLACK ? 1 : 0);
}

int main() {
    tipochave ch;
    noAVL *raizAVL = NULL;
    noRBT *raizRBT = NULL;

    while (scanf("%d", &ch)) {
        if (ch < 0) break;
        raizAVL = insereAVL(raizAVL, ch);
        raizRBT = insereRBT(raizRBT, ch);
    }

    int hAVL = alturaAVL(raizAVL) + 1;
    int heAVL = raizAVL->esq ? alturaAVL(raizAVL->esq) + 1 : 0;
    int hdAVL = raizAVL->dir ? alturaAVL(raizAVL->dir) + 1 : 0;
    printf("%d, %d, %d
", hAVL, heAVL, hdAVL);

    int hRBT = alturaRBT(raizRBT) + 1;
    int heRBT = raizRBT->esq ? alturaRBT(raizRBT->esq) + 1 : 0;
    int hdRBT = raizRBT->dir ? alturaRBT(raizRBT->dir) + 1 : 0;
    printf("%d, %d, %d
", hRBT, heRBT, hdRBT);

    printf("%d
", alturaNegra(raizRBT));
    printf("%d, %d, %d
", trocasCor, rotacoesRBT, rotacoesAVL);
    return 0;
}
