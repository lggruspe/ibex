#!/usr/bin/env bash

source env/bin/activate
LD_LIBRARY_PATH="../build:../../sagl/build" python generate_scanner.py
