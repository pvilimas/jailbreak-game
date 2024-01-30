all: build run

build:
    gcc -std=c99 -Wall -Wextra -o build/game src/*.c lib/libraylib.a -lm

run:
    ./build/game