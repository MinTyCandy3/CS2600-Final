#!/bin/bash
HOST="test.mosquitto.org"
TOPIC="mint/tictactoe"
PUBLISH="mosquitto_pub -h $HOST -t $TOPIC -m"
p="backpipe";pid=$(cat pidfile)
tiles=("_" "_" "_" "_" "_" "_" "_" "_")

PLAYER=2

# When someone exits script
ctrl_c() {

    $PUBLISH "Q2"
    echo "Cleaning up... "
    rm -f $p;kill $pid 2>/dev/null
    if [[ "$?" -eq "0" ]];
    then
        echo "Exit success";exit 0
    else
        exit 1
    fi
}

# Check winning patterns
checkThree() {
    if [[ ${tiles[${1}]} = ${tiles[${2}]} && ${tiles[${1}]} = ${tiles[${3}]} && ${tiles[${2}]} = ${tiles[${3}]} ]]
    then    
        if [[ ${tiles[${1}]} = "O" || ${tiles[${1}]} = "X" ]]
        then
            sleep 6
            ctrl_c
        fi
    fi
}

# Check if there is a tie
checkTie() {
    counter=0
    for t in ${tiles[@]}; do
        if [ $t = "_" ]
        then
            counter=1
        fi
    done

    if [ $counter = 0 ]
    then
        ctrl_c
    fi
}

# Check everything
checkAll()
{
    checkThree 0 1 2
    checkThree 3 4 5
    checkThree 6 7 8

    checkThree 0 3 6
    checkThree 1 4 7
    checkThree 2 5 8

    checkThree 0 4 8
    checkThree 6 7 8

    checkTie
}

# AUTOMATIC MOVES
input_move() {
    number=$(( $RANDOM % 9 + 1))
    index=${number}-1;
    echo "${tiles[${index}]}"

    while [ ${tiles[${index}]} != "_" ]
    do
        number=$(( $RANDOM % 9 + 1 ))
        index=${number}-1;
    done
    
    tiles[${index}]="O";

    $PUBLISH "${number}2"
}

# RECEIVE MQTT MESSESGES
listen(){
    ([ ! -p "$p" ]) && mkfifo $p
    (mosquitto_sub -h $HOST -t $TOPIC >$p 2>/dev/null) &
    echo "$!" > pidfile
    while read line <$p
    do
        # Code handling automatic input
        INPUT=$line
        PLAYER=${INPUT:1:1}
        TILE=${INPUT:0:1}
        echo $TILE  $PLAYER;

        if [ "$PLAYER" = "1" ]
        then
            if [[ "$TILE" = "Q" || "$TILE" = "W" || "$TILE" = "T" || "$TILE" = "P" || "$TILE" = "H" ]]
            then
                ctrl_c
            else
                case "$TILE" in
                "1")
                    tiles[0]="X";
                    input_move
                    # echo ${tiles[@]}
                ;;
                "2")
                    tiles[1]="X";
                    input_move
                ;;
                "3")
                    tiles[2]="X";
                    input_move
                ;;
                "4")
                    tiles[3]="X";
                    input_move
                ;;
                "5")
                    tiles[4]="X";
                    input_move
                ;;
                "6")
                    tiles[5]="X";
                    input_move
                ;;
                "7")
                    tiles[6]="X";
                    input_move
                ;;
                "8")
                    tiles[7]="X";
                    input_move
                ;;
                "9")
                    tiles[8]="X";
                    input_move
                ;;
                esac
            fi
        fi

        checkAll



    done
}

while [ true ]
do
    trap ctrl_c INT
    $PUBLISH "y"
    listen
done