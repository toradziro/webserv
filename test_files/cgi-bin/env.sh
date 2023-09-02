#!/bin/bash

echo -e 'Content-Type: text/html\r\n'
echo '<html>'
echo '<h3>'
echo 'Environment:'
echo '</h3>'
echo ''
echo '<pre>' 
env 
echo '</pre>'
echo '</html>'