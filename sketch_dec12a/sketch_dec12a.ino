#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#define LED_BUILTIN1  2
#define LED_BUILTIN2  0
#define LED_BUILTIN3  4
#define LED_BUILTIN4  5
#define LED_BUILTIN5  23
#define LED_BUILTIN6  22
#define LED_BUILTIN7  21
#define LED_BUILTIN8  19
#define LED_BUILTIN9  18
#define LED_BUILTIN10  32
#define LED_BUILTIN11 33
#define LED_BUILTIN12 25
#define LED_BUILTIN13  26
#define LED_BUILTIN14  27
#define LED_BUILTIN15  15
#define LED_BUILTIN16  14
#define LED_BUILTIN17  12
#define LED_BUILTIN18  13


// Infared Remote variables
const uint16_t recvPin = 34; // Infrared receiving pin
IRrecv irrecv(recvPin);      // Create a class object used to receive class
decode_results results;       // Create a decoding results class object

// WiFi
const char *ssid = "NETGEAR20"; // Enter your WiFi name
const char *password = "elegantpiano135";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "test.mosquitto.org";
const char *topic = "mint/tictactoe";
// const char *mqtt_username = "emqx";
// const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// TICTACTOE Variables
String player2Input;
bool running = false;
bool ourTurn = true;
char tiles[9] = {'_','_','_',  '_','_','_'  ,'_','_','_'};

void setup() {

  pinMode(LED_BUILTIN1, OUTPUT);
  pinMode(LED_BUILTIN2, OUTPUT);
  pinMode(LED_BUILTIN3, OUTPUT);
  pinMode(LED_BUILTIN4, OUTPUT);
  pinMode(LED_BUILTIN5, OUTPUT);
  pinMode(LED_BUILTIN6, OUTPUT);
  pinMode(LED_BUILTIN7, OUTPUT);
  pinMode(LED_BUILTIN8, OUTPUT);
  pinMode(LED_BUILTIN9, OUTPUT);
  pinMode(LED_BUILTIN10, OUTPUT);
  pinMode(LED_BUILTIN11, OUTPUT);
  pinMode(LED_BUILTIN12, OUTPUT);
  pinMode(LED_BUILTIN13, OUTPUT);
  pinMode(LED_BUILTIN14, OUTPUT);
  pinMode(LED_BUILTIN15, OUTPUT);
  pinMode(LED_BUILTIN16, OUTPUT);
  pinMode(LED_BUILTIN17, OUTPUT);
  pinMode(LED_BUILTIN18, OUTPUT);

  irrecv.enableIRIn();        // Start the receiver
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(recvPin);   //print the infrared receiving pin

  // Set software serial baud to 115200;
  Serial.begin(115200);
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp32-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), NULL, NULL)) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  // publish and subscribe
  client.publish(topic, "Hi, I'm Player 1, let's have a good game! ^^");
  client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  // Serial.println();

  for (int i = 0; i < length; i++) {
      // Serial.print((char) payload[i]);
      player2Input = player2Input + (char) payload[i];
  }

  char input = player2Input.charAt(0);
  char playerNum = player2Input.charAt(1);

  switch(input)
  {
    case 'y':
      turnOffAll();
      running = true;
      ourTurn = true;
      break;
  }

  if(!running && input == 'R')
  {
    client.publish(topic, "R1");
  }

  if(running && input == 'q' || input == 'Q')
  {
    endGame();
  }

  // Serial.printf("PLAYER: %c\n", playerNum);
  if(running && !ourTurn && playerNum == '2')
  {
    ourTurn = true;
    switch(input)
    {
      case '1':
        digitalWrite(LED_BUILTIN2, HIGH);
        tiles[0] = 'O';
        break;
      case '2':
        digitalWrite(LED_BUILTIN4, HIGH);
        tiles[1] = 'O';
        break;
      case '3':
        digitalWrite(LED_BUILTIN6, HIGH);
        tiles[2] = 'O';
        break;
      case '4':
        digitalWrite(LED_BUILTIN8, HIGH);
        tiles[3] = 'O';
        break;
      case '5':
        digitalWrite(LED_BUILTIN10, HIGH);
        tiles[4] = 'O';
        break;
      case '6':
        digitalWrite(LED_BUILTIN12, HIGH);
        tiles[5] = 'O';
        break;
      case '7':
        digitalWrite(LED_BUILTIN14, HIGH);
        tiles[6] = 'O';
        break;
      case '8':
        digitalWrite(LED_BUILTIN16, HIGH);
        tiles[7] = 'O';
        break;
      case '9':
        digitalWrite(LED_BUILTIN18, HIGH);
        tiles[8] = 'O';
        break;
      default:
        ourTurn = false;
        break;
    }

    // Player 2 / Computer wins
    if(checkWinStatus(tiles[0], tiles[1], tiles[2], 'O'))
    {
      turnOnWinningPattern(LED_BUILTIN2, LED_BUILTIN4, LED_BUILTIN6);
    }
    else if(checkWinStatus(tiles[3], tiles[4], tiles[5], 'O'))
    {
      turnOnWinningPattern(LED_BUILTIN8, LED_BUILTIN10, LED_BUILTIN12);
    }
    else if(checkWinStatus(tiles[6], tiles[7], tiles[8], 'O'))
    {
      turnOnWinningPattern(LED_BUILTIN14, LED_BUILTIN16, LED_BUILTIN18);
    }
    else if(checkWinStatus(tiles[0], tiles[3], tiles[6], 'O'))
    {
      turnOnWinningPattern(LED_BUILTIN2, LED_BUILTIN8, LED_BUILTIN14);
    }
    else if(checkWinStatus(tiles[1], tiles[4], tiles[7], 'O'))
    {
      turnOnWinningPattern(LED_BUILTIN4, LED_BUILTIN10, LED_BUILTIN16);
    }
    else if(checkWinStatus(tiles[2], tiles[5], tiles[8], 'O'))
    {
      turnOnWinningPattern(LED_BUILTIN6, LED_BUILTIN12, LED_BUILTIN18);
    }
    else if(checkWinStatus(tiles[0], tiles[4], tiles[8], 'O'))
    {
      turnOnWinningPattern(LED_BUILTIN2, LED_BUILTIN10, LED_BUILTIN18);
    }
    else if(checkWinStatus(tiles[6], tiles[4], tiles[2], 'O'))
    {
      turnOnWinningPattern(LED_BUILTIN14, LED_BUILTIN10, LED_BUILTIN6);
    }
    else
    {
      checkIfTie();
    }

  }

  Serial.println(player2Input);
  Serial.println("-----------------------");
}

