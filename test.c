#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "cgl.h"

extern bool **cells;
extern unsigned int size;

#define ALIVE_CHAR 'X'
#define DEAD_CHAR '-'

#define H_BORDER_CHAR '='
#define V_BORDER_CHAR '|'

// Stop after a set number of generations (or -1 to run forever)
#define MAX_GENERATIONS -1

// Enable/disable a delay between generations
#define DELAY 1
useconds_t delay = 100000; // 0.1 sec

void print_cells(bool **c, unsigned int s);
void print_horizontal_border(unsigned int s);

int main(int argc, char **argv) {
    srand(time(NULL));
    cells = init_cells();
    randomize_cells(cells);
    unsigned int generation_count = 0;
    while (generation_count <= MAX_GENERATIONS) {
        loop();
        print_cells(cells, size);
        if (DELAY) usleep(delay);
        ++generation_count;
    }
    free_cells(cells);
    return 0;
}

void print_cells(bool **c, unsigned int s) {
    print_horizontal_border(s);
    for (unsigned int row = 0; row < s; ++row) {
        printf("%c", V_BORDER_CHAR);
        for (unsigned int col = 0; col < s; ++col) {
            bool alive = c[row][col];
            if (alive) {
                printf("%c", ALIVE_CHAR);
            } else {
                printf("%c", DEAD_CHAR);
            }
            if (col < s - 1) {
                printf(" ");
            }
        }
        printf("%c\n", V_BORDER_CHAR);
    }
    print_horizontal_border(s);
}

void print_horizontal_border(unsigned int s) {
    printf(" ");
    for (unsigned int i = 0; i < (s * 2) - 1; ++i) {
        printf("%c", H_BORDER_CHAR);
    }
    printf("\n");
}
