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
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "esp32/test";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// TICTACTOE Variables
String player2Input;
bool running = false;

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
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  // publish and subscribe
  client.publish(topic, "Hi EMQX I'm ESP32 ^^");
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

  switch(input)
  {
    case 'y':
      running = true;
      break;
    case 'q':
    case 'Q':
      turnOffAll();
      break;
    case '1':
    digitalWrite(LED_BUILTIN2, HIGH);
      break;
    case '2':
    digitalWrite(LED_BUILTIN4, HIGH);
      break;
    case '3':
    digitalWrite(LED_BUILTIN6, HIGH);
      break;
    case '4':
    digitalWrite(LED_BUILTIN8, HIGH);
      break;
    case '5':
    digitalWrite(LED_BUILTIN10, HIGH);
      break;
    case '6':
    digitalWrite(LED_BUILTIN12, HIGH);
      break;
    case '7':
    digitalWrite(LED_BUILTIN14, HIGH);
      break;
    case '8':
    digitalWrite(LED_BUILTIN16, HIGH);
      break;
    case '9':
    digitalWrite(LED_BUILTIN18, HIGH);
      break;
      
  }

  Serial.println(player2Input);
  Serial.println("-----------------------");
}

void loop() {
  client.loop();

  if(!running)
  {
    // Indicating that esp32 is waiting for a player
    tripleLight(LED_BUILTIN1, LED_BUILTIN7, LED_BUILTIN13);
    tripleLight(LED_BUILTIN2, LED_BUILTIN8, LED_BUILTIN14);
    tripleLight(LED_BUILTIN3, LED_BUILTIN9, LED_BUILTIN15);
    tripleLight(LED_BUILTIN4, LED_BUILTIN10, LED_BUILTIN16);
    tripleLight(LED_BUILTIN5, LED_BUILTIN11, LED_BUILTIN17);
    tripleLight(LED_BUILTIN6, LED_BUILTIN12, LED_BUILTIN18);
    tripleLight(LED_BUILTIN5, LED_BUILTIN11, LED_BUILTIN17);
    tripleLight(LED_BUILTIN4, LED_BUILTIN10, LED_BUILTIN16);
    tripleLight(LED_BUILTIN3, LED_BUILTIN9, LED_BUILTIN15);
    tripleLight(LED_BUILTIN2, LED_BUILTIN8, LED_BUILTIN14);
  }

  handleInput();

  player2Input = "";
}

void tripleLight(int led1, int led2, int led3)
{
  if(!running)
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    delay(200);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  }
  else
  {
    turnOffAll();
    handleInput();
  }
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
  if(running)
  {
    switch (value) {
      case 0xFFA25D:              // Receive the Power Button, exit game
        Serial.println("Pressed the Power Button");
        turnOffAll();
        break;
    }

    switch (value) {
      case 0xFF30CF:              // Receive the number '1'
        digitalWrite(LED_BUILTIN1, HIGH);
        Serial.println("Pressed 1");
        break;
      case 0xFF18E7:              // Receive the number '2'
        digitalWrite(LED_BUILTIN3, HIGH);
        Serial.println("Pressed 2");
        break;
      case 0xFF7A85:              // Receive the number '3'
        digitalWrite(LED_BUILTIN5, HIGH);
        Serial.println("Pressed 3");
        break;
      case 0xFF10EF:              // Receive the number '4'
        digitalWrite(LED_BUILTIN7, HIGH);
        Serial.println("Pressed 4");
        break;
      case 0xFF38C7:              // Receive the number '2'
        digitalWrite(LED_BUILTIN9, HIGH);
        Serial.println("Pressed 5");
        break;
      case 0xFF5AA5:              // Receive the number '3'
        digitalWrite(LED_BUILTIN11, HIGH);
        Serial.println("Pressed 6");
        break;
      case 0xFF42BD:              // Receive the number '1'
        digitalWrite(LED_BUILTIN13, HIGH);
        Serial.println("Pressed 7");
        break;
      case 0xFF4AB5:              // Receive the number '2'
        digitalWrite(LED_BUILTIN15, HIGH);
        Serial.println("Pressed 8");
        break;
      case 0xFF52AD:              // Receive the number '3'
        digitalWrite(LED_BUILTIN17, HIGH);
        Serial.println("Pressed 9");
        break;
    }
  }
}

void startGame()
{
  turnOffAll();
  running = true;
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