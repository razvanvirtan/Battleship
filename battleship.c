#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "init_game.h"
#include "init_harti.h"
#include "game_functions.h"

/* functia cu care vom elibera vectorul de harti alocate dinamic din memoria
programului */
void free_mem(char ****harti, int argc, char **argv) {
	int i, j;

	for (i = 0; i < argc - 1; i++) {
		for (j = 0; j < 10; j++) {
			free((*harti)[i][j]);
		}
		free((*harti)[i]);
	}
	free((*harti));
}

// functia care introduce un delay in program (pentru mutarile calculatorului)
void wait(int interval) {
	int t0;
	t0 = time(0);
	while (time(0) < (t0 + interval)) {
	}
}

/* functia care implementeaza varianta usoara a jocului, primeste ca parametrii
 pointeri la ferestre si scoruri (pentru a le putea modifica in timpul 
 jocului) si hartile computer-ului si utilizatorului */
void easy_game(WINDOW **game_window, WINDOW *menu_window, WINDOW *grid_om[10][10],
			   WINDOW *grid_comp[10][10],char harta_comp[10][10], 
			   char harta_om[10][10], int *scor_om, int *scor_comp, int *resume_activ) {

	int i, j;
	int c, cont = 1, schimb, finish = 0, scor = 0;
	struct { int x, y; } hlight, ant;

	/* ascundem meniul afisand fereastra de joc; peste aceasta afisam cele
		2 grid-uri create anterior */
	mvwprintw(*game_window, LINES - 4, 3, "Key R - Randomize map");
	mvwprintw(*game_window, LINES - 3, 3, "Key D - Distroy in advance");
	mvwprintw(*game_window, LINES - 2, 3, "Key Q - Back to Menu");
	touchwin(*game_window);
	wrefresh(*(game_window));	
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			touchwin(grid_om[i][j]);
			wrefresh(grid_om[i][j]);
			touchwin(grid_comp[i][j]);
			wrefresh(grid_comp[i][j]);
		}
	}
	fflush(stdin);

  
  while (!finish) {
	  cont = 1;
	  // omul poate muta si selecta campuri pana rateaza (cont devine 0)
		while (cont) {
	   // pozitionam cursorul in primul camp nelovit
		init_cursor(&hlight.y, &hlight.x, harta_comp, grid_comp);

			while ((c = wgetch(*game_window)) != 10 && c != 'q') {
				schimb = 0;
				ant.x = hlight.x; ant.y = hlight.y; // salvam pozitia curenta

				if (c == 'r') {
					randomize_map(harta_om, game_window);
					refresh_grid(grid_om, harta_om);
				}
	/* implementarea lui destroy in advance, atacam pe rand 10 campuri selectate
	 aleatoriu, daca la unul din pasi toate navele de pe o harta sunt distruse
	 se iese (nu exista remiza) */
				else if (c == 'd') {
						for (i = 0; i < 10; i++) {
							atac_automat(harta_comp, grid_comp, &cont, scor_om, &finish);
							if (finish == 1) { break; }
							atac_automat(harta_om, grid_om, &cont, scor_comp, &finish);
							if (finish == 1) { break; }
						}
	 // secventa de repozitionare a cursorului dupa "Destroy in advance"
						if (harta_comp[ant.y][ant.x] == 'm') {
							wbkgd(grid_comp[ant.y][ant.x], COLOR_PAIR(3));
						}
						else {
							wbkgd(grid_comp[ant.y][ant.x], COLOR_PAIR(5));
						}
						wrefresh(grid_comp[ant.y][ant.x]);
						init_cursor(&hlight.y, &hlight.x, harta_comp, grid_comp);
						if (finish == 1) { break; }
					}
	 // daca nu se selecteaza o optiune speciala se trece la deplasarea normala
				else {
					deplasare(c, &hlight.y, &hlight.x, &schimb, harta_comp);
					if (schimb) {
						move_cursor(ant.y, ant.x, hlight.y, hlight.x, harta_comp, grid_comp);
					}
			  	}
	   		}

	  // la Enter este atacat campul pe care suntem pozitionati
			if (c == 10) {
				atac(harta_comp, grid_comp, hlight.y, hlight.x, &cont, scor_om, &finish);
	   		} 
	   // la 'q' se iese din fereastra jocului, deci din bucla
			else {
				finish = 1;
				cont = 0;
			} 
		}

		if (finish) { break; } // daca dupa un atac se termina jocul, iesim

	// etapa in care actioneaza computer-ul; acesta ataca aleatoriu pana rateaza
		cont = 1;
		while (cont) {
			wait(3);
			atac_automat(harta_om, grid_om, &cont, scor_comp, &finish);
		}
		if (finish) { break; } // daca dupa etapa automata se tarmina jocul, iesim
   	}

   /* daca unul din scoruri e 20, inseamna ca s-a iesit din bucla fiindca
   jocul e gata (nu prin Quit), caz in care calculam numarul navelor
   doborate si afisam scorul final; la meniu ne intoarcem cu Q */
   if (*scor_comp == 20) {
		scor = nave_doborate(harta_comp);
		wattron(*game_window, A_BOLD | A_BLINK | COLOR_PAIR(9));
		mvwprintw(*game_window, LINES - 4, COLS/2 - 6, "COMPUTER WON");
		wattroff(*game_window, A_BLINK);
		mvwprintw(*game_window, LINES - 3, COLS/2 - 6, "Your score: %d", scor);
		mvwprintw(*game_window, LINES - 2, COLS/2 - 6, "Computer's score: 10");
		wrefresh(*game_window);
		wattroff(*game_window, A_BOLD | COLOR_PAIR(9));
		*resume_activ = 0;
		while((c = wgetch(*game_window)) != 'q') {}
   }
   else if (*scor_om == 20) {
		scor = nave_doborate(harta_om);
		wattron(*game_window, A_BOLD | A_BLINK | COLOR_PAIR(9));
		mvwprintw(*game_window, LINES - 4, COLS/2 - 5, "Congrats, you won!");
		wattroff(*game_window, A_BLINK);
		mvwprintw(*game_window, LINES - 3, COLS/2 - 6, "Your score: 10");
		mvwprintw(*game_window, LINES - 2, COLS/2 - 6, "Computer's score: %d", scor);
		wattroff(*game_window, A_BOLD | A_BLINK | COLOR_PAIR(9));
	  	wrefresh(*game_window);
		*resume_activ = 0;
		while((c = wgetch(*game_window)) != 'q') {}
   }

	touchwin(menu_window);
	wrefresh(menu_window);
}

