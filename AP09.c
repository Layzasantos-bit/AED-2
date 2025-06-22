#include <stdio.h>
#include <stdlib.h>

typedef struct Pagina {
    int numChaves;
    int *chaves;
    struct Pagina **filhos;
    int folha;
} Pagina;

int ordem; // ordem da arvore B

// Cria uma pagina nova
Pagina *criarPagina() {
    Pagina *p = (Pagina*)malloc(sizeof(Pagina));
    p->numChaves = 0;
    p->chaves = (int*)malloc((ordem*2-1)*sizeof(int));
    p->filhos = (Pagina**)malloc((ordem*2)*sizeof(Pagina*));
    p->folha = 1;
    for(int i=0;i<2*ordem;i++) p->filhos[i]=NULL;
    return p;
}

// Busca um valor
Pagina* busca(Pagina *raiz, int chave, int *pos) {
    int i = 0;
    while (i < raiz->numChaves && chave > raiz->chaves[i]) i++;
    if (i < raiz->numChaves && chave == raiz->chaves[i]) {
        if (pos) *pos = i;
        return raiz;
    }
    if (raiz->folha) return NULL;
    return busca(raiz->filhos[i], chave, pos);
}

// Divide o filho cheio
void splitChild(Pagina *pai, int i, Pagina *cheia) {
    Pagina *nova = criarPagina();
    nova->folha = cheia->folha;
    nova->numChaves = ordem-1;
    for(int j=0;j<ordem-1;j++)
        nova->chaves[j] = cheia->chaves[j+ordem];
    if(!cheia->folha)
        for(int j=0;j<ordem;j++)
            nova->filhos[j]=cheia->filhos[j+ordem];

    cheia->numChaves = ordem-1;

    for(int j=pai->numChaves;j>=i+1;j--) 
        pai->filhos[j+1]=pai->filhos[j];
    pai->filhos[i+1]=nova;

    for(int j=pai->numChaves-1;j>=i;j--) 
        pai->chaves[j+1]=pai->chaves[j];
    pai->chaves[i]=cheia->chaves[ordem-1];
    pai->numChaves++;
}

// Insere num nó que não está cheio
void inserirNaoCheio(Pagina *no, int chave) {
    int i = no->numChaves-1;
    if (no->folha) {
        while(i>=0 && no->chaves[i]>chave){
            no->chaves[i+1]=no->chaves[i]; i--;
        }
        no->chaves[i+1]=chave;
        no->numChaves++;
    } else {
        while(i>=0 && no->chaves[i]>chave) i--;
        i++;
        if (no->filhos[i]->numChaves==2*ordem-1) {
            splitChild(no,i,no->filhos[i]);
            if (chave>no->chaves[i]) i++;
        }
        inserirNaoCheio(no->filhos[i], chave);
    }
}

// Insere uma nova chave
Pagina* inserir(Pagina *raiz, int chave) {
    if(busca(raiz, chave, NULL)) return raiz; // evita repetidos
    if (raiz->numChaves==2*ordem-1) {
        Pagina *novaRaiz = criarPagina();
        novaRaiz->folha=0;
        novaRaiz->filhos[0]=raiz;
        splitChild(novaRaiz,0,raiz);
        inserirNaoCheio(novaRaiz,chave);
        return novaRaiz;
    } else {
        inserirNaoCheio(raiz,chave);
        return raiz;
    }
}

// Libera a memória
void libera(Pagina *no) {
    if(!no->folha) {
        for(int i=0;i<=no->numChaves;i++)
            libera(no->filhos[i]);
    }
    free(no->chaves);
    free(no->filhos);
    free(no);
}

int main() {
    scanf("%d",&ordem); 
    Pagina *raiz = criarPagina();
    int valor;
    while(scanf("%d",&valor)==1 && valor>=0) {
        raiz=inserir(raiz,valor);
    }
    int buscaValor;
    scanf("%d",&buscaValor);
    printf("%d\n", raiz->numChaves);

    int pos;
    Pagina *encontrado = busca(raiz,buscaValor,&pos);
    if(!encontrado) {
        printf("Valor nao encontrado\n");
    } else {
        printf("%d\n",encontrado->numChaves);
        for(int i=0;i<encontrado->numChaves;i++) {
            printf("%d%c", encontrado->chaves[i], i==encontrado->numChaves-1?'\n':' ');
        }
    }

    libera(raiz);
    return 0;
}
