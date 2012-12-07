#!/bin/bash

FILE="tests/DadosUsina1.txt"
NUMTURBINES=$(cat $FILE | head -n1)
POWERLIST=$(cat $FILE | head -n3 | tail -n1)
HEIGHTLIST=$(cat $FILE | head -n5 | tail -n1)
MINPOWER=$(echo $POWERLIST | awk '{print $1}')
MAXPOWER=$(echo $POWERLIST | awk '{print $NF}')

make rebuild CFLAGS= >/dev/null
for HEIGHT in $HEIGHTLIST; do
    echo -n "" > $FILE.data.$HEIGHT
    for OBJPOW in `seq $MINPOWER 10 $((MAXPOWER*NUMTURBINES))`; do
        echo -n "$OBJPOW " >> $FILE.data.$HEIGHT
        /usr/bin/time -o $FILE.data.$HEIGHT -a -f %e bin/otimiza $HEIGHT $OBJPOW < $FILE >/dev/null
    done
done
