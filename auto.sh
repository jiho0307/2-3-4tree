#!/bin/bash

g++ -g -lm -o tree tree.cpp
./tree < input.txt
rm tree
