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
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <MySQL_Encrypt_Sha1.h>
#include <MySQL_Packet.h>
#include<SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <MFRC522.h>
#include <time.h>
#include <stdlib.h>
/* wiring the MFRC522 to ESP8266 (ESP-12)
RST     = GPIO0 D3
SDA(SS) = GPIO15 D8
MOSI    = GPIO13 D7
MISO    = GPIO12 D6
SCK     = GPIO14 D5
GND     = GND
3.3V    = 3.3V
*/


const uint16_t mqtt_port = 11113; //Port của CloudMQTT
#define RST_PIN  0  // RST-PIN für RC522 - RFID - SPI - Modul GPIO0 
#define SS_PIN  15  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO15 

const char* ssid ="Hackathon 2017";     // change according to your Network - cannot be longer than 32 characters!
const char* pass  ="www.eiu.edu.vn" ; // change according to your Network
char *msg;
String UID1 = " 54 24 0A 18";
String UID2 = " 55 E3 39 C4";
//String stringOne;
//UID1 = String("54 24 0a 18");

int timezone = 7;
int countPerson =0;
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClient client;
MySQL_Connection conn((Client *)&client);
//char INSERT_SQL[] = "INSERT INTO officeto_plants.TBL_READINGS(ID_PLANT, AIR_HUMIDITY, AIR_TEMPERATURE, SOIL_MOISTURE_1) VALUES (%d, NULL, NULL, %d)";
char INSERT_SQL[] = "INSERT INTO akat.tam(STT,UID,Ngay) VALUES (%d , '%s' , '%s' )";
//char INSERT_SQL[] = "INSERT INTO officeto_plants.TBL_READINGS(ID_PLANT, AIR_HUMIDITY, AIR_TEMPERATURE, SOIL_MOISTURE_1, SOIL_MOISTURE_2) VALUES (1, NULL, NULL, %d, %d)";
char query[128];


IPAddress server_addr(172,16,3,17);          // MySQL server IP
//
//IPAddress server_addr(192,168,43,235);          // MySQL server IP
char user[] = "root";           // MySQL user
char password[] = "akatsuki";       // MySQL password

void setup() {
  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  //  lcd.begin(16,2);
  lcd.init();

  // Turn on the backlight.
  lcd.backlight();

   // Move the cursor characters to the right and
  // zero characters down (line 1).
  lcd.setCursor(5, 0);

  // Print HELLO to the screen, starting at 5,0.
  lcd.print("Akatsuki");

  // Move the cursor to the next line and print
  // WORLD.
  lcd.setCursor(5, 1);      
  lcd.print("RFID");
  
  Serial.begin(115200);    // Initialize serial communications
  delay(10);
  //Serial.println(F("Booting...."));
  pinMode(16,OUTPUT);
  digitalWrite (16,HIGH);
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  
  if (WiFi.status() != WL_CONNECTED) {
    //WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
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

  configTime(timezone * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println("Connecting to database");

  while (conn.connect(server_addr, 3306, user, password) != true) { 
    delay(200);
    Serial.print ( "." );
  }

  Serial.println("");
  Serial.println("Connected to SQL Server!");  
  
  Serial.println(F("Ready!"));
  Serial.println(F("======================================================")); 
  Serial.println(F("Scan for Card and print UID:"));
  digitalWrite (16,LOW);
}


void loop() { 
  time_t now =time(nullptr);
  struct tm * timeinfo;
  time(&now);
  timeinfo = localtime(&now);  
  if(timeinfo->tm_hour==00&&timeinfo->tm_min ==0)
  {
    countPerson=0;
  }
  // Look for new cards

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
  Serial.println(ctime(&now));
  
  String postStr;
  for(int i=0;i<mfrc522.uid.size;i++)
  {
    postStr += mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
    postStr += String(mfrc522.uid.uidByte[i],HEX);
    postStr.toUpperCase();
  }
  msg=ctime(&now);
  
    countPerson++;

    digitalWrite (16,HIGH);
    sprintf(query,INSERT_SQL,countPerson , postStr.c_str(), msg);
    Serial.println("Recording data.");
    Serial.println(query);
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    
    cur_mem->execute(query);
    
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("STT:");
    lcd.setCursor(11, 0);
    lcd.print(countPerson);
    lcd.setCursor(0, 1);
    lcd.print(ctime(&now));
    delete cur_mem;
//    delay(2000);
    digitalWrite (16,LOW);
//   }
//  else   
//  { 
//    if(postStr==UID2)
//    {
//      digitalWrite (16,HIGH);
//      lcd.clear();
//      lcd.setCursor(2, 0);
//      lcd.print("Ta Thoai An");
//      lcd.setCursor(2, 1);
//      lcd.print("14520011");
//      Serial.println("Tên: Tạ Thoại Ân");
//      Serial.println("MSSV: 14520011");
//    
//    // Sau mỗi 2s sẽ thực hiện publish dòng hello world lên MQTT broker
//  //  long now = millis();
//  ////  if (now - lastMsg > 2000) {
//  ////    lastMsg = now;
//  ////    ++value;
//       
//      delay(2000);
//      digitalWrite (16,LOW);
//  // }
//     }
//     else
//      {
//        Serial.println("Invalid");
//        delay(3000);
//      }
//     
//  }
}
// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
