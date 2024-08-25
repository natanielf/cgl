#include <emscripten/emscripten.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cgl.h"

extern bool **cells;
extern unsigned int size;

int main(int argc, char **argv) {
    srand(time(NULL));
    cells = init_cells();
    randomize_cells(cells);
    emscripten_set_main_loop(loop, 0, true);
    return 0;
}
