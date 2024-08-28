#include <GLES2/gl2.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cgl.h"

extern bool **cells;
extern unsigned int size;

int main(int argc, char **argv) {
    // Set WebGL context attributes
    struct EmscriptenWebGLContextAttributes attrs;
    attrs.alpha = false;
    attrs.depth = true;
    attrs.stencil = true;
    attrs.antialias = true;
    attrs.premultipliedAlpha = false;
    attrs.preserveDrawingBuffer = false;
    attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_LOW_POWER;
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;

    // Create WebGL context
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("canvas", &attrs);
    if (!ctx) {
        printf("Error: WebGL context could not be created.");
    }

    // Set current WebGL context
    emscripten_webgl_make_context_current(ctx);
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Seed the rand function
    srand(time(NULL));

    // Initialize cells
    cells = init_cells();
    randomize_cells(cells);

    // Set main loop
    emscripten_set_main_loop(loop, 0, true);
    return 0;
}
