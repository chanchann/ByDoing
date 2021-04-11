#!/bin/bash

numArg=$#
# numArg=$#; echo "\$#: $#;\$NumArg: $numArg"
clear 
if [[ $numArg != 2 ]] ; then
    echo "Your enter too many or too little arguments to scrip when expected 2, asm file"
    echo "you supplied only $numArg"
    echo "Usage: ./compile <input.asm> <program name>"
else 
    echo "Compling asm using NASM"
    input=$1
    output=$2 
    inter=asm000.o 
    nasm -g -f elf64 -o $inter $input 
    ret=$?
    if [[ $ret != 0 ]] ; then 
        echo "Nasm compilation failed"
        exit
    else 
        echo "Compling using GCC for final output"
        gcc -no-pie -o $output $inter 
        ret=$?
        if [[ $ret != 0 ]] ; then 
            echo "There was a problem with gcc compilation"
            exit 
        else 
            rm $inter 
            echo "Compilation successful"
        fi 
    fi 
fi 