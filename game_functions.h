void init_cursor(int *y, int *x, char harta_comp[10][10],
				 WINDOW *grid_comp[10][10]);

void deplasare(int c, int *y, int *x, int *schimb, char harta_comp[10][10]);

void move_cursor(int anty, int antx, int y, int x, char harta_comp[10][10],
				 WINDOW *grid_comp[10][10]);

void atac(char harta[10][10], WINDOW *grid[10][10], int y, int x, int *cont,
			int *scor, int *finish);

void atac_automat(char harta[10][10], WINDOW *grid[10][10], int *cont,
				 int *scor, int *finish);

void atac_hard(char harta[10][10], WINDOW *grid[10][10], int y, int x,
				int *cont, int *scor, int *finish, int *nava_gasita);

void atac_automat_hard(char harta[10][10], WINDOW *grid[10][10], int *cont,
				 int *scor, int *finish, int *nava_gasita, int *y, int *x);