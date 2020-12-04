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

#define PIPE_ARBITRO "pipeArbitro"
#define PIPE_CLIENTE "pipe%d"       // PIPE_CLIENTE -->> pipe + pid

typedef struct {
    int pid;
    char nomeJogador[200];
    char mensagem[BUFF_SIZE];
    char resposta[BUFF_SIZE];
} comunicacao;

#endif //ARBITRO_COMUNICACAO_H
