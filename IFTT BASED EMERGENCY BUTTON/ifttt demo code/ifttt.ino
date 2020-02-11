#include <ESP8266WiFi.h>

const char* ssid = "puru";
const char* password = "1234567890";
const char* host = "maker.ifttt.com";
const char* apiKey = "AKd82lpYhUUjZqcnY1qTV";

int pin = 2;

volatile int flag = false;
const char* PIN_state = "not pressed";

void PINStatus() {
            PIN_state = "SOS pressed";  
             flag = true;
}


void setup() {
    Serial.begin(115200);

    Serial.println("SOS button integration with IFTTT");
    delay(100);
    pinMode(pin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(pin), PINStatus, FALLING);

    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());  
}

void loop() {  

          
          Serial.print("connecting to ");
          Serial.println(host);
          
          WiFiClient client;
          const int httpPort = 80;
          if (!client.connect(host, httpPort)) {
            Serial.println("connection failed");
            return;
          }
    
          String url = "/trigger/callme/with/key/";
          url += apiKey;
          
          Serial.print("Requesting URL: ");
          Serial.println(url);
          client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" + 
                       "Content-Type: application/x-www-form-urlencoded\r\n" + 
                       "Content-Length: 13\r\n\r\n" +
                       "value1=" + PIN_state + "\r\n");
          flag = false;
      
      delay(30000);
      
}
