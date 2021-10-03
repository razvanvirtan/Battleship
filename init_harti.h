void rand_harta(char ***harti, char harta_om[10][10], int nr);

int check_free(char harta_comp[10][10], int i, int j, int sens, int nr);

void gen_harta(char harta_comp[10][10]);

void create_grids(WINDOW **game_window, WINDOW *grid_om[10][10],
		WINDOW *grid_comp[10][10], char harta_om[10][10], char harta_comp[10][10]);

void refresh_grid(WINDOW *grid[10][10], char harta[10][10]);

void parcurge_nava(char harta[10][10], int i, int j, int sens, int *nr);

void randomize_map(char harta[10][10], WINDOW **game_window);

int nave_doborate(char harta[10][10]);
