typedef struct {
	WINDOW *opt[3];
} meniu;

void init_screen();
int citire_harti(char ****harti, int argc, char **argv);
void init_menu(meniu *game_menu, WINDOW **menu_window);
void refresh_menu(meniu *game_menu, int selected);
