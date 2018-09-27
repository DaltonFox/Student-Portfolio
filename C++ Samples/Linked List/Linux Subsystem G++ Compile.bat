set main=Main
set mainext=.cpp
set include=LinkedList.h
set source=
set g++args=-std=c++11 -Wall
set args=""


type NUL > g++compile.sh
set in="g++ %include% %main%%mainext% %source% -o %main%  %g++args%"
set out=%in:"=%
echo #^!/bin/bash >> g++compile.sh
echo %out:"=% ^&^& ^./%main% -xterm %args% >> g++compile.sh
echo rm -f "%main%" >> g++compile.sh
echo rm -f "g++compile.sh" >> g++compile.sh
start cmd /K bash "./g++compile.sh" 