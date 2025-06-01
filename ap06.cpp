

#include <iostream>
#include <algorithm>
using namespace std;


enum Color { BLACK = 0, RED = 1 };


struct AVLNode {
    int chave;
    AVLNode* esq;
    AVLNode* dir;
    int h;
    
    AVLNode(int value) : chave(value), esq(nullptr), dir(nullptr), h(0) {}
};


struct RBNode {
    int chave;
    RBNode* esq;
    RBNode* dir;
    RBNode* pai;
    int cor;
    
    RBNode(int value) : chave(value), esq(nullptr), dir(nullptr), pai(nullptr), cor(RED) {}
};


int rotacoesAVL = 0;
int rotacoesRBT = 0;
int trocasCor = 0;


int maximo(int a, int b) { 
    return a > b ? a : b; 
}


int alturaAVL(AVLNode* raiz) { 
    return raiz ? raiz->h : -1; 
}

AVLNode* rotacaoDireitaAVL(AVLNode* k2) {
    AVLNode* k1 = k2->esq;
    k2->esq = k1->dir;
    k1->dir = k2;
    k2->h = maximo(alturaAVL(k2->esq), alturaAVL(k2->dir)) + 1;
    k1->h = maximo(alturaAVL(k1->esq), alturaAVL(k1->dir)) + 1;
    rotacoesAVL++;
    return k1;
}

AVLNode* rotacaoEsquerdaAVL(AVLNode* k1) {
    AVLNode* k2 = k1->dir;
    k1->dir = k2->esq;
    k2->esq = k1;
    k1->h = maximo(alturaAVL(k1->esq), alturaAVL(k1->dir)) + 1;
    k2->h = maximo(alturaAVL(k2->esq), alturaAVL(k2->dir)) + 1;
    rotacoesAVL++;
    return k2;
}

AVLNode* rotacaoEsqDirAVL(AVLNode* raiz) {
    raiz->esq = rotacaoEsquerdaAVL(raiz->esq);
    return rotacaoDireitaAVL(raiz);
}

AVLNode* rotacaoDirEsqAVL(AVLNode* raiz) {
    raiz->dir = rotacaoDireitaAVL(raiz->dir);
    return rotacaoEsquerdaAVL(raiz);
}

AVLNode* novoNoAVL(int ch) {
    AVLNode* no = new AVLNode(ch);
    return no;
}

AVLNode* insereAVL(AVLNode* raiz, int ch) {
    if (!raiz) return novoNoAVL(ch);
    
    if (ch < raiz->chave) {
        raiz->esq = insereAVL(raiz->esq, ch);
        if (alturaAVL(raiz->esq) - alturaAVL(raiz->dir) == 2) {
            if (ch < raiz->esq->chave) {
                raiz = rotacaoDireitaAVL(raiz);
            } else {
                raiz = rotacaoEsqDirAVL(raiz);
            }
        }
    } else if (ch > raiz->chave) {
        raiz->dir = insereAVL(raiz->dir, ch);
        if (alturaAVL(raiz->dir) - alturaAVL(raiz->esq) == 2) {
            if (ch > raiz->dir->chave) {
                raiz = rotacaoEsquerdaAVL(raiz);
            } else {
                raiz = rotacaoDirEsqAVL(raiz);
            }
        }
    }
    
    
    raiz->h = maximo(alturaAVL(raiz->esq), alturaAVL(raiz->dir)) + 1;
    return raiz;
}

RBNode* novoNoRBT(int chave) {
    RBNode* no = new RBNode(chave);
    return no;
}

RBNode* rotacaoEsquerdaRBT(RBNode* raiz, RBNode* x) {
    RBNode* y = x->dir;
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

RBNode* rotacaoDireitaRBT(RBNode* raiz, RBNode* x) {
    RBNode* y = x->esq;
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

RBNode* corrigeRBT(RBNode* raiz, RBNode* z) {
    while (z->pai && z->pai->cor == RED) {
        if (z->pai == z->pai->pai->esq) {
            RBNode* y = z->pai->pai->dir;
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
            RBNode* y = z->pai->pai->esq;
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

RBNode* insereRBT(RBNode* raiz, int chave) {
    RBNode* z = novoNoRBT(chave);
    RBNode* y = nullptr;
    RBNode* x = raiz;
    
    while (x) {
        y = x;
        if (chave < x->chave) {
            x = x->esq;
        } else if (chave > x->chave) {
            x = x->dir;
        } else {
            // Chave já existe - não insere duplicata
            delete z;
            return raiz;
        }
    }
    
    z->pai = y;
    if (!y) {
        raiz = z;
    } else if (chave < y->chave) {
        y->esq = z;
    } else {
        y->dir = z;
    }
    
    return corrigeRBT(raiz, z);
}

int alturaRBT(RBNode* raiz) {
    if (!raiz) return -1;
    int ae = alturaRBT(raiz->esq);
    int ad = alturaRBT(raiz->dir);
    return 1 + maximo(ae, ad);
}

int alturaNegra(RBNode* raiz) {
    if (!raiz) return 0;
    int alt = maximo(alturaNegra(raiz->esq), alturaNegra(raiz->dir));
    return alt + (raiz->cor == BLACK ? 1 : 0);
}

int main() {
    int ch;
    AVLNode* raizAVL = nullptr;
    RBNode* raizRBT = nullptr;
    
    while (cin >> ch) {
        if (ch < 0) break;
        raizAVL = insereAVL(raizAVL, ch);
        raizRBT = insereRBT(raizRBT, ch);
    }
    
    int hAVL = alturaAVL(raizAVL);
    int heAVL = raizAVL->esq ? alturaAVL(raizAVL->esq) + 1 : 0;
    int hdAVL = raizAVL->dir ? alturaAVL(raizAVL->dir) + 1 : 0;
    cout << hAVL + 1 << ", " << heAVL << ", " << hdAVL << endl;
    
    int hRBT = alturaRBT(raizRBT);
    int heRBT = raizRBT->esq ? alturaRBT(raizRBT->esq) + 1 : 0;
    int hdRBT = raizRBT->dir ? alturaRBT(raizRBT->dir) + 1 : 0;
    cout << hRBT + 1 << ", " << heRBT << ", " << hdRBT << endl;
    
 
    cout << alturaNegra(raizRBT) << endl;
    

    cout << trocasCor << ", " << rotacoesRBT << ", " << rotacoesAVL << endl;
    
    return 0;
}
