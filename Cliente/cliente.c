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
#include <string.h>
#include <signal.h>
#include <ctype.h>

#include "cliente.h"

// Dados do Jogador
comCliente p;

void encerraCliente(int* fd) {
    // Fechar o pipe do Arbitro
    close(*fd);

    // Fechar o pipe do Cliente
    unlink(p.pipeCliente);
    remove(p.pipeCliente);
}

void trataCodigoErro(int* fd) {

    unsigned int codigo = p.cdgErro;
    switch (codigo) {

        case 1:
            // Nome já se encontra registado no arbitro
            printf("\n[ERRO] O nome que introduziu \"%s\" ja se encontra registado no Campeonato!\n",
                   p.nomeJogador);
            printf("Introduza o novo nome: ");
            char novoNome[200];
            scanf("%s", novoNome);
            strcpy(p.nomeJogador, novoNome);
            formataNome();
            p.cdgErro = 0;
            enviaMensagemArbitro(&p);
            break;

        case 2:
            // Comando #mygame
            printf("\nNome do Jogo: \"%c", toupper(p.mensagem[2]));
            for(int i = 3; i < strlen(p.mensagem); ++i)
                printf("%c", p.mensagem[i]);
            printf("\"\n");
            p.cdgErro = 0;
            break;

        case 3:
            // Arbitro foi encerrado
            printf("\n[AVISO] O Arbitro foi encerrado e o campeonato terminou!\n");
            encerraCliente(fd);
            exit(0);
            break;

        case 4:
            // Removido pelo utilizador -->> k<nomeJogador>
            printf("\n[AVISO] O jogador %s foi removido do campeonato pelo administrador!\n",
                   p.nomeJogador);
            encerraCliente(fd);
            exit(0);
            break;

        case 5:
            // Numero maximo de players atingido
            printf("\n[AVISO] Já foi atingido o limite maximo de jogadores registados no campeonato!\n");
            encerraCliente(fd);
            exit(0);
            break;

        default:
            return ;
    }

}

void formataNome() {
    char nomeF[200];
    for (int i = 0; i < strlen(p.nomeJogador); ++i)
        nomeF[i] = (char) tolower(p.nomeJogador[i]);
    nomeF[strlen(p.nomeJogador)] = '\0';
    strcpy(p.nomeJogador, nomeF);
}

void enviaMensagemArbitro(comCliente* coms) {

    int fd, n;

    // Enviar mensagem ao Arbitro
    fd = open(FIFO_ARB, O_WRONLY);
    n = write(fd, coms, sizeof(comCliente));
    close(fd);
}

int main() {
    int fd, n, fdr, res, abort = 0;
    fd_set  fds;

    // Verifica se existe o Named Pipe do Arbitro
    if(access(FIFO_ARB , F_OK) != 0){
        fprintf(stderr , "[ERRO] O Arbitro nao se encontra em execucao\n");
        exit(1);
    }

    // Preenche a struct de comunicacao
    p.pid = getpid();
    printf("Nome: ");
    fflush(stdout);
    scanf("%s",p.nomeJogador);
    formataNome();
    sprintf(p.pipeCliente, FIFO_CLI, p.pid);
    strcpy(p.mensagem, " ");
    strcpy(p.resposta, " ");
    p.cdgErro = 0;
    p.pontuacao = 0;

    // Cria e abre o Named Pipe do Cliente
    mkfifo(p.pipeCliente,0600);
    fdr = open(p.pipeCliente, O_RDWR);

    // Informar o arbitro que existe
    enviaMensagemArbitro(&p);

    do {
        // Prompt de Resposta
        printf("\nCliente > ");
        fflush(stdout);

        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(fdr, &fds);

        res = select(fdr + 1, &fds, NULL, NULL, NULL);
        if(res > 0 && FD_ISSET(0, &fds)) {

            // Pede resposta para enviar ao Arbitro
            scanf("%s", p.resposta);

            // Termina a execucao do Cliente
            if(strcmp(p.resposta, "#quit") == 0) {
                abort = 1;
            }

            // Enviar mensagem ao Arbitro
            enviaMensagemArbitro(&p);
        }
        else {
            if(res > 0 && FD_ISSET(fdr, &fds)) {
                // Ler a mensagem recebida do Arbitro
                n = read(fdr, &p, sizeof(comCliente));

                // Trata de Erros na Comunicacao
                if (p.cdgErro == 0) {
                    // Imprime informacao passada pelo Arbitro
                    printf("\n%s", p.mensagem);
                }
                else {
                    trataCodigoErro(&fd);
                }
            }
        }

    } while(!abort);

    // Encerra a execucao do Cliente
    encerraCliente(&fd);

    // Termina
    exit(EXIT_SUCCESS);
}


/* printf("Recebi: %d\n %s\n %s\n %s\n %d\n %s\n %d\n",p.pid, p.nomeJogador,
               p.mensagem, p.resposta, p.cdgErro, p.pipeCliente, p.pontuacao); */