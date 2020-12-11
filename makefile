# Compilacao de todos os programas
all: jogo arbitro cliente

# Ficheiros *.o do Jogo (Perguntas)
Games/Perguntas/jogo.o: ./Games/Perguntas/perguntas.c ./Games/Perguntas/perguntas.h ./Games/Perguntas/sinais.h
	gcc -c ./Games/Perguntas/perguntas.c -o ./Games/Perguntas/jogo.o
	
Games/Perguntas/sinais.o: ./Games/Perguntas/sinais.c ./Games/Perguntas/sinais.h
	gcc -c ./Games/Perguntas/sinais.c -o ./Games/Perguntas/sinais.o

# Ficheiros *.o do Arbitro
Arbitro/arbitro.o: ./Arbitro/arbitro.c ./Arbitro/arbitro.h ./Arbitro/initConfig.h ./Arbitro/comunicacao.h ./Cliente/cliente.h
	gcc -c ./Arbitro/arbitro.c -o ./Arbitro/arbitro.o 

Arbitro/initConfig.o: ./Arbitro/initConfig.c  ./Arbitro/initConfig.h 
	gcc -c ./Arbitro/initConfig.c -o ./Arbitro/initConfig.o
	
Arbitro/comunicacao.o: ./Arbitro/comunicacao.c ./Arbitro/comunicacao.h ./Cliente/cliente.h
	gcc -c ./Arbitro/comunicacao.c -o ./Arbitro/comunicacao.o

Arbitro/jogadores.o: ./Arbitro/jogadores.c ./Arbitro/arbitro.h ./Cliente/cliente.h
	gcc -c ./Arbitro/jogadores.c -o ./Arbitro/jogadores.o
	
# Ficheiros *.o do Cliente	
Cliente/cliente.o: ./Cliente/cliente.c ./Cliente/cliente.h
	gcc -c ./Cliente/cliente.c -o Cliente/cliente.o

# Executavel do Jogo (Perguntas)
Games/g_perguntas: Games/Perguntas/jogo.o Games/Perguntas/sinais.o
	gcc -o Games/g_perguntas ./Games/Perguntas/jogo.o ./Games/Perguntas/sinais.o
	
jogo: Games/g_perguntas
	
# Executavel do Arbitro
arbitro: Arbitro/arbitro.o Arbitro/initConfig.o Arbitro/comunicacao.o Arbitro/jogadores.o
	gcc -o arbitro Arbitro/arbitro.o Arbitro/initConfig.o Arbitro/comunicacao.o Arbitro/jogadores.o
	 
# Executavel do Cliente
cliente: Cliente/cliente.o
	gcc -o cliente Cliente/cliente.o

# Limpa todos os ficheiros *.o criados
clean:
	find . -name '*.o' -delete