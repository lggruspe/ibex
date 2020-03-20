#!/usr/bin/env bash

LD_LIBRARY_PATH="../../build" python3 generate_code.py
make -B -C c
make -B -C cpp
make -B -C dot
make -B -C python
TEST_DIR=c python3 test.py
TEST_DIR=cpp python3 test.py
TEST_DIR=python python3 test.py
make clean -C c
make clean -C cpp
make clean -C dot
make clean -C python
