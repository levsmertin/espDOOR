#include <ESP8266WiFi.h>
#include <SPI.h>
#include "MFRC522.h"

const char* ssid     = "Keenetic-8750";
const char* password = "19lolo55";
int pin = 16;

const char* host = "192.168.1.41";
const char* streamId   = "....................";
const char* privateKey = "....................";

#define RST_PIN  5  // RST-PIN für RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN  4  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO4 

MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup() {
   Serial.begin(9600);
   delay(250);
   SPI.begin();           // Init SPI bus
   mfrc522.PCD_Init();

   pinMode(pin, OUTPUT);
   digitalWrite(pin, HIGH);  

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(5000);
  ++value;


  Serial.print("connecting to ");
  Serial.println(host);

  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 5555;
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("connection failed");
	  Serial.println(analogRead(0));
    return;
  }

  String str; 
  while (str != "poki")
  {
    delay(1000); 
    unsigned long idKey, idKeyTemp;
    String txtKey = "";
    while (Serial.available() == 0 && txtKey == "") 
    {
      if (mfrc522.PICC_IsNewCardPresent()) 
      {
        delay(50);
        if (mfrc522.PICC_ReadCardSerial())
           {
            delay(50);
            //dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
            for (byte i = 0; i < mfrc522.uid.size; i++)
            {
              idKeyTemp = mfrc522.uid.uidByte[i];
              idKey = idKey*256+idKeyTemp;
            }
            txtKey = "1::" + String(idKey);
            //Serial.print(txtKey);
            //delay(500);
           }
        //return;
      }
    }

  String input_string = "";
  if (Serial.available() || txtKey != "")
  {
    if (txtKey != "")
    {
       client.println(txtKey);
       Serial.print(txtKey);
       txtKey = "";
    }
    else
    {
      delay(100);
	    while (Serial.available() > 0) 
	    {  
		  char c = Serial.read();
		  input_string += c;
	    }
	    client.println(input_string);
      Serial.print(input_string);
	    input_string = "";
    }
  }
     
    
    //client.print("Hello");             
    int timeout = millis() + 5000;
    while (client.available() == 0) 
    {
      if (timeout - millis() < 0) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    
	String str_in="";
	if (client.available())
	{
      delay(100);
	  //Serial.println(client.available());
	  while (client.available() > 0) 
	  {  
		char c = client.read();
		str_in += c;
	  }
	  if (str_in == "Allow")
      {
        Serial.println(str_in);
        Serial.println("Open");
        digitalWrite(pin, LOW); 
        delay(500);                      
        digitalWrite(pin, HIGH);       
      }
      else
      {
        Serial.println(str_in);
        Serial.println("access violation");
      }
	}
  }

  client.stop();
  Serial.println();
  Serial.println("closing connection");
}

void dump_byte_array(byte *buffer, byte bufferSize) 
{
  Serial.print("1::");
  for (byte i = 0; i < bufferSize; i++) 
  {
    Serial.print(buffer[i] < 0x10 ? "0" : "");
    Serial.print(buffer[i], HEX);
  }
}

