#!/bin/bash

if [ -z $WEBSERV_ROOT ]
then
    echo "Enviroment variable WEBSERV_ROOT must be set"
    return
fi

if [ -z $CYCLE_COUNT ]
then
    echo "Enviroment variable WEBSERV_ROOT must be set it needs to determine how much of messages will be processed before server will finish process"
fi

cd $WEBSERV_ROOT
make re MODE=MemoryCheck CYCLE_COUNT=$CYCLE_COUNT
cd "$WEBSERV_ROOT/utilites/autotest"
go build -o autotest autotest.go > /dev/null
./autotest&
# has to start server root directory, otherwise some of config won't be found 
cd $WEBSERV_ROOT
valgrind --leak-check=full --show-leak-kinds=all --log-file="valgrind.log" -v "$WEBSERV_ROOT/webserv" "$WEBSERV_ROOT/config/test_config.conf"
