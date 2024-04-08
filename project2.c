/*
 * Ficheiro:  project2.c
 * Autor: Carlota Domingos
 * Descricação: Ficheiro principal do programa.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

/* Le espaços. Devolve 0 se chegou ao final de linha ou 1 caso contrário. */

int leEspacos() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n')
        return 0;
    ungetc(c, stdin);
    return 1;
}

/* Le um nome para a string que recebe como parametro. */

void leNome(char *s) {
    int i = 0, c;
    s[0] = getchar();
    if (s[0] != '"') {
        i = 1;
        while ((c = getchar()) != ' ' && c != '\t' && c != '\n')
            s[i++] = c;
        ungetc(c, stdin);
    }
    else {
        while((c = getchar()) != '"')
            s[i++] = c;
    }
    s[i++] = '\0';
}

/* Le todo o texto até ao final de linha. */

void leAteFinalLinha(char *s) {
    char c;
    int i = 0;
    while ((c = getchar()) != '\n') {
        s[i++] = c;
    }
    s[i] = '\0';
}



/* Verifica se a string é um prefixo de tamanho pelo menos 3 da
   palavra inverso. */

int verificaInversoOk(char *s) {
    char inv[] = "inverso";
    int size = strlen(s), i;

    if (size < 3 || size > 7)
        return FALSO;
    for (i = 0; i < size; i++)
        if (inv[i] != s[i])
            return FALSO;
    return VERDADE;
}

/* Funcao relacionada com o comando 'l'
 Le nome de carreira e paragens de uma ligacao. */

void leNomesComando(char *nomeCarreira, char *nomeOrigem, char *nomeDestino) {
    leEspacos();
    leNome(nomeCarreira);
    leEspacos();
    leNome(nomeOrigem);
    leEspacos();
    leNome(nomeDestino);
}

/* Função para tratar do comando 'c'. */

int carreiras(Car lstC) {
    int EXIT= FALSO, fimLinha = leEspacos();
    char nome[MAX_TAMANHO_LINHA], _buffer[MAX_TAMANHO_LINHA];
    carreira *carreira;

    if (!fimLinha) {
        listaCarreiras(lstC);
        return EXIT;
    }

    leNome(nome);
    fimLinha = leEspacos();
    if (!fimLinha) {
        if ((carreira = procuraCarreira(lstC,nome))==NULL)
            EXIT = criaCarreira(lstC, nome);
        else
            mostraLigacoesCarreira(carreira, FALSO);
    } else {
        leNome(_buffer);
        carreira = procuraCarreira(lstC,nome);

        if (verificaInversoOk(_buffer) && carreira != NULL)
            mostraLigacoesCarreira(carreira, VERDADE);
        else
            printf("incorrect sort option.\n");

        leAteFinalLinha(_buffer);
    }
    return EXIT;
}


/* Função para tratar o comando 'p'. */

int paragens(Par lstP) {
    int EXIT= FALSO, fimLinha = leEspacos();
    char _buffer[MAX_TAMANHO_LINHA], nome[MAX_TAMANHO_LINHA];
    paragem *paragem;
    
    if (!fimLinha) {
        listaParagens(lstP);
        return EXIT;
    }

    leNome(nome);
    fimLinha = leEspacos();

    if (!fimLinha) {
        if ((paragem = procuraParagem(lstP, nome)) == NULL)
            printf("%s: no such stop.\n", nome);
        else
            printf("%16.12f %16.12f\n", paragem->latitude, paragem->longitude);
    } else {
        double latitude, longitude;
        scanf("%lf%lf", &latitude, &longitude);
        if (procuraParagem(lstP, nome) == NULL)
            EXIT = criaParagem(lstP, nome, latitude, longitude);
        else
            printf("%s: stop already exists.\n", nome);
        leAteFinalLinha(_buffer);
    }
    return EXIT;
}



/* Função para tratar o comando 'l'. */

