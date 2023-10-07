#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <LittleFS.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include "wManager.h"

// JSON configuration file
#define JSON_CONFIG_FILE "/config.json"

// Flag for saving data
bool shouldSaveConfig = false;

// Variables to hold data from custom textboxes
char wsApiURL[80] = "/v1/wallet/";
char wsServer[80] = "api.lnpay.co";
char wsApiKey[37] = "pak_FrrLdOMTbQhSz64kllVvs3FoL57rrPYG";// "pak_***";
char wsWalletKey[37] = "wakr_Bg3drZJcpy0pOcAqTJftHUm"; //"wakr_***";

unsigned long relay1Sats = 100;
unsigned long relay2Sats = 2000;
unsigned char relay1pulses = 1;
unsigned char relay2pulses = 0;
unsigned char relay1timeS = 5;
unsigned char relay2timeS = 15;
unsigned char relay1Pin = 5;
unsigned char relay2Pin = 15;

//WiFiClass WiFi;

// Define WiFiManager Object
WiFiManager wm;
 
void saveConfigFile()
// Save Config in JSON format
{
  Serial.println(F("Saving configuration..."));
  
  // Create a JSON document
  StaticJsonDocument<512> json;
  json["wsapikey"] = wsApiKey;
  json["wsapiurl"] = wsApiURL;
  json["wsserver"] = wsServer;
  json["wswalletKey"] = wsWalletKey;
  json["relay1sats"] = relay1Sats;
  json["relay1pulses"] = relay1pulses;
  json["relay1times"] = relay1timeS;
  json["relay1pin"] = relay1Pin;
  json["relay2sats"] = relay2Sats;
  json["relay2pulses"] = relay2pulses;
  json["relay2times"] = relay2timeS;
  json["relay2pin"] = relay2Pin;
  
  // Open config file
  File configFile = LittleFS.open(JSON_CONFIG_FILE, "w");
  if (!configFile)
  {
    // Error, file did not open
    Serial.println(F("failed to open config file for writing"));
  }

  // Serialize JSON data to write to file
  serializeJsonPretty(json, Serial);
  if (serializeJson(json, configFile) == 0)
  {
    // Error writing file
    Serial.println(F("Failed to write to file"));
  }
  // Close file
  configFile.close();
}

bool loadConfigFile()
// Load existing configuration file
{
  // Uncomment if we need to format filesystem
  // SPIFFS.format();

  // Read configuration from FS json
  Serial.println("Mounting File System...");

  // May need to make it begin(true) first time you are using SPIFFS
  if (LittleFS.begin() || LittleFS.begin())
  {
    Serial.println("mounted file system");
    if (LittleFS.exists(JSON_CONFIG_FILE))
    {
      // The file exists, reading and loading
      Serial.println("reading config file");
      File configFile = LittleFS.open(JSON_CONFIG_FILE, "r");
      if (configFile)
      {
        Serial.println(F("Opened configuration file"));
        StaticJsonDocument<512> json;
        DeserializationError error = deserializeJson(json, configFile);
        configFile.close();
        serializeJsonPretty(json, Serial);
        if (!error)
        {
          Serial.println("Parsing JSON");
          strcpy(wsApiKey, json["wsapikey"] | wsApiKey );
          strcpy(wsApiURL, json["wsapiurl"] | wsApiURL );
          strcpy(wsServer, json["wsserver"] | wsServer );
          strcpy(wsWalletKey,json["wswalletkey"] | wsWalletKey);          
          relay1Sats = json["relay1sats"].as<long>();
          relay1pulses = json["relay1pulses"].as<int>();
          relay1timeS = json["relay1times"].as<int>();
          relay1Pin = json["relay1pin"].as<int>();
          if (json.containsKey("relay2Sats")) {
            relay2Sats = json["relay2sats"].as<long>();
            relay2pulses = json["relay2pulses"].as<int>();
            relay2timeS = json["relay2times"].as<int>();
            relay2Pin = json["relay2pin"].as<int>();    
          }      
          //Serial.println("JSON Ok!");
          return true;
        }
        else
        {
          // Error loading JSON data
          Serial.println("Failed to load json config");
        }
      }
    }
  }
  else
  {
    // Error mounting file system
    Serial.println("Failed to mount FS");
  }
  return false;
}


