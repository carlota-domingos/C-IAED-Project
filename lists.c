/*
 * Ficheiro:  lists.c
 * Autor: Carlota Domingos
 * Descricação: Ficheiro com as funções que manipulam as estruturas de dados
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

/*cria a lista de paragens*/

Par initParagens(){
    Par lstP = (Par)malloc(sizeof(struct lstParagem));
    lstP->head = NULL;
    lstP->tail = NULL;
    return lstP;
}

/*cria a lista de carreiras*/

Car initCarreiras(){
    Car lstC = (Car)malloc(sizeof(struct lstCarreiras));
    lstC->head = NULL;
    lstC->tail = NULL;
    return lstC;
}

/*atribui o nome latitude e longitude e dados iniciais à paragem new*/

void iniciaParagem(paragem *new, char *nome, double lat, double lon){
    strcpy(new->nome, nome);
    new->latitude = lat;
    new->longitude = lon;
    new->carreiras = NULL;
    new->numCarreiras = 0;
    new->next = NULL;
}

/*cria uma nova paragem e adiciona a paragem à lista de paragens*/

int criaParagem(Par lstP, char *nome, double lat, double lon){
    int EXIT = FALSO;
    paragem *new;

    /*aloca memoria para a paragem*/
    if ((new = (paragem *)malloc(sizeof(paragem))) == NULL)
        return VERDADE;

    /*aloca memoria para o nome da paragem*/
    if ((new->nome = (char *)malloc((strlen(nome) + 1) * sizeof(char))) == NULL)
        return VERDADE;

    /*cria a paragem*/
    iniciaParagem(new, nome, lat, lon);
    if (lstP->head == NULL && lstP->tail == NULL){
        /*adiciona paragem inicial*/
        lstP->head = new;
        lstP->tail = new;
    }
    else{
        /*caso geral*/
        lstP->tail->next = new;
        lstP->tail = new;
    }
    return EXIT;
}

/*procura se existe uma paragem com o nome passado no argumento
e devolve ou essa paragem ou NULL*/

paragem *procuraParagem(Par lstP, char *nome){
    paragem *cur = lstP->head;

    while (cur != NULL && strcmp(cur->nome, nome) != 0)
        cur = cur->next;

    return cur;
}

/*lista todas as paragens da carreira*/

void listaParagens(Par lstP){
    paragem *p = lstP->head;
    while (p != NULL)
    {
        printf("%s: %16.12f %16.12f %d\n", p->nome, p->latitude,
               p->longitude, p->numCarreiras);
        p = p->next;
    }
}

/*mostra a latitude e longitude de uma paragem*/

void mostraParagem(paragem *paragem){
    printf("%16.12f %16.12f\n", paragem->latitude, paragem->longitude);
}

/*ordena o pointer de strings com os nomes das carreiras existentes
em cada paragem por ordem alfabetica e corre o comando 'i'*/

void ordenaCarreiras(Par lstP){
    int i, j;
    paragem *cur = lstP->head;

    while (cur != NULL)
    {
        for (i = 0; i < cur->numCarreiras - 1; i++)
        {
            for (j = 0; j < cur->numCarreiras - i - 1; j++)
            {
                if (strcmp(*(cur->carreiras + j), *(cur->carreiras + j + 1)) > 0)
                {
                    char *temp = *(cur->carreiras + j);
                    *(cur->carreiras + j) = *(cur->carreiras + j + 1);
                    *(cur->carreiras + j + 1) = temp;
                }
            }
        }
        if (cur->numCarreiras > 1)
            listaCarreirasParagem(cur);
        cur = cur->next;
    }
}

/*lista uma intersecao*/

void listaCarreirasParagem(paragem *paragem) {
    int i;
    printf("%s %d:", paragem->nome, paragem->numCarreiras);
    for (i = 0; i < paragem->numCarreiras; i++)
        printf(" %s", *(paragem->carreiras + i));
    putchar('\n');
}

/*associa um nome e argumentos iniciais a uma nova carreira*/

