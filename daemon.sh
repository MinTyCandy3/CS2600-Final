#!/bin/bash
HOST2="test.mosquitto.org"
TOPIC2="mint/tictactoe"
PUBLISH2="mosquitto_pub -h $HOST2 -t $TOPIC2 -m"
p2="backpipe2";pid2=$(cat pidfile2)
READY=0

listen(){
    ([ ! -p "$p2" ]) && mkfifo $p2
    (mosquitto_sub -h $HOST2 -t $TOPIC2 >$p2 2>/dev/null) &
    echo "$!" > pidfile
    while read line <$p2
    do
        ESPINPUT=$line
        FIRST=${ESPINPUT:0:1}

        if[[ "$FIRST" = "R" ]]
        {
            READY=1
        }
    done
}

listen
while true
do
    echo "Waiting. . ."
    sleep 10
    if [[ $READY = 1 ]]
    then
        echo "CONNECTED. . ."
        sleep 2
        bash tictactoebot.sh
        READY=0
    else
        echo "NOT CONNECTED. . ."
        $PUBLISH2 "R"
    fi
done