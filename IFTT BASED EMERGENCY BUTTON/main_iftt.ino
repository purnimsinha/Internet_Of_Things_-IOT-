#include <DoubleResetDetector.h>
#include <ESP8266WiFi.h>

const char* ssid = "puru";
const char* password = "1234567890";
const char* host = "maker.ifttt.com";
const char* apiKey = "bGlojuhBoTsJnlEHqjQ56Q";

int pin = 2;

volatile int flag = false;
const char* PIN_state = "not pressed";

void PINStatus() {
            PIN_state = "SOS pressed";  
             flag = true;
}






// Number of seconds after reset during which a 
// subseqent reset will be considered a double reset.
#define DRD_TIMEOUT 10

// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS 0

DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  
   Serial.begin(115200);
  Serial.println();
  Serial.println("DoubleResetDetector Example Program");
  Serial.println("-----------------------------------");
  if (drd.detectDoubleReset()) {
    Serial.println("Double Reset Detected");
    digitalWrite(LED_BUILTIN, LOW);

    
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




      Serial.print("connecting to ");
          Serial.println(host);
          
          WiFiClient client;
          const int httpPort = 80;
          if (!client.connect(host, httpPort)) {
            Serial.println("connection failed");
            return;
          }
    
          String url = "/trigger/call_me/with/key/";
          url += apiKey;
          
          Serial.print("Requesting URL: ");
          Serial.println(url);
          client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" + 
                       "Content-Type: application/x-www-form-urlencoded\r\n" + 
                       "Content-Length: 13\r\n\r\n" +
                       "value1=" + PIN_state + "\r\n");
         delay(30000);
      
  } else {
    Serial.println("No Double Reset Detected");
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void loop()
{
  drd.loop();
}
