#include "NetflixTAB.h"

TAB* novoNo(int t){
    TAB *novo = (TAB *) malloc(sizeof(TAB));
    novo->nchaves = 0;
    novo->folha = 1;
    novo->chaves = (Filme *) malloc(sizeof(Filme) * (2*t -1));
    novo->filhos = (int*) malloc(sizeof(int) *2*t);
    for(int i = 0; i < 2*t; i++) novo->filhos[i] = 0;
    return novo;
}

TAB* dividir(TAB *x, int pos, TAB *y, int t, int *qtd_nos){

    //1º - Criar irmão.
    TAB *brother = novoNo(t);
    *qtd_nos = *qtd_nos + 1;
    brother->folha = y->folha;
    brother->nchaves = t - 1;

    //2º - Copiar os sucessores de y[t-1] em seu novo irmão, assim como seus filhos.
    int i;
    for(i = 0; i < t - 1; i++) brother->chaves[i] = y->chaves[t + i];
    if(!y->folha)
        for(i = 0; i < t; i++){
            brother->filhos[i] = y->filhos[i + t];
            y->filhos[t + i] = 0;
        }

    // 3º - Gravar irmão.
    char bro[12];
    sprintf(bro, "nodes/%d", *qtd_nos);
    gravarNo(bro, brother, t);
    free(brother);

    y->nchaves = t - 1;
    // Y deve ser atualizado após o fim das operações

    // 4º - Copiar y[t-1] em X, ordenadamente.
    for(i = x->nchaves; i >= pos; i--) x->filhos[i+1] = x->filhos[i];
    for(i = x->nchaves; i >= pos; i--) x->chaves[i] = x->chaves[i-1];
    x->filhos[pos] = *qtd_nos;
    x->chaves[pos-1] = y->chaves[t-1];
    x->nchaves++;
    return x;
}

TAB* inserir_nao_completo(TAB *a, int t, Filme filme, int *qtd_nos){
    int i = a->nchaves-1;

    // 1º - Se for folha, insira ordenadamente.
    if (a->folha){
        while((i >= 0) && (precedencia(a->chaves[i], filme) > 0)){
            a->chaves[i+1] = a->chaves[i];
            i--;
        }
        a->chaves[i+1] = filme;
        a->nchaves++;
        return a;
    }

    // 2º - Se não, procure pelo filho ideal.
    while((i >= 0) && precedencia(a->chaves[i], filme) > 0) i--;
    i++;

    // Carregue o  filho.
    TAB *filho = novoNo(t);
    char child[12];
    sprintf(child, "nodes/%d", a->filhos[i]);
    filho = carregarNo(child, filho, t);

    // Filho cheio? Divisão necessária.
    if(filho->nchaves == 2*t-1){
        a = dividir(a, i+1, filho, t, qtd_nos);        // Divisão ocorre, mas nenhum novo pai é criado.
        gravarNo(child, filho, t);                                // Salva filho modificado.

        if (precedencia(a->chaves[i], filme) < 0) {        // Procura o filho correto onde o filme deve ser inserido.
                i++;
                sprintf(child, "nodes/%d", a->filhos[i]);
                filho = carregarNo(child, filho, t);
                filho = inserir_nao_completo(filho, t, filme, qtd_nos);
                gravarNo(child, filho, t);
        }
        else {
            filho = inserir_nao_completo(filho, t, filme, qtd_nos);
            // Salva filho modificado.
            gravarNo(child, filho, t);
        }
        free(filho);
        return a;
    }

    // Filho não cheio?
    filho = inserir_nao_completo(filho, t, filme, qtd_nos);
    gravarNo(child, filho, t);

    return a;                                                     // Pai será atualizado após fim das operações.
}


int inserir_chave(Filme ch, int t, int *qtd_nos, int *pai, int *qtd_filmes){

    TAB *no = novoNo(t);
    int posicao;
    int arq = buscaFilme_arquivo(*pai, ch, t, no, &posicao);

    if(arq){
        char sno[12];
        sprintf(sno, "nodes/%d", arq);
        no->chaves[posicao] = ch;
        gravarNo(sno, no, t);
        free(no);
        return 0;
    }

    char spai[12];
    sprintf(spai, "nodes/%d", *pai);

    // Carrega o pai.
    no = carregarNo(spai, no, t);

    // Se estiver cheio, é preciso dividir.
    if (no->nchaves == (2*t - 1)){
        //printf("No cheio, divisao necessaria.\n");

        // Novo pai criado.
        TAB *s = novoNo(t);
        *qtd_nos = *qtd_nos + 1;
        char newDaddy[10];
        sprintf(newDaddy, "nodes/%d", *qtd_nos);
        s->folha = 0;
        s->filhos[0] = *pai;
        *pai = *qtd_nos;
        gravarNo(newDaddy, s, t);
        //printf("Novo pai criado, No %d, sob o nome de %s.\n", *pai, newDaddy);

        s = dividir(s, 1, no, t, qtd_nos);
        // Após ser modificado, o antigo pai deve ser atualizado.
        gravarNo(spai, no, t);

        s = inserir_nao_completo(s, t, ch, qtd_nos);
        // Novo pai deve ser atualizado.
        gravarNo(newDaddy, s, t);
        free(s);
        *qtd_filmes = *qtd_filmes + 1;
        return 1;
    }

    // Se não estiver cheio e for folha, basta inserir.
    //printf("No incompleto, insercao direta.\n");
    no = inserir_nao_completo(no, t, ch, qtd_nos);
    gravarNo(spai, no, t);
    free(no);
    *qtd_filmes = *qtd_filmes + 1;
    return 1;
}


