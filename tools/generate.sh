#!/usr/bin/env bash

LD_LIBRARY_PATH="../build" python3 example_parser/generate.py
LD_LIBRARY_PATH="../build" python3 rnd_parser/generate.py
LD_LIBRARY_PATH="../build" python3 sagl_parser/generate.py
