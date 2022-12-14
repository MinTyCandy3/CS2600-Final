
all: tictactoe

tictactoe: tictactoe.c
	gcc tictactoe.c -o tictactoe -lpaho-mqtt3c

run: tictactoe
	./tictactoe
