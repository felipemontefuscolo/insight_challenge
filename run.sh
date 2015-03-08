#!/bin/sh

CC=g++

SOURCE=`ls src/*.cpp`
EXEC=`ls src/*.cpp | sed 's/\.cpp//g'`

for i in ${SOURCE}; do
  echo compiling $i
  ${CC} $i -o ${i/.cpp}
done

echo ""

./src/count  ./wc_input ./wc_output/wc_result.txt
./src/median ./wc_input ./wc_output/med_result.txt

echo "\ndone"
