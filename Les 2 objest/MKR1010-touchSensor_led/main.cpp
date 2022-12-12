#include <Arduino.h>
#include "Timer.h"

bool btn_Jaune;
bool btn_Rouge;

#include "WIFI_NINA_Connector.h"
#include "MQTTConnector.h"

const int relaiLED  = 8;
const int EN1_2     = 1;
const int P1_A      = 2;
const int P2_A      = 3;
const int MOTEUR    = 4;

void setup() {
  ClientMQTT.loop();

  pinMode(EN1_2, OUTPUT); // EN1_2 en mode output
  //pinMode(relaiLED, OUTPUT); // Relai en mode output
  pinMode(P1_A, OUTPUT); // 1A en mode output
  pinMode(P2_A, OUTPUT); // 2A en mode output

  wifiConnect(); //  Branchement au réseau WIFI
  MQTTConnect();
  //pinMode(MOTEUR, OUTPUT); // 2A en mode output
}

void loop() {
  ClientMQTT.loop();

 if (runEveryShort(2000))
 {
    if ((btn_Jaune == HIGH) && (btn_Rouge == LOW)) { // check if the input is HIGH
      Serial.println("Droit vers Gauche");
      Serial.print("Jaune : ");
      Serial.println(btn_Jaune);
      digitalWrite(EN1_2, HIGH);
      digitalWrite(P1_A, HIGH);
      digitalWrite(P2_A, LOW);
    }
    else if((btn_Jaune == LOW) && (btn_Rouge == HIGH)){
      
      Serial.print("Rouge : ");
      Serial.println(btn_Rouge);
      Serial.println("CHangemet de rotation");
      Serial.println("Gauche vers Droite");
      digitalWrite(EN1_2, HIGH);
      digitalWrite(P1_A, LOW);
      digitalWrite(P2_A, HIGH);
    } 
    else if (btn_Jaune == HIGH && btn_Rouge == HIGH)
    {
      Serial.println("Les butons ont ete presse moteur en arret");
      digitalWrite(EN1_2, LOW);
    }
    else if (btn_Jaune == LOW && btn_Rouge == LOW)
    {
      Serial.println("Pressez un boutton pour la direction du moteur");
      digitalWrite(EN1_2, LOW);
    }
    appendPayload("btn_Jaune", btn_Jaune);
    appendPayload("btn_Rouge", btn_Rouge);
    sendPayload();
 }
  
}
 