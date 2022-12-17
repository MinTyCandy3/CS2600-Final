#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "MQTTClient.h"
#define DASHES      "---------------\n"
#define ADDRESS     "tcp://test.mosquitto.org:1883"
#define CLIENTID    "Player 2"
#define TOPIC       "minhtran"
#define PAYLOAD     "y"
#define QOS         1
#define TIMEOUT     10000L
volatile MQTTClient_deliveryToken deliveredtoken;

void printStatus();
void checkTile(int tile);
void checkIfTie();
bool checkWinStatus(char t1, char t2, char t3);

// TICTACTOE VARIABLES
char input[50]; 
bool ourTurn = false;
bool endGame = false;
bool player1Wins = false;
bool player2Wins = false;
bool isTie = false;
char tiles[9] = {'_','_','_',  '_','_','_'  ,'_','_','_'};

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    // printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{

    int i;
    char* payloadptr;
    payloadptr = message->payload;

    payloadptr++;
    if(*payloadptr-- == '1')
    {
        switch(*payloadptr)
        {
            case 'W':
                player1Wins = true;
                break;
            case 'Q':
                endGame = true;
                ourTurn = false;
                break;
            case '1':
                tiles[0] = 'X';
                ourTurn = true;
                checkIfTie();
                break;
            case '2':
                tiles[1] = 'X';
                ourTurn = true;
                checkIfTie();
                break;
            case '3':
                tiles[2] = 'X';
                ourTurn = true;
                checkIfTie();
                break;
            case '4':
                tiles[3] = 'X';
                ourTurn = true;
                checkIfTie();
                break;
            case '5':
                tiles[4] = 'X';
                ourTurn = true;
                checkIfTie();
                break;
            case '6':
                tiles[5] = 'X';
                ourTurn = true;
                checkIfTie();
                break;
            case '7':
                tiles[6] = 'X';
                ourTurn = true;
                checkIfTie();
                break;
            case '8':
                tiles[7] = 'X';
                ourTurn = true;
                checkIfTie();
                break;
            case '9':
                tiles[8] = 'X';
                ourTurn = true;
                checkIfTie();
                break;
        }
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}
void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}
int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    
    pubmsg.payload = PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    // printf("Waiting for up to %d seconds for publication of %s\n"
    //         "on topic %s for client with ClientID: %s\n",
    //         (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    // printf("Message with delivery token %d delivered\n", token);

    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);
    do
    {
        if(player2Wins)
        {
            printStatus();
            printf("You win! Good game! Goodbye~");

            pubmsg.payload = input;
            pubmsg.payloadlen = strlen(input);
            pubmsg.qos = QOS;
            pubmsg.retained = 0;

            MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
            rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);

            return rc;
        }

        if(player1Wins)
        {
            printStatus();
            printf("%s\n","Sorry, Player 1 Wins! Try again next time, goodbye~");
            return rc;
        }

        if(isTie)
        {
            printStatus();
            printf("%s\n", "Oof, looks like a tie! Good game, goodbye~");
            return rc;
        }

        if(endGame)
        {
            printf("%s\n","Player 1 ended the game... goodbye!");
            return rc;
        }

        if(!ourTurn)
        {
            printStatus();
            printf("%s\n","Waiting for Player 1. . . (Press <Enter> to check if it's your turn or <Q> to Quit)");
        }
        else
        {
            printStatus();
            printf("%s\n","It's your turn!. . . (Press <1-9> or <Q> to Quit)");
        }

        fflush(stdin);
        fgets(input,sizeof(input),stdin);

        if(input[0] == 'q' || input[0] == 'Q')
        {
            printf("\nYou Ended The Game... Goodbye!\n");
            endGame = true;
        }

        if(ourTurn && !endGame)
        {
            switch(input[0])
            {
                case '1':
                    checkTile(0);
                    break;
                case '2':
                    checkTile(1);
                    break;
                case '3':
                    checkTile(2);
                    break;
                case '4':
                    checkTile(3);
                    break;
                case '5':
                    checkTile(4);
                    break;
                case '6':
                    checkTile(5);
                    break;
                case '7':
                    checkTile(6);
                    break;
                case '8':
                    checkTile(7);
                    break;
                case '9':
                    checkTile(8);
                    break;
                case '\n':
                    break;
                default:
                    if(ourTurn)
                    {
                        printf("Invalid input, please try again");
                    }
                    break;
            }
            
        }

        input[1] = '2';

        pubmsg.payload = input;
        pubmsg.payloadlen = strlen(input);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    } while((input[0] != 'Q' && input[0] != 'q'));

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}

// prints status of the tictactoe board
void printStatus()
{
    printf("\nThe current status:\n");
    printf("+-----------+ \n| %c | %c | %c | \n", tiles[0], tiles[1], tiles[2]);
    printf("+-----------+ \n| %c | %c | %c | \n", tiles[3], tiles[4], tiles[5]);
    printf("+-----------+ \n| %c | %c | %c | \n", tiles[6], tiles[7], tiles[8]);
    printf("+-----------+ \n\n");
}

void checkTile(int tile)
{
    if(tiles[tile] == '_')
    {
        tiles[tile] = 'O';
        ourTurn = false;
        if(
        checkWinStatus(tiles[0], tiles[1], tiles[2]) ||
        checkWinStatus(tiles[3], tiles[4], tiles[5]) ||
        checkWinStatus(tiles[6], tiles[7], tiles[8]) ||

        checkWinStatus(tiles[0], tiles[3], tiles[6]) ||
        checkWinStatus(tiles[1], tiles[4], tiles[7]) ||
        checkWinStatus(tiles[2], tiles[5], tiles[8]) ||

        checkWinStatus(tiles[0], tiles[4], tiles[8]) ||
        checkWinStatus(tiles[6], tiles[4], tiles[2]))
        {
            player2Wins = true;
        }
        else{
            checkIfTie();
        }
    }
    else
    {
        printf("\nTile is already occupied!\n");
        input[0] = '0';
    }

}

void checkIfTie()
{
    for(int i=0; i<9; i++)
    {
        if(tiles[i] == '_')
        {
            break;
        }
        else if (i == 8)
        {
            isTie = true;
        }
    }

}

bool checkWinStatus(char t1, char t2, char t3)
{
    if(t1 != 'O' || t2 != 'O' || t3 != 'O')
    {
        return false;
    }

    if(t1 == t2 && t1 == t3 && t2 == t3)
    {
        return true;
    }
}