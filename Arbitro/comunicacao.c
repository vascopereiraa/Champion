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
#include <sys/select.h>
#include <fcntl.h>

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
    exit(1);
}

