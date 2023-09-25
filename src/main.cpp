#include <PageBuilder.h>
#include <PageStream.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h> 
#include <ArduinoJson.h> 
#include <ESP8266HTTPClient.h>
#include "wManager.h"

#ifdef PIN_BUTTON_1
  #include  <OneButton.h>
  OneButton button1(PIN_BUTTON_1);
#endif

#define PARAM_FILE "/elements.json"

/////////////////////////////////
/////////// LEDS ////////////////
/////////////////////////////////

#include <Adafruit_NeoPixel.h>

#define PIN 5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

const uint32_t WS_RECONNECT_INTERVAL = 10000;  // websocket reconnect interval (ms)
const uint32_t WS_HB_PING_TIME = 15000;          // ping server every WS_HB_PING_TIME ms (set to 0 to disable heartbeat)
const uint32_t WS_HB_PONG_WITHIN = 10000;        // expect pong from server within WS_HB_PONG_WITHIN ms
const uint32_t WS_HB_PONGS_MISSED = 3;             // consider connection disconnected if pong is not received WS_HB_PONGS_MISSED times

/////////////////////////////////
/////////////////////////////////
/////////////////////////////////

String payloadStr;
String password;
String serverFull;
String lnbitsServer;
String lnbitsWSApiURL;
String ssid;
String wifiPassword;
String deviceId;
String highPin;
String timePin;
String pinFlip = "true";
String lnurl;

bool paid = false;
bool connerr = true;
bool triggerUSB = false;

bool showconnect = true;

WebSocketsClient webSocket;

struct KeyValue {
  String key;
  String value;
};
//////////////////HELPERS///////////////////

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
  pinMode (highPin.toInt(), OUTPUT);
  if(pinFlip == "true"){
    digitalWrite(highPin.toInt(), LOW);
    delay(timePin.toInt());
    digitalWrite(highPin.toInt(), HIGH); 
    //delay(2000);
  }
  else{
    digitalWrite(highPin.toInt(), HIGH);
    delay(timePin.toInt());
    digitalWrite(highPin.toInt(), LOW); 
    //delay(2000);
  }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//////////////////WEBSOCKET///////////////////
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            //Serial.println(F("[WSc] Disconnected!"));
            Serial.printf("[WSc] Disconnected to url: %s\n",  payload);
            connerr = true;
            showconnect = true;
            break;
        case WStype_CONNECTED:
            {
            Serial.printf("[WSc] Connected to url: %s\n",  payload);
			      webSocket.sendTXT("Connected");
            connerr = false;
            #ifdef USELCD
              showqr = true;
            #endif  
            }
            break;
        case WStype_TEXT:
            payloadStr = (char*)payload;
            paid = true;
            break;
        case WStype_PING:
            // pong will be send automatically
            //Serial.println("[WSc] ping received");
            Serial.print("ping ");
            break;
        case WStype_PONG:
            // answer to a ping we send
            //Serial.println("[WSc] pong received");
            Serial.print("pong ");
            break;    
    		case WStype_ERROR:
        // answer to a ping we send
            Serial.println("[WSc] error received");
            break; 			
    		case WStype_FRAGMENT_TEXT_START:
    		case WStype_FRAGMENT_BIN_START:
    		case WStype_FRAGMENT:
        case WStype_BIN:
    		case WStype_FRAGMENT_FIN:
    			break;
    }
}

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
  Serial.println(F("\n\nBOOT!")); 

  // Ajusta leds
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
  // Iniciando configuração...
  theaterChase(strip.Color(127, 0, 0), 50); // Red 
  //turn off onboard led (usually gpio 2)
  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  #ifdef PIN_BUTTON_1
  button1.attachLongPressStart(reset_configuration);
  #endif
  
  init_WifiManager();

  // Serial.println(lnbitsServer + lnbitsWSApiURL + deviceId);
  webSocket.beginSSL(lnbitsServer.c_str(), 443, (lnbitsWSApiURL + deviceId).c_str());
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(WS_RECONNECT_INTERVAL);
  if (WS_HB_PING_TIME != 0) {
    /* Serial.print(F("Enabling WS heartbeat with ping time "));
    Serial.print(WS_HB_PING_TIME);
    Serial.print(F("ms, pong time "));
    Serial.print(WS_HB_PONG_WITHIN);
    Serial.print(F("ms, "));
    Serial.print(WS_HB_PONGS_MISSED);
    Serial.println(F(" missed pongs to reconnect.")); */
    webSocket.enableHeartbeat(WS_HB_PING_TIME, WS_HB_PONG_WITHIN, WS_HB_PONGS_MISSED);
  }
  // Finalizou configuração - anuncia AwTrix
  notificaAwtrix("SWITCH! INICIADO!");

  // Desliga leds depois de configurado o WS
  desligaLeds();
}

void loop() {
    
  webSocket.loop();
    
  if(paid){
    paid = false;
    Serial.println("Paid");
    Serial.println(payloadStr);
    highPin = getValue(payloadStr, '-', 0);
    Serial.println(highPin);
    timePin = getValue(payloadStr, '-', 1);
    Serial.println(timePin);    
    notificaAwtrix("PAGAMENTO RECEBIDO - SATOSHIS NA CONTA!");
    efeitosLeds();
    onOff();  
    desligaLeds();       
  }
}
