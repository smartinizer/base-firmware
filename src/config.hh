#include <SPIFFS.h>
#include <tuple>
#include <HTTPClient.h>


namespace config{

    void writeWifiCredentials(String ssid, String pw) {
        File wpa = SPIFFS.open("/wpa.conf", "w");
        if (!wpa){
            Serial.println("Konnte wpa.conf nicht schreiben");
        }
        wpa.print(ssid +","+pw);
        wpa.close(); 
    }

    void deleteWifiCredentials(){
        File wpa = SPIFFS.open("/wpa.conf", "w");
        if (!wpa){
            Serial.println("Konnte wpa.conf nicht schreiben");
        }else{
            SPIFFS.remove("/wpa.conf");
        }

    }

    
    void writeUpdateConfig(String firmware_url){
        File config = SPIFFS.open("/update.conf", "w");
        if (!config){
            Serial.println("Konnte update.conf nicht schreiben");
        }else{
            config.print(firmware_url);
            config.close();
        }
    }

    boolean checkForUpdateConfig(){
        return SPIFFS.exists("/update.conf");
    }

    String getUpdateConfig(){
        File config = SPIFFS.open("/update.conf", "r");
        String update_config = "";
        if (!config){
            Serial.println("Konnte update.conf nicht lesen");
        }else{
            update_config = config.readString();
        }
        return update_config;
    }

    void deleteUpdateConfig(){
        File config = SPIFFS.open("/update.conf", "w");
        if (!config){
            Serial.println("Konnte update.conf nicht öffnen");
        }else{
            config.close();
            SPIFFS.remove("/update.conf");
        }
    }


    std::tuple<String, String> getWifiCredentialsfromwpaconf() {
        File wpar = SPIFFS.open("/wpa.conf", "r");
        if (!wpar){
            Serial.println("Konnte wpa.conf nicht einlesen");
        }
        if(wpar.available()){
            String ssid = wpar.readStringUntil(',');
            String pw = wpar.readStringUntil('\n');
            wpar.close();
            return std::make_tuple(ssid, pw);
        }
        
        wpar.close();
        return std::make_tuple("","");
    }

    void downloadFirmwareList(String url){
        File file = SPIFFS.open("/downloads/firmwares.json", "w");
        if(!file){
            Serial.println("Konnte /downloads/firmwares.json nicht schreiben");
            return;
        }

        HTTPClient http;
        Serial.print("[HTTP] begin...\n");

        http.begin(url); //HTTP
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                file.print(http.getString());
                Serial.println("Wrote Firmware-List to SPIFFS");
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        file.close();
        http.end();
    }

    String getFirmwareList(){
        File file = SPIFFS.open("/downloads/firmwares.json", "r");
        String firmware_list = "";
        if (!file){
            Serial.println("Konnte update.conf nicht lesen");
        }else{
            firmware_list = file.readString();
        }
        return firmware_list;
    }
}
