#include "lnpay.h"

// Busca lista de NFTS de um contrato na rede Polygon via PolygonScanner API
String consultaWallet(WiFiClient WiFic, String walletkey, String accessKey) {
  
  // Fazer a requisição GET para obter os tokens ERC-721 transacionados
  HTTPClient http;
  // https://api.lnpay.co/v1/wallet/wakr_Bg3drZJcpy0pOcAqTJftHUm/transactions?fields=num_satoshis,created_at,type,message,passThru

  String url = "https://api.lnpay.co/v1/wallet/" + walletkey + "/transactions?fields=num_satoshis,created_at,type,message,passThru";                                
  //http.setAuthorization();
  http.begin( WiFic, url ); // Iniciar a conexão HTTP
  http.addHeader("X-Api-Key", accessKey);

  int httpResponseCode = http.GET(); // Enviar a requisição GET

  if (httpResponseCode == 200) {
    String payload = http.getString(); // Obter a resposta da requisição

    Serial.println(payload);
    return payload.c_str();

  } else {
    Serial.print("Erro na requisição, código de resposta: ");
    Serial.println(httpResponseCode);
    return "Erro";
  }
  http.end(); // Fechar a conexão HTTP
}

DynamicJsonDocument buscaGotas(String walletAddress, int startBlock, int endBlock) {
  
  // Fazer a requisição GET para obter os NFTs recebidos na carteira
  HTTPClient http;
  String url = "https://api.polygonscan.com/api?module=account&action=tokennfttx&address=" + walletAddress + "&startblock=" + startBlock + "&endblock=" + endBlock + "&page=1&offset=6&sort=desc";
  //http.begin(WiFi, url); // Iniciar a conexão HTTP

  int httpResponseCode = http.GET(); // Enviar a requisição GET

  if (httpResponseCode == 200) {

   /*  StaticJsonDocument<200> filter;
    filter["status"] = true;
    filter["result"][0]["blockNumber"] = true;
    filter["result"][0]["tokenID"] = true;
    filter["result"][0]["contractAddress"] = true;
    //filter["result"][0]["tokenSymbol"] = "GOTAS"; */

    StaticJsonDocument<96> filter;
    filter["status"] = true;
    JsonObject filter_result_0 = filter["result"].createNestedObject();
    filter_result_0["blockNumber"] = true;
    filter_result_0["tokenID"] = true;
    filter_result_0["contractAddress"] = true;

    // Realizar o parse do JSON
    DynamicJsonDocument doc(1536);
    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    
   /*  Serial.print("Free heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.print(" / ");
    Serial.print(ESP.getHeapSize());
    Serial.print(" Max Aloc: ");
    Serial.println(ESP.getMaxAllocHeap()); */

    // Imprime JSON
    //serializeJsonPretty(doc, Serial);

    if (error) {
        Serial.print("Erro ao fazer o parse do JSON: ");
        Serial.println(error.c_str());
        StaticJsonDocument<48> erro;
        erro["status"] = "0";
        return erro;
    }

    return doc;

  } else {
    Serial.print("Erro na requisição, código de resposta: ");
    Serial.println(httpResponseCode);
    StaticJsonDocument<48> erro;
    erro["status"] = "0";
    return erro;
  }
  http.end(); // Fechar a conexão HTTP
}

DynamicJsonDocument buscaImgGotas(const char* ipfsAddr) {
  
  // Fazer a requisição GET para obter os NFTs recebidos na carteira
  HTTPClient http;
  //http.begin(WiFi, ipfsAddr); // Iniciar a conexão HTTP
  int httpResponseCode = http.GET(); // Enviar a requisição GET

  if (httpResponseCode == 200) {
    StaticJsonDocument<64> filter;
    filter["name"] = true;
    filter["description"] = true;
    filter["image"] = true;
    filter["gotaid"] = true;
    
    // Realizar o parse do JSON
    DynamicJsonDocument doc(768);
    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    
    // Imprime JSON
    //serializeJsonPretty(doc, Serial);

    if (error) {
        Serial.print("Erro ao fazer o parse do JSON: ");
        Serial.println(error.c_str());
        StaticJsonDocument<48> erro;
        erro["status"] = "0";
        return erro;
    }
    http.end();
    return doc; // ["image"].as<String>();

  } else {
    Serial.print("Erro na requisição, código de resposta: ");
    Serial.println(httpResponseCode);
    StaticJsonDocument<48> erro;
    erro["status"] = "0";
    http.end();
    return erro;
  }
  http.end(); // Fechar a conexão HTTP
}
