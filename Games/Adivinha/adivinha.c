/*
 * FILE: adivinha.c
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 12/30/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#include "../sinais.h"

#define LIM 50

int pontuacao = 0;

int main () {

    // setbuf(stdout, NULL);

    // Manipulacao de Sinais
    struct sigaction actionEndGame;
    actionEndGame.sa_sigaction = endGame;
    actionEndGame.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &actionEndGame, NULL);

    int valor1, valor2, operador, res, correta;
    int count = 0, n = 0;

    // Inicializar o gerador de numeros
    srand((unsigned) time(NULL));

    // Descricao do Jogo
    printf("                      JOGO DO ADIVINHA\n");
    printf("O jogo consiste em acertar o maior numero de equacoes possivel.\n"
           "Sera apresentada a conta, que e sorteada aleatoriamente. \n"
           "Para responder basta indicar a solucao pretendida.\n");

    do {

        puts("-----------------------------------------------");
		printf("Pontuacao: %d\n", pontuacao);
        valor1 = rand() % 100;
        valor2 = rand() % 100;
        operador = rand() % 4;

        switch (operador) {
            case 0:
                printf("Conta %d -> %d + %d = ?\n", count+1, valor1, valor2);
                correta = valor1 + valor2;
                break;
            case 1:
                printf("Conta %d -> %d - %d = ?\n", count+1, valor1, valor2);
                correta = valor1 - valor2;
                break;
            case 2:
                printf("Conta %d -> %d * %d = ?\n", count+1, valor1, valor2);
                correta = valor1 * valor2;
                break;
            case 3:
                printf("Conta %d -> %d / %d = ?\n", count+1, valor1, valor2);
                correta = valor1 / valor2;
                break;
            default:
                // printf("%d -- %d %d %d\n", cont+1, valor1, operador, valor2);
                break;
        }
		
		fflush(stdout);
		
        // Verifica a resposta dada
        do {
        	n = 0;
        	n = scanf("%d", &res);
        } while(n == 0);
        
        if (n == 1) {
            if (res == correta) {
                ++pontuacao;
                printf("Resposta Correta!!\n");
                fflush(stdout);
            } else {
                printf("Resposta Errada!!\n");
                fflush(stdout);
                if(pontuacao > 0) {
                    --pontuacao;
                }
            }
        }

        ++count;
    } while(count < LIM);

	puts("-------------------------------------------------------------");
    printf("Pontuacao Final: %d\n\n", pontuacao);
    fflush(stdout);
    printf("fimJogo\n");
    fflush(stdout);
    
    exit(pontuacao);
}