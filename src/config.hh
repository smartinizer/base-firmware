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
