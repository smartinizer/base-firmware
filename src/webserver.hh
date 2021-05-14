#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <iostream>
#include <string>
#include <webbuilder.hh>

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
        server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/wifi.html", String(), false);
        });
        server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(200,"text/html", webbuilder::getConfigPage());
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
        //configure-firmware
        server.on("/config-firmware", HTTP_GET, [](AsyncWebServerRequest *request){
            Serial.print("Connect pressed!!");
            AsyncWebParameter* p = request->getParam(0);
            String firmware = p->value();
            request->send(200,"text/html", webbuilder::getFirmwareConfigPage(firmware));
        });
        // Install new Firmware
        server.on("/install", HTTP_GET, [](AsyncWebServerRequest *request){
            AsyncWebParameter* p = request->getParam(0);
            String config = p->value();
            p = request->getParam(1);
            String firmwareUrl = p->value();
            Serial.println("Firmware Config: " + config);
            Serial.println("Firmware Url: " + firmwareUrl);
            request->send(SPIFFS, "/installing-firmware.html", String(), false);
            server.end();
            config::writeFirmwareConfig(config);
            update::shedule_update(firmwareUrl);
        });
        server.onNotFound(notFound);

        server.begin();

        Serial.println("HTTP server started");
    }

};
