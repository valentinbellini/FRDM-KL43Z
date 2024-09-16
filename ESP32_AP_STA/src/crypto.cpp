#include "crypto.h"

// Función genérica para obtener el precio de un token
float getTokenPrice(String token) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.coingecko.com/api/v3/simple/price?ids=" + token + "&vs_currencies=usd";
    http.begin(url);
    
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String response = http.getString();
      
      // Parsear el JSON
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, response);
      if (!error) {
        // Sin error: return token price
        float tokenPrice = doc[token]["usd"];
        return tokenPrice;
      } else {
        Serial.println("Error al parsear JSON");
        return -1.0; // Error
      }
    } else {
      Serial.print("Error en la solicitud HTTP: ");
      Serial.println(httpResponseCode);
      return -1.0; // Error
    }
    http.end();
  } else {
    Serial.println("Error en la conexión WiFi.");
    return -1.0; // Error
  }
}
