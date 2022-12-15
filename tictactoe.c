#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "MQTTClient.h"
#define DASHES      "---------------\n"
#define ADDRESS     "tcp://broker.emqx.io:1883"
#define CLIENTID    "Player 2"
#define TOPIC       "mint/tictactoe"
#define PAYLOAD     "y"
#define QOS         1
#define TIMEOUT     10000L
volatile MQTTClient_deliveryToken deliveredtoken;

void printStatus();

// TICTACTOE VARIABLES
bool ourTurn = false;
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

    // printf(DASHES);
    // printf("Message arrived\n");
    // printf("     topic: %s\n", topicName);
    // printf("   message: ");

    payloadptr = message->payload;
    // char payloadCh = payloadptr[0];
    // char playerCh = payloadptr[1];
    char payloadCh[10];
    // // printf("%c", payloadCh);
    // for(i=0; i<message->payloadlen; i++)
    // {
    //     payloadCh[i] += *payloadptr;
    //     printf("%c", *payloadptr++);
    // }
    // printf("\n");
    // printf(DASHES);

    // printf("\n%c", payloadCh[0]);
    // printf("\n%c", payloadCh[1]);
    payloadptr++;
    
    if(*payloadptr-- == '1')
    {
        switch(*payloadptr)
        {
            case 'q':
            case 'Q':
                ourTurn = false;
                exit(1);
                break;
            case '1':
                tiles[0] = 'X';
                ourTurn = true;
                break;
            case '2':
                tiles[1] = 'X';
                ourTurn = true;
                break;
            case '3':
                tiles[2] = 'X';
                ourTurn = true;
                break;
            case '4':
                tiles[3] = 'X';
                ourTurn = true;
                break;
            case '5':
                tiles[4] = 'X';
                ourTurn = true;
                break;
            case '6':
                tiles[5] = 'X';
                ourTurn = true;
                break;
            case '7':
                tiles[6] = 'X';
                ourTurn = true;
                break;
            case '8':
                tiles[7] = 'X';
                ourTurn = true;
                break;
            case '9':
                tiles[8] = 'X';
                ourTurn = true;
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

    char input[50]; 

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
        if(!ourTurn)
        {
            printStatus();
            printf("%s\n","Waiting for Player 1. . . (Press <Enter> to check if it's your turn or <Q> to Quit))");
        }
        else
        {
            printStatus();
            printf("%s\n","It's your turn!. . . (Press <1-9> or <Q> to Quit)");
        }
        fflush(stdin);
        fgets(input,sizeof(input),stdin);

        if(ourTurn)
        {
            switch(input[0])
            {
                case 'q':
                case 'Q':
                    printf("\nEnding game...\n");
                    break;
                case '1':
                    tiles[0] = 'O';
                    ourTurn = false;
                    break;
                case '2':
                    tiles[1] = 'O';
                    ourTurn = false;
                    break;
                case '3':
                    tiles[2] = 'O';
                    ourTurn = false;
                    break;
                case '4':
                    tiles[3] = 'O';
                    ourTurn = false;
                    break;
                case '5':
                    tiles[4] = 'O';
                    ourTurn = false;
                    break;
                case '6':
                    tiles[5] = 'O';
                    ourTurn = false;
                    break;
                case '7':
                    tiles[6] = 'O';
                    ourTurn = false;
                    break;
                case '8':
                    tiles[7] = 'O';
                    ourTurn = false;
                    break;
                case '9':
                    tiles[8] = 'O';
                    ourTurn = false;
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
    } while(input[0] != 'Q' && input[0] != 'q');

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