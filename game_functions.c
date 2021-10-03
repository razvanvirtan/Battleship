#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/* functia care verifica daca un camp de pozitie i si j apartine unei nave de
 harta data a fost nimerit; apelata de deplasare() */
int ver(char harta_comp[10][10], int i, int j){
	return harta_comp[i][j] != 'h';
}

/* functia care verifica daca linia v[10] contine doar campuri deja lovite,
apelata de atacurile automate */
int ver2(char v[10]){
	int nr = 0, i;

	for(i = 0; i < 10; i++){ 
	  if(v[i] == 'h' || v[i] == 'm')
	  	nr++;
    }

    if(nr == 10) return 1; else return 0;
}

/* functia care initializeaza pozitia cursorului, plasandu-l in campul cel mai 
apropiat pe orizontala de coltul stanga-sus si care nu contine o bucata 
lovita dintr-o nava; in x si y retinem pozitia curenta a cursorului */
void init_cursor(int *y, int *x, char harta_comp[10][10], WINDOW *grid_comp[10][10]){
	*x = 0; *y = 0; 
	  while(!ver(harta_comp, *y, *x)){
	  	*x = *x + 1; 
	  }
	  wbkgd(grid_comp[*y][*x], COLOR_PAIR(2));
      wrefresh(grid_comp[*y][*x]);
}

/* functia care implementeaza deplasarea cursorului galben pe grid,
la apasarea sagetilor de la tastaura */
void deplasare(int c, int *y, int *x, int *schimb, char harta_comp[10][10]){
	int contor = 0;

	/* cand incercam sa ne deplasam in sus, mergem in primul camp care nu 
	 a fost inca nimerit (adica nu a fost lovit facand parte dintr-o nava);
	 daca ne putem deplasa retinem asta cu schimb, pentru a schimba apoi
	 culorile, si modifciam linia curenta */
	if(c == KEY_UP){
				contor = 1;
			  	while(contor <= 9 && (*y - contor) >= 0){
			  		if(ver(harta_comp, *y - contor, *x)){
			  			*schimb = 1;
			  			*y = *y - contor;
			  			break;
			  		}
			  		else contor++;
			  	}
			}
		    else if(c == KEY_DOWN){
				contor = 1;
			  	while(contor <= 9 && (*y + contor) < 10){
			  		if(ver(harta_comp, *y + contor, *x)){
			  			*schimb = 1;
			  			*y = *y + contor;
			  			break;
			  		}
			  		else contor++;
			  	}
		    }
		 	else if(c == KEY_RIGHT){
			 	contor = 1;
			  	while(contor <= 9 && (*x + contor) < 10){
			  		if(ver(harta_comp, *y, *x + contor)){
			  			*schimb = 1;
			  			*x = *x + contor;
			  			break;
			  		}
			  		else contor++;
			  	}
		 	}
		    else if(c == KEY_LEFT){ 
			 	contor = 1;
			  	while(contor <= 9 && (*x - contor) >= 0){
			  		if(ver(harta_comp, *y, *x - contor)){
			  			*schimb = 1;
			  			*x = *x - contor;
			  			break;
			  		}
			  		else contor++;
			  	}
			  }
}

/* functia care muta grafic cursorul de la pozitia (antx, anty) la pozitia
(x, y), actualizand culorile din grid-ul dat ca parametru in functie de
valorile din matricea de caractere a grid-ului */
void move_cursor(int anty, int antx, int y, int x, char harta_comp[10][10],
				 WINDOW *grid_comp[10][10]){
	wbkgd(grid_comp[y][x], COLOR_PAIR(2));
		  		if(harta_comp[anty][antx] != 'h'){
		  			if(harta_comp[anty][antx] == 'm'){
		  				wbkgd(grid_comp[anty][antx], COLOR_PAIR(3));
		  			}
		  	 		else wbkgd(grid_comp[anty][antx], COLOR_PAIR(5));
		  		}
		  		wrefresh(grid_comp[y][x]);
		  		wrefresh(grid_comp[anty][antx]);
}

