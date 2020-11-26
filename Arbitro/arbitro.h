/*
 * FILE: arbitro.h
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 11/1/20
 */

#ifndef ARBITRO_ARBITRO_H
#define ARBITRO_ARBITRO_H

// CONSTANTS
#define PLAYER_LIMIT 10
#define GAMES "./Games"
#define TIME 10
#define WAITING_TIME 2

typedef struct {
    char nome[200];
    char jogo[200];
    int pontuacao;
} jogadores;

typedef struct {
    int pid_jogo;
} msg_jogo;


#endif //ARBITRO_ARBITRO_H