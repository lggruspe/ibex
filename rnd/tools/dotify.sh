#!/usr/bin/env bash

./dot.test "$1" > ".dotify.$1.dot"
dot -Tpdf ".dotify.$1.dot" -o "$1.pdf"
rm ".dotify.$1.dot"
