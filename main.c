#include <GLES2/gl2.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "cgl.h"

// Constants
const unsigned int NUM_ENTRIES_PER_QUAD = 6;
const unsigned int NUM_ENTRIES_PER_VERTEX = 6;
const float COLOR_ALPHA = 1.0f;

// Parameters
extern bool **cells;
extern unsigned int size;

// Enable/disable a delay between generations
#define DELAY 1
useconds_t delay = 100000; // 0.1 sec

// OpenGL state
float *buffer;    // Buffer for graphics context
GLuint vbo;       // Vertex buffer object
GLint attr_pos;   // Position attribute
GLint attr_color; // Color attribute
GLuint program;   // OpenGL program

// Vertex shader source
const char *vertex_shader_glsl = "attribute vec2 a_position; // Input position\n"
                                 "attribute vec4 a_color; // Input color\n"
                                 "varying vec4 v_color;\n"
                                 "void main() {\n"
                                 "   gl_Position = vec4(a_position, 0.0, 1.0); // Output position\n"
                                 "   v_color = a_color; // Pass color to fragment shader\n"
                                 "}";

// Fragment shader source
const char *fragment_shader_glsl = "precision highp float; // Precision for floating point values\n"
                                   "varying vec4 v_color; // Input color\n"
                                   "void main() {\n"
                                   "    gl_FragColor = v_color; // Output color\n"
                                   "}";

void render();

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
        printf("WebGL context could not be created\n");
    }

    // Set current WebGL context
    emscripten_webgl_make_context_current(ctx);

    GLint status;

    // Compile vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_glsl, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        printf("Error compiling vertex shader\n");
    }

    // Compile fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_glsl, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        printf("Error compiling fragment shader\n");
    }

    // Create program
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        printf("Error linking program\n");
    }

    // Delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glUseProgram(program);

    attr_pos = glGetAttribLocation(program, "a_position");
    attr_color = glGetAttribLocation(program, "a_color");

    unsigned int num_vertices = (size * size) * NUM_ENTRIES_PER_QUAD;
    buffer = malloc(sizeof(float) * num_vertices * NUM_ENTRIES_PER_QUAD);
    unsigned int idx = 0;
    for (unsigned int y = 0; y < size; ++y) {
        for (unsigned int x = 0; x < size; ++x) {
            // Calculate vertices as normalized device coordinates
            float x0 = (((float)x / (float)size) * 2.0f) - 1.0f;
            float y0 = (((float)y / (float)size) * 2.0f) - 1.0f;
            float x1 = (((float)(x + 1) / (float)size) * 2.0f) - 1.0f;
            float y1 = (((float)(y + 1) / (float)size) * 2.0f) - 1.0f;
            float color = 0.5f;
            float r = color;
            float g = color;
            float b = color;

            // Each cell is a quad made up of two triangles of three vertices each
            float quad[] = {
                // First triangle
                // First vertex
                x0,
                y0,
                r,
                g,
                b,
                COLOR_ALPHA,
                // Second vertex
                x1,
                y0,
                r,
                g,
                b,
                COLOR_ALPHA,
                // Third vertex
                x1,
                y1,
                r,
                g,
                b,
                COLOR_ALPHA,

                // Second triangle
                // First vertex
                x0,
                y0,
                r,
                g,
                b,
                COLOR_ALPHA,
                // Second vertex
                x1,
                y1,
                r,
                g,
                b,
                COLOR_ALPHA,
                // Third vertex
                x0,
                y1,
                r,
                g,
                b,
                COLOR_ALPHA,
            };

            // Copy quad to buffer
            memcpy(&buffer[idx], quad, sizeof(quad));
            idx += sizeof(quad) / sizeof(quad[0]);
        }
    }

    // Generate and bind buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * NUM_ENTRIES_PER_QUAD, buffer,
                 GL_STATIC_DRAW);

    // Seed the random number generator
    srand(time(NULL));

    // Initialize cells
    cells = init_cells();
    randomize_cells(cells);

    // Set main loop
    emscripten_set_main_loop(render, 0, true);

    // Free dynamically allocated memory
    free_cells(cells);

    return 0;
}

void render() {
    unsigned int num_vertices = (size * size) * NUM_ENTRIES_PER_QUAD;
    unsigned int vertex_offset = NUM_ENTRIES_PER_QUAD * NUM_ENTRIES_PER_VERTEX;
    unsigned int vertex = 0;
    // Update cell colors
    for (unsigned int row = 0; row < size; ++row) {
        for (unsigned int col = 0; col < size; ++col) {
            bool alive = cells[row][col];
            // Alive cells are black, dead cells are white
            float color = alive ? 0.0f : 1.0f;
            // First triangle
            // First vertex
            buffer[vertex + 2] = color;
            buffer[vertex + 3] = color;
            buffer[vertex + 4] = color;
            // Second vertex
            buffer[vertex + 8] = color;
            buffer[vertex + 9] = color;
            buffer[vertex + 10] = color;
            // Third vertex
            buffer[vertex + 14] = color;
            buffer[vertex + 15] = color;
            buffer[vertex + 16] = color;

            // Second triangle
            // First vertex
            buffer[vertex + 20] = color;
            buffer[vertex + 21] = color;
            buffer[vertex + 22] = color;
            // Second vertex
            buffer[vertex + 26] = color;
            buffer[vertex + 27] = color;
            buffer[vertex + 28] = color;
            // Third vertex
            buffer[vertex + 32] = color;
            buffer[vertex + 33] = color;
            buffer[vertex + 34] = color;
            vertex += vertex_offset;
        }
    }

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * num_vertices * NUM_ENTRIES_PER_QUAD,
                    buffer);
    glEnableVertexAttribArray(attr_pos);
    glVertexAttribPointer(attr_pos, 2, GL_FLOAT, GL_FALSE, sizeof(float) * NUM_ENTRIES_PER_QUAD,
                          (void *)0);
    glEnableVertexAttribArray(attr_color);
    glVertexAttribPointer(attr_color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * NUM_ENTRIES_PER_QUAD,
                          (void *)(sizeof(float) * 2));
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    loop();
    if (DELAY) usleep(delay);
}
