#!/bin/sh

cd "$(dirname "$0")"
rm -f ../bin/xifrat-func-m-test
if
    cc -o ../bin/xifrat-func-m-test -Wall -Wextra \
       xifrat-func-m-test.c \
       xifrat-primitives.c
then
    ../bin/xifrat-func-m-test < /dev/urandom
fi
