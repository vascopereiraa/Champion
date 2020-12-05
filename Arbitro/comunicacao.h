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

#define FIFO_ARB "./Pipes/pipeArbitro"
#define FIFO_CLI "./Pipes/pipe%d"       // PIPE_CLIENTE -->> pipe + pid
#define BUFF_SIZE 4096

typedef struct {
    unsigned int pid;
    char nomeJogador[200];
    char mensagem[BUFF_SIZE];
    char resposta[BUFF_SIZE];
    int cdgErro;    // 0 -> Correu tudo na perfeicao
    char pipeCliente[200];
} comunicacao;

int criaPipeArbitro(fd_set* fds);

#endif //ARBITRO_COMUNICACAO_H
