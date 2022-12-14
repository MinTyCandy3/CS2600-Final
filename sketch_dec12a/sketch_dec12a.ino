#include <WiFi.h>
#include <PubSubClient.h>

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

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

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

String player2Input;

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
  Serial.println(player2Input);
  Serial.println("-----------------------");
}

void loop() {
  client.loop();

  if (irrecv.decode(&results)) {          // Waiting for decoding
    handleControl(results.value);
    irrecv.resume();                      // Release the IRremote. Receive the next value
  }

  player2Input = "";
}

void handleControl(unsigned long value) {
  // Handle the commands
  switch (value) {
    case 0xFFA25D:              // Receive the Power Button, exit game
      Serial.println("Pressed the Power Button");
      digitalWrite(LED_BUILTIN1, LOW);
      digitalWrite(LED_BUILTIN3, LOW);
      digitalWrite(LED_BUILTIN5, LOW);
      digitalWrite(LED_BUILTIN7, LOW);
      digitalWrite(LED_BUILTIN9, LOW);
      digitalWrite(LED_BUILTIN11, LOW);
      digitalWrite(LED_BUILTIN13, LOW);
      digitalWrite(LED_BUILTIN15, LOW);
      digitalWrite(LED_BUILTIN17, LOW);
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