/* implementarea modului de joc dificil, singura diferenta fata de easy_game()
 este la etapa de joc a computer-ului */
void hard_game(WINDOW **game_window, WINDOW *menu_window, WINDOW *grid_om[10][10],
			   WINDOW *grid_comp[10][10],char harta_comp[10][10], 
			   char harta_om[10][10], int *scor_om, int *scor_comp, int *resume_activ) {

	int i, j, y, x, stanga, dreapta, sus, jos, aux;
	int c, cont = 1, schimb, finish = 0, scor = 0;
	int nava_gasita = 0, orientation = 0;
	struct { int x, y; } hlight, ant;

	mvwprintw(*game_window, LINES - 4, 3, "Key R - Randomize map");
	mvwprintw(*game_window, LINES - 3, 3, "Key D - Distroy in advance");
	mvwprintw(*game_window, LINES - 2, 3, "Key Q - Back to Menu");
	touchwin(*game_window);
	wrefresh(*(game_window));	
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			touchwin(grid_om[i][j]);
			wrefresh(grid_om[i][j]);
			touchwin(grid_comp[i][j]);
			wrefresh(grid_comp[i][j]);
		}
	}
	fflush(stdin);

	while (!finish) {
		cont = 1;
		while (cont) {
			init_cursor(&hlight.y, &hlight.x, harta_comp, grid_comp);
			while ((c = wgetch(*game_window)) != 10 && c != 'q') {
				schimb = 0;
				ant.x = hlight.x; ant.y = hlight.y;

				if (c == 'r') {
					randomize_map(harta_om, game_window);
					refresh_grid(grid_om, harta_om);
				} else if (c == 'd') {
					for (i = 0; i < 10; i++) {
						atac_automat(harta_comp, grid_comp, &cont, scor_om, &finish);
						if (finish == 1)
							break;
						atac_automat(harta_om, grid_om, &cont, scor_comp, &finish);
						if (finish == 1)
							break;
					}
					if (harta_comp[ant.y][ant.x] == 'm') {
						wbkgd(grid_comp[ant.y][ant.x], COLOR_PAIR(3));
					} else { 
						wbkgd(grid_comp[ant.y][ant.x], COLOR_PAIR(5));
					}
					wrefresh(grid_comp[ant.y][ant.x]);
					init_cursor(&hlight.y, &hlight.x, harta_comp, grid_comp);
					if (finish == 1)
						break;
				} else {
					deplasare(c, &hlight.y, &hlight.x, &schimb, harta_comp);
					if (schimb) {
						move_cursor(ant.y, ant.x, hlight.y, hlight.x, harta_comp, grid_comp);
					}
			  	}
	   		}
	   if (c == 10) {
			atac(harta_comp, grid_comp, hlight.y, hlight.x, &cont, scor_om, &finish);
	   } else {
		   finish = 1;
		   cont = 0; 
		}
	}

	if (finish)
		break;

// codul pentru jocul "inteligent" al computer-ului
	cont = 1; 
	while (cont) {
		wait(3);
		/* daca nu exista nicio nava partial descoperita, apelam o functie similara
		cu atac automat, care retine insa daca casuta lovita apartine unei nave */
		if(nava_gasita == 0) {
		  atac_automat_hard(harta_om, grid_om, &cont, scor_comp, 
							  &finish, &nava_gasita, &y, &x);
		  if(nava_gasita == 1) {
			  stanga = 1; sus = 1; dreapta =  1; jos = 1;
		  }
		}
		// daca exista o nava partial descoperita, verificam vecinii primei casute lovite
		else {
		/* verificam ca exista vecini "deasupra" care merita incercati 
		  (nu au fost deja ratati) */
			if(sus == 1) {
				aux = y;
				while(aux > 0 && cont) {
					/* verificam prima casuta nelovita inca, daca aceasta a fost deja 
					 incercata nu mai are sens sa o lovim si vom alege alt sens de deplasare */
					aux --;
					while(harta_om[aux][x] == 'h' && aux > 0) aux --;
					if(harta_om[aux][x] == 'm') { sus = 0; break; }
					// daca nu a fost lovita prima casuta, o atacam
					atac_hard(harta_om, grid_om, aux, x, &cont, scor_comp,
							   &finish, &nava_gasita);
					// daca am nimerit nava, retinem ca e dispusa vertical
					if(harta_om[aux][x] == 'h') orientation = 1;
					wait(3);
				}
				sus = 0;
			}
		/* daca nu mai are sens sa verificam vecinii de sus, 
		 ii verificam similar pe cei de jos */
			 if(jos == 1 && cont){
				aux = y;
				while(aux < 9 && cont) {
					aux ++;
					while(harta_om[aux][x] == 'h' && aux < 9) aux ++;
					if(harta_om[aux][x] == 'm') {
						jos = 0;
						break;
					}
					atac_hard(harta_om, grid_om, aux, x, &cont, scor_comp,
							 &finish, &nava_gasita);
					if(harta_om[aux][x] == 'h')
						orientation = 1;
					wait(3);
				}
				jos = 0;
			}
		// daca nava e dispusa vertical, nu mai are rost sa verificam pe orizontala
			if(orientation) {
				stanga = 0;
				dreapta = 0;
			}

		// verificarile stanga-dreapta, similare cu sus - jos
			 if(stanga == 1 && cont){
				aux = x;
				while(aux > 0 && cont) {
					aux --;
					while(harta_om[y][aux] == 'h' && aux > 0) aux --;
					if(harta_om[y][aux] == 'm') {
						stanga = 0;
						break;
					}
					atac_hard(harta_om, grid_om, y, aux, &cont, scor_comp,
							 &finish, &nava_gasita);
					wait(3);
				}
				stanga = 0;
			}

			 if(dreapta == 1 && cont){
				aux = x;
				while(aux < 9 && cont) {
					aux ++;
					while(harta_om[y][aux] == 'h' && aux < 9) aux ++;
					if(harta_om[y][aux] == 'm') {
						dreapta = 0;
						break;
					}
					atac_hard(harta_om, grid_om, y, aux, &cont, scor_comp,
							&finish, &nava_gasita);
					wait(3);
				}
				dreapta = 0;
			}

		/* daca nu mai are sens sa verificam niciun vecin, nu mai avem o nava
		partial distrusa si trebuie sa atacam din nou aleatoriu */
			 if(stanga == 0 && dreapta == 0 && sus == 0 && jos == 0) { nava_gasita = 0;}
		  }
		}
	if (finish)
		break;
   }

   if (*scor_comp == 20) {
		scor = nave_doborate(harta_comp);
		wattron(*game_window, A_BOLD | A_BLINK | COLOR_PAIR(9));
		mvwprintw(*game_window, LINES - 4, COLS/2 - 6, "COMPUTER WON");
		wattroff(*game_window, A_BLINK);
		mvwprintw(*game_window, LINES - 3, COLS/2 - 6, "Your score: %d", scor);
		mvwprintw(*game_window, LINES - 2, COLS/2 - 6, "Computer's score: 10");
		wrefresh(*game_window);
		wattroff(*game_window, A_BOLD | COLOR_PAIR(9));
		*resume_activ = 0;
		while((c = wgetch(*game_window)) != 'q') {}
   }
   else if (*scor_om == 20) {
		scor = nave_doborate(harta_om);
	   	wattron(*game_window, A_BOLD | A_BLINK | COLOR_PAIR(9));
	   	mvwprintw(*game_window, LINES - 4, COLS/2 - 5, "Congrats, you won!");
		wattroff(*game_window, A_BLINK);
		mvwprintw(*game_window, LINES - 3, COLS/2 - 6, "Your score: 10");
		mvwprintw(*game_window, LINES - 2, COLS/2 - 6, "Computer's score: %d", scor);
		wattroff(*game_window, A_BOLD | A_BLINK | COLOR_PAIR(9));
		wrefresh(*game_window);
		*resume_activ = 0;
		while((c = wgetch(*game_window)) != 'q') {}
   }

	touchwin(menu_window);
	wrefresh(menu_window);
}


