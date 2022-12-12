
/*
  Titre      : Projet web full stack Pour IoT AHT20
  Auteur     : Yvan Tankeu
  Date       : 12/12/2022
  Description: Persistance de donne et affiche sur une page web
  Version    : 0.0.1
*/
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_AHTX0.h>
#include "RTClib.h"
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
const char BROKER[] = "192.168.2.32";
const int port     = 1883;
const long interval = 5000;

unsigned long previousMillis = 0;
int count = 0;
String stringToSend, valHumidite, valTemperature, unixTime = "";

RTC_DS3231 rtc;
Adafruit_AHTX0 aht;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT BROKER: ");
  Serial.println(BROKER);

  if (!mqttClient.connect(BROKER, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT BROKER!");
  Serial.println();

  // Init de AHT20
  if (!aht.begin())
  {
    Serial.println("Impossible de trouver l'AHT ? Vérifiez le câblage");
    while (1)
      delay(10);
  }
  Serial.println(" AHT20 trouve");

  // Init du RTC
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      delay(10);
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
// Appel Poll () régulièrement pour permettre à la bibliothèque d'envoyer MQTT garder à aliments qui
  // évite d'être déconnecté par le courtier
  mqttClient.poll();
  DateTime now = rtc.now();

  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  valTemperature = temp.temperature;
  valHumidite = humidity.relative_humidity;
  unixTime = now.unixtime() + 10800;

  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
// Enregistrer la dernière fois qu'un message a été envoyé
    previousMillis = currentMillis;

    appendTimestamps(unixTime.toFloat());
    appendPayload("Temperature", valTemperature.toFloat());
    appendPayload("Humidite", valHumidite.toFloat());
    sendPayload();
  }
}