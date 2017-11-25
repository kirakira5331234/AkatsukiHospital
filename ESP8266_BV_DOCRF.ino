//Skip to content
//This repository
//Search
//Pull requests
//Issues
//Marketplace
//Explore
// @AnTa95
// Sign out
// Watch 2
//  Star 4  Fork 4 DeligenceTechnologies/Attendance-System-using-ESP8266-Wi-Fi-with-MySQL
// Code  Issues 1  Pull requests 0  Projects 0  Wiki  Insights
//Branch: master Find file Copy pathAttendance-System-using-ESP8266-Wi-Fi-with-MySQL/nodemcu_rfid.ino
//c273173  on Jul 10
//@pawan-deligence pawan-deligence Update nodemcu_rfid.ino
//2 contributors @deligence @pawan-deligence
//RawBlameHistory     
//163 lines (144 sloc)  3.92 KB

#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <MFRC522.h>

/* wiring the MFRC522 to ESP8266 (ESP-12)
RST     = GPIO0 D3
SDA(SS) = GPIO15 D8
MOSI    = GPIO13 D7
MISO    = GPIO12 D6
SCK     = GPIO14 D5
GND     = GND
3.3V    = 3.3V
*/

#define RST_PIN  0  // RST-PIN für RC522 - RFID - SPI - Modul GPIO0 
#define SS_PIN  15  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO15 

const char* ssid ="ID wifi";     // change according to your Network - cannot be longer than 32 characters!
const char* password  ="your wifi password" ; // change according to your Network
char msg[50];
String UID1 = " 54 24 0A 18";
String UID2 = " 55 E3 39 C4";
//String stringOne;
//UID1 = String("54 24 0a 18");
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  
  Serial.begin(115200);    // Initialize serial communications
  delay(10);
  //Serial.println(F("Booting...."));
  pinMode(16,OUTPUT);
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  
  if (WiFi.status() != WL_CONNECTED) {
    //WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
  }
  
    while ((WiFi.status() != WL_CONNECTED)) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
//  server.begin();
  Serial.println("Server started");
  Serial.print(WiFi.localIP());
  delay(1000);
  Serial.println("connecting...");
  
  Serial.println(F("Ready!"));
  Serial.println(F("======================================================")); 
  Serial.println(F("Scan for Card and print UID:"));

}


void loop() { 
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
  
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  
  String postStr;
  for(int i=0;i<mfrc522.uid.size;i++)
  {
    postStr += mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
    postStr += String(mfrc522.uid.uidByte[i],HEX);
    postStr.toUpperCase();
  }
  
  if(postStr==UID1)
  {
    digitalWrite (16,HIGH);
    Serial.println("Tên: NaSa");
    Serial.println("MSSV: 987411");
  
    delay(2000);
    digitalWrite (16,LOW);
// }
   }
  else   
  { 
    if(postStr==UID2)
    {
      digitalWrite (16,HIGH);
      Serial.println("Tên: Tạ Thoại Ân");
      Serial.println("MSSV: 14520011");
       
      delay(2000);
      digitalWrite (16,LOW);
  // }
     }
     else
      {
        Serial.println("Invalid");
        delay(3000);
      }
     
  }
}
// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
