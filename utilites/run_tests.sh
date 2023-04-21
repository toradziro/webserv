#!/bin/bash

function killServer() {
    # we have to kill server to release socket
    serverProcessId=$(ps | grep webserv | sed -r 's/^[^0-9]*([0-9]+).*/\1/')
    echo "serverProcessId is $serverProcessId"
    if [ "$serverProcessId" != "" ]
    then
        kill $serverProcessId
    fi
}

export WEBSERV_ROOT=$PWD
echo "WEBSERV_ROOT is $WEBSERV_ROOT"
export CYCLE_COUNT=5
echo "CYCLE_COUNT is $CYCLE_COUNT"
bash "$WEBSERV_ROOT/utilites/memory_check.sh"
killServer
bash "$WEBSERV_ROOT/utilites/full_test.sh"
killServer
bash "$WEBSERV_ROOT/utilites/speed_test_scenario.sh"
killServer
