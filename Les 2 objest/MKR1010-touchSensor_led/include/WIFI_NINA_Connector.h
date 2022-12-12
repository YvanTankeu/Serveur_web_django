/*
  Titre      : WIFI_NINA_Connector.h
  Auteur     : André Roussel
  Date       : 03/11/2021
  Description: Fonctions nécessaire pour branché un Arduino MKR1000 a un réseau WIFI
  Droits     : Reproduction permise pour usage pédagogique
  Version    : 0.0.2
*/

// mkr1010
// nano iot 33


#include "Secrets.h"
#include <SPI.h>
#include <wifiNINA.h>
#include "wifiStatus.h"



const char SSID[] = WIFI_SECRET_SSID; // WiFI ssid provenant du fichier Secrets.h
const char USER[] = WIFI_SECRET_USER; // Nom d'usage WIFI si nécessaire provenant aussi du fichier Secrets.h
const char PASS[] = WIFI_SECRET_PASS; // WiFI mot de passe provenant du fichier Secrets.h

int status = WL_IDLE_STATUS;          // Statut de la radio WIFI



WiFiClient ClientWIFI;                //Objets représantant un client se branchant a un réseau WIFI


/*
Fonction de connectivité a un réseau WIFI
*/

void wifiConnect()
{
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // Boucle de branchement au réseau WIFI.  Essaie de branchement a tout les 10 secondes jusqu'a réussite
  while (status != WL_CONNECTED) 
  {
    Serial.print("Tentative de branchement au réseau: ");
    Serial.println(SSID);

    // Vérification, si on as un nom d'usager, alors WPA2 Enterprise, sinon, WPA classique
    // strcmp -> si les deux chaines sont identique, strcmp retourne 0

    if( strcmp(USER,"") != 0) 
    {
      status = WiFi.beginEnterprise(SSID, USER, PASS);
      Serial.println(WiFi.reasonCode());
      printWifiData();
    }
    else{
      status = WiFi.begin(SSID, PASS);
      Serial.println("WPA2");
      printWifiData();
    }
                   
      
    Serial.println(status);

    
    delay(2000);  // Délai de 10 secondes entre les tentatives de connexion
  }

  //Sortie de la boucle, alors branché au réseau SSID
  Serial.print("Maintenant branché au réseau WIFI ");
  Serial.println(SSID);

  //Impression de l'information de connexion
  printCurrentNet();
  printWifiData();

}
