CC=gcc
CFLAGS=-Wall -g -pedantic

SRC = $(wildcard src/*.c) \
	  $(wildcard src/font/*.c) \
	  $(wildcard src/collections/robin_hood_hashing/*.c) \
	  $(wildcard src/game_map/*.c) \
	  $(wildcard src/game/*.c) \
	  $(wildcard src/components/*.c) \
	  $(wildcard src/systems/*.c) 

TARGET_NAME = ./out/target_build

LIB_FLAGS = -lm `pkg-config --cflags --libs sdl2 freetype2` -DLOG_USE_COLOR

build: $(SRC)
	$(CC) $(CFLAGS) $^ $(LIB_FLAGS) -o $(TARGET_NAME)

run: build
	./out/target_build
