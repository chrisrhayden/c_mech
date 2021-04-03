CC=gcc
CFLAGS=-Wall -ggdb3 -pedantic


SRC = $(wildcard ./src/*.c)

TARGET_NAME = ./out/target_build

LIB_FLAGS = `pkg-config --cflags --libs sdl2` -I./libs/include -lm

LIBS = ./libs/built/log.o ./libs/built/hashmap.o

build: $(SRC)
	$(CC) $(CFLAGS) $^ $(LIB_FLAGS) $(LIBS) -o $(TARGET_NAME)

build_libs:
	./libs/init_libs.sh

run: build
	./out/target_build
