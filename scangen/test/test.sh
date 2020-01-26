#!/usr/bin/env bash

source ../env/bin/activate
export LD_LIBRARY_PATH=/usr/local/lib
python example.py
make -B -C c
make -B -C cpp
make -B -C dot
make -B -C python
export TEST_DIR=c
python test.py
export TEST_DIR=cpp
python test.py
export TEST_DIR=python
python test.py
make clean -C c
make clean -C cpp
make clean -C dot
make clean -C python
