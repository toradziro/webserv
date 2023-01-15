#!/bin/bash

if [ -z $WEBSERV_ROOT ]
then
    echo "Enviroment variable WEBSERV_ROOT must be set"
    return
fi

cd $WEBSERV_ROOT
make re MODE=Final
echo "building server"
$WEBSERV_ROOT/webserv $WEBSERV_ROOT/config/test_config.conf > server.log 2>&1 &
cd "$WEBSERV_ROOT/utilites/autotest"
echo "building test"
go build -o autotest autotest.go
./autotest
