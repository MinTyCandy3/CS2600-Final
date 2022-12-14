#include <WiFi.h>
#include <PubSubClient.h>
#include <Keypad.h>

#define PIN_LED 2


// define the symbols on the buttons of the keypad
char keys[3][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
};

byte rowPins[3] = {14, 27, 26}; // connect to the row pinouts of the keypad
byte colPins[3] = {13, 21, 22};   // connect to the column pinouts of the keypad

// initialize an instance of class NewKeypad
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 3, 3);

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

  pinMode(PIN_LED, OUTPUT);

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

  // Get the character input
  char keyPressed = myKeypad.getKey();
  // If there is a character input, sent it to the serial port
  if (keyPressed) {
    Serial.println(keyPressed);
    client.publish(topic, "KEY PRESSED");
  }

  if(player2Input == "YES")
  {
    digitalWrite(PIN_LED, HIGH);
    player2Input = "";
  }

  if(player2Input == "NO")
  {
    digitalWrite(PIN_LED, LOW);
    player2Input = "";
  }

  player2Input = "";
}