#!/bin/bash

if [ -z $WEBSERV_ROOT ]
then
    echo "Enviroment variable WEBSERV_ROOT must be set"
    return
fi

export WEBSERV_SPEED=1

cd $WEBSERV_ROOT
make re MODE=Final
echo "building server"
$WEBSERV_ROOT/webserv $WEBSERV_ROOT/config/test_config.conf > server.log 2>&1 &
cd "$WEBSERV_ROOT/utilites/autotest"
echo "building test"
go build -o autotest autotest.go
cd $WEBSERV_ROOT
$WEBSERV_ROOT/utilites/autotest/autotest > speed_test.log
hasErrors=$(cat speed_test.log | grep FAILED)

if [ "$hasErrors" != "" ]
then
    echo "There were errors during execution speed test scenario, please, check speed_test.log to see what has gone wrong"
fi
