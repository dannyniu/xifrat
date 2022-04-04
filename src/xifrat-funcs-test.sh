#!/bin/sh

cd "$(dirname "$0")"

bin=xifrat-funcs-test
src="\
xifrat-funcs-test.c
xifrat-funcs.c
../src-crypto/endian.c
"

wflags="-Wall -Wextra"

cc $wflags -o ../bin/$bin $src &&
    dd if=/dev/urandom bs=8 | time ../bin/$bin
