#include <PageBuilder.h>
#include <PageStream.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h> 
#include <ArduinoJson.h> 
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include "wManager.h"
//#include "lnpay.h"

extern char wsApiURL[80];
extern char wsServer[80];
extern char wsApiKey[37];
extern char wsWalletKey[37];

extern unsigned long relay1Sats;
extern unsigned long relay2Sats;
extern unsigned char relay1pulses;
extern unsigned char relay2pulses;
extern unsigned char relay1timeS;
extern unsigned char relay2timeS;
extern unsigned char relay1Pin;
extern unsigned char relay2Pin;

#ifdef PIN_BUTTON_1
  #include  <OneButton.h>
  OneButton button1(PIN_BUTTON_1);
#endif

/////////////////////////////////
/////////// LEDS ////////////////
/////////////////////////////////
#include <Adafruit_NeoPixel.h>
#define PIN 5
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

// String payloadStr;
/* String password;
String serverFull;
String lnpayServer;
String lnpayWSApiURL;
String ssid;
String wifiPassword;
String deviceId;
String highPin;
String timePin;
String lnurl; */

//Change the pin logic
String pinFlip = "true";

/* struct KeyValue {
  String key;
  String value;
}; */

bool paid = false;
bool connerr = true;
bool triggerUSB = false;

bool showconnect = true;

typedef struct{
  String message;
  char id[28];
  int num_satoshis;
  unsigned long time; 
}last_message;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000);
unsigned long start = millis();
const char* ntpServer = "pool.ntp.org";

//////////////////HELPERS///////////////////

String consultaWallet(String walletkey, String accessKey) {
   
  BearSSL::WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;  
  // ex.: http://api.lnpay.co/v1/wallet/wakr_Bg3drZJcpy0pOcAqTJftHUm/transactions?fields=num_satoshis,created_at,type,message,passThru

  const String url = wsServer;
  const String path = wsApiURL + walletkey + "/transactions?page=1&per-page=5&fields=id,user_label,num_satoshis,created_at,passThru";                               
  
  Serial.println("https://"+url+path);
  http.begin(client, url, 443, path); // Iniciar a conexão HTTPS
  http.addHeader("X-Api-Key", accessKey);

  int httpResponseCode = http.GET(); // Enviar a requisição GET

  if (httpResponseCode == 200) {
    String payload = http.getString(); // Obter a resposta da requisição    
    StaticJsonDocument<200> filter;
    filter[0]["id"] = true;
    filter[0]["created_at"] = true; 
    filter[0]["num_satoshis"] = true;
    filter[0]["user_label"] = true;   
    filter[0]["passThru"]["type"] = true;
    filter[0]["passThru"]["message"] = true;

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, payload, DeserializationOption::Filter(filter));
    // serializeJsonPretty(doc, Serial);
    if (error) {
        Serial.print("Erro ao fazer o parse do JSON: ");
        Serial.println(error.c_str());
        //return;
    } else {
        // Imprime JSON
        serializeJsonPretty(doc, Serial);
    }
    //Serial.println(payload);
    http.end();
    return payload.c_str();

  } else {
    Serial.print("Erro na requisição, código de resposta: ");
    Serial.println(httpResponseCode);
    Serial.println(http.errorToString(httpResponseCode).c_str());
    http.end();
    return "Erro";
  }
  Serial.println("Encerrou");
  http.end(); // Fechar a conexão HTTP
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void onOff()
{ 
  pinMode (relay1Pin, OUTPUT);
  if(pinFlip == "true"){
    digitalWrite(relay1Pin, LOW);
    delay(relay1timeS);
    digitalWrite(relay1Pin, HIGH); 
    //delay(2000);
  }
  else{
    digitalWrite(relay1Pin, HIGH);
    delay(relay1timeS);
    digitalWrite(relay1Pin, LOW); 
    //delay(2000);
  }
}

#ifdef AWTRIX
void notificaAwtrix(String mensagem) {
  // Criar o JSON payload
  StaticJsonDocument<256> jsonPayload;
  jsonPayload["text"] = mensagem;
  jsonPayload["color"] = "#FFA500";
  jsonPayload["repeat"] = 3;
  jsonPayload["wakeup"] = true;
  jsonPayload["duration"] = 20;
  jsonPayload["icon"] = "630";
  jsonPayload["pushIcon"] = 2;
  
  // Serializar o JSON para uma string
  String payload;
  serializeJson(jsonPayload, payload);
  //Serial.println(payload);

  // Fazer a requisição POST
  WiFiClient wifi;
  HTTPClient http;
  http.begin(wifi, "http://192.168.1.76/api/notify");
  http.addHeader("Content-Type", "application/json");  
  
  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.print("Resposta do Awtrix: ");
    Serial.println(response);
  } else {
    Serial.print("Erro na requisição, código de resposta: ");
    Serial.println(httpResponseCode);
    Serial.printf("[HTTP] POST... falhou, erro: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end();
}
#endif

void efeitosLeds()
{
    colorWipe(strip.Color(255, 0, 0), 50); // Red
    colorWipe(strip.Color(0, 255, 0), 50); // Green
    colorWipe(strip.Color(0, 0, 255), 50); // Blue
    theaterChase(strip.Color(127, 127, 0), 20); // Mix
    theaterChase(strip.Color(0, 255, 0), 50); // Green
    theaterChase(strip.Color(0, 255, 0), 100); // Green
}

void desligaLeds()
{
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i,0);
    strip.show();
    delay(10);
  }
  strip.setBrightness(50);
  strip.show();
}

void setup()
{
 Serial.begin(115200);
 init_WifiManager();

 #ifdef AWTRIX
  // Finalizou configuração - anuncia AwTrix
  notificaAwtrix("SWITCH! INICIADO!");
 #endif
}

void loop() {
  if(WiFi.status()== WL_CONNECTED){
    //Serial.println(consultaWallet("wal_7ihvT2Bnkq6UYi","sak_Tsg8JM8k5ImYIDhD3DciiFUPPM56"));
    consultaWallet("wakr_Bg3drZJcpy0pOcAqTJftHUm","pak_FrrLdOMTbQhSz64kllVvs3FoL57rrPYG");
    delay(5*1000);
    Serial.println(ESP.getFreeHeap());
  }
}
