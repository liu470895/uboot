#!/bin/sh
username=ubuntu #read username
echo "输入IP地址:"
read ip
echo "连接中---"
sleep 1
ssh $username@$ip

