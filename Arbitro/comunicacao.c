/*
 * FILE: comunicacao.c
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 12/4/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include "../Cliente/cliente.h"
#include "comunicacao.h"

int criaPipeArbitro(fd_set* fds, char* pipe) {

    int fd;

    // Acede/Cria named pipe do Arbitro
    if(access(pipe, F_OK) != 0) {
        mkfifo(pipe, 0600);

        fd = open(pipe, O_RDWR);
        return fd;
    }

    // Caso nao consiga aceder ao FIFO
    printf("Nao foi possivel aceder ao canal de comunicacao do Arbitro\n");
    exit(3);
}

void enviaMensagemCliente(comCliente* coms) {
    int fdr, n;

    // Enviar informacao ao Cliente pelo respetivo Named Pipe
    fdr = open(coms->pipeCliente, O_WRONLY);
    n = write(fdr, coms, sizeof(comCliente));
    close(fdr);
    // puts(" ");

}

void verificaLocalPipes() {

    if(mkdir("./Pipes", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
        return ;
    }
    else
        if(errno == EEXIST)
            return ;

    fprintf(stderr, "[ERRO] A pasta de armazenamento de named pipes encontra-se indisponivel!\n");
    exit(2);

}

void terminaTodosClientes(info *jogadores, int valor) {

    info* aux = jogadores;

    while(aux != NULL) {
        comCliente coms;
        strcpy(coms.pipeCliente, aux->pipeCliente);
        coms.cdgErro = valor;
        enviaMensagemCliente(&coms);
        aux = aux->next;
    }
}

// Funcao apenas serve para anunciar o fim ou o inicio do campeonato aos jogadores
void enviaMensagemTodosClientes(info* jogadores, acao action) {

    info* aux = jogadores;
    comCliente coms;

    while(aux != NULL) {

        coms.pid = aux->pidCliente;
        strcpy(coms.pipeCliente, aux->pipeCliente);
        strcpy(coms.nomeJogador, aux->nomeJogador);
        if(action == inicio) {
            strcpy(coms.pipeArbitro, aux->pipeThread);
            coms.cdgErro = 9;
        }
        else {
            info* vencedor = obtemVencedor(jogadores);
            sprintf(coms.mensagem, "O vencedor do Campeonato foi o %s com a pontuacao total de %d pontos\n",
                        vencedor->nomeJogador, vencedor->pontuacao);
            strcpy(coms.pipeArbitro, FIFO_ARB);
            coms.pontuacao = aux->pontuacao;
            coms.cdgErro = 10;
        }
        enviaMensagemCliente(&coms);
        aux = aux->next;
    }
}

void trocaIntComs(info* jogadores) {

    info* aux = jogadores;

    while(aux != NULL) {
        aux->intComunicacao = !aux->intComunicacao;
        aux = aux->next;
    }

}

int intComsCliente(info* jogadores, char* nomeJogador) {

    info* aux = jogadores;

    while(aux != NULL && strcmp(aux->nomeJogador, nomeJogador) != 0)
        aux = aux->next;
    // Se o nao houver um jogador com esse nome registado
    if(aux == NULL)
        return 0;
    else {
        if(aux->intComunicacao == 1)
            return 0;
        aux->intComunicacao = 1;
        return 1;
    }

}

int retComsCliente(info* jogadores, char* nomeJogador) {

    info* aux = jogadores;

    while(aux != NULL && strcmp(aux->nomeJogador, nomeJogador) != 0)
        aux = aux->next;
    // Se o nao houver um jogador com esse nome registado
    if(aux == NULL)
        return 0;
    else {
        if(aux->intComunicacao == 0)
            return 0;
        aux->intComunicacao = 0;
        return 1;
    }

}

void avisaClientesEspera(unsigned int* clientesEspera, int nCliEspera) {

    union sigval value;
    for(int i = 0; i < nCliEspera; ++i)
        sigqueue(*clientesEspera + i, SIGUSR2, value);
}

void fechaClientesEspera(unsigned int* clientesEspera, int nCliEspera) {

    comCliente coms;
    for(int i = 0; i < nCliEspera; ++i) {
        sprintf(coms.pipeCliente, FIFO_CLI, clientesEspera[i]);
        coms.cdgErro = 3;
        enviaMensagemCliente(&coms);
    }
}


