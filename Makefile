CC=gcc
CFLAGS=-Wall -g -pedantic

SRC = $(wildcard ./src/*.c)

TARGET_NAME = ./out/target_build

LIB_FLAGS = -I./libs/include -lm \
			`pkg-config --cflags --libs sdl2 freetype2`

LIBS = ./libs/built/log.o ./libs/built/hashmap.o

build: $(SRC)
	$(CC) $(CFLAGS) $^ $(LIB_FLAGS) $(LIBS) -o $(TARGET_NAME)

build_libs:
	./libs/init_libs.sh

run: build
	./out/target_build
