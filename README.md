# Conway's Game of Life in WebAssembly

## Environment Setup

- Install the [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html)

```sh
source path/to/emsdk/emsdk_env.sh
```

## Build

```sh
emcc main.c -o main.js
```

## Run

```sh
python3 -m http.server
```

Open http://0.0.0.0:8000/ in your browser.

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
