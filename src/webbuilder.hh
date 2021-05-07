#include <string>
#include <SPIFFS.h>
#include <ArduinoJson.h>


namespace webbuilder{
    // Takes the config.html from the SPIFFS and injects the available firmwares as RadioButtons
    String getConfigPage(){
        File file = SPIFFS.open("/config.html", "r");
        StaticJsonDocument<1024> doc;
        deserializeJson(doc, config::getFirmwareList());
        JsonObject obj = doc.as<JsonObject>();
        JsonArray firmwares = obj["firmwares"];
        String htmlOptions = "";
        for (int i = 0; i < firmwares.size(); i++) {
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
        return htmlPage;
    }

}