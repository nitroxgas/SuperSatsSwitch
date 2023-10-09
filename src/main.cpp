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
//bool pinFlip = true;

char fails = 0;

typedef struct{
  String message;
  String id;
  unsigned long num_satoshis;
  unsigned long time = 0; 
}last_message;

last_message lastMessage;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org"); //, 0, 60000);
unsigned long start; 
//unsigned long lastMessage;
//const char* ntpServer = "pool.ntp.org";

//////////////////HELPERS///////////////////
#ifdef AWTRIX
extern char awtrixServer[80];

void notificaAwtrix(String mensagem, int repeat) {
  // Criar o JSON payload
  StaticJsonDocument<256> jsonPayload;
  jsonPayload["text"] = mensagem;
  jsonPayload["color"] = "#FFA500";
  jsonPayload["repeat"] = repeat;
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
  String awserver = "http://"+String(awtrixServer)+"/api/notify";
  http.begin(wifi, awserver);
  http.addHeader("Content-Type", "application/json");  
  
  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.print("Awtrix: ");
    Serial.println(response);
  } else {
    Serial.print("Error, response code: ");
    Serial.println(httpResponseCode);
    Serial.printf("[HTTP] POST... Failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end();
}
#endif

unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}

String urldecode(String str)
{    
    String encodedString="";
    char c;
    char code0;
    char code1;
    for (unsigned int i =0; i < str.length(); i++){
        c=str.charAt(i);
      if (c == '+'){
        encodedString+=' ';  
      }else if (c == '%') {
        i++;
        code0=str.charAt(i);
        i++;
        code1=str.charAt(i);
        c = (h2int(code0) << 4) | h2int(code1);
        encodedString+=c;
      } else{
        
        encodedString+=c;  
      }
      
      yield();
    }
    
   return encodedString;
}

void onOff(char relayP, int timeP, bool pinFlip )
{ 
  pinMode (relayP, OUTPUT);
  if(pinFlip){
    digitalWrite(relayP, LOW);
    delay(timeP*1000);
    digitalWrite(relayP, HIGH);     
  }
  else {
    digitalWrite(relayP, HIGH);
    delay(timeP*1000);
    digitalWrite(relayP, LOW);     
  }
}

// Do something when a valid message is received
void doStuff(){
  #ifdef AWTRIX
    notificaAwtrix(lastMessage.message, 1);
  #endif

  if (lastMessage.num_satoshis>=relay1Sats){
    onOff(relay1Pin, relay1timeS, true);
  }
  if (lastMessage.num_satoshis>=relay2Sats){
   onOff(relay2Pin, relay2timeS, true);
  }
}

String consultaWallet(String walletkey, String accessKey) {
   
  BearSSL::WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;  
  // ex.: http://api.lnpay.co/v1/wallet/wakr_Bg3drZJcpy0pOcAqTJftHUm/transactions?fields=num_satoshis,created_at,type,message,passThru

  const String url = wsServer;
  const String path = wsApiURL + walletkey + "/transactions?page=1&per-page=5&fields=id,user_label,num_satoshis,created_at,passThru";                               
  
  Serial.println("https://"+url+path);
  //Serial.println(accessKey);
  
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
        for (char i=doc.size(); i>0; i--){        
          const JsonObject& mensagem = doc[i-1];                    
   //       Caso seja mais recente que a hora de inicialização do SuSaSw       
          unsigned long messCreated = mensagem["created_at"];          
          if (( messCreated > start ) && (messCreated > lastMessage.time))
            if ( (mensagem["user_label"].as<String>() == "supersats (via LNPAY.co)") || (mensagem["passThru"]["type"].as<String>() == "supersats")){ // If it's a SuperChat...                          
              // serializeJsonPretty(mensagem, Serial);
              Serial.printf("Start: %lu - Obj: %d - Created: %lu\n", start, i, mensagem["created_at"].as<long>());
              lastMessage.id = mensagem["id"].as<String>();
              lastMessage.time = mensagem["created_at"].as<long>();
              lastMessage.message = urldecode(mensagem["passThru"]["message"].as<String>());                            
              lastMessage.num_satoshis = mensagem["num_satoshis"].as<int>();
              Serial.println(lastMessage.message);
              doStuff();
            }
        }
    }
    //Serial.println(payload);
    fails=0;
    http.end();
    return payload.c_str();

  } else {
    Serial.print("Erro na requisição, código de resposta: ");
    Serial.println(httpResponseCode);
    Serial.println(http.errorToString(httpResponseCode).c_str());
    http.end();
    fails++;
    if (fails>5) ESP.restart();
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
 timeClient.begin();
 // Find if offset time is needed 
 //timeClient.setTimeOffset(3600 * -3);
 
 if(timeClient.update()) start = timeClient.getEpochTime(); 
 
 Serial.print("Inicio:");
 Serial.println(start);

 #ifdef AWTRIX
  // Finalizou configuração - anuncia AwTrix
  notificaAwtrix("SuperSats SWITCH! INICIADO!",2);
 #endif
}

unsigned long currentTime = 0;
unsigned long mTriggerUpdate = 0;

void loop() {
  if(WiFi.status()== WL_CONNECTED){          
    if(timeClient.update()){
       mTriggerUpdate = millis();
       currentTime = timeClient.getEpochTime(); 
       //Serial.print("Time:");
       //Serial.println(currentTime);
    }
    consultaWallet(wsWalletKey, wsApiKey);
    delay(5*1000);    
    //Serial.println(ESP.getFreeHeap());
  }
}
