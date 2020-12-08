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

void trataSIGUSR1(int s, siginfo_t* info, void* context) {
    int valor = info->si_value.sival_int;
    switch (valor) {
        case 0:
            printf("\n[AVISO] O Arbitro foi encerrado e o campeonato terminou!\n");
            exit(0);
            break;

        case 1:
            printf("\n[AVISO] O jogador %s desisitiu!\n", p.nomeJogador);
            break;

        case 2:
            printf("\n[AVISO] O jogador %s foi removido do campeonato pelo administrador!\n",
                        p.nomeJogador);
            exit(0);
            break;
            
        case 3:
        	printf("\n[AVISO] Já foi atingido o limite maximo de jogadores registados no campeonato!\n");
        	exit(0);
        	break;
        	
        default:
            return ;
    }
}

void trataCodigoErro() {

    unsigned int codigo = p.cdgErro;
    switch (codigo) {

        // Nome já se encontra registado no arbitro
        case 1:
            printf("\n[ERRO] O nome que introduziu \"%s\" ja se encontra registado no Campeonato!\n",
                   p.nomeJogador);
            printf("Introduza o novo nome: ");
            char novoNome[200];
            scanf("%s", novoNome);
            strcpy(p.nomeJogador, novoNome);
            p.cdgErro = 0;
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
    printf("Nome: ");
    fflush(stdout);
    scanf("%s",p.nomeJogador);
    formataNome();
    printf("%s", p.nomeJogador);
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
		/* printf("Enviei: %d\n %s\n %s\n %s\n %d\n %s\n %d\n",p.pid, p.nomeJogador,
                p.mensagem, p.resposta, p.cdgErro, p.pipeCliente, p.pontuacao); */

		// Ler a resposta do Arbitro
		fdr = open(p.pipeCliente, O_RDONLY);
		n = read(fdr, &p, sizeof(comCliente));
		close(fdr);

        printf("Recebi: %d\n %s\n %s\n %s\n %d\n %s\n %d\n",p.pid, p.nomeJogador,
               p.mensagem, p.resposta, p.cdgErro, p.pipeCliente, p.pontuacao);

		if(p.cdgErro != 0) {
		    trataCodigoErro();
		}

		printf("%s", p.nomeJogador);


        printf("\nResposta: ");
        fflush(stdout);
        scanf("%s", p.resposta);


        if(strcmp(p.resposta, "#quit") == 0)
            abort = 1;

    } while(!abort);

    // Avisar o servidor que o jogador desistiu
    n = write(fd, &p, sizeof(comCliente));

    // Fechar o pipe do Arbitro
    close(fd);

    // Fechar o pipe do Cliente
    unlink(p.pipeCliente);

    exit(EXIT_SUCCESS);
}
