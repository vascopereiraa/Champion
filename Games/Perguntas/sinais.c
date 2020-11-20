/*
 * FILE: signals.c
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 11/19/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "sinais.h"

void timer(int s, siginfo_t *info, void *context) {
    if (info->si_pid == 0) {
        printf("\n[TEMPO] Esgotou o tempo para responder! -> Resposta Errada\n");
        printf("ASD\n");
        if (pontuacao > 0)
            pontuacao--;
        sleep(INT);
    }
}

void endGame(int s, siginfo_t *info, void *context) {
    if(s == SIGUSR1) {
        printf("%d\n", info->si_value);
        exit(pontuacao);
    }
}