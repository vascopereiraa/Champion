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

// Named Pipes
int criaPipeArbitro(fd_set* fds);
void enviaMensagemCliente(comCliente* coms);
void verificaLocalPipes();
void terminaTodosClientes(info *jogadores, const int* nJogadores, int valor);

#endif //ARBITRO_COMUNICACAO_H