void iniciaCarreira(carreira *new, char *nome) {
    strcpy(new->nome, nome);
    new->custoTotal = 0;
    new->duracaoTotal = 0;
    new->destino = NULL;
    new->origem = NULL;
    new->next = NULL;
    new->numLigacoes = 0;
}

/*cria uma carreira*/

int criaCarreira(Car lstC, char *nome) {
    int EXIT = FALSO;
    carreira *new;

    /*aloca memoria para a nova carreira*/
    if ((new = (carreira *)malloc(sizeof(carreira))) == NULL)
        return VERDADE;

    /*aloca memoria para o nome da carreira*/
    if ((new->nome = (char *)malloc(sizeof(char) * (strlen(nome) + 1))) == NULL)
        return VERDADE;

    iniciaCarreira(new, nome);

    if (lstC->head == NULL && lstC->tail == NULL){
        /*poe a carreira no inicio da lista de carreira*/
        lstC->head = new;
        lstC->tail = new;
        new->prev = NULL;
    } else {
        /*caso global*/
        lstC->tail->next = new;
        new->prev = lstC->tail;
        lstC->tail = new;
    }
    return EXIT;
}

/*procura se existe uma carreira com o nome passado no argumento e devolve ou 
    essa carreira ou NULL*/

carreira *procuraCarreira(Car lstC, char *nome) {
    carreira *cur = lstC->head;
    while (cur != NULL && strcmp(cur->nome, nome) != 0)
        cur = cur->next;
    return cur;
}

/*lista todas as carreiras existentes*/

void listaCarreiras(Car lstC){
    carreira *c = lstC->head;

    while (c != NULL){
        mostraCarreira(c);
        c = c->next;
    }
}

/*mostra os dados de uma carreira*/

void mostraCarreira(carreira *carreira) {
    int numLigacoes = carreira->numLigacoes;

    printf("%s ", carreira->nome);
    if (numLigacoes == 0) {
        printf("%d %.2f %.2f\n", 0, 0.0, 0.0);
        return;
    }

    if (numLigacoes == 1)
        /*caso so haja uma ligacao tem de escrever o destino dessa ligacao*/
        printf("%s %s ", carreira->origem->origem->nome,
               carreira->origem->destino->nome);
    else
        /*caso geral*/
        printf("%s %s ", carreira->origem->origem->nome,
               carreira->destino->destino->nome);

    printf("%d ", numLigacoes + 1);
    printf("%.2f ", carreira->custoTotal);
    printf("%.2f\n", carreira->duracaoTotal);
}

/*mostra o percurso da carreira dependendo do estado inverso ou nao inverso*/

void mostraLigacoesCarreira(carreira *carreira, int inverso){
    ligacao *cur;
    /*variavel que vai percorrer as ligacoes das carrieras*/

    if (inverso == FALSO && carreira->numLigacoes > 0) {
        /*caso ordenado*/
        cur = carreira->origem;
        printf("%s", cur->origem->nome);
        while (cur != NULL) {
            printf(", %s", cur->destino->nome);
            cur = cur->next;
        }
        putchar('\n');
    } else if (inverso == VERDADE)  {
        /*caso inverso*/
        if (carreira->numLigacoes == 1) {
            /*se so ha uma ligacao tem de mostrar o destino e origem desta*/
            cur = carreira->origem;
            printf("%s, %s\n", cur->destino->nome, cur->origem->nome);
        }
        if (carreira->numLigacoes > 1) {
            /*caso geral*/
            cur = carreira->destino;
            printf("%s", cur->destino->nome);
            while (cur != NULL) {
                printf(", %s", cur->origem->nome);
                cur = cur->prev;
            }
            putchar('\n');
        }
    }
}

/*verifica se uma paragem existe numa carreira*/

int procParagemCarreira(paragem *paragem, carreira *carreira) {
    int i;

    for (i = 0; i < paragem->numCarreiras; i++) {
        if (strcmp(carreira->nome, *(paragem->carreiras + i)) == 0)
            return VERDADE;
    }
    return FALSO;
}

