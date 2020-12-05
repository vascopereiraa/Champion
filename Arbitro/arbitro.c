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
#include <sys/select.h>
#include <fcntl.h>

#include "initConfig.h"
#include "comunicacao.h"

/* void criaJogo(const init* setup) {

    int i = 0, res, pid, canalLeitura[2], canalEscrita[2], estado;
    DIR* dir;
    struct dirent* conteudo;
    char listaJogos[10][100];

    // Abrir pasta
    dir = opendir(setup->GAMEDIR);
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
        printf("[%4d] Sou o filho\n", pid);
        
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
        chdir(setup->GAMEDIR);
        execl(listaJogos[0], listaJogos[0], NULL);

        fprintf(stderr, "[%4d] FILHO: Erro a executar o jogo\n", pid);
        exit(10);
    }
    close(canalLeitura[1]);
    close(canalEscrita[0]);

    int n = 1;
    while(n) {
        char string[BUFF_SIZE];

        n = read(canalLeitura[0], string, sizeof(string) - 1);
        if(n <= 0)
            break;
        string[n] = '\0';
        printf("%s", string);

        n = scanf("%s", string);
        string[n] = '\0';
        write(canalEscrita[1], string, sizeof(string));
    }

    // Troca de dados com o cliente
    wait(&estado);
    if(WIFEXITED(estado))
        printf("%d\n\n", WEXITSTATUS(estado));
} */

int main(int argc, char* argv[]) {

    setbuf(stdout, NULL);

    comunicacao coms;

    // Definicao do Ambiente
    const init setup = initialization(argc, argv);
    printInit(setup);

    // Criacao do NamedPipe do Arbitro
    fd_set  fds;
    FD_ZERO(&fds);
    int fd = criaPipeArbitro(&fds);

    FD_SET(0, &fds);
    char cmd[200];
    do {
        printf("Comando: ");
        fflush(stdout);

        int res = select(fd + 1, &fds, NULL, NULL, NULL);
        if (res > 0 && FD_ISSET(0, &fds))
            scanf("%s", cmd);
        else
            if (res > 0 && FD_ISSET(fd, &fds)) {
               int n = read(fd, &coms, sizeof(comunicacao));
               printf("RECEBI:\nPID: %d\nNome: %sMensagem: %s\n", coms.pid, coms.nomeJogador, coms.mensagem);

               strcpy(coms.resposta, "ok!");
               sprintf(coms.pipeCliente, FIFO_CLI, coms.pid);
               coms.cdgErro = 0;
               int fdr = open(coms.pipeCliente, O_WRONLY);
               n = write(fdr, &coms, sizeof(comunicacao));
               close(fdr);
               puts(" ");
               printf("ESCREVI:\nPID: %d\nNome: %s\nMensagem: %s\nResposta: %s\nCodigo_Erro: %d\nPipe_Cliente: %s",
                      coms.pid, coms.nomeJogador, coms.mensagem, coms.resposta, coms.cdgErro, coms.pipeCliente);
            }
    } while(strcmp(cmd, "sair") != 0);


    // Fechar o NamedPipe do Arbitro
    close(fd);
    unlink(FIFO_ARB);

    free(setup.GAMEDIR);
    exit(EXIT_SUCCESS);
}
