#!/bin/sh
cd ./release
chmod 777 test2
chmod 777 test2.sh
chmod 777 last3
while true
do
procnum=`ps -ef|grep "test2"|grep -v "grep"|wc -l`
if [ $procnum -eq 0 ]; then
killall -9 last3
./last3 &
sh ./test2.sh &
else
echo "already started"   
fi
sleep 1
done
