#!/usr/bin/env bash

LD_LIBRARY_PATH="../../build" python generate_code.py
make -B -C c
make -B -C cpp
make -B -C dot
make -B -C python
TEST_DIR=c python test.py
TEST_DIR=cpp python test.py
TEST_DIR=python python test.py
make clean -C c
make clean -C cpp
make clean -C dot
make clean -C python
