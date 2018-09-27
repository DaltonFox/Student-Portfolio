#!/bin/bash
g++ SIMD2.cpp -o SIMD2.compiled -std=c++11 -mavx -mavx2 -Wall -lpng -lz && ./SIMD2.compiled  -xterm "input/test-rgb8.png"