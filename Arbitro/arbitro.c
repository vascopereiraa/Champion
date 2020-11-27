/*
 * FILE: arbitro.c
 * NOME: Andre Lima Domingues
 * NR: 2019127839
 *
 * NOME: Vasco Daniel Matos Pereira
 * NR: 2019134744
 *
 * CREATED ON 11/1/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>

#include "arbitro.h"

/*void criaJogo() {

    int i = 0, res, pid, canalLeitura[2], canalEscrita[2], estado;
    DIR* dir;
    struct dirent* conteudo;
    char listaJogos[10][100];

    // Abrir pasta
    dir = opendir(GAMES);
    if(dir == NULL)
        exit(2);

    // Listar todos os jogos na pasta
    while((conteudo = readdir(dir)) != NULL) {
        if(conteudo->d_name[0] == 'g' && conteudo->d_name[1] == '_')
            strcpy(listaJogos[i++], conteudo->d_name);
    }

    closedir(dir);

    for(int k = 0; k < i; k++)
        printf("%s\n", listaJogos[k]);
    printf("\n");

    // todo: separar em diversas funcoes

    // Criar o pipe anonimo de comunicaocao com o filho
    pipe(canalLeitura);
    pipe(canalEscrita);
    
    // Criar o filho
    res = fork();
    if(res == 0) {
        pid = getpid();
        printf("Sou o filho");
        
        // Atribuir saida e entrada de dados para o pipe
        close(canalLeitura[0]);
        close(1);
        dup(canalLeitura[1]);
        close(canalLeitura[1]);
        
        close(canalEscrita[1]);
        close(0);
        dup(canalEscrita[0]);
        close(canalEscrita[0]);

        // Mudar diretoria corrente
        // todo: acrescentar verificacao da diretoria
        chdir(GAMES);
        execl(listaJogos[0], listaJogos[0], NULL);

        fprintf(stderr, "[%4d] FILHO: Erro a executar o jogo", pid);
        exit(10);
    }
    close(canalLeitura[1]);
    close(canalEscrita[0]);

    // Troca de dados com o cliente

    wait(&estado);
    if(WIFEXITED(estado))
        printf("%d\n\n", WEXITSTATUS(estado));
}*/

int main(int argc, char* argv[]) {

    setbuf(stdout, NULL);

    const init setup = initialization(argc, argv);
    printInit(setup);

    // msg_jogo jogo1 = criaJogo();
    // criaJogo();


    exit(EXIT_SUCCESS);
}
