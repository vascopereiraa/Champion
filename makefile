# Compilacao de todos os programas
all: jogo arbitro cliente

# Ficheiros *.o do Jogo (Adivinha)
Games/Adivinha/adivinha.o: ./Games/Adivinha/adivinha.c ./Games/sinais.h
	gcc -c ./Games/Adivinha/adivinha.c -o ./Games/Adivinha/adivinha.o

# Ficheiros *.o do Jogo (Perguntas)
Games/Perguntas/perguntas.o: ./Games/Perguntas/perguntas.c ./Games/Perguntas/perguntas.h ./Games/sinais.h
	gcc -c ./Games/Perguntas/perguntas.c -o ./Games/Perguntas/perguntas.o

# Ficheiros *.o dos sinais dos Jogos	
Games/sinais.o: ./Games/sinais.c ./Games/sinais.h
	gcc -c ./Games/sinais.c -o ./Games/sinais.o

# Ficheiros *.o do Arbitro
Arbitro/arbitro.o: ./Arbitro/arbitro.c ./Arbitro/arbitro.h ./Arbitro/initConfig.h ./Arbitro/comunicacao.h ./Cliente/cliente.h
	gcc -c ./Arbitro/arbitro.c -o ./Arbitro/arbitro.o

Arbitro/initConfig.o: ./Arbitro/initConfig.c  ./Arbitro/initConfig.h 
	gcc -c ./Arbitro/initConfig.c -o ./Arbitro/initConfig.o
	
Arbitro/comunicacao.o: ./Arbitro/comunicacao.c ./Arbitro/comunicacao.h ./Cliente/cliente.h
	gcc -c ./Arbitro/comunicacao.c -o ./Arbitro/comunicacao.o

Arbitro/jogadores.o: ./Arbitro/jogadores.c ./Arbitro/arbitro.h ./Cliente/cliente.h
	gcc -c ./Arbitro/jogadores.c -o ./Arbitro/jogadores.o
	
Arbitro/threads.o: ./Arbitro/threads.c ./Arbitro/arbitro.h ./Cliente/cliente.h ./Arbitro/comunicacao.h ./Arbitro/initConfig.h 
	gcc -c ./Arbitro/threads.c -o ./Arbitro/threads.o
	
# Ficheiros *.o do Cliente	
Cliente/cliente.o: ./Cliente/cliente.c ./Cliente/cliente.h
	gcc -c ./Cliente/cliente.c -o Cliente/cliente.o

# Executavel do Jogo (Perguntas)
Games/g_perguntas: Games/Perguntas/perguntas.o Games/sinais.o
	gcc -o Games/g_perguntas ./Games/Perguntas/perguntas.o ./Games/sinais.o
	
# Executavel do Jogo (Adivinha)
Games/g_adivinha: Games/Adivinha/adivinha.o Games/sinais.o
	gcc -o Games/g_adivinha ./Games/Adivinha/adivinha.o ./Games/sinais.o

jogo: Games/g_perguntas Games/g_adivinha
	
# Executavel do Arbitro
arbitro: Arbitro/arbitro.o Arbitro/initConfig.o Arbitro/comunicacao.o Arbitro/jogadores.o Arbitro/threads.o
	gcc -o arbitro Arbitro/arbitro.o Arbitro/initConfig.o Arbitro/comunicacao.o Arbitro/jogadores.o Arbitro/threads.o -l pthread -Wall -Wextra
	 
# Executavel do Cliente
cliente: Cliente/cliente.o
	gcc -o cliente Cliente/cliente.o

# Limpa todos os ficheiros *.o criados
clean:
	find . -name '*.o' -delete
	
# Limpa todos os pipes
clnpipe:
	rm ./Pipes/*