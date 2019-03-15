#include "NetflixTAB.h"

void strLower(char *str){

    int i = 0;
    while (str[i]){
      str[i]=tolower(str[i]);
      i++;
   }
}


void strUpper(char *str){

    int i = 0;
    while (str[i]){
      str[i]=toupper(str[i]);
      i++;
   }
}


void limparStdin(void){
    int ch;
    do {
        ch = fgetc(stdin);
    } while (ch != EOF && ch != '\n');
}

void limparTela(void){

    #ifdef linux
    printf("\e[H\e[2J");

    #elif defined WIN32 || win32 || Win32
    system ("cls");
    #endif

    return;
}


void menu_busca(TL* lista, int *pai, int *qtd_nos, int *ordem, int *qtd_filmes){

    int voltar = 0, opcao, acao_seguinte, criterio_num;
    char criterio_txt[81];

    while(!voltar){
        limparTela();
        printf("\n            PROJETO NETFLIX\n\n\
                        MENU:\n\n\
                        [1] Buscar por titulo.\n\
                        [2] Buscar por diretor.\n\
                        [3] Buscar por ano de lancamento.\n\
                        [4] Buscar por genero.\n\
                        [5] Buscar curta-metragens.\n\
                        [6] Buscar longa-metragens.\n\n\
                        [0] Voltar.\n\n  - ");
        scanf("%i", &opcao);
        switch(opcao){

            //Título
            case 1:

                printf("\n\n        Digite o titulo: ");
                scanf(" %[^\n]s", criterio_txt);

                printf("\n");
                Filme procurado;
                if (buscarFilme_titulo(*pai, criterio_txt, *ordem, &procurado)){
                    imprimirFilme_formatado(procurado);
                    printf("\n\n  Deseja remover o filme? (1/0)\n   - ");
                    scanf(" %d", &acao_seguinte);
                    if (acao_seguinte){
                        remover_chave(*pai, pai, procurado, qtd_nos, qtd_filmes, *ordem);
                        printf("\n  Removido com sucesso do catalogo.");
                    }
                }
                else
                    printf("\n     Filme nao encontrado no catalogo.\n");

                printf("\n\n  Tecle algo para continuar...");
                limparStdin();
                scanf("%*c");
                voltar = 1;
                break;

            // Diretor
            case 2:

                printf("\n      Digite o nome do diretor: ");
                scanf(" %[^\n]s", criterio_txt);

                lista = buscarFilme_diretor(lista, criterio_txt, *pai, *ordem, *qtd_nos);

                if (lista) {
                        imprimirLista(lista);
                        lista = liberarLista(lista);
                }
                else
                    printf("\n      Nada encontrado.\n");

                printf("\n\n  Tecle algo para continuar...");
                limparStdin();
                scanf("%*c");
                voltar = 1;
                break;

            // Ano
            case 3:

                printf("\n\n        Digite o ano de lancamento: ");
                scanf("%d", &criterio_num);

                lista = buscarFilme_ano(lista, criterio_num, *pai, *ordem, *qtd_nos);
                if(lista){
                    printf("\n      %d filme(s) encontrados.\n\n", size(lista));
                    imprimirLista(lista);
                    printf("\n\n  Deseja remover todos? (1/0)\n   - ");
                    scanf(" %d", &acao_seguinte);
                    if (acao_seguinte){
                        remover_lista_de_chaves(lista, pai, qtd_nos, qtd_filmes, *ordem);
                        printf("\n  Removido com sucesso do catalogo.");
                    }
                    lista = liberarLista(lista);
                }
                else
                    printf("\n      Nada encontrado.\n");

                printf("\n\n  Tecle algo para continuar...");
                limparStdin();
                scanf("%*c");
                voltar = 1;
                break;

            // Gênero
            case 4:

                printf("\n\n        Digite o genero: ");
                scanf(" %[^\n]s", criterio_txt);
                printf("%s\n", criterio_txt);

                lista = buscarFilme_genero(lista, criterio_txt, *pai, *ordem, *qtd_nos);
                if (lista){
                    printf("\n      %d filme(s) encontrados.\n\n", size(lista));
                    imprimirLista(lista);
                    lista = liberarLista(lista);
                }
                else
                    printf("\n      Nada encontrado.\n");

                printf("\n\n  Tecle algo para continuar...");
                limparStdin();
                scanf("%*c");
                voltar = 1;
                break;

            // Curtas
            case 5:

                lista = buscarFilme_duracao(lista, -1, *pai, *ordem, *qtd_nos);
                if (lista){
                    printf("\n      %d filme(s) encontrados.\n\n", size(lista));
                    imprimirLista(lista);
                    lista = liberarLista(lista);
                }
                else
                    printf("\n      Nada encontrado.\n");

                printf("\n\n  Tecle algo para continuar...");
                limparStdin();
                scanf("%*c");
                voltar = 1;
                break;

            // Longas
            case 6:

                lista = buscarFilme_duracao(lista, 1, *pai, *ordem, *qtd_nos);
                if (lista){
                    printf("\n      %d filme(s) encontrados.\n\n", size(lista));
                    imprimirLista(lista);
                    printf("    Deseja remover todos? (1/0).\n");
                    scanf(" %d", &acao_seguinte);
                    if (acao_seguinte){
                        remover_lista_de_chaves(lista, pai, qtd_nos, qtd_filmes, *ordem);
                        printf("\n  Filmes removidos com sucesso do catalogo.");
                    }
                    lista = liberarLista(lista);
                }
                else
                    printf("\n      Nada encontrado.\n");

                printf("\n\n  Tecle algo para continuar...");
                limparStdin();
                scanf("%*c");
                voltar = 1;
                break;

            // Voltar.
            case 0:
                voltar = 1;
                break;
        }
    }
    return;
}

