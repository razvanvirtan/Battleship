#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/* functia care primeste ca parametru un vector de matrici de caractere
si numarul de elemente din vector si converteste una din matrici 
in matricea hart_om*/
void rand_harta(char ***harti, char harta_om[10][10], int nr) {
	int i = 0, j = 0, sel = 0;

// ne asiguram ca functia rand() va intoarce rezultate diferite la orice rulare

	srand(time(0));
	sel = rand() % nr;

/* convertim formatul human readable al hartii din fisier intr-un
cu care va fi usor de lucrat in cod */
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			if (harti[sel][i][2 * j + 1] == ' ')
			harta_om[i][j] = '0';
		    else harta_om[i][j] = 'x';
		}
	}
}

/* functia care verifiica daca putem pozitiona o nava de lungime nr in
 harta_comp la pozitia (j, i), cu parcurgere in sensul indicat */
int check_free(char harta_comp[10][10], int i, int j, int sens, int nr) {
	int k1 = 0, k2 = 0;
	int first_col = 0, last_col = 0, first_line = 0, last_line = 0;

	if (j == 0) first_col = j; else first_col = j - 1;
	if (j == 9) last_col = j; else last_col = j + 1;
	if (i == 0) first_line = i; else first_line = i -1;
	if (i == 9) last_line = i; else last_line = i + 1;

	switch (sens) {
	/* pentru fiecare sens, parcurgem casutele in care ar urma sa
	sa plasam nava si cele din jurul lor si verificam ca niciuna sa nu apartina 
	altei nave; avem grija sa nu iesim in afara limitelor hartii*/
		case 0:
			if ((j + nr - 1) < 10) {
				if ((j + nr) < 10) last_col = j + nr;
				  else last_col = j + nr - 1;
				for (k1 = first_line; k1 <= last_line; k1++) {
					for (k2 = first_col; k2 <= last_col; k2++) {
						if (harta_comp[k1][k2] == 'x') return 0;
					}
				}
				return 1;
			}
			else return 0;

		case 1:
			if ((i - nr + 1) >= 0) {
				if ((i - nr) >= 0) first_line = i - nr;
				  else first_line = i - nr + 1;
				for (k1 = last_line; k1 >= first_line; k1--) {
					for (k2 = first_col; k2 <= last_col; k2++) {
						if (harta_comp[k1][k2] == 'x') return 0;
					}
				}
				return 1;
			}
			else return 0;

		case 2:
			if ((j - nr + 1) >= 0) {
				if ((j - nr) >= 0) first_col = j - nr;
				  else first_col = j - nr + 1;
				for (k1 = first_line; k1 <= last_line; k1++) {
					for (k2 = last_col; k2 >= first_col; k2--) {
						if (harta_comp[k1][k2] == 'x') return 0;
					}
				}
				return 1;
			}
			else return 0;

		case 3:
			if ((i + nr - 1) < 10) {
				if((i + nr) < 10) last_line = i + nr;
				  else last_line = i + nr - 1;
				for(k1 = first_line; k1 <= last_line; k1++) {
					for (k2 = first_col; k2 <= last_col; k2++) {
						if (harta_comp[k1][k2] == 'x') return 0;
					}
				}
				return 1;
			}
			else return 0;
	}
	return 0;
}

