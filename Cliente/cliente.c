/*
 * FILE: cliente.c
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 11/1/20
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#include "cliente.h"

int main() {
    int fd,n,fdr;
    comunicacao p;

    if(access(FIFO_ARB , F_OK)!=0){
        fprintf(stderr , "[ERRO]O servidor nao se encontra em execucao\n");
    }

    p.pid =getpid();
    printf("Nome:");
    fflush(stdout);
    scanf("%s",p.nomeJogador);

    sprintf(p.pipeCliente,FIFO_CLI,p.pid);
    mkfifo(p.pipeCliente,0600);
    fd = open(FIFO_ARB , O_WRONLY);

    do{
        //preencher a struct
    n=write(fd,&p,sizeof(comunicacao));
    printf("Enviei: \n  %d  \n %s \n %s",p.pid,p.nomeJogador,p.pipeCliente);

    fdr = open(p.pipeCliente,O_RDONLY);
    n= read(fdr,&p,sizeof(comunicacao));
    close(fdr);

    printf("Recebi: \n  %d  \n %s \n %s",p.pid,p.nomeJogador,p.pipeCliente);

    }while(1);

    close(fd);
    unlink(p.pipeCliente);

    exit(0);
}