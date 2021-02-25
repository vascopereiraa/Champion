/*
 * FILE: initConfig.h
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 12/4/20
 */

#ifndef ARBITRO_INITCONFIG_H
#define ARBITRO_INITCONFIG_H

// SETUP CONSTANTS
#define PLAYER_LIMIT 5
#define GAMES "."
#define TIME 10
#define WAITING_TIME 2

typedef struct {
    int MAXPLAYERS;
    int DURATION;
    int WAIT;
    char *GAMEDIR;
} init;

extern init setup;

char* getGamedir();
int getMaxPlayers();
void getCmdLnArgs(int argc, char **argv, int* D, int* W);
init initialization(int argc, char **argv);
void printInit(init a);

#endif //ARBITRO_INITCONFIG_H
