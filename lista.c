#include "NetflixTAB.h"


TL* novoNoLista(Filme filme){
    TL *novo = (TL *)malloc(sizeof(TL));
    novo->info = filme;
    novo->prox = NULL;
    return novo;
}

TL *add_inicio(TL *lista, Filme filme){
    TL *novo = novoNoLista(filme);
    if(!lista) return novo;
    novo->prox = lista;
    return novo;
}

TL *add_final(TL *lista, Filme filme){
    TL *novo = novoNoLista(filme);
    if(!lista) return novo;
    TL *aux = lista;
    while(aux->prox) aux = aux->prox;
    aux->prox = novo;
    return lista;
}

TL* liberarLista(TL *lista){
    if(!lista) return NULL;
    TL *aux = lista;
    lista = lista->prox;
    free(aux);
    liberarLista(lista);
    return NULL;
}

void imprimirLista(TL *lista){
    if(!lista) return;
    TL *aux = lista;
    while(aux){
        imprimirFilme_formatado(aux->info);
        aux = aux->prox;
    }
    return;
}


int size(TL *lista){
    int tam = 0;
    TL *aux = lista;
    while(aux){
        tam++;
        aux = aux->prox;
    }
    return tam;
}

