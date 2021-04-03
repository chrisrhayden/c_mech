#!/usr/bin/env bash

if [[ $(basename $PWD) != 'c_mech' ]]; then
    echo 'pleas run in the base c_mech dir'
    return 1
fi

base_dir="$PWD"

mkdir ./out &>/dev/null
mkdir ./libs/{include,built} &>/dev/null

git submodule init

# git submodule update --init --recursive
 git submodule update --init --remote --merge

cd ./libs/c_hashmap/

mkdir ./out/ &>/dev/null

make build

cp ./out/hashmap.o ../built
cp ./src/hashmap*.h ../include

cd "$base_dir"

cd ./libs/log.c/

GCC_CMD='gcc -Wall -pedantic -c -o2 ./src/log.c -o ../built/log.o'
echo "$GCC_CMD"
$GCC_CMD

cp ./src/log.h ../include
