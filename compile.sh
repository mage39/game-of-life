#! /usr/bin/env bash
# CFLAGS="${CFLAGS/\-O3/-O0}"
echo $CFLAGS
gcc $CFLAGS -lraylib -o out main.c
