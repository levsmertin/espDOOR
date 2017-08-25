#include <ESP8266WiFi.h>

const char* ssid     = "Keenetic-8750";
const char* password = "19lolo55";
int pin = 16;

const char* host = "192.168.1.41";
const char* streamId   = "....................";
const char* privateKey = "....................";

void setup() {
  Serial.begin(9600);
  delay(10);

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
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
	//Serial.println(analogRead(0));
    return;
  }

  String str; 
  while (str != "poki")
  {
    delay(1000);
    while (Serial.available() == 0) {}

    char incomingBytes[10];
    char sendingBytes[10];
    
//    if (Serial.available() > 0)
//     {
//        Serial.readBytes(incomingBytes, 9);
//        int i = 0;
//        str = "";
//        while (incomingBytes[i] != '\r')
//        {
//          str+=incomingBytes[i];
//          i++;
//        }
//        client.println(incomingBytes);
//     }
  String input_string = "";
  if (Serial.available())
  {
      delay(100);
	  while (Serial.available() > 0) 
	  {  
		char c = Serial.read();
		input_string += c;
	  }
	  client.println(input_string);
	  input_string = "";
  }
     
    
    //client.print("Hello");             
    int timeout = millis() + 5000;
    while (client.available() == 0) {
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
	
    // while(client.available())
	// {
	  // Serial.print(client.available(), DEC);
      // String line = client.readStringUntil('\r');
      // if (line == "10")
      // {
        // Serial.println(line);
        // Serial.println("Open");
        // digitalWrite(pin, LOW); 
        // delay(1000);                      
        // digitalWrite(pin, HIGH);       
      // }
      // else
      // {
        // Serial.println(line);
        // Serial.println("access violation");
      // }
    // }
  }

  client.stop();
  Serial.println();
  Serial.println("closing connection");
}

