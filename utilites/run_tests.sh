#!/bin/bash

export WEBSERV_ROOT=$PWD
echo "WEBSERV_ROOT is $WEBSERV_ROOT"
export CYCLE_COUNT=5
echo "CYCLE_COUNT is $CYCLE_COUNT"
#bash "$WEBSERV_ROOT/utilites/memory_check.sh"
bash "$WEBSERV_ROOT/utilites/full_test.sh"
