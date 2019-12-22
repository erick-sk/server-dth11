#include <DHTesp.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>


//Credenciales del WiFi
const char* ssid = "MORENO";
const char* password = "43699760";

WiFiServer server(80);
DHTesp dht;

void setup()
{
  Serial.begin(115200); //Inicio el puerto serie 
  delay(10);   
  Serial.println();
  
  //Serial.println("Estado\tHumedad (%)\tTemperatura (C)\t");
  
  dht.setup(4, DHTesp::DHT11); 

  // Conecto con la red WiFi en modo estación (STA)
  Serial.println();
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
 
  // Inicializo el servidor
  server.begin();
  Serial.println("Servidor iniciado");
 
  // Muestro la IP local asignada. URL del servidor
  
  Serial.print("Usa esta URL para conectar al servidor: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  float h = dht.getHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.getTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  if (req.indexOf("/data") != -1){
    client.flush();
    client.println("HTTP/1.1 200 OK");           // This tells the browser that the request to provide data was accepted
    client.println("Content-Type: text/html");
    client.println("Connection: close");         // Will close the connection at the end of data transmission.
    client.println();                            // You need to include this blank line - it tells the browser that it has reached the end of the Server reponse header.
    
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head></head><body><h1>ESP8266 - Temperature and Humidity</h1><h3>NO HAY DATA</h3>");
    
  }

  else {
    client.flush();
    client.println("HTTP/1.1 200 OK");           // This tells the browser that the request to provide data was accepted
    client.println("Content-Type: text/html");
    client.println("Connection: close");           // The data is coming from an Arduino Web Server (this line can be omitted)
    client.println();                            // You need to include this blank line - it tells the browser that it has reached the end of the Server reponse header.
                          // This is tha starting bracket of the JSON data
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head></head><body><h1>ESP8266 - Temperature and Humidity</h1><h3>Temperature in Celsius: ");
    client.println(t);
    client.println("*C");
    client.println("</h3><h3>Humidity: ");
    client.println(h);
    client.println("%</h3><h3>");
    client.println("</body></html>");                   
                       // This is the final bracket of the JSON data
  }
 
    delay(1);
    Serial.println("Client disonnected");


  delay(dht.getMinimumSamplingPeriod());

    
  float humidity = dht.getHumidity(); /* Obtener valor de Humedad */
  float temperature = dht.getTemperature(); /* Obtener valor de Temperatura */

  Serial.println("Estado\tHumedad (%)\tTemperatura (C)\t");
  Serial.print(dht.getStatusString());  
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println();
  delay(2000);
 
}

  
  
  

   
 
