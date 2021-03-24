#include <WiFi.h>

IPAddress AP_IP(192,168,0,1);
IPAddress AP_SUBNET(255,255,255,0);

class Connection {
  private:
    bool connected = false;
    const char* AP_SSID = "Smartinizer";  
    const char* AP_PWD = "12345678"; 

  public: 
    Connection() {
    }

    bool wifi_ap_setup(){
        Serial.print("Setting soft-AP ... ");
        bool setup_status =WiFi.softAP(AP_SSID, AP_PWD);
        delay(2000);
    
        Serial.print("Setting soft-AP configuration ... ");
        Serial.println(WiFi.softAPConfig(AP_IP, AP_IP, AP_SUBNET) ? "Ready" : "Failed!");
        Serial.println(setup_status ? "Ready" : "Failed!");
        
        Serial.print("Soft-AP IP address = ");
        Serial.println(WiFi.softAPIP());

        return setup_status;
    }


    bool wifi_sta_setup( String ssid_input,String pw_input){

      WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
      WiFi.setHostname("smartinizer");
      delay(2000);
      WiFi.begin(ssid_input.c_str(), pw_input.c_str());

      Serial.print("Connecting...");
      int try_count = 0;
      wl_status_t status;

      do {
        status=WiFi.status();
        delay(1000);
        Serial.print(".");
        try_count++;
        if(try_count>=15){
          Serial.println("Cant connect...");
          config::deleteWifiCredentials();
          ESP.restart();
        }
      }while(status !=WL_CONNECTED);
  
      Serial.println();
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());

      return true;
    }


    boolean isConnected(){
      return connected;
    }   
    
};
