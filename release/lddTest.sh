#!/bin/sh
exe="test2"
des="/home/ubuntu/project/release"
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')
cp $deplist $des
