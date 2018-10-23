#!/bin/bash
source env.sh
COMPILER=$P4C_BM_PATH/p4c_bm/__main__.py

if [ -f "autosketch.json" ]; then
    rm autosketch.json
fi

$COMPILER --json autosketch.json main.p4
