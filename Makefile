CC = clang
CFLAGS = -Wall
WASM_CC = emcc
WASM_CFLAGS = -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2
RM = rm -f

TARGETS = main.js main.wasm cgl.o test

all : $(TARGETS)
.PHONY : all

main.js main.wasm : main.c cgl.c
	$(WASM_CC) $(CFLAGS) $(WASM_CFLAGS) main.c cgl.c -o main.js

test : test.c cgl.o
	$(CC) $(CFLAGS) test.c cgl.o -o test

cgl.o : cgl.c
	$(CC) $(CFLAGS) -c cgl.c -o cgl.o

clean :
	$(RM) $(TARGETS)
.PHONY : clean