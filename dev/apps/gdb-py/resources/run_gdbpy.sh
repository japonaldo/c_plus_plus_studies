#!/usr/bin/env bash

# TODO remove absolute paths
source /home/fujioka/gdbpy/bin/activate

gdb --batch \
    -x "~/Projects/japonaldo/cpp_studies/dev/apps/gdb-py/resources/test.gdb" \
    /home/fujioka/Projects/japonaldo/cpp_studies/.build/dev/apps/gdb-py/src/gdb-py 
