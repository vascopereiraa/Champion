/*
 * FILE: jogadores.c
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 12/6/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include "../Cliente/cliente.h"
#include "arbitro.h"

int verificaNomeCliente(info* jogadores, char* nome) {

    info* aux = jogadores;

    while(aux != NULL && strcmp(aux->nomeJogador, nome) != 0)
        aux = aux->next;
    // Se o nao houver um jogador com esse nome registado
    if(aux == NULL)
        return 1;
    else
        return 0;

}

int verificaPidCliente(info* jogadores, comCliente* coms) {

    info* aux = jogadores;

    while(aux != NULL && aux->pidCliente != coms->pid)
        aux = aux->next;
    // Se o nao houver um jogador com esse PID registado
    if(aux == NULL)
        return 0;
        // Caso ainda nao exista esse PID no sistema
    else
        return 1;

}

info* adicionaCliente(info *jogadores, int *nJogadores, comCliente *coms, char **jogos, const int *nJogos) {

    // Alocar espaco para um novo jogador
    info* novo = (info*) malloc(sizeof(info));
    if(novo == NULL) {
        fprintf(stderr, "[ERRO] Erro ao adicionar novo cliente!\n");
        return jogadores;
    }

    // Preencher os dados do jogador
    novo = preencheDadosCliente(coms, novo);

    // Sorteia o jogo para o jogador
    strcpy(novo->nomeJogo, sorteiaJogos(jogos, nJogos));

    // Insere node na lista ligada
    novo->next = jogadores;
    jogadores = novo;
    ++(*nJogadores);

    /*printf("\n\n[DEBUG] NOME: %s\tPIPECLI: %s", jogadores->nomeJogador,
           jogadores->pipeCliente);*/

    return jogadores;
}

info* preencheDadosCliente(comCliente* coms, info* novo) {

    novo->intComunicacao = 1;
    novo->pidCliente = coms->pid;
    strcpy(novo->pipeCliente, coms->pipeCliente);
    sprintf(novo->pipeThread, FIFO_THR, novo->pidCliente);
    strcpy(novo->nomeJogador, coms->nomeJogador);
    novo->pontuacao = 0;
    novo->abort = 0;

    return novo;
}

void listaJogadores(info* jogadores) {

    info* aux = jogadores;

    if(jogadores == NULL) {
        printf("[AVISO] Ainda nenhum jogador entrou no campeonato!\n");
    }
    else {
        int i = 1;
        while (aux != NULL) {
            printf("Jogador %d:\nNome: %s\nJogo: %s\n\n", i++, aux->nomeJogador, aux->nomeJogo);
            aux = aux->next;
        }
    }
}

void removeJogador(int* nJogadores, char* jogadorRemover, int* res) {

    info *atual = jogadores;
    info *anterior = NULL;

    while (atual != NULL && strcmp(atual->nomeJogador, jogadorRemover) != 0) {
        anterior = atual;
        atual = atual->next;
    }

    // Caso nao exista
    if (atual == NULL) {
        printf("[AVISO] O jogador que pretende remover nao existe!\nNome: %s",
               jogadorRemover);
        *res = 0;
        return ;
    }

    // Caso seja o primeiro da lista
    if (atual == jogadores) {
        jogadores = jogadores->next;

        // Fecha a thread -> A thread fecha o jogo
        atual->abort = 1;
        pthread_kill(*(atual->thread), SIGUSR2);
        pthread_join(*(atual->thread), NULL);
    }
    else {
        anterior->next = atual->next;

        // Fecha a thread -> A thread fecha o jogo
        atual->abort = 1;
        pthread_kill(*(atual->thread), SIGUSR2);
        pthread_join(*(atual->thread), NULL);
    }
    *res = 1;
    free(atual);
    --(*nJogadores);

    return ;
}

info* libertaJogadores(info* jogadores, int* nJogadores) {

    info* aux;
    while(jogadores != NULL) {
        aux = jogadores;
        jogadores = jogadores->next;
        free(aux->thread);
        free(aux);
    }

    return jogadores;
}

char* obtemNamedPipeCliente(info* jogadores, char* nomeJogador) {

    info* aux = jogadores;

    while(aux != NULL && strcmp(aux->nomeJogador, nomeJogador) != 0)
        aux = aux->next;
    // Se o já houver um jogador com esse nome registado
    if(aux != NULL)
        return aux->pipeCliente;
        // Caso ainda nao exista esse nome no sistema
    else
        return " ";

}

char* obtemNamedPipeThread(info* jogadores, char* nomeJogador) {

    info* aux = jogadores;

    while(aux != NULL && strcmp(aux->nomeJogador, nomeJogador) != 0)
        aux = aux->next;
    // Se o já houver um jogador com esse nome registado
    if(aux != NULL)
        return aux->pipeThread;
        // Caso ainda nao exista esse nome no sistema
    else
        return " ";

}

int obtemPidCliente(info* jogadores, char* nomeJogador) {

    info* aux = jogadores;

    while(aux != NULL && strcmp(aux->nomeJogador, nomeJogador) != 0)
        aux = aux->next;
    // Se o já houver um jogador com esse nome registado
    if(aux != NULL)
        return aux->pidCliente;
    // Caso ainda nao exista esse nome no sistema
    else
        return -1;

}

info* obtemVencedor(info* jogadores) {

    int max;
    info* vencedor;
    info* aux = jogadores;
    max = aux->pontuacao;
    vencedor = aux;
    aux = aux->next;
    while(aux != NULL) {
        if(aux->pontuacao > max) {
            max = aux->pontuacao;
            vencedor = aux;
        }
        aux = aux->next;
    }

    return vencedor;
}

