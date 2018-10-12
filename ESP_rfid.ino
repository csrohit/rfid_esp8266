#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
const char* ssid = "OnePlus3";
const char* password = "node_mcu1";
String id;
const char* host = "http://192.168.43.236:3000/node";
#define SS_PIN D4
#define RST_PIN D3

MFRC522 mfrc522(SS_PIN, RST_PIN); 
void setup() {
Serial.begin(9600);
SPI.begin();
mfrc522.PCD_Init();
Serial.println("RFID reading UID");
  WiFi.mode(WIFI_OFF);        
  delay(1000);
  WiFi.mode(WIFI_STA);        
  Serial.printf("Connecting to %s",ssid);
  WiFi.begin(ssid, password);    
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
}

void loop() {
if ( mfrc522.PICC_IsNewCardPresent())
    {
        if ( mfrc522.PICC_ReadCardSerial())
        {
          String id="";
            WiFiClient client;
           Serial.print("Tag UID:");
           for (byte i = 0; i < mfrc522.uid.size; i++) {
                  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                  Serial.print(mfrc522.uid.uidByte[i], DEC);
                  id.concat(mfrc522.uid.uidByte[i]);
            }
            String data = "rollNo=1234&flag=";
            data.concat(id);
 int result = postData(host,data);
 if(result = 200)
 Serial.println("Data sent succesfully");
            Serial.println();
            Serial.print("String Id :-");
            Serial.println(id);
            Serial.println();
            mfrc522.PICC_HaltA();
        }
}
}
int postData(String url,String data){
  HTTPClient http; 
  http.begin(url);              
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(data); 
  http.end(); 
  return httpCode;  
}
