build:	battleship.c init_game.c init_harti.c game_functions.c
	gcc -Wall -o battleship init_game.c init_harti.c game_functions.c  battleship.c -lncurses
run: battleship
	./battleship harta1 harta2
clean: battleship
	rm battleship	
