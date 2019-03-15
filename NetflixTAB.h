#ifndef NETFLIXTAB_H_INCLUDED
#define NETFLIXTAB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct filme{
    char titulo[81], diretor[51], genero[31];
    int ano, duracao;
} Filme;

typedef struct no{
    Filme *chaves;
    int nchaves, folha;
    int *filhos;
} TAB;

typedef struct lista{
    Filme info;
    struct lista *prox;
} TL;


//++++++++++++++++ FUNÇÕES DE MANUPULAÇÃO DA LISTA +++++++++++++++++++++++++++++++++

    TL* novoNoLista(Filme filme);
    TL *add_inicio(TL *lista, Filme filme);
    TL *add_final(TL *lista, Filme filme);
    TL* liberarLista(TL *lista);
    void imprimirLista(TL *lista);
    int size(TL *lista);

//++++++++++++++++ DEMAIS FUNÇÕES +++++++++++++++++++++++++++++++++++

    //CARREGAMENTO +++++++++++++++++++++++++++++++++++++
    void gravarNo(char *nome, TAB *no, int t);// OK
    void carregarFilme(Filme *filme);// OK
    TAB* carregarNo(char *nome, TAB *no, int t);// OK
    void carregarCatalogo(char *nome, int t, int *qtd_nos, int *pai, int *qtd_filmes);// OK

    // IMPRESSAO +++++++++++++++++++++++++++++++++++++++
    void imprimirFilme(Filme aux);
    void imprimirFilme_formatado(Filme filme);
    void imprimirNO(TAB *node);
    void imprimirNO_formatado(TAB *node);
    void imprimir_no_a_no(int *qtd_nos, int t);
    void imprimirCatalogo(int t, int primeiro);
    void imprimeArvore(int pai, int qtd_filmes,int t);
    void Imprime(char *spai, TAB *a, int andar, int t);

    // Entrada / Saída +++++++++++++++++++++++++++++++++++++
    TAB* novoNo(int t);// OK
    TAB* dividir(TAB *x, int pos, TAB *y, int t, int *qtd_nos);// OK
    TAB* inserir_nao_completo(TAB *a, int t, Filme filme, int *qtd_nos);// OK
    int inserir_chave(Filme ch, int t, int *qtd_nos, int *pai, int *qtd_filmes);// OK
    void destruirCatalogo(int *pai, int *ordem, int *qtd_nos, int *qtd_filmes);// OK
    TAB *remover_chave(int primeiro, int *pai, Filme ch, int *qtd_nos, int *qtd_filmes, int t);// OK
    void remover_lista_de_chaves(TL *lista, int *pai, int *qtd_nos, int *qtd_filmes, int t);// OK

    // BUSCAS +++++++++++++++++++++++++++++++++++++++++
    int buscaFilme_arquivo(int no_nome, Filme ch, int t, TAB *no,int *posicao);
    int buscarFilme_titulo(int no_nome, char *titulo, int t, Filme *destino);// OK
    TL* buscarFilme_diretor(TL *lista, char *nome, int primeiro, int t, int qtd_nos);// OK
    TL* buscarFilme_ano(TL *lista, int ano, int primeiro, int t, int qtd_nos);// OK
    TL* buscarFilme_genero(TL *lista, char *genero, int primeiro, int t, int qtd_nos);// OK
    TL* buscarFilme_duracao(TL *lista, int modo, int primeiro, int t, int qtd_nos);
    int precedencia(Filme a, Filme b);

    // MENU, LIMPEZA DE TELA/BUFFER +++++++++++++++++++
    void menu_busca(TL* lista, int *pai, int *qtd_nos, int *ordem, int *qtd_filmes);// Ok
    void menu_principal(TL *lista, int *pai, int *qtd_nos, int *ordem, int *qtd_filmes);// Ok
    void strLower(char *str);// Ok
    void strUpper(char *str);// Ok
    void limparStdin(void);// OK
    void limparTela(void);// OK


#endif // NETFLIXTAB_H_INCLUDED
