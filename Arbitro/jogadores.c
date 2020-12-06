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

#include "../Cliente/cliente.h"
#include "arbitro.h"

int verificaCliente(info* jogadores, const int* tamJogadores, comCliente* coms) {

    // Se a lista de jogadores tiver vazia
    if(jogadores == NULL)
        return 0;

    // Verifica se o jogador existe na lista
    for (int i = 0; i < *tamJogadores; ++i) {
        if(jogadores[i].pidCliente == coms->pid)
            return 1;
    }

    // Caso o jogador nao exista na lista
    return 0;
}

info* adicionaCliente(info* jogadores, int* tamJogadores, comCliente* coms) {

    info* array;

    // Realocar espaco para um novo jogador
    array = realloc(jogadores, sizeof(info) * ((*tamJogadores) + 1));
    if(array == NULL) {
        fprintf(stderr, "[ERRO] Erro ao adicionar novo cliente!\n");
        coms->cdgErro = 1;
        return jogadores;
    }

    // Preencher os dados do jogador
    jogadores = array;
    jogadores[(*tamJogadores)++] = preencheDadosCliente(coms);

    return jogadores;
}

info preencheDadosCliente(comCliente* coms) {

    info novo;

    novo.pidCliente = coms->pid;
    strcpy(novo.pipeCliente, coms->pipeCliente);
    strcpy(novo.nomeJogador, coms->nomeJogador);

    return novo;
}

void listaJogadores(const info* jogadores, const int* tamJogadores) {

    for(int i = 0; i < (*tamJogadores); ++i) {
        printf("Jogador %d:\nNome: %s\nJogo: %s\n\n", i+1, jogadores[i].nomeJogador, jogadores[i].nomeJogo);
    }

}