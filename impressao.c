#include "NetflixTAB.h"

void imprimirFilme(Filme aux){
    printf("%s/%d/%s/%s/%d\n", aux.titulo, aux.ano, aux.diretor, aux.genero, aux.duracao);
    return;
}

void imprimirFilme_formatado(Filme filme){
    printf("\
            Titulo: %s\n\
            Duracao: %dh%d     Ano: %d\n\
            Genero(s): %s\n\
            Diretor(es): %s\n",
    filme.titulo, filme.duracao/60, filme.duracao%60, filme.ano, filme.genero, filme.diretor);
    printf("\n");
    return;
}


void imprimirNO(TAB *node){
    for(int i = 0; i < node->nchaves; i++) imprimirFilme(node->chaves[i]);
    return;
}


void imprimirNO_formatado(TAB *node){
    for(int i = 0; i < node->nchaves; i++) imprimirFilme_formatado(node->chaves[i]);
    return;
}

void imprimir_no_a_no(int *qtd_nos, int t){
    printf("%d nos, de ordem %d.\n\n", *qtd_nos, t);

    int aux = 1;
    TAB *no = novoNo(t);
    char nome[12];
    while(aux <= *qtd_nos){
        sprintf(nome, "nodes/%d", aux);
        no = carregarNo(nome, no, t);
        if(no){
            printf("No %d:\n", aux);
            imprimirNO(no);

            int i;
            int possuiFilhos = 0;
            for(i = 0; i <= no->nchaves; i++)
                if(no->filhos[i]) possuiFilhos = 1;

            if(possuiFilhos){
                printf("Filhos: \n");
                for(i = 0; i <= no->nchaves; i++)
                    if(no->filhos[i]) printf("%d ", no->filhos[i]);
                printf("\n");
            }
            else printf("\nNao possui filhos.\n");
            printf("\n");
        }
        aux++;
    }
    free(no);
    return;
}


void imprimirCatalogo(int t, int primeiro){
    char first[12];
    sprintf(first, "nodes/%d", primeiro);

    TAB *no = novoNo(t);
    no = carregarNo(first, no, t);
    if(no){

        if (no->folha) {
                imprimirNO_formatado(no);
                return;
        }
        for(int i = 0; i < no->nchaves; i++) {
                imprimirCatalogo(t, no->filhos[i]);
                imprimirFilme_formatado(no->chaves[i]);
        }
        imprimirCatalogo(t, no->filhos[no->nchaves]);
    }
    return;
}


void Imprime(char *spai, TAB *a, int andar, int t){
  if(a){
    //printf("Entrei %d %s\n", andar, a->chaves[0].titulo);
    int i,j;
    for(i=0; i<= a->nchaves-1; i++){
        if(a->filhos[i]){
            sprintf(spai, "nodes/%d", a->filhos[i]);
            TAB * a = novoNo(t);
            a = carregarNo(spai, a, t);
            Imprime(spai,a,andar+1, t);
            }

        for(j=0; j<=andar; j++) printf("       ");
        printf("%s\n", a->chaves[i].titulo);

    }
    sprintf(spai, "nodes/%d", a->filhos[i]);
    a = carregarNo(spai, a, t);
    Imprime(spai, a, andar+1, t);
  }
}



void imprimeArvore(int pai, int qtd_filmes,int t){
    if(qtd_filmes == 0)
        return;
    char spai[12];
    sprintf(spai,"nodes/%d", pai);

    TAB *novo = novoNo(t);
    novo = carregarNo(spai,novo,t);
    Imprime(spai,novo,0, t);
    free(novo);
    printf("\n\n  Tecle algo para continuar...");
    scanf("%*c");
    return;
}