/* functia care ataca casuta de pozitie (y,x) din grid-ul dat ca parametru
in functie de valorile din matricea "harta" si retine daca a ratat sau
daca jocul s-a terminat */
void atac(char harta[10][10], WINDOW *grid[10][10], int y, int x, int *cont,
		  int *scor, int *finish){
	/* casuta lovita se coloreaza in rosu daca apartine unei nave si in gri
	in caz contrar, iar valorile din harta de caractere se actualizeaza
	pentru a retine rezultatul atacului (vezi in README codificarea) */
	if(harta[y][x] == 'x'){
	  		harta[y][x] = 'h';
			wbkgd(grid[y][x], COLOR_PAIR(7));
			wrefresh(grid[y][x]);
			/* daca a fost nimerita o nava scorul creste; daca devine 20 inseamna
			ca e final de joc */
			*scor = *scor +1; 
			if(*scor == 20){ 
				*finish = 1; 
				*cont = 0;
			}
	    }
	    else {
	  		harta[y][x] = 'm';
	  		wbkgd(grid[y][x], COLOR_PAIR(3));
			wrefresh(grid[y][x]);
			*cont = 0; //retinem ca am ratat pentru a trece la mutarea oponentului
	    }	
}

/* functia care ataca aleatoriu un camp din grid-ul dat ca parametru;
 are aceeasi parametrii ca si atac()*/
void atac_automat(char harta[10][10], WINDOW *grid[10][10], int *cont,
				 int *scor, int *finish){
	int liber[11], y, x, i, j;

	// alegem aleatoriu o linie care are minim o casuta inca nelovita
	do{
		y = rand() % 10;
	   } while(ver2(harta[y]));
	j = 0;
	/* retinem campurile libere din linia selectata anterior si alegem
	tot aleatoriu unul din ele */
	for(i = 0; i <= 9; i++){
		if(harta[y][i] != 'h' && harta[y][i] != 'm'){
			liber[j] = i; j++;
		}
	}
	x = liber[rand() % j];

	// atacam camp-ul ales
	atac(harta, grid, y, x, cont, scor, finish);

}

/* functie utilizata la varianta dificila a jocului, similara cu atac,
doar ca primeste pointer la o variabila in care se retine daca a
nimerit o nava  */
void atac_hard(char harta[10][10], WINDOW *grid[10][10], int y, int x, int *cont,
		  int *scor, int *finish, int *nava_gasita){
	if(harta[y][x] == 'x'){
	  		harta[y][x] = 'h';
			wbkgd(grid[y][x], COLOR_PAIR(7));
			wrefresh(grid[y][x]);
			*scor = *scor +1; 
			if(*scor == 20){ 
				*finish = 1; 
				*cont = 0;
			}
			*nava_gasita = *nava_gasita + 1;
	    }
	    else  {
	  		harta[y][x] = 'm';
	  		wbkgd(grid[y][x], COLOR_PAIR(3));
			wrefresh(grid[y][x]);
			*cont = 0;
	    }	
}

/* functie utilizata la varianta dificila a jocului, similara cu 
atac_automat(), doar ca primeste pointer la o variabila in care se
 retine daca a nimerit o nava  */
void atac_automat_hard(char harta[10][10], WINDOW *grid[10][10], int *cont,
				 int *scor, int *finish, int *nava_gasita, int *y, int *x){
	int liber[11], i, j;

	do{
		*y = rand() % 10;
	   } while(ver2(harta[*y]));
	j = 0;
	for(i = 0; i <= 9; i++){
		if(harta[*y][i] != 'h' && harta[*y][i] != 'm'){
			liber[j] = i; j++;
		}
	}
	*x = liber[rand() % j];

	atac_hard(harta, grid, *y, *x, cont, scor, finish, nava_gasita);
}

