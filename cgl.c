#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "cgl.h"

#define EXTERN

// Configuration
#define DEFAULT_SIZE 50
#define PROB_ALIVE 70  // %

// State
unsigned int size = DEFAULT_SIZE;
bool paused = false;
bool **cells = NULL;
unsigned int generation = 0;
unsigned int population = 0;

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
bool **init_cells(unsigned int s) {
    bool **c = (bool **)malloc(size * sizeof(bool *));
    assert(c);
    for (unsigned int i = 0; i < size; ++i) {
        c[i] = (bool *)malloc(size * sizeof(bool));
    }

    for (unsigned int row = 0; row < s; ++row) {
        assert(c[row]);
        for (unsigned int col = 0; col < s; ++col) {
            c[row][col] = false;
        }
    }
    return c;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void randomize_cells(bool **c, unsigned int s) {
    for (unsigned int row = 0; row < s; ++row) {
        // printf("c: %d c[%d]: %d\n", c, row, c[row]);
        assert(c[row]);
        for (unsigned int col = 0; col < s; ++col) {
            int prob = rand() % 100;  // [0, 100]
            bool alive = (prob < PROB_ALIVE);
            // printf("ROW: %d COL: %d ALIVE: %d\n", row, col, alive);
            c[row][col] = alive;
        }
    }
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void free_cells(bool **c, unsigned int s) {
    for (unsigned int i = 0; i < size; ++i) {
        if (c[i]) free(c[i]);
    }
    if (c) free(c);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void loop() {
    printf("Generation: %d, Population: %d\n", generation, population);
    tick();
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void tick() {
    bool **next = NULL;
    next = init_cells(size);
    advance(cells, next, size);
    free_cells(cells, size);
    cells = next;
    ++generation;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
bool is_paused() { return paused; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void toggle_play_pause() {
    paused = !paused;
    if (paused) {
#ifdef __EMSCRIPTEN__
        emscripten_pause_main_loop();
#endif
        printf("PAUSED\n");
    } else {
#ifdef __EMSCRIPTEN__
        emscripten_resume_main_loop();
#endif
        printf("RESUMED\n");
    }
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
unsigned int get_size() { return size; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
unsigned int get_population() { return population; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
unsigned int get_adj_cells_alive(bool **c, unsigned int s, unsigned int row,
                                 unsigned int col) {
    /*
    1 2 3
    4 X 6
    7 8 9
    */
    unsigned int count = 0;
    if (row > 0 && col > 0 && c[row - 1][col - 1]) ++count;              // 1
    if (row > 0 && c[row - 1][col]) ++count;                             // 2
    if (row < (s - 1) && col < (s - 1) && c[row + 1][col + 1]) ++count;  // 3
    if (col > 0 && c[row][col - 1]) ++count;                             // 4
    if (col < (s - 1) && c[row][col + 1]) ++count;                       // 6
    if (row < (s - 1) && col > 0 && c[row + 1][col - 1]) ++count;        // 7
    if (row < (s - 1) && c[row + 1][col]) ++count;                       // 8
    if (row < (s - 1) && col < (s - 1) && c[row + 1][col + 1]) ++count;  // 9
    return count;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void advance(bool **curr, bool **next, unsigned int s) {
    unsigned int next_population = 0;
    for (unsigned int row = 0; row < s; ++row) {
        for (unsigned int col = 0; col < s; ++col) {
            bool alive = curr[row][col];
            unsigned int adj_cells_alive =
                get_adj_cells_alive(curr, s, row, col);
            if (alive) {
                if (adj_cells_alive < 2 || adj_cells_alive > 3) {
                    alive = false;
                }
            } else {
                if (adj_cells_alive == 3) {
                    alive = true;
                }
            }
            next[row][col] = alive;
            if (alive) ++next_population;
        }
    }
    population = next_population;
}
