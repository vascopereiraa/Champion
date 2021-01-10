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

typedef struct {
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
    pthread_mutex_t *trinco;    // Mutex para manipulacao de dados Cliente - Jogo
} info;

info* jogadores;
int nJogadores;

// Lista de Jogadores em execucao
int verificaNomeCliente(info* jogadores, const int* nJogadores, comCliente* coms);
int verificaPidCliente(info* jogadores, const int* nJogadores, comCliente* coms);
info* adicionaCliente(info* jogadores, int* nJogadores, comCliente* coms, char** jogos, const int* nJogos);
info preencheDadosCliente(comCliente* coms);
void listaJogadores(const info* jogadores, const int* nJogadores);
int existeJogador(info* jogadores, const int* nJogadores, char* jogadorRemover);
int removeJogador(info** jogadores, int* nJogadores, const int* posicao);
void libertaJogadores(info* jogadores, int* nJogadores);

// Lista de Jogos
char** obtemJogos(char** jogos, int* nJogos, const init* setup);
void listaJogos(const char** jogos, const int* nJogos);
void libertarJogos(char** jogos, int* nJogos);
void obtemJogoCliente(comCliente* coms, info* jogadores, const int* nJogadores);
char* sorteiaJogos(char** jogos, const int* nJogos);

// Comandos do Administrador
void gestorComandos(char* comando, info* jogadores, int* nJogadores, const char** jogos,
                    const int* nJogos);
void trataComandosCliente(comCliente* comando, info* jogadores, int* nJogadores);

// Gestao do Campeonato
void iniciaCampeonato();
void terminaCampeonato();

#endif //ARBITRO_ARBITRO_H