/*
 * FILE: threads.c
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 1/6/21
 */

#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>

#include "initConfig.h"
#include "../Cliente/cliente.h"
#include "comunicacao.h"
#include "arbitro.h"

int max(int a, int b) {
    return (a > b) ? a : b;
}

void* threadsClientes(void* dados_t) {

    int fd;
    info* dados = (info*) dados_t;

    // Criacao do NamedPipe exclusivo do Jogador
    fd_set fds;
    fd = criaPipeArbitro(&fds, dados->pipeThread);

    // Cria o filho -->> Jogo sorteado
    int estado, pid, res, canalJogo_Arb[2], canalArb_Jogo[2];
    // Criar o pipe anonimo de comunicaocao com o filho
    pipe(canalJogo_Arb);
    pipe(canalArb_Jogo);

    // Criar o filho
    res = fork();
    if(res == 0) {
        pid = getpid();
        // printf("[%4d] Sou o filho\n", pid);

        // Atribuir saida e entrada de dados para o pipe
        close(canalJogo_Arb[0]);
        close(1);
        dup(canalJogo_Arb[1]);
        close(canalJogo_Arb[1]);

        close(canalArb_Jogo[1]);
        close(0);
        dup(canalArb_Jogo[0]);
        close(canalArb_Jogo[0]);

        // Mudar diretoria corrente
        chdir(setup.GAMEDIR);
        execl(dados->nomeJogo, dados->nomeJogo, NULL);

        fprintf(stderr, "[%4d] FILHO: Erro a executar o jogo\n", pid);
        exit(10);
    }

    // Guardar dados do Jogo
    dados->pidJogo = res;

    close(canalJogo_Arb[1]);
    close(canalArb_Jogo[0]);

    // Implementar a comunicacao entre os clientes e jogos --> Select
    int sel, n;
    comCliente* coms = (comCliente*) malloc(sizeof(comCliente));
    if (coms == NULL) {
        fprintf(stderr, "[ERRO] Nao foi possivel criar o armazenamento das comunicaoes!\n");
        exit(4);
    }
    // Preencher a informaçao da struct de comunicacao
    coms->pid = dados->pidCliente;
    strcpy(coms->pipeCliente, dados->pipeCliente);
    strcpy(coms->pipeArbitro, dados->pipeThread);
    strcpy(coms->nomeJogador, dados->nomeJogador);

    int abort = 0;
    do {
        // Definicao dos descritores abertos
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        FD_SET(canalJogo_Arb[0], &fds);

        sel = select(max(fd, canalJogo_Arb[0]) + 1, &fds, NULL, NULL, NULL);
        pthread_mutex_lock(dados->trinco);
        // Caso receba dados pelo seu NamedPipe
        if(sel > 0 && FD_ISSET(fd, &fds)) {
            // Ler informacao passada pelo NamedPipe
            n = read(fd, coms, sizeof(comCliente));
            if(coms->pid == dados->pidCliente) {
                // Tratamento da resposta do Cliente
                if(coms->resposta[0] == '#') {
                    // Mensagem destinada ao Arbitro
                    trataComandosCliente(coms, jogadores, &nJogadores);
                    enviaMensagemCliente(coms);

                    if(strcmp(coms->resposta, "#quit") == 0)
                        abort = 1;
                }
                else {
                    strcat(coms->resposta, "\n");
                    printf("\nMensagem a enviar: %s\n", coms->resposta);
                    fflush(stdout);

                    // Mensagem destinada ao Jogo
                    n = write(canalArb_Jogo[1], coms->resposta, strlen(coms->resposta));
                    printf("\n\nEnviado para o jogo: %d\n", n);
                    fflush(stdout);
                }
            }
            else {
                // Manda trocar o canal de comunicacao do cliente p/ arbitro certo
                coms->cdgErro = 6;

                // Envia mensagem cliente
                enviaMensagemCliente(coms);
            }
        }

        // Verifica se pode comunicar
        if(dados->intComunicacao == 0) {

            // Dados passados pelo pipe anonimo
            if (sel > 0 && FD_ISSET(canalJogo_Arb[0], &fds)) {
                memset(coms->mensagem, 0, BUFF_SIZE);
                n = read(canalJogo_Arb[0], coms->mensagem, sizeof(coms->mensagem));
                printf("\nEntra do jogo: %d\n", n);
                fflush(stdout);
                coms->mensagem[n] = '\0';
                /* if ((n > 0) && (coms->mensagem[strlen(coms->mensagem) - 1] == '\n'))
                    coms->mensagem[strlen(coms->mensagem) - 1] = '\0';
                coms->cdgErro = 0; */
                printf("\nMensagem recebida: %s\n", coms->mensagem);
                fflush(stdout);
                enviaMensagemCliente(coms);
            }

        }
        pthread_mutex_unlock(dados->trinco);

    } while(strcmp(coms->resposta, "#quit") != 0);

    // Terminar jogo & Colocar pid do jogo a -1

    // Armazenar pontuacao e enviar ao jogador

    // Fechar thread do arbitro para o determinado cliente

}

void* threadTemporizacao() {

    // Temporiza a espera
    printf("\nCampeonato prestes a começar!\n");
    fflush(stdout);

    // Enviar uma mensagem aos clientes


    // sleep(setup.WAIT * 60);
    sleep(10);

    // Mandar o campeonato executar
    iniciaCampeonato();

    // Enviar mensagem aos clientes


    // Temporizar o campeonato
    sleep(setup.DURATION * 60);

    // Mandar o campeonato terminar
    terminaCampeonato();
}