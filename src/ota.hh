#include <ESP32httpUpdate.h>

namespace update {

    void shedule_update(String firmware_url){
        Serial.println("Scheduling update and reboot");
        config::writeUpdateConfig(firmware_url);
        ESP.restart();
    }

    void update_if_sheduled(){
        if(config::checkForUpdateConfig()){
            Serial.println("Found update config, starting firmware update!");
            String firmware_url = config::getUpdateConfig();
            config::deleteUpdateConfig();

            t_httpUpdate_return ret = ESPhttpUpdate.update(firmware_url);
            switch(ret) {
                case HTTP_UPDATE_FAILED:
                    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                    break;

                case HTTP_UPDATE_NO_UPDATES:
                    Serial.println("HTTP_UPDATE_NO_UPDATES");
                    break;

                case HTTP_UPDATE_OK:
                    Serial.println("HTTP_UPDATE_OK");
                    break;
            }
        }
        
    }
}