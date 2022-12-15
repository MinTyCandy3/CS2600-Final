#!/bin/bash
# file: foo.sh
HOST="test.mosquitto.org"
SUB="mosquitto_sub"
PUB="mosquitto_pub"
TOPIC="mint/tictactoe"
INPUT="HELLO"

gnome-terminal --wait -- bash -c "$SUB -h $HOST -t $TOPIC; read; exit"

echo "$INPUT"