void loop() {
  client.loop();

  if(!running)
  {
    // Indicating that esp32 is waiting for a player
    digitalWrite(LED_BUILTIN2, HIGH);
    digitalWrite(LED_BUILTIN6, HIGH);
    digitalWrite(LED_BUILTIN10, HIGH);
    digitalWrite(LED_BUILTIN14, HIGH);
    digitalWrite(LED_BUILTIN18, HIGH);
  }

  handleInput();

  player2Input = "";
}

void handleInput()
{
    if (irrecv.decode(&results)) {          // Waiting for decoding
      handleControl(results.value);
      irrecv.resume();                      // Release the IRremote. Receive the next value
    }
}

void handleControl(unsigned long value) {
  // Handle the commands
  switch (value) {
    case 0xFFA25D:              // Receive the Power Button, exit game
      endGame();
      client.publish(topic, "Q1");
      break;
  }

  if(running && ourTurn)
  {

    switch (value) {
      case 0xFF30CF:              // Receive the number '1'
        checkTile(LED_BUILTIN1, 1);
        break;
      case 0xFF18E7:              // Receive the number '2'
        checkTile(LED_BUILTIN3, 2);
        break;
      case 0xFF7A85:              // Receive the number '3'
        checkTile(LED_BUILTIN5, 3);
        break;
      case 0xFF10EF:              // Receive the number '4'
        checkTile(LED_BUILTIN7, 4);
        break;
      case 0xFF38C7:              // Receive the number '5'
        checkTile(LED_BUILTIN9, 5);
        break;
      case 0xFF5AA5:              // Receive the number '6'
        checkTile(LED_BUILTIN11, 6);
        break;
      case 0xFF42BD:              // Receive the number '7'
        checkTile(LED_BUILTIN13, 7);
        break;
      case 0xFF4AB5:              // Receive the number '8'
        checkTile(LED_BUILTIN15, 8);
        break;
      case 0xFF52AD:              // Receive the number '9'
        checkTile(LED_BUILTIN17, 9);
        break;
    }
  }
}

