#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// structura de date care va usura scrierea functiilor pentru meniu
typedef struct {
	WINDOW *opt[4];
} meniu;

// functia cu care initializam ecranul jocului si culorile folosite
void init_screen() {
	initscr();
	keypad(stdscr, TRUE);
	cbreak();
	noecho();
	curs_set(0);
	start_color();
	init_pair(1, COLOR_RED, COLOR_YELLOW); // highlight meniu
	init_pair(2, COLOR_BLACK, COLOR_YELLOW); // highlight cursor
	init_pair(3, COLOR_WHITE, COLOR_BLACK); // meniu si spatii ratate
	init_pair(4, COLOR_BLACK, COLOR_GREEN); // navele jucatorului
	init_pair(5, COLOR_BLACK, COLOR_CYAN); // harta computer
	init_pair(6, COLOR_BLACK, COLOR_BLUE); // harta jucator
	init_pair(7, COLOR_BLACK, COLOR_RED); // spatiu lovit
	init_pair(8, COLOR_BLACK, COLOR_WHITE); // bckgd joc
	init_pair(9, COLOR_RED, COLOR_WHITE); //highlight scor final
}

/*  functia care pastreaza in vectorul de matrici harti datele citite
din fisierele date ca argumente */
int citire_harti(char ****harti, int argc, char **argv) {
	int i, j;
	FILE **f; char s[24];

/* alocam dinamic un vector de fisiere (nu este un numar standard)
si un vector de matrici */
	f = (FILE **) calloc(argc - 1, sizeof(FILE*));
	(*harti) = (char ***) calloc(argc - 1, sizeof(char **));

// deschidem pe rand fisierele si copiem continutul lor in cate o matrice
	for (i = 0; i < argc - 1; i++) {
		f[i] = fopen(argv[i + 1], "r");
	// daca nu putem deschide un fisier afisam mesajul de eroare si iesim
		if (f[i] == NULL) {
			printf("Fisierul %s nu poate fi deschis", argv[i + 1]);
			return 0;
		}
	// alocam memoria pentru fiecare matrice si o citim linie cu linie
		(*harti)[i] = (char **) calloc(10, sizeof(char *));
		for(j = 0; j < 10; j++) {
			fgets(s, 23, f[i]);
			(*harti)[i][j] = (char *) calloc(24, sizeof(char));
			strcpy((*harti)[i][j], s);
		}
		fclose(f[i]);
	}
	// eliberam memoria alocata vectorului de fisiere
	free(f); 
	return 1;
}

//functia care intializeaza meniul jocului
void init_menu(meniu *game_menu, WINDOW **menu_window) {
	int menu_height, menu_width, colty, coltx;

	// cream fereastra mare a meniului
	*menu_window = newwin(LINES, COLS, 0, 0); wrefresh(*menu_window);
	wbkgd(*menu_window, COLOR_PAIR(3));

	mvwprintw(*menu_window, LINES - LINES / 10, 3, "Created by Razvan Virtan");

	// calculam dimensiunile ferestrelor pentru optiuni si le cream pe rand
	menu_height = LINES/10;
	menu_width = COLS/8;

	coltx = COLS / 2 - menu_width / 2;
	colty = LINES / 2 - 2*menu_height;
	game_menu -> opt[0] = newwin(menu_height, menu_width, colty, coltx);
	colty = LINES / 2 - menu_height;
	game_menu -> opt[1] = newwin(menu_height, menu_width, colty, coltx);
	colty = LINES / 2;
	game_menu -> opt[2] = newwin(menu_height, menu_width, colty, coltx);
	colty = LINES / 2 + menu_height;
	game_menu -> opt[3] = newwin(menu_height, menu_width, colty, coltx);


	keypad(game_menu -> opt[0], TRUE);
	wrefresh(*menu_window);
	wrefresh(game_menu -> opt[0]);
	wrefresh(game_menu -> opt[1]);
	wrefresh(game_menu -> opt[2]);
	wrefresh(game_menu -> opt[3]);
}

/* functia care afiseaza si reafiseaza optiunile din meniu, cu highlight 
pe optiunea selectata */
void refresh_menu(meniu *game_menu, int selected) {
	int i;
	char *options[4] = {"New game", "Resume", "Hard Game", "Quit"};

	// stergem optiunile si le reafisam in functie de valoarea lui selected
	for (i = 0; i < 4; i++) {
		werase(game_menu -> opt[i]);
		if (selected == i) {
			wattron(game_menu -> opt[i], COLOR_PAIR(1));
			wprintw(game_menu -> opt[i], options[i]);
			wattroff(game_menu -> opt[i], COLOR_PAIR(1));
		}
		else {
			wprintw(game_menu -> opt[i], options[i]);
		}
		wrefresh(game_menu -> opt[i]);
	}
}
