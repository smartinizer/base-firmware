#include <SPIFFS.h>
#include <tuple>


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

}
