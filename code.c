#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

 #include "DHT.h"  
 DHT dht2(14,DHT11);  
char ssid[] = "iot1";   // your network SSID (name) 
char pass[] = "12345678";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 1921601;
const char * myWriteAPIKey = "RK4QIOF5G5B1IMIG";
int count=0;
// Initialize our values
float temp = 0;
float hum = 0;
int gas = 0;
int noise = 0;
String myStatus = "";

void setup() {
  Serial.begin(115200);  // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  pinMode(5, OUTPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

 Serial.println("Temperature in C:");  
   Serial.println((dht2.readTemperature( )));  
   Serial.println("Humidity in C:");  
   Serial.println((dht2.readHumidity()));  


temp=dht2.readTemperature();
hum=dht2.readHumidity();
gas=!(digitalRead(12));
noise=!(digitalRead(13));
   Serial.print("GAS: ");  
   Serial.println((gas));
      Serial.print("NOISE: ");  
   Serial.println((noise));
   
   if(temp>30)
   {
     digitalWrite(5,HIGH);
   }
   else
   {
      digitalWrite(5,LOW);
   }
   count=count+1;
if(count==20)
{
  // set the fields with the values
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);
  ThingSpeak.setField(3, gas);
  ThingSpeak.setField(4, noise);


  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  count=0;
}

  delay(1000); // Wait 20 seconds to update the channel again
}