/*adiciona a ligacao a uma carreira sem ligacoes*/

void addPrimeiraLigacao(carreira *carreira, ligacao *ligacao) {
    ligacao->next = NULL;
    ligacao->prev = NULL;
    carreira->origem = ligacao;
    carreira->numLigacoes++;
    carreira->custoTotal += ligacao->custo;
    carreira->duracaoTotal += ligacao->duracao;
}

/*adiciona uma ligacao no inicio da carreira*/

void addLigacaoInicio(carreira *carreira, ligacao *ligacao) {
    carreira->origem->prev = ligacao;
    ligacao->next = carreira->origem;
    ligacao->prev = NULL;
    /*caso onde ainda so existe uma ligacao na carreira*/
    if (carreira->destino == NULL)
        carreira->destino = carreira->origem;
    carreira->origem = ligacao;
    carreira->numLigacoes++;
    carreira->custoTotal += ligacao->custo;
    carreira->duracaoTotal += ligacao->duracao;
}

/*adiciona uma ligacao ao fim da carreira*/

void addLigacaoFim(carreira *carreira, ligacao *ligacao) {

    if (carreira->numLigacoes == 1) {
        /*se só existir uma ligacao tem em conta que a nova ligacao tem de se
            ligar à origem da carreira*/
        carreira->origem->next = ligacao;
        ligacao->prev = carreira->origem;
    } else {
        /*no caso geral a nova ligacao liga-se ao destino da carreira*/
        carreira->destino->next = ligacao;
        ligacao->prev = carreira->destino;
    }
    ligacao->next = NULL;
    carreira->destino = ligacao;
    carreira->numLigacoes++;
    carreira->custoTotal += ligacao->custo;
    carreira->duracaoTotal += ligacao->duracao;
}

/*atribui os argumentos a uma nova ligacao*/

void atualizaLigacao(ligacao *new, paragem *origem, paragem *destino,
                     float custo, float duracao) {
    new->origem = origem;
    new->destino = destino;
    new->custo = custo;
    new->duracao = duracao;
}

/*adiciona o nome da carreira ao double pointer de carreiras de uma paragem*/

int atualizaCarreira(carreira *carreira, paragem *paragem) {
    int EXIT = FALSO;

    if ((paragem->carreiras = (char **)realloc(paragem->carreiras,
                        (paragem->numCarreiras + 1) * sizeof(char *))) == NULL)
        return VERDADE;

    if ((paragem->carreiras[paragem->numCarreiras] =
            (char *)malloc((strlen(carreira->nome) + 1)*sizeof(char))) == NULL)
        return VERDADE;

    strcpy(paragem->carreiras[paragem->numCarreiras], carreira->nome);
    paragem->numCarreiras++;
    return EXIT;
}

/*funcao geral para criar uam ligação*/

int criaLigacao(carreira *carreira, paragem *origem, paragem *destino,
                float custo, float duracao) {
    int EXIT = FALSO;
    ligacao *new;

    /*caso inicial*/
    if (carreira->numLigacoes == 0) {
        if ((new = (ligacao *)malloc(sizeof(ligacao))) == NULL)
            return VERDADE;
        atualizaLigacao(new, origem, destino, custo, duracao);
        addPrimeiraLigacao(carreira, new);
        /*adiciona o nome da carreira a origem*/
        if ((EXIT = atualizaCarreira(carreira, origem)) == VERDADE)
            return EXIT;
        if (origem != destino){
            /*adiciona o nome da carreira ao destino se nao for igual a origem*/
            if ((EXIT = atualizaCarreira(carreira, destino)) == VERDADE)
                return EXIT;
        }
    }

    else if ((carreira->numLigacoes == 1 && origem == carreira->origem->destino) || (carreira->numLigacoes > 1 && origem == carreira->destino->destino)){
        /*caso a ligacao for no fim*/
        if (procParagemCarreira(destino, carreira) == FALSO) {
            /*verifica se a paragem adicionada faz ja parte da carreira e
            adiciona a carreira à lista nas paragens caso nao*/
            if ((EXIT = atualizaCarreira(carreira, destino)) == VERDADE)
                return EXIT;
        }

        if ((new = (ligacao *)malloc(sizeof(ligacao))) == NULL)
            return VERDADE;
        atualizaLigacao(new, origem, destino, custo, duracao);
        addLigacaoFim(carreira, new);

    } else if (destino == carreira->origem->origem){
        /*caso a ligacao for no inicio*/
        if (procParagemCarreira(origem, carreira) == FALSO){
            /*verifica se a paragem adicionada faz ja parte da carreira e
            adiciona a carreira à lista nas paragens caso nao*/
            if ((EXIT = atualizaCarreira(carreira, origem)) == VERDADE)
                return EXIT;
        }

        if ((new = (ligacao *)malloc(sizeof(ligacao))) == NULL)
            return VERDADE;
        atualizaLigacao(new, origem, destino, custo, duracao);
        addLigacaoInicio(carreira, new);
    }
    else
        printf("link cannot be associated with bus line.\n");
    return EXIT;
}

