#!/usr/bin/env bash

base_dir="$PWD"

if [[ ! -d "${base_dir}/.git" ]]; then
    while [[ $base_dir != / ]] && [[ ! -d "${base_dir}/.git" ]]; do
        base_dir="$(realpath "${base_dir}/..")"
    done
fi

cd "$base_dir"

mkdir ./out &>/dev/null
mkdir ./libs/{include,built} &>/dev/null

git submodule init

# git submodule update --init --recursive
 git submodule update --init --remote --merge

cd ./libs/hashmap/

mkdir ./out/ &>/dev/null

make build

cp ./out/hashmap.o ../built
cp ./src/hashmap*.h ../include

cd "$base_dir"

cd ./libs/dynamic_array

mkdir ./out/ &>/dev/null

make build

cp ./out/dynamic_array.o ../built
cp ./src/dynamic_array.h ../include

cd "$base_dir"

cd ./libs/log.c/

GCC_CMD='gcc -Wall -pedantic -c -o2 -DLOG_USE_COLOR ./src/log.c -o ../built/log.o'
echo "$GCC_CMD"
$GCC_CMD

cp ./src/log.h ../include
