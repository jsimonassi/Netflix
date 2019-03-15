#include "NetflixTAB.h"

int buscaFilme_arquivo(int no_nome, Filme ch, int t, TAB *no,int *posicao){
    char node[12];
    sprintf(node, "nodes/%d", no_nome);
    no = carregarNo(node, no, t);
    if(!no)
        return 0;

    int ind = 0;
    while((ind < no->nchaves) && precedencia(no->chaves[ind], ch) < 0)
        ind++;

    if (precedencia(no->chaves[ind],ch) == 0){     // Encontrado.
            *posicao = ind; //Chave em que o filme foi encontrado
            return no_nome; //Retorna o arquivo em que o nó está
    }
    if (no->folha) return 0;
    return buscaFilme_arquivo(no->filhos[ind], ch, t, no, posicao);
}

int buscarFilme_titulo(int no_nome, char *titulo, int t, Filme *destino){
    char node[12];
    sprintf(node, "nodes/%d", no_nome);
    TAB *no = novoNo(t);
    no = carregarNo(node, no, t);
    if(!no)
        return 0;

    int ind = 0;
    while((ind < no->nchaves) && (strcmp(no->chaves[ind].titulo, titulo) < 0)) ind++;
    if (strcmp(no->chaves[ind].titulo, titulo) == 0) {     // Encontrado.
            *destino = no->chaves[ind];
            return 1;
    }
    if (no->folha) return 0;
    int filho = no->filhos[ind];
    free(no);
    return buscarFilme_titulo(filho, titulo, t, destino);
}

TL* buscarFilme_diretor(TL *lista, char *nome, int primeiro, int t, int qtd_nos){             // Não testado.
    int ind;
    char arquivo[12];
    TAB *node = novoNo(t);
    sprintf(arquivo, "nodes/%d", primeiro);
    node = carregarNo(arquivo, node, t);
    if(!node)
        return NULL;

    for(ind = 0; ind < node->nchaves; ind++){
        if (node->filhos[ind])
            lista = buscarFilme_diretor(lista, nome, node->filhos[ind], t, qtd_nos);
        if (strcmp(node->chaves[ind].diretor, nome) == 0)
            lista = add_final(lista, node->chaves[ind]);
    }
    if (node->filhos[ind])
        lista = buscarFilme_diretor(lista, nome, node->filhos[ind], t, qtd_nos);
    free(node);
    return lista;
}


TL* buscarFilme_ano(TL *lista, int ano, int primeiro, int t, int qtd_nos){
    int ind;
    char arquivo[12];
    TAB *node = novoNo(t);
    sprintf(arquivo, "nodes/%d", primeiro);
    node = carregarNo(arquivo, node, t);
    if(!node)
        return NULL;

    for(ind = 0; ind < node->nchaves; ind++){
        if (node->filhos[ind])
            lista = buscarFilme_ano(lista, ano, node->filhos[ind], t, qtd_nos);
        if (node->chaves[ind].ano == ano)
            lista = add_final(lista, node->chaves[ind]);
    }
    if (node->filhos[ind])
        lista = buscarFilme_ano(lista, ano, node->filhos[ind], t, qtd_nos);
    free(node);
    return lista;
}

TL* buscarFilme_genero(TL *lista, char *genero, int primeiro, int t, int qtd_nos){
    int ind;
    char arquivo[12];
    TAB *node = novoNo(t);
    sprintf(arquivo, "nodes/%d", primeiro);
    node = carregarNo(arquivo, node, t);
    if(!node)
        return NULL;

    for(ind = 0; ind < node->nchaves; ind++){
        if (node->filhos[ind])
            lista = buscarFilme_genero(lista, genero, node->filhos[ind], t, qtd_nos);
        if (strcmp(node->chaves[ind].genero, genero) == 0)
            lista = add_final(lista, node->chaves[ind]);
    }
    if (node->filhos[ind])
        lista = buscarFilme_genero(lista, genero, node->filhos[ind], t, qtd_nos);
    free(node);
    return lista;
}


TL* buscarFilme_duracao(TL *lista, int modo, int primeiro, int t, int qtd_nos){
    int ind, longa = 0;
    char arquivo[12];
    TAB *node = novoNo(t);
    sprintf(arquivo, "nodes/%d", primeiro);
    node = carregarNo(arquivo, node, t);

    for(ind = 0; ind < node->nchaves; ind++){
        if (node->filhos[ind])
            lista = buscarFilme_duracao(lista, modo, node->filhos[ind], t, qtd_nos);
        if (node->chaves[ind].duracao > 30) longa  = 1;
        if (modo == 1 && longa) lista = add_final(lista, node->chaves[ind]);
        else if (modo == -1 && !longa) lista = add_final(lista, node->chaves[ind]);
    }
    if (node->filhos[ind])
        lista = buscarFilme_duracao(lista, modo, node->filhos[ind], t, qtd_nos);
    free(node);
    return lista;
}



int precedencia(Filme a, Filme b){
    int res = strcmp(a.titulo, b.titulo);
    if(res == 0){
        if (a.ano < b.ano) return -1;
        else if (a.ano == b.ano) return 0;
        else return 1;
    }
    return res;
}

