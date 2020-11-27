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
#define GAMES "."
#define TIME 10
#define WAITING_TIME 2

typedef struct {
    int MAXPLAYERS;
    int DURATION;
    int WAIT;
    char *GAMEDIR;
} init;

typedef struct {
    char nome[200];
    char jogo[200];
    int pontuacao;
} jogadores;

typedef struct {
    int pid_jogo;
} msg_jogo;

char* getGamedir();
int getMaxPlayers();
void getCmdLnArgs(int argc, char **argv, int* D, int* W);
init initialization(int argc, char **argv);
void printInit(init a);

#endif //ARBITRO_ARBITRO_H