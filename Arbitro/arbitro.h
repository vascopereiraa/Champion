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

typedef struct {
    // Dados Cliente
    char nomeJogador[200];
    char pipeCliente[200];
    unsigned int pidCliente;

    // Dados Jogo
    char nomeJogo[200];
    unsigned int pidJogo;
    int pontuacao;
} info;

int verificaCliente(info* jogadores, const int* tamJogadores, const comCliente* coms);

#endif //ARBITRO_ARBITRO_H