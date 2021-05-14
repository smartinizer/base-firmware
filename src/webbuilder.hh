#include <string>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>


namespace webbuilder{

    JsonArray getFirmwares(){
        StaticJsonDocument<2014> doc;
        deserializeJson(doc, config::getFirmwareList());
        JsonObject obj = doc.as<JsonObject>();
        return obj["firmwares"];
    }


    // Takes the config.html from the SPIFFS and injects the available firmwares as RadioButtons
    String getConfigPage(){
        File file = SPIFFS.open("/config.html", "r");
        Serial.println("Opened config.html");
        StaticJsonDocument<2014> doc;
        deserializeJson(doc, config::getFirmwareList());
        JsonObject obj = doc.as<JsonObject>();
        JsonArray firmwares = obj["firmwares"];
        Serial.println("Loaded Firmwares");
        String htmlOptions = "";
        for (int i = 0; i < firmwares.size(); i++) {
            Serial.println("Loop Runde " + i);
            String htmlOption = "<input type=\"radio\" id=\"$ID\" name=\"firmware\" value=\"$NAME\" $CHECKED><label for=\"$NAME\"><b>$NAME</b> - $DESCRIPTION</label>";
            htmlOption.replace("$ID", String(i));
            String name = firmwares[i]["name"];
            htmlOption.replace("$NAME", name);
            String description = firmwares[i]["description"];
            htmlOption.replace("$DESCRIPTION", description);
            if(i==0){
                htmlOption.replace("$CHECKED","checked");
                htmlOptions = htmlOption;
            }else{
                htmlOption.replace("$CHECKED","");
                htmlOptions = htmlOptions + "<br>" + htmlOption;
            }
        }
        String htmlPage = file.readString();
        htmlPage.replace("$OPTIONS",htmlOptions);
        Serial.println("End of getConfigPage");
        return htmlPage;
    }

    String getFirmwareConfigPage(String firmwareName){
        HTTPClient http;
        File file = SPIFFS.open("/config-firmware.html", "r");
        StaticJsonDocument<2014> doc;
        deserializeJson(doc, config::getFirmwareList());
        JsonObject obj = doc.as<JsonObject>();
        JsonArray firmwares = obj["firmwares"];
        // Looking for Config
        String configUrl = "";
        for(int i = 0; i < firmwares.size(); i++){
            Serial.println("Compare: " + firmwares[i]["name"].as<String>() + " = " + firmwareName);
            if(firmwares[i]["name"] == firmwareName){
                configUrl = firmwares[i]["url"].as<String>();
                Serial.println("Found ConfigUrl for " + firmwareName + " at " + configUrl);
                break;
            }
        }
        
        Serial.println("Firmware Config URL: " + configUrl);
        String configJson = config::getFirmwareConfig(configUrl);
        String pageHTML = file.readString();
        pageHTML.replace("$CONFIG", configJson);
        file.close();
        return pageHTML;
    }



}