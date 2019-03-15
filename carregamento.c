#include "NetflixTAB.h"

void gravarNo(char *nome, TAB *no, int t){
    FILE *arq = fopen(nome, "wb");
    if(!arq){
        printf("Nao foi possivel abrir o arquivo para gravacao.\n");
        exit(1);
    }
    fwrite(&no->folha, sizeof(int), 1, arq);
    fwrite(&no->nchaves, sizeof(int), 1, arq);
    fwrite(no->chaves, sizeof(Filme), (2*t -1), arq);
    fwrite(no->filhos, sizeof(int), 2*t, arq);
    fclose(arq);
    return;
}


void carregarFilme(Filme *filme){
    printf("\n\n        Titulo: ");
    scanf("\n%[^\n]s", (*filme).titulo);
    printf("        Ano de lancamento: ");
    scanf("\n%d", &(*filme).ano);
    printf("        Diretor: ");
    scanf("\n%[^\n]s", (*filme).diretor);
    printf("        Genero: ");
    scanf("\n%[^\n]s", (*filme).genero);
    printf("        Duracao: ");
    scanf("\n%d", &(*filme).duracao);
    return;
}


TAB* carregarNo(char *nome, TAB *no, int t){
    FILE *arq = fopen(nome, "rb");
    if(!arq){
        //printf("Nao foi possivel abrir o arquivo para leitura.\n");
        return NULL;
        //exit(1);
    }
    fread(&no->folha, sizeof(int), 1, arq);
    fread(&no->nchaves, sizeof(int), 1, arq);
    fread(no->chaves, sizeof(Filme), (2*t -1), arq);
    fread(no->filhos, sizeof(int), 2*t, arq);
    fclose(arq);
    return no;
}


void carregarCatalogo(char *nome, int t, int *qtd_nos, int *pai, int *qtd_filmes){
    FILE *filmes = fopen(nome, "r");
    if (!filmes){
        printf("        \n\nErro na abertura dos arquivos, certifique-se de que estejam aptos a edicao.");
        printf("\n\n  Tecle para continuar.\n   - ");
        limparStdin();
        scanf("%*c");
        return;
    }
    printf("\n\n        Criacao de catalogo iniciada.");
    printf("\n        Carregando...");

    if (*pai == 0){
        *qtd_nos = 1;
        *pai = 1;

        TAB *primeiro = novoNo(t);
        gravarNo("nodes/1", primeiro, t);
        free(primeiro);
    }

    int adicionados = 0;
    int atualizados = 0;
    Filme novo;
    while(fscanf(filmes, "%[^/]", novo.titulo) != EOF ){
        fscanf(filmes, "/%d", &novo.ano);
        fscanf(filmes, "/%[^/]", novo.diretor);
        fscanf(filmes, "/%[^/]", novo.genero);
        fscanf(filmes, "/%d\n", &novo.duracao);
        if(inserir_chave(novo, t, qtd_nos, pai, qtd_filmes))
            adicionados++;
        else
            atualizados++;
    }
    fclose(filmes);
    if(adicionados)
        printf("\n        Atualização de catálogo encerrada com sucesso. %d filme(s) adicionado(s).\n\n", adicionados);
    if(atualizados)
        printf("\n        Atualização de catálogo encerrada com sucesso. %d filme(s) atualizados(s).\n\n", atualizados);
    return;
}

