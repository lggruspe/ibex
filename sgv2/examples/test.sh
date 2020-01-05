#!/usr/bin/env bash

export TEST_DIR=c
python test.py
export TEST_DIR=cpp
python test.py
export TEST_DIR=python
python test.py