// functia care genereaza aleatoriu harta computer-ului; o retine in harta_comp
void gen_harta(char harta_comp[10][10]) {
	/* variabila sens reprezinta sensul in care vom plasa campuri ale unei 
	nave, plecand de la un camp dat; codificare: 
	sens = 0 -> continuam nava in casutele din dreapta (col. de indice mai mare)
	sens = 1 -> continuam nava in casutele de deasupra (liniii de indice mai mic)
	sens = 2 -> continuam nava in casutele din stanga (coloane de indice mai mic)
	sens = 3 -> continuam nava in casutele de jos (linii de indice mai mare)
	*/
	int ok = 0, i = 0, j = 0, k1 = 0, k2 = 0, sens = 0;
	// retinem intr-un vector structura navelor de pe harta
	int v[11] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};

	// initializam harta cu toate campurile libere
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			harta_comp[i][j] = '0';
		}
	}

   // parcurgem toate navele ce trebuie pozitionate
	for (k1 = 0; k1 < 10; k1++) {
		ok = 0;
		do {
	/* selectam aleatoriu o casuta si verificam daca putem incepe nava curenta
	(de lungime v[k]) din aceasta pozitie */
			i = rand() % 10; j = rand() % 10;
			for (sens = 0; sens < 4; sens++) {
			   if (check_free(harta_comp, i, j, sens, v[k1])) {
				  ok = 1;
				  break;
			   }
	    	}
		} while (!ok);
	/* dupa ce am gasit o casuta si un sens de pozitionare potrivit, 
	modificam valorile din harta */
		switch (sens) {
			case 0:
				for (k2 = j; k2 < v[k1] + j; k2++) {
					harta_comp[i][k2] = 'x';
				}
				break;
			case 1:
				for (k2 = i; k2 > i - v[k1]; k2--) {
					harta_comp[k2][j] = 'x';
				}
				break;
			case 2:
				for (k2 = j; k2 > j - v[k1]; k2--) {
					harta_comp[i][k2] = 'x';
				}
				break;
			case 3:
				for (k2 = i; k2 < v[k1] + i; k2++) {
					harta_comp[k2][j] = 'x';
				}
				break;
		}
	}

}

/* functia care creeaza fereastra de joc si matricile de WINDOWS grid_om si
 grid_comp, plecand de la valorile din harta_om si harta_comp; functia primeste
 pointeri pentru a putea modifca permanent valorile de la adresele date ca
 parametrii*/
void create_grids(WINDOW **game_window, WINDOW *grid_om[10][10],
				 WINDOW *grid_comp[10][10], char harta_om[10][10],
				 char harta_comp[10][10]) {
	int i, j;
	int width, height, startx, starty;
	// calculam latimea si inaltimea unei ferestre pentru un efect vizual placut
	width = COLS / 20; height = LINES / 12;

	*(game_window) = newwin(LINES, COLS, 0, 0);
	keypad(*(game_window), TRUE);
	wbkgd(*(game_window), COLOR_PAIR(8));

	// cream 10X10 ferestre pentru fiecare grid
	for (i = 0; i < 10 ; i++) {
		for (j = 0; j < 10; j++) {
			starty = i * height; startx = j * width;
			grid_om[i][j] = newwin(height, width, starty, startx);
			box(grid_om[i][j], 0, 0);
			// pentru harta omului, coloram in verde ferestrele ce apartin unor nave
			if(harta_om[i][j] == 'x') wbkgd(grid_om[i][j], COLOR_PAIR(4));
			else wbkgd(grid_om[i][j], COLOR_PAIR(6));

			starty = i * height; startx = COLS - (j + 1) * width;
			grid_comp[i][9 - j] = newwin(height, width, starty, startx);
			box(grid_comp[i][9 - j], 0, 0);
			wbkgd(grid_comp[i][9 - j], COLOR_PAIR(5));
		}
	}
}

/* functia care imi actualizeaza un grid in functie de valorile din harta,
utila cand apelam randomize_map()*/
void refresh_grid(WINDOW *grid[10][10], char harta[10][10]) {
	int i, j;

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			if (harta[i][j] == 'x') wbkgd(grid[i][j], COLOR_PAIR(4));
			else wbkgd(grid[i][j], COLOR_PAIR(6));
			wrefresh(grid[i][j]);
			}
		}
}

/* functia care parcurge in sensul indicat de "sens" o nava de lungime nr de pe
 harta, incepand din casuta i si j, va fi apelata de nave_doborate() la calculul
 scorului final */