/*modifica todas as carreiras em que existe a paragem e remove as ligacoes 
    necessaeias*/

void modificaCarreiras(Car lstC, paragem *paragem) {
    int i;
    carreira *carreira;
    /*percorre todas as carreiras do pointer de caarreiras da paragem*/
    for (i = 0; i < paragem->numCarreiras; i++) {
        carreira = procuraCarreira(lstC, *(paragem->carreiras + i));
        atualizaLigacoes(carreira, paragem);
        free(*(paragem->carreiras + i));
        *(paragem->carreiras + i) = NULL;
    }
    /*elimina o pointer*/
    free(paragem->carreiras);
}

/*remove uma paragem*/

void removeParagem(Par lstP, Car lstC, paragem *par) {
    paragem *prev;

    /*atualiza a carreira*/
    modificaCarreiras(lstC, par);
    /*atualiza a lista de paragens*/

    if (par == lstP->head) {
        /*caso a paragem seja primeira*/
        lstP->head = par->next;
        if (par == lstP->tail)
            /*caso so haja uma paragem*/
            lstP->tail = NULL;
    } else {
        /*percorre a lista de paragens ate encontrar a anterior*/
        prev = lstP->head;
        while (prev->next != par)
            prev = prev->next;
        prev->next = par->next;
        /*caso a paragem seja a ultima*/
        if (prev->next == NULL)
            lstP->tail = prev;
    }
    /*elimina a paragem e o nome*/
    free(par->nome);
    free(par);
}

/* remove ligacoes no inicio do percurso*/

void removeLigacaoInicio(carreira *carreira, ligacao *ligacao) {
    carreira->origem = ligacao->next;

    if (carreira->numLigacoes == 2)
        /*caso passe a haver so uma ligacao*/
        carreira->destino = NULL;
    /*remove custo e duracao da carreira*/
    carreira->numLigacoes--;
    carreira->custoTotal -= ligacao->custo;
    carreira->duracaoTotal -= ligacao->duracao;
    if (carreira->numLigacoes > 0)
        /*caso a ligacao tenha uma ligacao a seguir*/
        ligacao->next->prev = NULL;
    free(ligacao);
}

/*remove ligacoes no fim do percurso*/

void removeLigacaoFim(carreira *carreira, ligacao *ligacao) {
    if (carreira->numLigacoes == 1)
        /*caso a carreira tenha uma ligacao que vai ser apagada*/
        carreira->origem = NULL;
    else if (carreira->numLigacoes == 2)
        /*caso a carreira tenha duas ligacoes o destino passa a 
            apontar para NULL*/
        carreira->destino = NULL;
    else
        /*caso geral*/
        carreira->destino = ligacao->prev;
    
    ligacao->prev->next = NULL;
    /*remove custo e duracao da carreira*/
    carreira->numLigacoes--;
    carreira->custoTotal -= ligacao->custo;
    carreira->duracaoTotal -= ligacao->duracao;
    /*elimina a ligacao*/
    free(ligacao);
}

