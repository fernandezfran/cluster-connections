#!/bin/bash

# compilation
cd ../src/
make clean
make
cp cluster-connections ../example/

# execution
cd ../example/ 
./cluster-connections < in.config

# analysis
python3 plot.py
