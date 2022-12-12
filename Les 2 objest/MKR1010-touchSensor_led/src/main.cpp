#include <Arduino.h>
#include "mqtt.hpp"


//  Uncoment to use IDO open network
/*const char ssid[] = "IDO";
const char pass[] = "99Bidules!";*/

#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#endif

/*const char ssid[] = "HerveStella";
const char pass[] = "Maman123";*/
const char ssid[] = "Galaxy";
const char pass[] = "12345678";

//const char BROKER[] = "test.mosquitto.org";
//const char BROKER[] = "192.168.1.98";
const char BROKER[]         = "192.168.2.32";
const int port              = 1883;
const int LED               = 5;
const int TOUCH_SENSOR      = 4;
const long interval         = 5000;

unsigned long previousMillis = 0;
unsigned long currentMillis  = 0;
unsigned long touchVal       = 0;
unsigned long ledVal         = 0;

void setup() {
  
  pinMode(LED, OUTPUT);      
  pinMode(TOUCH_SENSOR, INPUT);      

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Tenter de se connecter à WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("Vous êtes connecté au réseau");
  Serial.println();

  Serial.print("Tenter de se connecter au courtier MQTT: ");
  Serial.println(BROKER);

  if (!mqttClient.connect(BROKER, port)) {
    Serial.print("La connexion MQTT a échoué! Code d'erreur =");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("Vous êtes connecté au broker MQTT!");
  Serial.println();
}

void loop() {
// Appel Poll () régulièrement pour permettre à la bibliothèque d'envoyer MQTT garder à aliments qui
  // évite d'être déconnecté par le courtier
  mqttClient.poll();
  currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
// Enregistrer la dernière fois qu'un message a été envoyé
    previousMillis = currentMillis;
    touchVal = digitalRead(TOUCH_SENSOR);
    ledVal = digitalRead(LED);
    if (touchVal == HIGH)                             
      digitalWrite(LED, HIGH); 
    else
      digitalWrite(LED, LOW);
    appendTimestamps(0);
    appendPayload("Touch", touchVal);
    appendPayload("Lumiere", ledVal);
    sendPayload();
  }
}