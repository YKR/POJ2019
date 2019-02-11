#!/bin/bash
read -p 'problem id: ' problemId
genCpp=$problemId'_gen.cpp'
problemCpp=$problemId'.cpp'
bfCpp=$problemId'_bf.cpp'
while true; do
  g++ $genCpp
  ./a.out > input.txt
  g++ -std=gnu++98 $problemCpp
  ./a.out < input.txt > output.txt
  g++ $bfCpp
  ./a.out < input.txt > output1.txt
  if diff output.txt output1.txt; then
    echo AC
  else
    echo WA
    exit 0
  fi
  done
