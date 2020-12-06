/*
 * FILE: cliente.h
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 11/1/20
 */

#ifndef CLIENTE_CLIENTE_H
#define CLIENTE_CLIENTE_H

#define FIFO_ARB "./Pipes/pipeArbitro"
#define FIFO_CLI "./Pipes/pipe%d"       // PIPE_CLIENTE -->> pipe + pid
#define BUFF_SIZE 4096

typedef struct {
    unsigned int pid;
    char nomeJogador[200];
    char mensagem[BUFF_SIZE];	// Cliente << Arbitro -- Enviado pelo Arbitro
    char resposta[BUFF_SIZE];	// Cliente >> Arbitro -- Resposta do Cliente 
    unsigned int cdgErro;    // 0 -> Correu tudo na perfeicao
    char pipeCliente[200];
    int pontuacao;
} comCliente;


#endif //CLIENTE_CLIENTE_H
