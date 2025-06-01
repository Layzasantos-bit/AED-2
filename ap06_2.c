/*
 AP06 - Árvores AVL e Vermelho-Preto
 Implementação em C conforme especificado no material do curso
 Entrada: sequência de inteiros positivos, termina com inteiro negativo
 Saída:
   1) Altura total da árvore AVL, altura da subárvore esquerda e direita da raiz
   2) Altura total da árvore Vermelho-Preto, altura da subárvore esquerda e direita da raiz
   3) Altura negra da árvore Vermelho-Preto a partir da raiz
   4) Total de trocas de cor na RBT, total de rotações na RBT e total de rotações na AVL
*/

#include <stdio.h>
#include <stdlib.h>

#define RED 1    /* Cor vermelha em RBT */
#define BLACK 0  /* Cor preta em RBT */

typedef int tipochave;  /* Tipo de chave para nós das árvores */

/* Estrutura de nó para árvore AVL */
typedef struct noAVL {
    tipochave chave;
    struct noAVL *esq, *dir;
    int h;  /* Altura do nó (distância até folha mais distante) */
} noAVL;

/* Estrutura de nó para árvore Vermelho-Preto (RBT) */
typedef struct noRBT {
    tipochave chave;
    struct noRBT *esq, *dir, *pai;
    int cor;  /* RED ou BLACK */
} noRBT;

/* Retorna o máximo entre dois inteiros */
int max(int a, int b) { return a > b ? a : b; }

/* ================= Funções AVL ================= */

/* Retorna a altura do nó ou -1 se for NULL */
int alturaAVL(noAVL *raiz) {
    return raiz ? raiz->h : -1;
}

int rotacoesAVL = 0;  /* Contador de rotações realizadas na AVL */

/* Rotação simples à direita em k2 (caso LL) */
noAVL *rotacaoDireitaAVL(noAVL *k2) {
    noAVL *k1 = k2->esq;
    k2->esq = k1->dir;
    k1->dir = k2;
    /* Atualiza alturas após rotação */
    k2->h = max(alturaAVL(k2->esq), alturaAVL(k2->dir)) + 1;
    k1->h = max(alturaAVL(k1->esq), alturaAVL(k1->dir)) + 1;
    rotacoesAVL++;
    return k1;
}

/* Rotação simples à esquerda em k1 (caso RR) */
noAVL *rotacaoEsquerdaAVL(noAVL *k1) {
    noAVL *k2 = k1->dir;
    k1->dir = k2->esq;
    k2->esq = k1;
    /* Atualiza alturas após rotação */
    k1->h = max(alturaAVL(k1->esq), alturaAVL(k1->dir)) + 1;
    k2->h = max(alturaAVL(k2->esq), alturaAVL(k2->dir)) + 1;
    rotacoesAVL++;
    return k2;
}

/* Rotação dupla esquerda-direita (caso LR) */
noAVL *rotacaoEsqDirAVL(noAVL *raiz) {
    raiz->esq = rotacaoEsquerdaAVL(raiz->esq);
    return rotacaoDireitaAVL(raiz);
}

/* Rotação dupla direita-esquerda (caso RL) */
noAVL *rotacaoDirEsqAVL(noAVL *raiz) {
    raiz->dir = rotacaoDireitaAVL(raiz->dir);
    return rotacaoEsquerdaAVL(raiz);
}

/* Cria novo nó AVL com chave ch, filhos NULL, altura 0 */
noAVL *novoNoAVL(tipochave ch) {
    noAVL *no = (noAVL *)malloc(sizeof(noAVL));
    no->chave = ch;
    no->esq = no->dir = NULL;
    no->h = 0;
    return no;
}

/* Insere chave ch na árvore AVL apontada por raiz, balanceia conforme necessário */
noAVL *insereAVL(noAVL *raiz, tipochave ch) {
    if (!raiz) {
        /* Caso base: encontrou posição, cria nó novo */
        return novoNoAVL(ch);
    }
    if (ch < raiz->chave) {
        /* Insere à esquerda */
        raiz->esq = insereAVL(raiz->esq, ch);
        /* Verifica fator de balanceamento */
        if (alturaAVL(raiz->esq) - alturaAVL(raiz->dir) == 2) {
            if (ch < raiz->esq->chave) {
                /* Caso LL */
                raiz = rotacaoDireitaAVL(raiz);
            } else {
                /* Caso LR */
                raiz = rotacaoEsqDirAVL(raiz);
            }
        }
    }
    else if (ch > raiz->chave) {
        /* Insere à direita */
        raiz->dir = insereAVL(raiz->dir, ch);
        /* Verifica fator de balanceamento */
        if (alturaAVL(raiz->dir) - alturaAVL(raiz->esq) == 2) {
            if (ch > raiz->dir->chave) {
                /* Caso RR */
                raiz = rotacaoEsquerdaAVL(raiz);
            } else {
                /* Caso RL */
                raiz = rotacaoDirEsqAVL(raiz);
            }
        }
    }
    /* Atualiza altura desse nó */
    raiz->h = max(alturaAVL(raiz->esq), alturaAVL(raiz->dir)) + 1;
    return raiz;
}

/* ================= Funções RBT (Vermelho-Preto) ================= */

int rotacoesRBT = 0, trocasCor = 0;  /* Contadores para RBT */

