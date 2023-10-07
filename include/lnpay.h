#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

String consultaWallet(WiFiClient WiFic, String walletkey, String accessKey);
DynamicJsonDocument buscaGotas(String walletAddress, int startBlock, int endBlock);
DynamicJsonDocument buscaImgGotas(const char* ipfsAddr);