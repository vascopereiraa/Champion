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

int criaPipeArbitro(fd_set* fds) {

    int fd;

    // Acede/Cria named pipe do Arbitro
    if(access(FIFO_ARB, F_OK) != 0) {
        mkfifo(FIFO_ARB, 0600);

        fd = open(FIFO_ARB, O_RDWR);
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
    puts(" ");

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

void terminaTodosClientes(info *jogadores, const int* nJogadores, int valor) {

    for(int i = 0; i < (*nJogadores); ++i) {
        comCliente coms;
        strcpy(coms.pipeCliente, jogadores[i].pipeCliente);
        coms.cdgErro = valor;
        enviaMensagemCliente(&coms);
    }

}