void destruirCatalogo(int *pai, int *ordem, int *qtd_nos, int *qtd_filmes){
    if (*ordem == 0){
        printf("\n\n        Nao ha catalogos a deletar.\n");
        return;
    }
    int aux = 1;
    char lixo[12];
    while(aux <= *qtd_nos){
        sprintf(lixo, "nodes/%d", aux);
        remove(lixo);
        aux++;
    }

    aux = 0;
    FILE *newSettings = fopen("config", "wb");
    for(int i = 0; i < 4; i++) fwrite(&aux, sizeof(int), 1, newSettings);
    fclose(newSettings);

    *pai = 0;
    *ordem = 0;
    *qtd_filmes = 0;
    *qtd_nos = 0;

    return;
}

TAB *remover_chave(int primeiro, int *pai, Filme ch, int *qtd_nos, int *qtd_filmes, int t){

    printf("    Remocao iniciada. No %d e o primeiro a ser averiguado.\n", primeiro);
    char sprimeiro[12], schild[12], schildB[12];
    sprintf(sprimeiro, "nodes/%d", primeiro);

    TAB *no , *child, *childB;
    no = novoNo(t);
    no = carregarNo(sprimeiro, no, t);

    if (!no){
        printf("    Remocao interrompida.\n");
        return NULL;
    }

    printf("    No carregado.\n");
    int i = 0;
    printf("    %d chaves no no analisado.\n", no->nchaves);
    while((i <  no->nchaves) && (precedencia(no->chaves[i], ch) < 0)) i++;
    printf("    Busca em no concluida. Indice atingiu valor %d\n", i);
    sprintf(schild, "nodes/%d", no->filhos[i]);
    printf("    Filho precedente: No %d\n", no->filhos[i]);

    if ((i < no->nchaves) && (precedencia(no->chaves[i], ch) == 0)){ // Casos 1 e 2, chave encontrada em primeiro nó analisado.
        printf("    Chave encontrada no primeiro no analisado.\n");

        if (no->folha){ // Caso 1, nó analisado é folha.
            printf("    Caso 1, no analisado e folha.\n");
            while(i < no->nchaves){
                no->chaves[i] = no->chaves[i+1];
                i++;
            }
            no->nchaves--;
            *qtd_filmes = *qtd_filmes - 1;
            if (no->nchaves <= 0){
                    printf("    No vazio, deve ser deletado.\n");
                    //printf("\n\nNome do cara: %s\n\n", sprimeiro);
                    if (remove(sprimeiro))
                        printf(" Arquivo deletado com sucesso.\n");
                    return no;
            }
            gravarNo(sprimeiro, no, t);
            return no;
        }

        printf("    No analisado nao e folha.\n");
        sprintf(schildB, "nodes/%d", no->filhos[i+1]);

        child = novoNo(t);
        child = carregarNo(schild, child, t);
        childB = novoNo(t);
        childB = carregarNo(schildB, childB, t);
        TAB *aux = novoNo(t);
        char saux[12];

        if (child->nchaves >= t){ // Caso 2A, filho que precede a chave possui ao menos t chaves.
            printf("    Caso 2A, filho que precede a chave possui ao menos t chaves.\n");
            aux = carregarNo(schild, aux, t);
            while(!aux->folha){
                sprintf(saux, "nodes/%d", aux->filhos[aux->nchaves]);
                aux = carregarNo(schild, aux, t);
            }

            no->chaves[i] = aux->chaves[aux->nchaves-1];
            child = remover_chave(no->filhos[i], pai, aux->chaves[aux->nchaves-1], qtd_nos, qtd_filmes, t);

            gravarNo(sprimeiro, no, t);
            gravarNo(schild, child, t);
            free(child);
            free(childB);
            free(aux);
            return no;
        }

        if (childB->nchaves >= t){//Caso 2B, filho que procede a chave possui ao menos t chaves.
            printf("    Caso 2B, filho que sucede a chave possui ao menos t chaves.\n");
            aux = carregarNo(schildB, aux, t);
            while(!aux->folha){
                sprintf(saux, "nodes/%d", aux->filhos[0]);
                aux = carregarNo(saux, aux, t);
            }

            no->chaves[i] = aux->chaves[0];
            childB = remover_chave(no->filhos[i+1], pai, aux->chaves[0], qtd_nos, qtd_filmes, t);
            gravarNo(sprimeiro, no, t);
            gravarNo(schildB, childB, t);
            free(child);
            free(childB);
            free(aux);
            return no;
        }

        if ((child->nchaves == t-1) && (childB->nchaves == t-1)){ // Caso 2C, ambos os filhos possuem t-1 chaves.
            printf("    Caso 2C, filhos imediatos da chave possuem t-1 chaves.\n");
            child->chaves[child->nchaves] = ch;
            printf("    Chave transferida para no %d\n", no->filhos[i]);
            int j;
            for(j = 0; j < t-1; j++)
                child->chaves[j+t] = childB->chaves[j];
            for(j = 0; j <= t; j++)
                child->filhos[j+t] = childB->filhos[j];
            printf("    No irmao (%d) transferido para no %d.\n", no->filhos[i+1], no->filhos[i]);
            for(j = i; j < no->nchaves; j++)
                no->chaves[j] = no->chaves[j+1];
            for(j = i+1; j <= no->nchaves; j++)
                no->filhos[j] = no->filhos[j+1];
            printf("    Chave removida de seu no de origem.\n");
            remove(schildB);
            printf("    Irmao deletado.\n");
            no->filhos[j] = 0;
            no->nchaves -= 1;

            child->nchaves = 2*t-1;
            gravarNo(schild, child, t);
            child = remover_chave(no->filhos[i], pai, ch, qtd_nos, qtd_filmes, t);
            gravarNo(schild, child, t);
            gravarNo(sprimeiro, no, t);
            if (no->nchaves == 0) {
                    printf("    Pai (%s) vazio, deve ser deletado.\n", sprimeiro);
                    *pai = no->filhos[i];
                    remove(sprimeiro);
            }
            return no;
        }
    }


    char strFilho[12], strIrmao[12];
    sprintf(strFilho, "nodes/%d", no->filhos[i]);
    TAB *noFilho = novoNo(t), *noIrmao = novoNo(t);
    noFilho = carregarNo(strFilho,noFilho,t);

    if (noFilho->nchaves == t-1){ //CASOS 3A e 3B

        sprintf(strIrmao, "nodes/%d", no->filhos[i+1]);
        noIrmao = carregarNo(strIrmao,noIrmao,t);
        if((i<no->nchaves) && (noIrmao) && (noIrmao->nchaves>=t)){ //CASO 3A, IRMAO DA DIREITA
            printf("CASO 3A: irmao da DIREITA tem ao menos t chaves\n");
            noFilho->chaves[t-1] = no->chaves[i];   //dar a y a chave i da arv
            noFilho->nchaves++;
            no->chaves[i] = noIrmao->chaves[0];     //dar a arv uma chave de z
            noFilho->filhos[noFilho->nchaves] = noIrmao->filhos[0]; //enviar ponteiro menor de z para o novo elemento em y
            int j;
            for(j=0; j < noIrmao->nchaves-1; j++)  //ajustar chaves de z
                noIrmao->chaves[j] = noIrmao->chaves[j+1];
            for(j=0; j < noIrmao->nchaves; j++)       //ajustar filhos de z
                noIrmao->filhos[j] = noIrmao->filhos[j+1];
            noIrmao->nchaves--;

            gravarNo(strFilho,noFilho,t);
            gravarNo(strIrmao,noIrmao,t);
            noFilho = remover_chave(no->filhos[i], pai, ch, qtd_nos, qtd_filmes, t);
            gravarNo(strFilho,noFilho,t);
            gravarNo(sprimeiro,no,t);
            //free(noFilho);
            //free(noIrmao);

            return no;
        }

        sprintf(strIrmao, "nodes/%d", no->filhos[i-1]);
        noIrmao = carregarNo(strIrmao,noIrmao,t);
        if((i>0) && (noIrmao) && (noIrmao->nchaves>=t)){ //CASO 3A, IRMAO DA ESQUERDA
            printf("CASO 3A: irmao da ESQUERDA tem ao menos t chaves\n");
            int j;
            for(j = noFilho->nchaves; j>0; j--)               //encaixar lugar da nova chave
                noFilho->chaves[j] = noFilho->chaves[j-1];
            for(j = noFilho->nchaves+1; j>0; j--)             //encaixar lugar dos filhos da nova chave
                noFilho->filhos[j] = noFilho->filhos[j-1];
            noFilho->chaves[0] = no->chaves[i-1];              //dar a y a chave i da arv
            noFilho->nchaves++;
            no->chaves[i-1] = noIrmao->chaves[noIrmao->nchaves-1];   //dar a arv uma chave de z
            noFilho->filhos[0] = noIrmao->filhos[noIrmao->nchaves];         //enviar ponteiro de z para o novo elemento em y
            noIrmao->nchaves--;

            gravarNo(strFilho,noFilho,t);
            gravarNo(strIrmao,noIrmao,t);
            noFilho = remover_chave(no->filhos[i], pai, ch, qtd_nos, qtd_filmes, t);
            gravarNo(strFilho,noFilho,t);
            gravarNo(sprimeiro,no,t);
            //free(noFilho);
            //free(noIrmao);

            return no;
        }

        sprintf(strIrmao, "nodes/%d", no->filhos[i+1]);
        noIrmao = carregarNo(strIrmao,noIrmao,t);
        if((noIrmao) && (i < no->nchaves) && noIrmao->nchaves == t-1){ // CASO 3B, IRMAO DA DIREITA
            printf("CASO 3B: irmao da DIREITA tem t-1 chaves\n");
            noFilho->chaves[t-1] = no->chaves[i];     //pegar chave [i] e coloca ao final de filho[i]
            noFilho->nchaves++;
            int j;
            for(j=0; j < t-1; j++){
                noFilho->chaves[t+j] = noIrmao->chaves[j];     //passar filho[i+1] para filho[i]
                noFilho->nchaves++;
            }
            if(!noFilho->folha){
                for(j=0; j<t; j++){
                    noFilho->filhos[t+j] = noIrmao->filhos[j];
                }
            }
            remove(strIrmao);
            printf("REMOVIDO o no %s\n", strIrmao);
            for(j=i; j < no->nchaves-1; j++){ //limpar referências de i
                no->chaves[j] = no->chaves[j+1];
                no->filhos[j+1] = no->filhos[j+2];
            }
            no->nchaves--;

            gravarNo(strFilho,noFilho,t);
            gravarNo(sprimeiro,no,t);
            no = remover_chave(primeiro, pai, ch, qtd_nos, qtd_filmes, t);
            gravarNo(sprimeiro,no,t);
            //free(noFilho);
            //free(noIrmao);

            return no;
        }

        sprintf(strIrmao, "nodes/%d", no->filhos[i-1]);
        noIrmao = carregarNo(strIrmao,noIrmao,t);
        if((noIrmao) && (i>0) && noIrmao->nchaves==t-1){ // CASO 3B, IRMAO DA ESQUERDA
            printf("CASO 3B: irmao da ESQUERDA tem t-1 chaves\n");

            if(i == no->nchaves)
                noIrmao->chaves[t-1] = no->chaves[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
            else
                noIrmao->chaves[t-1] = no->chaves[i];   //pegar chave [i] e poe ao final de filho[i-1]
            noIrmao->nchaves++;
            int j;
            for(j=0; j < t-1; j++){
                noIrmao->chaves[t+j] = noFilho->chaves[j];     //passar filho[i+1] para filho[i]
                noIrmao->nchaves++;
            }
            if(!noIrmao->folha){
                for(j=0; j<t; j++){
                    noIrmao->filhos[t+j] = noFilho->filhos[j];
                }
            }
            remove(strFilho);
            printf("REMOVIDO o no %s\n", strFilho);
            no->nchaves--;

            gravarNo(strIrmao,noIrmao,t);
            gravarNo(sprimeiro,no,t);
            no = remover_chave(primeiro, pai, ch, qtd_nos, qtd_filmes, t);
            gravarNo(sprimeiro,no,t);
            //free(noFilho);
            //free(noIrmao);

            return no;
        }
    }

    free(noFilho);
    free(noIrmao);

    child = novoNo(t);
    child = carregarNo(schild, child, t);

    printf("    Nenhum caso satisfeito.\n");
    child = remover_chave(no->filhos[i], pai, ch, qtd_nos, qtd_filmes, t);
    if (child) printf("  No atualizado.\n");
    gravarNo(schild, child, t);
    return no;
}

void remover_lista_de_chaves(TL *lista, int *pai, int *qtd_nos, int *qtd_filmes, int t){
    if (!lista) return;
    if(lista->prox)
        remover_lista_de_chaves(lista->prox, pai, qtd_nos, qtd_filmes, t);
    remover_chave(*pai, pai, lista->info, qtd_nos, qtd_filmes, t);
    return;
}
