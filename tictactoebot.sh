#!/bin/bash
HOST="test.mosquitto.org"
TOPIC="mint/tictactoe"
PUBLISH="mosquitto_pub -h $HOST -t $TOPIC -m"
p="backpipe";pid=$(cat pidfile)
OCCUPED=(0 0 0 0 0 0 0 0 0)

# When someone exits script
ctrl_c() {
  echo "Cleaning up... "
  rm -f $p;kill $pid 2>/dev/null
  if [[ "$?" -eq "0" ]];
  then
     echo "Exit success";exit 0
  else
     exit 1
  fi
}

# AUTOMATIC MOVES

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
            if [["$TILE" = "Q" || "$TILE" = "W"]]
            then
                ctrl_c
            else
                case "$TILE" in
                "1")

                ;;
                "2")

                ;;
                "3")

                ;;
                "4")

                ;;
                "5")

                ;;
                "6")

                ;;
                "7")

                ;;
                "8")

                ;;
                "9")

                ;;
                esac
            fi
        fi

    done
}


trap ctrl_c INT
$PUBLISH "y"
listen