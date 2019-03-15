#include "NetflixTAB.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    printf("%c[%d;%d;%dm", 0x1B, 0,0,41);

    struct stat st = {0};
    if (stat("nodes", &st) == -1)//Testa se a pasta já está criada
		mkdir("nodes", S_IRUSR | S_IWUSR | S_IXUSR); //Equivalente ao chmod 777

    int qtd_nos, pai, ordem, qtd_filmes;
    TL *lista = NULL;
    // É preciso carregar as configurações do catálogo, caso este exista.
    FILE *first = fopen("config", "rb");
    if(!first){
        pai = 0;
        qtd_nos = 0;
        ordem = 0;
        qtd_filmes = 0;
    }
    else{
        fread(&pai, sizeof(int), 1, first);
        fread(&qtd_nos, sizeof(int), 1, first);
        fread(&ordem, sizeof(int), 1, first);
        fread(&qtd_filmes, sizeof(int), 1, first);
        fclose(first);
    }

    menu_principal(lista, &pai, &qtd_nos, &ordem, &qtd_filmes);

    return 0;
}
