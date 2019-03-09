#!/bin/sh
echo make
make
echo clean output data
rm -rf output/
echo dataset assign
./dsassign
echo train
./train
echo classify
./classify
