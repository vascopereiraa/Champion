/*
 * FILE: comunicacao.h
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 12/4/20
 */

#ifndef ARBITRO_COMUNICACAO_H
#define ARBITRO_COMUNICACAO_H

#include "arbitro.h"

typedef enum {
    inicio,
    fim
} acao;

// Named Pipes
int criaPipeArbitro(fd_set* fds, char* pipe);
void enviaMensagemCliente(comCliente* coms);
void verificaLocalPipes();
void terminaTodosClientes(info *jogadores, int valor);
void enviaMensagemTodosClientes(info* jogadores, acao action);

// Funcoes de manipulacao de Threads
void* threadsClientes(void* dados_t);
void* threadTemporizacao();
void signalThread(int s, siginfo_t* info, void* context);

// Comunicacao entre Jogo e Cliente
void trocaIntComs(info* jogadores);
int intComsCliente(info* jogadores, char* nomeJogador);
int retComsCliente(info* jogadores, char* nomeJogador);

// Sinal
void avisaClientesEspera(unsigned int* clientesEspera, int nCliEspera);
void fechaClientesEspera(unsigned int* clientesEspera, int nCliEspera);

#endif //ARBITRO_COMUNICACAO_H
