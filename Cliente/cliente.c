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

#include "cliente.h"

// Dados do Jogador
comCliente p;

void trataSIGUSR1(int s, siginfo_t* info, void* context) {
    int valor = info->si_value.sival_int;
    switch (valor) {
        case 1:
            printf("[AVISO] O jogador %s desisitiu!\n", p.nomeJogador);
            break;

        case 2:
            printf("\n[AVISO] O jogador %s foi removido do campeonato pelo administrador!\n",
                        p.nomeJogador);
            exit(0);
            break;
        default:
            ;
    }
}

int main() {
    int fd, n, fdr, abort = 0;

    // Tratamento do sinal SIGUSR1
    struct sigaction act;
    act.sa_sigaction = trataSIGUSR1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    // Verifica se existe o NamedPipe do Arbitro
    if(access(FIFO_ARB , F_OK) != 0){
        fprintf(stderr , "[ERRO] O Arbitro nao se encontra em execucao\n");
        exit(1);
    }

    // Preenche a struct de comunicacao
    p.pid = getpid();
    printf("Nome:");
    fflush(stdout);
    scanf("%s",p.nomeJogador);
    sprintf(p.pipeCliente, FIFO_CLI, p.pid);
    strcpy(p.mensagem, " ");
    strcpy(p.resposta, "ok!");
    p.cdgErro = 0;
    p.pontuacao = 0;

    // Criar o NamedPipe do Cliente
    mkfifo(p.pipeCliente,0600);
    fd = open(FIFO_ARB , O_WRONLY);

    do {
        // Enviar mensagem ao Arbitro
		n = write(fd, &p, sizeof(comCliente));
		printf("Enviei: %d\n %s\n %s\n %s\n %d\n %s\n %d\n",p.pid, p.nomeJogador,
                p.mensagem, p.resposta, p.cdgErro, p.pipeCliente, p.pontuacao);

		// Ler a resposta do Arbitro
		fdr = open(p.pipeCliente, O_RDONLY);
		n = read(fdr, &p, sizeof(comCliente));
		close(fdr);

		/*
		 * if(p.cdgErro != 0)
		 *     ANALISA ERRO E RESOLVE
		 *
		 */

		printf("Recebi: %d\n %s\n %s\n %s\n %d\n %s\n %d\n",p.pid, p.nomeJogador,
               p.mensagem, p.resposta, p.cdgErro, p.pipeCliente, p.pontuacao);

        printf("\nResposta: ");
        fflush(stdout);
        scanf("%s", p.resposta);

        if(strcmp(p.resposta, "#quit") == 0)
            abort = 1;

    } while(!abort);

    // Avisar o servidor que o jogador desistiu
    strcpy(p.resposta, "O jogador desistiu\n");
    n = write(fd, &p, sizeof(comCliente));

    // Fechar o pipe do Arbitro
    close(fd);

    // Fechar o pipe do Cliente
    unlink(p.pipeCliente);

    exit(EXIT_SUCCESS);
}
