#include <WiFi.h>
#include <WebServer.h>
#include "SPIFFS.h"

/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

void setup() {
    Serial.begin(115200);

    if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
    }

    if (wifi_setup()==true){
        server.on("/", handle_OnConnect);
        server.on("/html", handle_OnHtml );

        server.begin();
        Serial.println("HTTP server started");
    }else{
        exit(-1);
    }
}

void loop() {

    server.handleClient();

}


String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}


void handle_OnConnect() {
  server.send(200, "text/html", SendHTML()); 
}

void handle_OnHtml(){
    server.send(200, "text/html", SPIFFS.open("/index.html", "r").readString());
}


bool wifi_setup(){
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.persistent(false);
    bool setup_status = WiFi.softAP(ssid, password);
    bool config_status = WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);
    
    if (setup_status==true && config_status==true){
        Serial.println("Wifi setup complete!!");
    }else
    {
        Serial.println("Wifi setup failed!!");
    }
    return setup_status && config_status;
}

