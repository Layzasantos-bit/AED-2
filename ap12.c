#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define true 1
#define false 0
#define INF INT_MAX/2

typedef int bool;
typedef int tipopeso;

typedef struct adjacencia {
    int vertice;
    tipopeso peso;
    struct adjacencia *prox;
} adjacencia;

typedef struct vertice {
    adjacencia *cab;
} vertice;

typedef struct grafo {
    int vertices;
    int arestas;
    vertice *adj;
} grafo;

grafo* criagrafo(int v) {
    grafo *g = (grafo*) malloc(sizeof(grafo));
    g->vertices = v;
    g->arestas = 0;
    g->adj = (vertice*) malloc(v * sizeof(vertice));
    for (int i = 0; i < v; i++)
        g->adj[i].cab = NULL;
    return g;
}

void criaaresta(grafo *g, int origem, int destino, tipopeso peso) {
    adjacencia *nova = (adjacencia*) malloc(sizeof(adjacencia));
    nova->vertice = destino;
    nova->peso = peso;
    
    // Inserção ordenada para manter a lista de adjacência em ordem crescente
    adjacencia **atual = &(g->adj[origem].cab);
    while (*atual != NULL && (*atual)->vertice < destino) {
        atual = &((*atual)->prox);
    }
    nova->prox = *atual;
    *atual = nova;
    g->arestas++;
}

void inicializaD(grafo *g, int *d, int *p, int s) {
    for (int v = 0; v < g->vertices; v++) {
        d[v] = INF;
        p[v] = -1;
    }
    d[s] = 0;
}

void relaxa(grafo *g, int *d, int *p, int u, int v, int restricao) {
    adjacencia *ad = g->adj[u].cab;
    while (ad && ad->vertice != v)
        ad = ad->prox;
    
    if (ad && ad->peso <= restricao) {
        if (d[v] > d[u] + ad->peso) {
            d[v] = d[u] + ad->peso;
            p[v] = u;
        }
    }
}

bool bellmanFord(grafo *g, int s, int *d, int *p, int restricao) {
    inicializaD(g, d, p, s);
    
    for (int i = 1; i < g->vertices; i++) {
        for (int u = 0; u < g->vertices; u++) {
            adjacencia *ad = g->adj[u].cab;
            while (ad) {
                relaxa(g, d, p, u, ad->vertice, restricao);
                ad = ad->prox;
            }
        }
    }
    
    // Verificação de ciclos negativos (não necessário para o problema, mas incluído)
    for (int u = 0; u < g->vertices; u++) {
        adjacencia *ad = g->adj[u].cab;
        while (ad) {
            if (ad->peso <= restricao && d[ad->vertice] > d[u] + ad->peso) {
                return false; // Ciclo negativo encontrado
            }
            ad = ad->prox;
        }
    }
    return true;
}

void imprimirCaminho(int *p, int destino) {
    if (p[destino] == -1) {
        printf("%d", destino);
        return;
    }
    imprimirCaminho(p, p[destino]);
    printf(" %d", destino);
}

int main() {
    int V_inicial, R, N, M;
    scanf("%d %d %d %d", &V_inicial, &R, &N, &M);
    
    grafo *g = criagrafo(N);
    
    for (int i = 0; i < M; i++) {
        int origem, destino, peso;
        scanf("%d %d %d", &origem, &destino, &peso);
        criaaresta(g, origem, destino, peso);
    }
    
    int destino;
    scanf("%d", &destino);
    
    int *d = (int*) malloc(N * sizeof(int));
    int *p = (int*) malloc(N * sizeof(int));
    
    bellmanFord(g, V_inicial, d, p, R);
    
    // Saída das distâncias
    for (int i = 0; i < N; i++) {
        if (d[i] == INF) {
            printf("INF");
        } else {
            printf("%d", d[i]);
        }
        if (i != N - 1) printf(" ");
    }
    printf("\n");
    
    // Saída do caminho
    if (d[destino] == INF) {
        printf("Destino nao alcancado\n");
    } else {
        imprimirCaminho(p, destino);
        printf("\n");
    }
    
    free(d);
    free(p);
    // Liberar memória do grafo (implementar se necessário)
    return 0;
}
