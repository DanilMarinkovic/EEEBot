000000000000000000000000000000000000000000000000//********************************************************//
//*  University of Nottingham                            *//
//*  Department of Electrical and Electronic Engineering *//
//*  UoN EEEBot 2023                                     *//
//*                                                      *//
//*  ESP32 MQTT Example Code                             *//
//*                                                      *//
//*  Nat Dacombe                                         *//
//********************************************************//

// the following code is modified from https://randomnerdtutorials.com by Rui Santos

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


// replace the next variables with your SSID/Password combination
const char* ssid = "dolphinc20";
const char* password = "Dolphin17";                

// add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.2.1";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int trigPin = 19;    // Trigger
int echoPin = 23;    // Echo
long duration, distance;

//uncomment the following lines if you're using SPI
/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5
*/

// LED Pin
const int ledPin = 4;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  //status = bme.begin();  
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // we start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageDist;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageDist += (char)message[i];
  }
  Serial.println();

  // feel free to add more if statements to control more GPIOs with MQTT

  // if a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageDist == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH)
      ;
    }
    else if(messageDist == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

  void reconnect() {
    // loop until we're reconnected
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // attempt to connect
      if (client.connect("ESP8266Client")) {
        Serial.println("connected");
        // subscribe
        client.subscribe("esp32/output");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // wait 5 seconds before retrying
        delay(5000);
      }
    }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 250) {
    lastMsg = now;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2)/29.1;
   /* if (distance < 10)
    digitalWrite(ledPin, HIGH);   
    else
    digitalWrite(ledPin, LOW); 
    */

    
    // convert the value to a char array
    char distString[8];
    dtostrf(distance, 1, 2, distString);
    Serial.print("Distance: ");
    Serial.println(distance);
    client.publish("esp32/distance", distString);

  }
}
