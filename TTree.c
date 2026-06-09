#include <stdio.h>
#include <stdlib.h>

#define MAX 4 // Tamanho máximo do vetor dentro de cada nó

// Estrutura do Nó da Árvore T
typedef struct No {
    int dados[MAX];
    int qtd;          // Quantos elementos existem no vetor atual.
    int altura;       // Para o balanceamento
    struct No *esq, *dir;
} No;

// FUNÇÕES DE APOIO 

int obterAltura(No *n) {
    return (n == NULL) ? 0 : n->altura;
}

int maior(int a, int b) {
    return (a > b) ? a : b;
}

void atualizarAltura(No *n) {
    if (n != NULL)
        n->altura = 1 + maior(obterAltura(n->esq), obterAltura(n->dir));
}

//  ROTAÇÕES 

No* rodarDireita(No *y) {
    No *x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    atualizarAltura(y);
    atualizarAltura(x);
    return x;
}

No* rodarEsquerda(No *x) {
    No *y = x->dir;
    x->dir = y->esq;
    y->esq = x;
    atualizarAltura(x);
    atualizarAltura(y);
    return y;
}

// INSERÇÃO NO VETOR 

void inserirNoVetor(No *n, int valor) {
    int i = n->qtd - 1;
    while (i >= 0 && n->dados[i] > valor) {
        n->dados[i + 1] = n->dados[i];
        i--;
    }
    n->dados[i + 1] = valor;
    n->qtd++;
}

//  INSERÇÃO NA ÁRVORE

No* inserir(No *n, int valor) {
    //  Se a árvore/ramo está vazia, cria o nó
    if (n == NULL) {
        No *novo = (No*)malloc(sizeof(No));
        novo->dados[0] = valor;
        novo->qtd = 1;
        novo->altura = 1;
        novo->esq = novo->dir = NULL;
        return novo;
    }

    // 2. Lógica da Árvore T: Verificar limites do nó atual
    int min = n->dados[0];
    int max = n->dados[n->qtd - 1];

    if (valor < min) {
        n->esq = inserir(n->esq, valor);
    } else if (valor > max) {
        n->dir = inserir(n->dir, valor);
    } else {
        // O valor está entre o min e o max, ou seja, "cabe" neste nó
        if (n->qtd < MAX) {
            inserirNoVetor(n, valor);
            return n;
        } else {
            
            n->esq = inserir(n->esq, valor);
        }
    }

  
    atualizarAltura(n);
    int fb = obterAltura(n->esq) - obterAltura(n->dir);

    if (fb > 1) {
        if (obterAltura(n->esq->esq) >= obterAltura(n->esq->dir))
            return rodarDireita(n);
        else {
            n->esq = rodarEsquerda(n->esq);
            return rodarDireita(n);
        }
    }
    if (fb < -1) {
        if (obterAltura(n->dir->dir) >= obterAltura(n->dir->esq))
            return rodarEsquerda(n);
        else {
            n->dir = rodarDireita(n->dir);
            return rodarEsquerda(n);
        }
    }

    return n;
}

void remover(No *n, int valor) {
    if (n == NULL)
        return;

    int min = n->dados[0];
    int max = n->dados[n->qtd - 1];

    if (valor < min) {
        remover(n->esq, valor);
    }
    else if (valor > max) {
        remover(n->dir, valor);
    }
    else {
        int pos = -1;

        for (int i = 0; i < n->qtd; i++) {
            if (n->dados[i] == valor) {
                pos = i;
                break;
            }
        }

        if (pos == -1)
            return;

        for (int i = pos; i < n->qtd - 1; i++) {
            n->dados[i] = n->dados[i + 1];
        }

        n->qtd--;

        printf("Valor %d removido.\n", valor);
    }
}

void imprimir(No *n) {
    if (n != NULL) {
        imprimir(n->esq);
        printf("[ ");
        for (int i = 0; i < n->qtd; i++) printf("%d ", n->dados[i]);
        printf("] ");
        imprimir(n->dir);
    }
}

