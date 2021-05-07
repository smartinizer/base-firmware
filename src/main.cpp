#include <iostream>
#include <SPIFFS.h>
#include <config.hh>
#include <connection.hh>
#include <ota.hh>
#include <webserver.hh>


Connection* connectionHandler;
WebServer* webserver;


void setup() {
    Serial.begin(115200);
    if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
    }

    connectionHandler = new Connection();
  
    using namespace std;
    String ssid, pw;
    tie(ssid, pw) = config::getWifiCredentialsfromwpaconf();
    if(ssid!=""){
      connectionHandler->wifi_sta_setup(ssid.c_str(), pw.c_str());
    }else{
      Serial.println("Cant find config");
      connectionHandler->wifi_ap_setup();
    }
    update::update_if_sheduled();
    config::downloadFirmwareList("https://smartinizer.devzero.cloud/firmwares.json");
    webserver = new WebServer();
}


void loop() {
  
}
