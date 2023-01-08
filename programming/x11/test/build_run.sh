#!/bin/bash

NAME=`echo $1 | cut -d '.' -f 1`
gcc -Wall -Wextra -o $NAME $1 -lX11
./$NAME

