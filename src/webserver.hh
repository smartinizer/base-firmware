#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <iostream>
#include <string>

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
            request->send(404, "text/plain", "Not found");
        }


class WebServer{

    public: WebServer(){
        // Send web page with input fields to client
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/index.html", String(), false);
        });
        server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/config.html", String(), false);
        });

        // Route to load style.css file
        server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/style.css", "text/css");
        });
        // Route to connect
        server.on("/connect", HTTP_POST, [](AsyncWebServerRequest *request){
            Serial.print("Connect pressed!!");
            String ssid = "";
            String pw = "";
            if(request->hasParam("ssid", true) && request->hasParam("pw", true)){
            ssid = request->getParam("ssid", true)->value();
            pw = request->getParam("pw", true)->value();
            }
            
            config::writeWifiCredentials(ssid,pw);
            request->redirect("/");
            ESP.restart();
        });
        // Install new Firmware
        server.on("/install", HTTP_POST, [](AsyncWebServerRequest *request){
            Serial.println("Starting new Firmware installation!!");
            server.end();
            update::shedule_update("https://smartinizer.devzero.cloud/firmware.bin");
        });
        server.onNotFound(notFound);

        server.begin();

        Serial.println("HTTP server started");
    }

};
