CC=gcc
CFLAGS=-Wall -g -pedantic

SRC = $(wildcard ./src/*.c) \
		$(wildcard ./src/font/*.c)

TEST_SRC = $(wildcard ./test/*.c) \
			$(wildcard ./test/font/*.c) \
			$(wildcard ./src/font/*.c)

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

build_test:
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB_FLAGS) $(LIBS) -o ./out/test

run_test: build_test
	./out/test