/*remove ligacoes no meio do percurso( qualquer caso onde a paragem nao 
    corresponde à origem nem ao destino da carreira)*/

void removeLigacao(carreira *carreira, ligacao *ligacao){

    if (carreira->numLigacoes == 2)
        /*caso a carreira tenha duas ligacoes o destino passa a
             apontar para NULL*/
        carreira->destino = NULL;
    if (ligacao->prev != NULL)
        /*caso a ligacao a apagar nao seja a primeira*/
        ligacao->prev->next = ligacao->next;
    else
        /*caso a ligacao a apagar seja a primeira atualiza a origem*/
        carreira->origem = ligacao->next;
    /*atualiza o custo e a duracao para a proxima ligacao
        e diminui numero de ligacoes da carreira*/
    carreira->numLigacoes--;
    ligacao->next->prev = ligacao->prev;
    ligacao->next->origem = ligacao->origem;
    ligacao->next->custo += ligacao->custo;
    ligacao->next->duracao += ligacao->duracao;
    /*elimina a ligacao*/
    free(ligacao);
}

/*atualiza as ligacoes da carreira em funcao da paragem que é eliminada*/

void atualizaLigacoes(carreira *carreira, paragem *paragem){
    ligacao *cur = carreira->origem;
    ligacao *temp;
    /*caso a carreira tenha apenas uma ligacao que vai ser apagada*/
    if (carreira->destino == NULL) {
        carreira->origem = NULL;
        carreira->custoTotal = 0;
        carreira->duracaoTotal = 0;
        carreira->numLigacoes = 0;
        free(cur);
        return;
    }

    while (cur != NULL){
        /*percorre a lista de ligacoes ate encontrar a paragem a apagar*/
        temp = cur;
        cur = cur->next;
        
        if (temp->origem == paragem && temp->prev == NULL)
            /*caso a paragem seja a origem da carreira*/
            removeLigacaoInicio(carreira, temp);
        else if (temp->destino == paragem && temp->next == NULL)
            /*caso a paragem seja o destino da carreira*/
            removeLigacaoFim(carreira, temp);
    
        else if (temp->destino == paragem)
            /*caso a paragem corresponda ao destino de uma ligacao que nao 
                seja a final*/
            removeLigacao(carreira, temp);
    }
}

/*elimina todas as ligacoes de uma carreira*/

void delLigacoes(carreira *carreira) {
    ligacao *cur = carreira->origem;
    ligacao *del;
    while (cur != NULL) {
        del = cur;
        cur = cur->next;
        free(del);
    }
}

/*apaga uma carreira*/

void delCarreira(Car lstC, carreira *carreira) {
    /*atualiza a lista de carreiras*/
    if (lstC->head != carreira)
        carreira->prev->next = carreira->next;
    else
        lstC->head = carreira->next;
    if (lstC->tail != carreira)
        carreira->next->prev = carreira->prev;
    else
        lstC->tail = carreira->prev;

    delLigacoes(carreira);
    /*apaga nome e a carreira*/
    free(carreira->nome);
    free(carreira);
}

/*modifica as paragens em funcao da carreira a apagar*/

void modificaParagens(Par lstP, carreira *carreira){
    int i, j;
    paragem *cur = lstP->head;
    char *nome = carreira->nome;
    /*percorre a lista de paragens*/
    while (cur != NULL) {
        for (i = 0; i < cur->numCarreiras; i++) {   
            /*caso a paragem tenha a carreira a apagar*/
            if (strcmp(nome, *(cur->carreiras + i)) == 0) {
                /*remove a carreira da lista de carreiras da paragem*/
                free(*(cur->carreiras + i));
                cur->numCarreiras--;
                for (j = i; j < cur->numCarreiras; j++)
                    /*atualiza a lista de carreiras da paragem*/
                    *(cur->carreiras + j) = *(cur->carreiras + j + 1);
                break;
            }
        }
        cur = cur->next;
    }
}

/*apaga as listas de paragens e carreiras*/

void delMemory(Car lstC, Par lstP){
    free(lstC);
    free(lstP);
}
