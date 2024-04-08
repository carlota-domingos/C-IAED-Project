/*
 * Ficheiro:  lists.h
 * Autor: Carlota Domingos
 * Descricação: Header file com as estruturas de dados utilizadas no programa e
 * a declaração das funções que as manipulam.
*/
#ifndef _LISTS_
#define _LISTS_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERDADE 1
#define FALSO 0
#define NAO_ENCONTRADO -1

#define ERRO_PARSING 1
#define MAX_TAMANHO_LINHA 65535

/* Definicao de Tipos de Dados */

typedef struct ligacao ligacao;
typedef struct carreira carreira;
typedef struct paragem paragem;

/*struct que representa uma paragem*/

struct paragem {
    char *nome;
    double latitude, longitude;
    char **carreiras;
    int numCarreiras;
    paragem *next;
};

/*lista com todas as paragens existentes*/

struct lstParagem{
    paragem *head , *tail;
};

/*struct que representa uma ligacao de uma paragem a outra*/

struct ligacao {
    paragem *origem, *destino;
    double custo, duracao;
    ligacao *next,*prev;
};

/*struct que representa uma carreira e o seu percurso*/

struct carreira {
    char *nome;
    double custoTotal;
    double duracaoTotal;
    ligacao *origem,*destino;
    carreira *next, *prev;
    int numLigacoes;
};

/*lista com todas as carreirasexistentes*/

struct lstCarreiras {
    carreira *head , *tail;
};

/*ponteiro para as listas que vai ser passado em todas as funcoes*/

typedef struct lstParagem* Par;
typedef struct lstCarreiras* Car; 

/*funções que inicializam as structs de linked lists*/

Par initParagens();
Car initCarreiras();

/*funcoes relacionadas com a struct de paragens*/

void iniciaParagem(paragem* new, char* nome, double lat, double lon);
int criaParagem(Par lstP, char *nome, double lat, double lon);
paragem* procuraParagem(Par lstP, char *nome); 
void listaParagens(Par lstP);
void listaPercurso(carreira *carreira, int est);
void mostraParagem(paragem *paragem);
void ordenaCarreiras(Par lstP);
void listaCarreirasParagem(paragem *Paragem);


/*funcoes relacionadas com a struct de carreiras*/

void iniciaCarreira(carreira* new, char* nome);
int criaCarreira(Car lstC, char *nome);
carreira* procuraCarreira(Car lstC, char *nome);
void listaCarreiras(Car lstC);
void mostraCarreira(carreira *carreira);
void mostraLigacoesCarreira(carreira *carreira, int estado);
int procParagemCarreira(paragem *paragem, carreira *carreira);

/*funcoes relacionadas com o comando 'l'*/

void atualizaLigacao(ligacao* new,paragem* origem, paragem*destino, float custo, float duracao);
int atualizaCarreira(carreira* carreira, paragem* paragem);
void addPrimeiraLigacao(carreira *carreira, ligacao *ligacao);
void addLigacaoInicio(carreira *carreira, ligacao *ligacao);
void addLigacaoFim(carreira *carreira, ligacao *ligacao);
int criaLigacao(carreira *carreira, paragem *origem, paragem *destino, float custo, float duracao);

/*funcoes relacionadas com o comando 'e'*/

void removeParagem(Par lstP, Car lstC, paragem* paragem);
void modificaCarreiras(Car lstC, paragem* paragem);
void removeLigacaoInicio(carreira* carreira, ligacao*ligacao);
void removeLigacaoFim(carreira* carreira, ligacao*ligacao);
void removeLigacao(carreira* carreira, ligacao* ligacao);
void atualizaLigacoes(carreira* carreira, paragem* paragem);

/*funcoes relacionadas com o comando 'r' */

void delLigacoes(carreira *carreira);
void delCarreira(Car lstC, carreira *carreira);
void modificaParagens(Par lstP,carreira *carreira);

/*funcao relacionada com o comando 'q'*/

void delMemory(Car lstC,Par lstP);


#endif