void checkTile(int led, int tileNum)
{
  if(tiles[tileNum-1] != '_')
  {
    Serial.println("INVALID, TILE ALREADY OCCUPIED");
  }
  else
  {
    digitalWrite(led, HIGH);
    ourTurn = false;
    switch(tileNum)
    {
      case 1:
        client.publish(topic, "11");
        break;
      case 2:
        client.publish(topic, "21");
        break;
      case 3:
        client.publish(topic, "31");
        break;
      case 4:
        client.publish(topic, "41");
        break;
      case 5:
        client.publish(topic, "51");
        break;
      case 6:
        client.publish(topic, "61");
        break;
      case 7:
        client.publish(topic, "71");
        break;
      case 8:
        client.publish(topic, "81");
        break;
      case 9:
        client.publish(topic, "91");
        break;
    }

    tiles[tileNum-1] = 'X';

    // Player 1 Wins
    if(checkWinStatus(tiles[0], tiles[1], tiles[2], 'X'))
    {
      client.publish(topic, "W1");
      turnOnWinningPattern(LED_BUILTIN1, LED_BUILTIN3, LED_BUILTIN5);
    }
    else if (checkWinStatus(tiles[3], tiles[4], tiles[5], 'X'))
    {
      client.publish(topic, "W1");
      turnOnWinningPattern(LED_BUILTIN7, LED_BUILTIN9, LED_BUILTIN11);
    }
    else if (checkWinStatus(tiles[6], tiles[7], tiles[8], 'X'))
    {
      client.publish(topic, "W1");
      turnOnWinningPattern(LED_BUILTIN13, LED_BUILTIN15, LED_BUILTIN17);
    }
    else if (checkWinStatus(tiles[0], tiles[3], tiles[6], 'X'))
    {
      client.publish(topic, "W1");
      turnOnWinningPattern(LED_BUILTIN1, LED_BUILTIN7, LED_BUILTIN13);
    }
    else if (checkWinStatus(tiles[1], tiles[4], tiles[7], 'X'))
    {
      client.publish(topic, "W1");
      turnOnWinningPattern(LED_BUILTIN3, LED_BUILTIN9, LED_BUILTIN15);
    }
    else if (checkWinStatus(tiles[2], tiles[5], tiles[8], 'X'))
    {
      client.publish(topic, "W1");
      turnOnWinningPattern(LED_BUILTIN5, LED_BUILTIN11, LED_BUILTIN17);
    }
    else if (checkWinStatus(tiles[0], tiles[4], tiles[8], 'X'))
    {
      client.publish(topic, "W1");
      turnOnWinningPattern(LED_BUILTIN1, LED_BUILTIN9, LED_BUILTIN17);
    }
    else if (checkWinStatus(tiles[6], tiles[4], tiles[2], 'X'))
    {
      client.publish(topic, "W1");
      turnOnWinningPattern(LED_BUILTIN13, LED_BUILTIN9, LED_BUILTIN5);
    }
    else
    {
      checkIfTie();
    }
  }
}

bool checkWinStatus(char t1, char t2, char t3, char symbol)
{
    if(t1 != symbol || t2 != symbol || t3 != symbol)
    {
        return false;
    }

    if(t1 == t2 && t1 == t3 && t2 == t3)
    {
        printf("huh");
        return true;
    }
}

void endGame()
{
  for(int i = 0; i < 9; i++)
  {
    tiles[i] = '_';
  }
  running = false;
  ourTurn = true;
  turnOffAll();
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
            endGame();
        }
    }

}

void turnOnWinningPattern(int led1, int led2, int led3)
{
  delay(1000);
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    delay(1000);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    delay(1000);
  }
  endGame();
}

void turnOffAll()
{
    digitalWrite(LED_BUILTIN1, LOW);
    digitalWrite(LED_BUILTIN2, LOW);
    digitalWrite(LED_BUILTIN3, LOW);
    digitalWrite(LED_BUILTIN4, LOW);
    digitalWrite(LED_BUILTIN5, LOW);
    digitalWrite(LED_BUILTIN6, LOW);
    digitalWrite(LED_BUILTIN7, LOW);
    digitalWrite(LED_BUILTIN8, LOW);
    digitalWrite(LED_BUILTIN9, LOW);
    digitalWrite(LED_BUILTIN10, LOW);
    digitalWrite(LED_BUILTIN11, LOW);
    digitalWrite(LED_BUILTIN12, LOW);
    digitalWrite(LED_BUILTIN13, LOW);
    digitalWrite(LED_BUILTIN14, LOW);
    digitalWrite(LED_BUILTIN15, LOW);
    digitalWrite(LED_BUILTIN16, LOW);
    digitalWrite(LED_BUILTIN17, LOW);
    digitalWrite(LED_BUILTIN18, LOW);
}