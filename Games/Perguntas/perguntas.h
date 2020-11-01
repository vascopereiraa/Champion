/*
 * FILE: perguntas.h
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 11/1/20
 */

#ifndef PERGUNTAS_PERGUNTAS_H
#define PERGUNTAS_PERGUNTAS_H

typedef struct Perguntas perguntas;

struct Perguntas {
    char pergunta[200];
    char respostas[4][100];
    int solucao;
};

#endif //PERGUNTAS_PERGUNTAS_H