int main(int argc, char **argv) {
	meniu game_menu;
	WINDOW *game_window; //fereastra pentru jocul efctiv
	WINDOW *menu_window; //fereastra de afisare a meniului
	// declaram matricile de ferestre cu care vom afisa grid-urile jucatorilor
	// si matricile in care retinem configuratiile hartilor
	WINDOW *grid_om[10][10], *grid_comp[10][10];
	char ***harti, harta_comp[10][10], harta_om[10][10];

	int c = 0, selected = 0, scor_om = 0, scor_comp = 0;
	int resume_activ = 0;
	int tip_joc = 0;
	
	// citim hartile din fisierele date ca argumente (verificam ca se poate, 
	// altfel iesim din program)
	if (!citire_harti(&harti, argc, argv)) {
		return 1;
	 }

	// daca am citit minim o harta, initializam interfata grafica a meniului
	if (argc > 1) {
		init_screen();
		init_menu(&game_menu, &menu_window);
		refresh_menu(&game_menu, selected);

		do {
		// implementam navigarea in meniu cu taste, punem highlight pe selectie
			if (c == KEY_UP) {
				if (selected > 0)
					selected--;
				refresh_menu(&game_menu, selected);
			} else if (c == KEY_DOWN) {
				if (selected < 3)
					selected++;
				refresh_menu(&game_menu, selected);
			}
		// coordonarea actiunilor la Enter
			else if (c == 10) {
		// daca e selectata optiunea "New game" se creaza si porneste un nou joc usor
				if (selected == 0) {
					scor_om = 0; scor_comp = 0;
					tip_joc = 0; resume_activ = 1;
		// pentru a crea un joc nou, generam harti noi
					rand_harta(harti, harta_om, argc - 1);
					gen_harta(harta_comp);
					create_grids(&game_window, grid_om, grid_comp, harta_om, harta_comp);
					easy_game(&game_window, menu_window, grid_om, grid_comp, harta_comp,
								harta_om, &scor_om, &scor_comp, &resume_activ);
		// cand se iese din joc reapare meniul pe ecran
					refresh_menu(&game_menu, selected);
				}
		// daca e selectat "Resume game" reporneste ultimul joc creat(usor sau greu),
		// daca Resume e activ (s-a pornit inainte cel putin un joc)
				else if (selected == 1 && resume_activ) {
					if(tip_joc == 0) {
						easy_game(&game_window, menu_window, grid_om, grid_comp, harta_comp,
								harta_om, &scor_om, &scor_comp, &resume_activ);
					}
				else {
						hard_game(&game_window, menu_window, grid_om, grid_comp, harta_comp,
								harta_om, &scor_om, &scor_comp, &resume_activ);
				}
						refresh_menu(&game_menu, selected);
				}
		// daca e selectat Hard game porneste un joc dificil(la fel ca jocul usor)
				else if(selected == 2){
					scor_om = 0; scor_comp = 0;
					tip_joc = 1; resume_activ = 1;
					rand_harta(harti, harta_om, argc - 1);
					gen_harta(harta_comp);
					create_grids(&game_window, grid_om, grid_comp, harta_om, harta_comp);
					hard_game(&game_window, menu_window, grid_om, grid_comp, harta_comp,
							harta_om, &scor_om, &scor_comp, &resume_activ);
					refresh_menu(&game_menu, selected);
				}
		// la Quit se iese din program
				else if (selected == 3) {
					endwin();
					return 0;
				}
			}
		} while ((c = wgetch(game_menu.opt[0])));
		endwin();
		// la final se elibereaza memoria din vectorul de matrici pt harti
		free_mem(&harti, argc, argv);
	}
	//daca nu s-au dat argumente se afiseaza mesajul de eroare
	else {
		printf("Nu exista harti!");
		return 1;
	}
	return 0;
}