void saveConfigCallback()
// Callback notifying us of the need to save configuration
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
  //wm.setConfigPortalBlocking(false);
}

void configModeCallback(WiFiManager *myWiFiManager)
// Called when config mode launched
{
  Serial.println("Entered Configuration Mode");

  Serial.print("Config SSID: ");
  Serial.println(myWiFiManager->getConfigPortalSSID());

  Serial.print("Config IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void init_WifiManager()
{
  // Change to true when testing to force configuration every time we run
  bool forceConfig = false;

  // Explicitly set WiFi mode
  WiFi.mode(WIFI_STA);

  bool spiffsSetup = loadConfigFile();
  if (!spiffsSetup)
  {
    Serial.println(F("Forcing config mode as there is no saved config"));
    forceConfig = true;    
  }
  
  Serial.println("Carregando aqui...");
  Serial.flush();
  
  // Reset settings (only for development)
  //wm.resetSettings();

  // Set config save notify callback
  wm.setSaveConfigCallback(saveConfigCallback);

  // Set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wm.setAPCallback(configModeCallback);
  //wm.setConfigPortalBlocking(false);

  wm.setConnectTimeout(50); // how long to try to connect for before continuing
  
  // Custom elements
  
  char convertedValueRelay[16];
  sprintf(convertedValueRelay, "%lu", relay1Sats); 
  // Text box (Number) - 3 characters maximum
  WiFiManagerParameter r1s_text_box_num("relay1sats", "Relay 1 Sats to activate", convertedValueRelay, 3);

  sprintf(convertedValueRelay, "%d", relay1pulses);   
  // Text box (Number) - 3 characters maximum
  WiFiManagerParameter r1p_text_box_num("relay1pulses", "Relay 1 pulses", convertedValueRelay, 3);

  sprintf(convertedValueRelay, "%d", relay1timeS);   
  // Text box (Number) - 4 characters maximum
  WiFiManagerParameter r1t_text_box_num("relay1times", "Relay 1 times to activate", convertedValueRelay, 3);

  sprintf(convertedValueRelay, "%d", relay1Pin);   
  // Text box (Number) - 3 characters maximum
  WiFiManagerParameter r1pin_text_box_num("relay1pin", "Relay 1 pin", convertedValueRelay, 3);

  WiFiManagerParameter wswallet_text_box("wswalletkey", "LNPay Wallet (war_***)", wsWalletKey, 80);

  WiFiManagerParameter wsapikey_text_box("wsapikey", "LNPay API Key", wsApiKey, 30);

  WiFiManagerParameter wsapiurl_text_box("wsapiurl", "LNPay URL", wsApiURL, 80);
  
  WiFiManagerParameter wsserver_text_box("wsserver", "LNPay Server", wsServer, 80);

  // Add all defined parameters  
  wm.addParameter(&wswallet_text_box);
  wm.addParameter(&wsapikey_text_box);
  wm.addParameter(&wsserver_text_box);  
  wm.addParameter(&wsapiurl_text_box);  
  wm.addParameter(&r1s_text_box_num);
  wm.addParameter(&r1p_text_box_num);
  wm.addParameter(&r1t_text_box_num);
  wm.addParameter(&r1pin_text_box_num);

  Serial.println(F("AllDone: "));
  if (forceConfig)
    // Run if we need a configuration
  {
    if (!wm.startConfigPortal("SuperSatsSwitch","MineYourCoins"))
    {
      Serial.println(F("failed to connect and hit timeout"));
            
      // Copy the string value    
      relay1Sats = atoi(r1s_text_box_num.getValue());
      Serial.print("Relay 1 Sats: ");
      Serial.println(relay1Sats);

      relay1pulses = atoi(r1p_text_box_num.getValue());
      Serial.print("Relay 1 Pulses: ");
      Serial.println(relay1pulses);

      relay1timeS = atoi(r1t_text_box_num.getValue());
      Serial.print("Relay 1 Times: ");
      Serial.println(relay1timeS);

      relay1Pin = atoi(r1pin_text_box_num.getValue());
      Serial.print("Relay 1 Pin: ");
      Serial.println(relay1Pin);

      strncpy(wsServer, wsserver_text_box.getValue(), sizeof(wsServer));
      Serial.print("LNPay Server: ");
      Serial.println(wsServer);

      strncpy(wsApiURL, wsapiurl_text_box.getValue(), sizeof(wsApiURL));
      Serial.print("LNPay API URL: ");
      Serial.println(wsApiURL);

      strncpy(wsWalletKey, wswallet_text_box.getValue(), sizeof(wsWalletKey));
      Serial.print("Wallet API Key: ");
      Serial.println(wsApiKey);

      strncpy(wsApiKey, wsapikey_text_box.getValue(), sizeof(wsApiKey));
      Serial.print("Wallet API Key: ");
      Serial.println(wsApiKey);

      saveConfigFile();
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }
  }
  else
  {
    //Tratamos de conectar con la configuración inicial ya almacenada
    //wm.setCaptivePortalEnable(false); // disable captive portal redirection
    if (!wm.autoConnect("SuperSatsSwitch","MineYourCoins"))
    {
      Serial.println(F("Failed to connect and hit timeout"));
    }
  }

  //Conectado a la red Wifi
  if(WiFi.status() == WL_CONNECTED){
    //tft.pushImage(0, 0, MinerWidth, MinerHeight, MinerScreen);
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Lets deal with the user config values
  
    // Copy the string value    
    relay1Sats = atoi(r1s_text_box_num.getValue());
    Serial.print("Relay 1 Sats: ");
    Serial.println(relay1Sats);

    relay1pulses = atoi(r1p_text_box_num.getValue());
    Serial.print("Relay 1 Pulses: ");
    Serial.println(relay1pulses);

    relay1timeS = atoi(r1t_text_box_num.getValue());
    Serial.print("Relay 1 Times: ");
    Serial.println(relay1timeS);

    relay1Pin = atoi(r1pin_text_box_num.getValue());
    Serial.print("Relay 1 Pin: ");
    Serial.println(relay1Pin);

    strncpy(wsServer, wsserver_text_box.getValue(), sizeof(wsServer));
    Serial.print("LNPay Server: ");
    Serial.println(wsServer);

    strncpy(wsApiURL, wsapiurl_text_box.getValue(), sizeof(wsApiURL));
    Serial.print("LNPay API URL: ");
    Serial.println(wsApiURL);

    strncpy(wsWalletKey, wswallet_text_box.getValue(), sizeof(wsWalletKey));
    Serial.print("Wallet API Key: ");
    Serial.println(wsApiKey);

    strncpy(wsApiKey, wsapikey_text_box.getValue(), sizeof(wsApiKey));
    Serial.print("Wallet API Key: ");
    Serial.println(wsApiKey);
  }
  
  // Save the custom parameters to FS
  if (shouldSaveConfig)
  {
    saveConfigFile();
  }
}

void reset_configurations() {
  Serial.println("Erasing Config, restarting");
  wm.resetSettings();
  LittleFS.remove(JSON_CONFIG_FILE); //Borramos fichero
  ESP.restart();
}


//----------------- MAIN PROCESS WIFI MANAGER --------------
int oldStatus = 0;

void wifiManagerProcess() {
  
  //wm.process(); // avoid delays() in loop when non-blocking and other long running code
  
  int newStatus = WiFi.status();
  if (newStatus != oldStatus) {
    if (newStatus == WL_CONNECTED) {
      Serial.println("CONNECTED - Current ip: " + WiFi.localIP().toString());
    } else {
      Serial.print("[Error] - current status: ");
      Serial.println(newStatus);
    }
    oldStatus = newStatus;
  }
}

void reset_configuration()
{
    Serial.println(F("Erasing Config, restarting"));
    wm.resetSettings();
    ESP.restart();
}
