#!/bin/bash
echo "Content-type: text/html"
echo ""
read -n "$CONTENT_LENGTH" LOCAL_QUERY_STRING
name=`echo "$LOCAL_QUERY_STRING" | awk '{split($0,array,"&")} END{print array[1]}' | awk '{split($0,array,"=")} END{print array[2]}'`
email=`echo "$LOCAL_QUERY_STRING" | awk '{split($0,array,"&")} END{print array[2]}' | awk '{split($0,array,"=")} END{print array[2]}'`

echo "<!DOCTYPE html>
<html>
<head>
        <title>Welcome to our application</title>
</head>
<body>
        <h1>Welcome $name </h1>
        <h2> Your e-mail address is $email <h2>
</body>
</html>"