void parcurge_nava(char harta[10][10], int i, int j, int sens, int *nr) {
	int k1, k2;
	k1 = i; k2 = j; *nr = 0;

/* exista doar doua densuri de parcurge, deoarece cand o apelam
 parcurgem de sus in jos si de la stanga la dreaota */
	switch (sens) {
		case 0:
			while (harta[k1][k2] == 'x' || harta[k1][k2] == 'h') {
				if (harta[k1][k2] == 'x') {
/* retinem numarul de casute inca nelovite din nava, pentru a le putea
repozitiona la randomizare */
					*nr = *nr + 1;
				}
// camourile lovite devin libere
				harta[k1][k2] = '0'; 
				k2++;
				if (k2 == 10) break; // ne asiguram ca nu iesim din matrice
			}
		    break;
		case 1:
			while (harta[k1][k2] == 'x' || harta[k1][k2] == 'h') {
				if (harta[k1][k2] == 'x') {
					*nr = *nr + 1;
				}
				harta[k1][k2] = '0';
			    k1++;
				if(k1 == 10) break;
			 }
			 break;
	}


}

// functia care randomizeaza o harta
void randomize_map(char harta[10][10], WINDOW **game_window) {

	int ok = 0, i = 0, j = 0, k = -1, sens = 0, nr_nave, k1, k2, cont;
	int v[11]; // vectorul care va contine lungimile navelor nou formate

// parcurgem de sus in jos si de la stanga la dreapta harta
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			cont = 1;
/* daca gasim un camp al unei nave inca nelovit, parcurgem nava 
 si retinem nr. de nave inca nelovite */
			if (harta[i][j] == 'x') {
				k++;
				// conditia pentru a nu iesi din matrice
				if (j < 9) {
				// conditia pentru a gasi sensul de parcurs al navei
				 if (harta[i][j + 1] == 'x' || harta[i][j + 1] == 'h') {
				    parcurge_nava(harta, i, j, 0, &v[k]);
				    cont = 0;
				 }
				}
				if (i < 9 && cont) {
			    if (harta[i + 1][j] == 'x' || harta[i + 1][j] == 'h') {
				    parcurge_nava(harta, i, j, 1, &v[k]);
				    cont = 0;
				 }
				}
				if (cont) v[k] = 1; // cazul in care nava are un singur camp
			}
		}
	}

// reinitializem harta cu campuri goale
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			harta[i][j] = '0';
		}
	}

	nr_nave = k + 1;

/* pentru fiecare nava nou formata, parcurgem mun algoritm similar celui
cu care am generat harta computer-ului */
	for (k1 = 0; k1 < nr_nave; k1++) {
		ok = 0;
		do {
			i = rand() % 10; j = rand() % 10;
			for (sens = 0; sens < 4; sens++) {
			   if (check_free(harta, i, j, sens, v[k1])) {
				  ok = 1;
				  break;
			   }
	    	}
		} while (!ok);
		switch (sens) {
			case 0:
				for (k2 = j; k2 < v[k1] + j; k2++) {
					harta[i][k2] = 'x';
				}
				break;
			case 1:
				for (k2 = i; k2 > i - v[k1]; k2--) {
					harta[k2][j] = 'x';
				}
				break;
			case 2:
				for (k2 = j; k2 > j - v[k1]; k2--) {
					harta[i][k2] = 'x';
				}
				break;
			case 3:
				for (k2 = i; k2 < v[k1] + i; k2++) {
					harta[k2][j] = 'x';
				}
				break;
		}
	}

}

/* functia care calculeaza la finalul jocului navele doborate de pe harta
castigatorului */
int nave_doborate(char harta[10][10]) {
	int i = 0, j = 0, k = 0, aux, cont;

// parcurgem harta de sus in jos si de la stanga la dreapta
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			cont = 1;
	/* atunci cand gasim un camp nelovit dintr-o nava, crestem numarul
	navelor nedoborate cu 1 si parcurgem nava */
			if (harta[i][j] == 'x') {
				k++;
				if (j < 9) {
				 if (harta[i][j + 1] == 'x') {
				 parcurge_nava(harta, i, j, 0, &aux);
				 cont = 0;
				 }
				}
				if (i < 9 && cont) {
			    if (harta[i + 1][j] == 'x') {
				 parcurge_nava(harta, i, j, 1, &aux);
				 }
				}
			}
		}
	}

// returnam 10 - (nr. navelor nedoborate)
	return 10 - k;
}
