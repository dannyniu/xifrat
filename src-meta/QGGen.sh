#!/bin/sh

cd "$(dirname "$0")"
rm -f ../bin/QGGen
main(){ date ; time ../bin/QGGen "$@" ; date ; }

if
    cc -o ../bin/QGGen -Wall -Wextra QGGen.c ../src-crypto/[!E]*.c
then
    result="$(main "$@" | tee QGGen-result."$*".txt)"
    echo "$result"
fi