void menu_principal(TL *lista, int *pai, int *qtd_nos, int *ordem, int *qtd_filmes){
    char entrada[81];

    int sair = 0;
    int num = -1;
    while(!sair){
        limparTela();
        printf("\n            PROJETO NETFLIX\n\n\
                    MENU:\n\n\
                    [1] Mostrar catalogo.\n\
                    [2] Imprimir Árvore\n\
                    [3] Buscar filme(s).\n\
                    [4] Inserir titulos(s) manualmente.\n\
                    [5] Inserir titulos(s) via arquivo .txt\n\
                    [6] Destruir catalogo.\n\
                    [0] Sair.\n\n  - ");
        scanf("%i", &num);
        limparStdin();

        switch (num)
        {

            // Sair.
            case 0:

                printf("\n\n  Finalizando...\n\n");
                FILE * fp = fopen("config", "wb");
                if (fwrite(pai, sizeof(int), 1, fp) != 1 || fwrite(qtd_nos, sizeof(int), 1, fp) != 1 || fwrite(ordem, sizeof(int), 1, fp) != 1 || fwrite(qtd_filmes, sizeof(int), 1, fp) != 1)
                    printf("    AVISO: O catalogo nao foi salvo corretamente.\n");
                fclose(fp);
                sair = 1;
                break;

            // mostrar catalogo
            case 1:

                limparTela();
                if (*pai == 0){
                    printf("\n      Nao ha catalogo salvo.\n");
                    printf("\n\n  Tecle algo para continuar...");
                    scanf("%*c");
                    break;
                }
                printf("\n  %d filme(s) no catalogo.\n\n", *qtd_filmes);
                imprimirCatalogo(*ordem, *pai);
                printf("\n\n  Tecle algo para continuar...");
                scanf("%*c");
                break;
            //Imprimir Arvore
            case 2:
                limparTela();
                imprimeArvore(*pai, *qtd_filmes, *ordem);
                printf("\n\n  Tecle algo para continuar...");
                scanf("%*c");
                break;
            // Buscar filmes.
            case 3:

                if (*pai){
                    printf("\n");
                    menu_busca(lista, pai, qtd_nos, ordem, qtd_filmes);
                }
                else
                    printf("\n      Nao ha filmes para buscar.\n");
                //printf("\n\n  Tecle para continuar.\n   - ");
                limparStdin();
                //scanf("%*c");
                break;

            // Inserir 1.
            case 4:

                limparTela();
                Filme filme;
                carregarFilme(&filme);
                if (*pai == 0){
                    printf("Digite a ordem desejada para a arvore que sera criada: ");
                    scanf("%d", ordem);
                    *pai = 1;
                    *qtd_nos = 1;
                    TAB *node = novoNo(*ordem);
                    gravarNo("nodes/1", node, *ordem);
                }
                inserir_chave(filme, *ordem, qtd_nos, pai, qtd_filmes);
                break;

            // Inserir via arquivo.txt
            case 5:

                printf("Digite o nome do arquivo: ");
                scanf("%s", entrada);

                if (*pai == 0){
                    printf("Digite a ordem desejada para a arvore que sera criada: ");
                    scanf("%d", ordem);
                    *pai = 1;
                    *qtd_nos = 1;
                    TAB *node = novoNo(*ordem);
                    gravarNo("nodes/1", node, *ordem);
                }

                printf("Carregando %s.\n", entrada);
                carregarCatalogo(entrada, *ordem, qtd_nos, pai, qtd_filmes);
                printf("\n\n  Tecle para continuar.\n   - ");
                limparStdin();
                scanf("%*c");
                //strLower(entrada);
            break;

            // Destruir Catalogo
            case 6:

                limparTela();
                destruirCatalogo(pai, ordem, qtd_nos, qtd_filmes);
                printf("\n\n  Tecle algo para continuar...");
                scanf("%*c");
                break;

            case 9:
                limparTela();
                printf("Pai: %d.\n", *pai);
                imprimir_no_a_no(qtd_nos, *ordem);
                limparStdin();
                break;

            default:

                printf("\n  Entre uma opcao valida.");
                scanf("%*c");
                limparTela();
                break;
        }
    }
    printf("\n\n    Ate a proxima.\n");
    return;
}
