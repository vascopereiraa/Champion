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

// Sinais
void signalTerminaExecucao(const unsigned int* pid, int valor);
void terminaTodosClientes(info *jogadores, const int* nJogadores, int valor);

// Named Pipes
int criaPipeArbitro(fd_set* fds);
void verificaLocalPipes();

#endif //ARBITRO_COMUNICACAO_H