/* Cria novo nó RBT com chave, filhos e pai NULL, cor RED */
noRBT *novoNoRBT(tipochave chave) {
    noRBT *no = (noRBT *)malloc(sizeof(noRBT));
    no->chave = chave;
    no->esq = no->dir = no->pai = NULL;
    no->cor = RED;
    return no;
}

/* Rotação simples à esquerda em x, retorna nova raiz da subárvore */
noRBT *rotacaoEsquerdaRBT(noRBT *raiz, noRBT *x) {
    noRBT *y = x->dir;
    x->dir = y->esq;
    if (y->esq) y->esq->pai = x;  /* Ajusta pai se existe filho */
    y->pai = x->pai;
    if (!x->pai) raiz = y;  /* x era raiz geral */
    else if (x == x->pai->esq) x->pai->esq = y;
    else x->pai->dir = y;
    y->esq = x;
    x->pai = y;
    rotacoesRBT++;
    return raiz;
}

/* Rotação simples à direita em x */
noRBT *rotacaoDireitaRBT(noRBT *raiz, noRBT *x) {
    noRBT *y = x->esq;
    x->esq = y->dir;
    if (y->dir) y->dir->pai = x;  /* Ajusta pai se existe filho */
    y->pai = x->pai;
    if (!x->pai) raiz = y;  /* x era raiz geral */
    else if (x == x->pai->dir) x->pai->dir = y;
    else x->pai->esq = y;
    y->dir = x;
    x->pai = y;
    rotacoesRBT++;
    return raiz;
}

/* Corrige propriedades de RBT após inserção de z */
noRBT *corrigeRBT(noRBT *raiz, noRBT *z) {
    while (z->pai && z->pai->cor == RED) {
        if (z->pai == z->pai->pai->esq) {
            noRBT *y = z->pai->pai->dir;  /* Tio de z */
            if (y && y->cor == RED) {
                /* Caso 1: pai e tio vermelhos -> recolore */
                z->pai->cor = BLACK;
                y->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
                trocasCor += 3;
            } else {
                if (z == z->pai->dir) {
                    /* Caso 2: triangle move */
                    z = z->pai;
                    raiz = rotacaoEsquerdaRBT(raiz, z);
                }
                /* Caso 3: line move */
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                raiz = rotacaoDireitaRBT(raiz, z->pai->pai);
                trocasCor += 2;
            }
        } else {
            /* Espelho do caso acima: pai é filho direito */
            noRBT *y = z->pai->pai->esq;
            if (y && y->cor == RED) {
                /* Caso 1 espelhado */
                z->pai->cor = BLACK;
                y->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
                trocasCor += 3;
            } else {
                if (z == z->pai->esq) {
                    /* Caso 2 espelhado */
                    z = z->pai;
                    raiz = rotacaoDireitaRBT(raiz, z);
                }
                /* Caso 3 espelhado */
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                raiz = rotacaoEsquerdaRBT(raiz, z->pai->pai);
                trocasCor += 2;
            }
        }
    }
    raiz->cor = BLACK;  /* Garante raiz preta */
    return raiz;
}

/* Insere chave em RBT e corrige propriedades */
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

/* Retorna altura (número de arestas) de RBT */
int alturaRBT(noRBT *raiz) {
    if (!raiz) return -1;
    int ae = alturaRBT(raiz->esq);
    int ad = alturaRBT(raiz->dir);
    return 1 + max(ae, ad);
}

/* Retorna altura negra (conta apenas nós pretos no caminho mais longo) */
int alturaNegra(noRBT *raiz) {
    if (!raiz) return 0;
    int ae = alturaNegra(raiz->esq);
    int ad = alturaNegra(raiz->dir);
    return max(ae, ad) + (raiz->cor == BLACK ? 1 : 0);
}

/* ================ Programa Principal ================ */
int main() {
    tipochave ch;
    noAVL *raizAVL = NULL;
    noRBT *raizRBT = NULL;

    /* Leitura da entrada: inteiro negativo encerra */
    while (scanf("%d", &ch) == 1) {
        if (ch < 0) break;
        raizAVL = insereAVL(raizAVL, ch);
        raizRBT = insereRBT(raizRBT, ch);
    }

    /* Cálculo e impressão das alturas da AVL */
    int hAVL = alturaAVL(raizAVL) + 1;           
    int heAVL = raizAVL->esq ? alturaAVL(raizAVL->esq) + 1 : 0;
    int hdAVL = raizAVL->dir ? alturaAVL(raizAVL->dir) + 1 : 0;
    printf("%d, %d, %d
", hAVL, heAVL, hdAVL);

    /* Cálculo e impressão das alturas da RBT */
    int hRBT = alturaRBT(raizRBT) + 1;
    int heRBT = raizRBT->esq ? alturaRBT(raizRBT->esq) + 1 : 0;
    int hdRBT = raizRBT->dir ? alturaRBT(raizRBT->dir) + 1 : 0;
    printf("%d, %d, %d
", hRBT, heRBT, hdRBT);

    /* Altura negra da RBT */
    printf("%d
", alturaNegra(raizRBT));

    /* Contadores finais: trocas de cor RBT, rotações RBT, rotações AVL */
    printf("%d, %d, %d
", trocasCor, rotacoesRBT, rotacoesAVL);

    return 0;
}
