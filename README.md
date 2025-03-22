# Conway's Game of Life in WebAssembly

Conway's Game of Life in WebAssembly and WebGL.

## Environment Setup

Install the [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html).

```sh
source path/to/emsdk/emsdk_env.sh
```

## Build

```sh
make cgl
```

or

```sh
emcc -Wall -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 main.c cgl.c -o main.js
```

## Run

First, start a HTTP server to serve files in the current working directory:

```sh
python3 -m http.server
```

Then, open http://0.0.0.0:8000/ in your browser.

## Test

Conway's Game of Life can also run in the terminal for testing purposes:

```sh
make test
```

```sh
./test
```

## References

- https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
- https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_Wasm
- https://emscripten.org/docs/getting_started/Tutorial.html
- https://emscripten.org/docs/api_reference/emscripten.h.html
- https://developer.mozilla.org/en-US/docs/Web/HTML/Element/input/range
- https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
- https://en.cppreference.com/w/c/numeric/random/srand
- https://en.cppreference.com/w/c/numeric/random/rand
- https://emscripten.org/docs/porting/multimedia_and_graphics/OpenGL-support.html
- https://emscripten.org/docs/porting/multimedia_and_graphics/EGL-Support-in-Emscripten.html
- https://github.com/emscripten-core/emscripten/blob/main/test/third_party/glbook/Common/esUtil.c
- https://github.com/emscripten-core/emscripten/blob/main/test/minimal_webgl
- https://doublejump.github.io/webgl-with-webassembly/index.html
