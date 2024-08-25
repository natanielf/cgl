#include <stdbool.h>

bool **init_cells();
void randomize_cells(bool **c);
void free_cells(bool **c);
void loop();
void tick();
bool is_paused();
void toggle_play_pause();
unsigned int get_size();
unsigned int get_population();
unsigned int get_adj_cells_alive(bool **c, unsigned int row, unsigned int col);
void advance(bool **curr, bool **next);
