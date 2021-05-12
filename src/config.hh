#include <SPIFFS.h>
#include <tuple>
#include <HTTPClient.h>
#include <ArduinoJson.h>


namespace config
{

    const char* ca_cert =
        "-----BEGIN CERTIFICATE-----\n"
        "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\n"
        "nLRbwHOoq7hHwg==\n"
        "-----END CERTIFICATE-----\n";


    void writeWifiCredentials(String ssid, String pw)
    {
        File wpa = SPIFFS.open("/wpa.conf", "w");
        if (!wpa)
        {
            Serial.println("Konnte wpa.conf nicht schreiben");
        }
        wpa.print(ssid + "," + pw);
        wpa.close();
    }

    void deleteWifiCredentials()
    {
        File wpa = SPIFFS.open("/wpa.conf", "w");
        if (!wpa)
        {
            Serial.println("Konnte wpa.conf nicht schreiben");
        }
        else
        {
            SPIFFS.remove("/wpa.conf");
        }
    }

    void writeUpdateConfig(String firmware_url)
    {
        File config = SPIFFS.open("/update.conf", "w");
        if (!config)
        {
            Serial.println("Konnte update.conf nicht schreiben");
        }
        else
        {
            config.print(firmware_url);
            config.close();
        }
    }

    boolean checkForUpdateConfig()
    {
        return SPIFFS.exists("/update.conf");
    }

    String getUpdateConfig()
    {
        File config = SPIFFS.open("/update.conf", "r");
        String update_config = "";
        if (!config)
        {
            Serial.println("Konnte update.conf nicht lesen");
        }
        else
        {
            update_config = config.readString();
        }
        return update_config;
    }

    void deleteUpdateConfig()
    {
        File config = SPIFFS.open("/update.conf", "w");
        if (!config)
        {
            Serial.println("Konnte update.conf nicht öffnen");
        }
        else
        {
            config.close();
            SPIFFS.remove("/update.conf");
        }
    }

    std::tuple<String, String> getWifiCredentialsfromwpaconf()
    {
        File wpar = SPIFFS.open("/wpa.conf", "r");
        if (!wpar)
        {
            Serial.println("Konnte wpa.conf nicht einlesen");
        }
        if (wpar.available())
        {
            String ssid = wpar.readStringUntil(',');
            String pw = wpar.readStringUntil('\n');
            wpar.close();
            return std::make_tuple(ssid, pw);
        }

        wpar.close();
        return std::make_tuple("", "");
    }

    void downloadFirmwareList(String url)
    {
        File file = SPIFFS.open("/downloads/firmwares.json", "w");
        if (!file)
        {
            Serial.println("Konnte /downloads/firmwares.json nicht schreiben");
            return;
        }

        WiFiClientSecure *client = new WiFiClientSecure;
        if (client)
        {
            client->setCACert(ca_cert);
            {
                // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
                HTTPClient https;

                Serial.print("[HTTPS] begin...\n");
                if (https.begin(*client, "https://smartinizer.devzero.cloud/firmwares.json"))
                { // HTTPS
                    Serial.print("[HTTPS] GET...\n");
                    // start connection and send HTTP header
                    int httpCode = https.GET();

                    // httpCode will be negative on error
                    if (httpCode > 0)
                    {
                        // HTTP header has been send and Server response header has been handled
                        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

                        // file found at server
                        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
                        {
                            String payload = https.getString();
                            file.print(payload);
                            Serial.println("Wrote Firmware-List to SPIFFS");
                        }
                    }
                    else
                    {
                        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
                    }
                    https.end();
                }
                else
                {
                    Serial.printf("[HTTPS] Unable to connect\n");
                }

                // End extra scoping block
            }
            file.close();
            delete client;
        }
        else
        {
            Serial.println("Unable to create client");
        }
    }

    String getFirmwareList()
    {
        File file = SPIFFS.open("/downloads/firmwares.json", "r");
        String firmware_list = "";
        if (!file)
        {
            Serial.println("Konnte update.conf nicht lesen");
        }
        else
        {
            firmware_list = file.readString();
        }
        return firmware_list;
    }

    String getFirmwareConfig(String url){
        WiFiClientSecure *client = new WiFiClientSecure;
        String payload = "";
        if (client)
        {
            client->setCACert(ca_cert);
            {
                // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
                HTTPClient https;

                Serial.print("[HTTPS] begin...\n");
                if (https.begin(*client, url))
                { // HTTPS
                    Serial.print("[HTTPS] GET...\n");
                    // start connection and send HTTP header
                    int httpCode = https.GET();

                    // httpCode will be negative on error
                    if (httpCode > 0)
                    {
                        // HTTP header has been send and Server response header has been handled
                        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

                        // file found at server
                        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
                        {
                            payload = https.getString();
                        }
                    }
                    else
                    {
                        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
                    }
                    https.end();
                }
                else
                {
                    Serial.printf("[HTTPS] Unable to connect\n");
                }
                // End extra scoping block
            }
            delete client;
        }
        else
        {
            Serial.println("Unable to create client");
        }
        payload.replace("\n", "");
        return payload;
    }

    void writeFirmwareConfig(String firmwareConfig){
        File config = SPIFFS.open("/firmware-config.json", "w");
        if (!config)
        {
            Serial.println("Konnte firmware-config.json nicht schreiben");
        }
        else
        {
            config.print(firmwareConfig);
            config.close();
        }
    }

    String getConfigByKey(String key){
        File file = SPIFFS.open("/firmware-config.json", "r");
        StaticJsonDocument<2014> doc;
        if (!file)
        {
            Serial.println("Konnte firmware-config.json nicht lesen");
        }
        else
        {
            String content = file.readString();
            deserializeJson(doc, content);
            file.close();
        }
        JsonObject obj = doc.as<JsonObject>();
        return obj[key].as<String>();
    }
}
