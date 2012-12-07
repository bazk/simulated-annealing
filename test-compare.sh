#!/bin/bash

for i in 1 2 3 4; do
    FILE="tests/DadosUsina$i.txt"
    echo File: $FILE
    NUMTURBINES=$(cat $FILE | head -n1)
    POWERLIST=$(cat $FILE | head -n3 | tail -n1)
    HEIGHTLIST=$(cat $FILE | head -n5 | tail -n1)
    MINPOWER=$(echo $POWERLIST | awk '{print $1}')
    MAXPOWER=$(echo $POWERLIST | awk '{print $NF}')
    
    make rebuild CFLAGS= >/dev/null
    START=$(date +%s)
    echo -n "" > $FILE.out.ane
    for HEIGHT in $HEIGHTLIST; do
        for OBJPOW in `seq $MINPOWER 10 $((MAXPOWER*NUMTURBINES))`; do
            echo -n "$HEIGHT $OBJPOW " >> $FILE.out.ane
            bin/otimiza $HEIGHT $OBJPOW < $FILE >> $FILE.out.ane
        done
    done
    END=$(date +%s)
    echo Simulated Annealing took: $(($END-$START))
    
#    make rebuild CFLAGS=-D_BRUTE_ >/dev/null
#    START=$(date +%s)
#    echo -n "" > $FILE.out.bru
#    for HEIGHT in $HEIGHTLIST; do
#        for OBJPOW in `seq $MINPOWER 10 $((MAXPOWER*NUMTURBINES))`; do
#            echo -n "$HEIGHT $OBJPOW " >> $FILE.out.bru
#            bin/otimiza $HEIGHT $OBJPOW < $FILE >> $FILE.out.bru
#        done
#    done
#    END=$(date +%s)
#    echo Brute force took: $(($END-$START))

    echo Num of diff lines: $(diff $FILE.out.ane $FILE.out.bru -y --suppress-common-lines | wc -l)
done
