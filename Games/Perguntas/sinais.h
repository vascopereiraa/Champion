/*
 * FILE: signals.h
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 11/19/20
 */

#ifndef PERGUNTAS_SINAIS_H
#define PERGUNTAS_SINAIS_H

#define WAIT 5
#define TIME 5
#define INT 2

int pontuacao;

void timer(int s, siginfo_t *info, void *context);

#endif //PERGUNTAS_SINAIS_H