int ligacoes(Car lstC, Par LstP) {
    int EXIT= FALSO;
    char _buffer[MAX_TAMANHO_LINHA], nomeCarreira[MAX_TAMANHO_LINHA], 
        nomeOrigem[MAX_TAMANHO_LINHA], nomeDestino[MAX_TAMANHO_LINHA];
    double custo, duracao;
    carreira* carreira;
    paragem *origem, *destino;

    leNomesComando(nomeCarreira, nomeOrigem, nomeDestino);
    scanf("%lf%lf", &custo, &duracao);
    leAteFinalLinha(_buffer);
    if ((carreira= procuraCarreira(lstC,nomeCarreira))==NULL)
        printf("%s: no such line.\n", nomeCarreira);
    else {
       
        if ((origem=procuraParagem(LstP,nomeOrigem)) == NULL)
            printf("%s: no such stop.\n", nomeOrigem);
        else {
            if ((destino = procuraParagem(LstP,nomeDestino)) == NULL)
                printf("%s: no such stop.\n", nomeDestino);
           
            else if (custo < 0.0 || duracao < 0.0)
                printf("negative cost or duration.\n");
            else
                EXIT= criaLigacao(carreira, origem, destino, custo, duracao);
        }
    }
    return EXIT;
}


/* Função para tratar o comando 'i'. */

void intersecoes(Par lstP) {
    char _buffer[MAX_TAMANHO_LINHA];
    leAteFinalLinha(_buffer);
    ordenaCarreiras(lstP);
}

/*função para tratar o comando 'r'.*/

void removecarreira(Car lstC, Par lstP) {
    char nome[MAX_TAMANHO_LINHA], _buffer[MAX_TAMANHO_LINHA];
    carreira *carreira;

    leEspacos();
    leNome(nome);
    leAteFinalLinha(_buffer);

    if ((carreira = procuraCarreira(lstC,nome))==NULL)
        printf("%s: no such line.\n", nome);
    else {
        modificaParagens(lstP, carreira);
        delCarreira(lstC, carreira);
    }
}

/*função para tratar o comando 'e'.*/

void eliminaparagem(Par lstP, Car lstC) {
    char nome[MAX_TAMANHO_LINHA], _buffer[MAX_TAMANHO_LINHA];
    paragem *paragem;
    leEspacos();
    leNome(nome);
    leAteFinalLinha(_buffer);

    if ((paragem = procuraParagem(lstP, nome)) == NULL)
        printf("%s: no such stop.\n", nome);
    else
        removeParagem(lstP,lstC,paragem);
}

/*função para tratar o comando 'a'.*/

void apaga(Car LstC,Par LstP) {
    char _buffer[MAX_TAMANHO_LINHA];
    paragem* cur;
    leAteFinalLinha(_buffer);
    /*apaga as carreiras*/
    while(LstC->head!=NULL) {
        modificaParagens(LstP,LstC->head);
        delCarreira(LstC,LstC->head);
    }
    /*apaga as paragens*/
    while(LstP->head!=NULL) {
        cur=LstP->head;
        if(LstP->head==LstP->tail)
            LstP->tail=NULL;
        LstP->head=LstP->head->next;
        free(cur->nome);
        free(cur);
    }

}

/*função para apagar toda a memoria à saida do programa.*/

void quit(Car lstC, Par lstP) {
    apaga(lstC,lstP);
    delMemory(lstC,lstP);
}



/* Função MAIN */

int main() {
    int c, EXIT= VERDADE;
    Car lstC;
    Par lstP;
    if ((lstC = initCarreiras())==NULL)
        EXIT= VERDADE;
    if ((lstP = initParagens())==NULL)
        EXIT= VERDADE;

    do {
        c = getchar();
        switch(c) {
        case 'c':
            EXIT = carreiras(lstC);
            break;
        case 'p':
            EXIT = paragens(lstP);
            break;
        case 'l':
            EXIT = ligacoes(lstC,lstP);
            break;
        case 'i':
            intersecoes(lstP);
            break;
        case 'r':
            removecarreira(lstC,lstP);
            break;
        case 'e':
            eliminaparagem(lstP,lstC);
            break;
        case 'a':
            apaga(lstC,lstP);
            break;
        case 'q':
            break;
        default:
            /* Ignorar linhas em branco */
            if (c == ' ' || c == '\t' || c == '\n') break;
        }
    } while (c != 'q' && EXIT == FALSO);
    if(EXIT==VERDADE)
        printf("no memory\n");
    quit(lstC,lstP);
    return 0;
}
