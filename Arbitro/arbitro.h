/*
 * FILE: arbitro.h
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 11/1/20
 */

#ifndef ARBITRO_ARBITRO_H
#define ARBITRO_ARBITRO_H

#include "initConfig.h"

typedef struct Info info;
struct Info {
    // Dados Cliente
    char nomeJogador[200];
    char pipeCliente[200];
    char pipeThread[200];
    unsigned int pidCliente;

    // Dados Jogo
    char nomeJogo[200];
    unsigned int pidJogo;
    int pontuacao;

    int intComunicacao;         // Flag de Comunicacoes
    pthread_mutex_t* trinco;    // Mutex para manipulacao de dados Cliente - Jogo
    pthread_t* thread;
    int abort;
    info* next;
};

extern info* jogadores;
extern int nJogadores;

// Lista de Jogadores em execucao
int verificaNomeCliente(info* jogadores, char* nome);
int verificaPidCliente(info* jogadores, comCliente* coms);
info* adicionaCliente(info* jogadores, int* nJogadores, comCliente* coms, char** jogos, const int* nJogos);
info* preencheDadosCliente(comCliente* coms, info* novo);
void listaJogadores(info* jogadores);
void removeJogador(int* nJogadores, char* jogadorRemover, int* res);
info* libertaJogadores(info* jogadores, int* nJogadores);
char* obtemNamedPipeCliente(info* jogadores, char* nomeJogador);
char* obtemNamedPipeThread(info* jogadores, char* nomeJogador);
int obtemPidCliente(info* jogadores, char* nomeJogador);
info* obtemVencedor(info* jogadores);

// Lista de Jogos
char** obtemJogos(char** jogos, int* nJogos, const init* setup);
void listaJogos(const char** jogos, const int* nJogos);
void libertarJogos(char** jogos, int* nJogos);
void obtemJogoCliente(comCliente* coms, info* jogadores);
char* sorteiaJogos(char** jogos, const int* nJogos);

// Comandos do Administrador
void gestorComandos(char* comando, info* jogadores, int* nJogadores, const char** jogos,
                    const int* nJogos);
void trataComandosCliente(comCliente* comando);

// Gestao do Campeonato
void iniciaCampeonato();
void terminaCampeonato();
void terminaTodasThreads();

#endif //ARBITRO_ARBITRO_H