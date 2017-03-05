
#include <SoftwareSerial.h>
#include <SPI.h>
#include <WiFi.h>


char ssid[] = "Fran6q";     //  your network SSID (name) 
char pass[] = "6qboolintchar";    // your network password
//char ssid[] = "HackUPC-Wifi";     //  your network SSID (name) 
//char pass[] = "wgBsdYcV";    // your network password
int keyIndex = 0;                                // your network key Index number
int status = WL_IDLE_STATUS;                     // the Wifi radio's status
WiFiClient client;
float latitude, longitude;
char dato=' ';

const int buttonPin = 2;     // the number of the pushbutton pin
const int buzzer = 9; //buzzer to arduino pin 9
int numPut = 30;

SoftwareSerial serialgps(11,10);


bool isRiderRiding = true;
bool isConnected = false;
void setup() 
{
    Serial.begin(115200);
    serialgps.begin(9600);
    pins_init();
    //connect();
    // attempt to connect to Wifi network:
}

void connect(){
  if(!isConnected) {
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WEP network, SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
    
        // wait 10 seconds for connection:
        delay(10000);
    }
    // once you are connected :
    Serial.print("You're connected to the network");
    isConnected=true;
   } 
}

void loop() 
{
  if(isRiderRiding) {
    //readgpsCoordinates();
    //isRiderRiding = !riderHasFall();
    isRiderRiding=false;
    //if(!isRiderRiding) {
      Serial.println();
      Serial.println("el genet ha caigut");
      sendPost(numPut,"{'lat':25.5,'lon':64.43}");
      ++numPut;
    //}
  }
  else {
    //makeSound();
  }
}

void pins_init()
{
    //pinMode(CURRENT_SENSOR, INPUT);
    pinMode(buttonPin, INPUT);
    pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
}

void readgpsCoordinates(){
   while(serialgps.available()) 
    {
    dato=serialgps.read();
    Serial.print(dato);
    delay (50);
  }
  //Serial.println("atempting connection");
}

void sendPost(int numPutp,String temp){
    connect();
    Serial.println("connecting...");
    Serial.println(numPutp);
    Serial.println(temp);
    //192.168.0.55
    if (client.connect("https://rescueme-2beff.firebaseio.com",80)) {                                 
        Serial.println("Sending to Server: ");     
        /*PUT /users/juanjo/positions/16.json HTTP/1.1
Host: rescueme-2beff.firebaseio.com
Content-Type: application/json
Cache-Control: no-cache
Postman-Token: e92b8360-ea52-2467-8aaf-06197fc4c3cd

{
    "lat": 25.5,
    "lon": 64.43
}*/               
        client.print("PUT /users/juanjo/positions/");
        client.print(numPutp, DEC);
        client.println(".json HTTP/1.1");
        Serial.print("PUT /users/juanjo/positions/");
        Serial.print(numPutp), DEC;
        Serial.println(".json HTTP/1.1");          
        client.println("Host: rescueme-2beff.firebaseio.com");
        client.println("Content-Type: application/json");
        client.println("Cache-Control: no-cache");
        client.print("Content-Length: ");
        client.println(24);
        client.println();
        client.print(temp);
        client.println();          
        Serial.println("Host: rescueme-2beff.firebaseio.com");
        Serial.println("Content-Type: application/json");
        Serial.println("Cache-Control: no-cache");
        Serial.print("Content-Length: ");
        Serial.println(24);
        Serial.println();
        Serial.print(temp);
        Serial.println();  
        Serial.print(readPage());
        Serial.println();  
        Serial.println("has read");                                     
    }
    else {
        Serial.println("Cannot connect to Server");               
    }
    //client.stop();
}

void makeSound(){
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec
}

bool riderHasFall(){
  int buttonState = digitalRead(buttonPin);
  return buttonState != HIGH;
 }


char inString[32]; // string for incoming serial data
int stringPos = 0; // string index counter
boolean startRead = false; // is reading?

 
 String readPage(){
  //read the page, and capture & return everything between '<' and '>'

  stringPos = 0;
  memset( &inString, 0, 32 ); //clear inString memory

  while(client.available()) {
      char c = client.read();

      if (c == '<' ) { //'<' is our begining character
        startRead = true; //Ready to start reading the part 
      }else if(startRead){

        if(c != '>'){ //'>' is our ending character
          inString[stringPos] = c;
          stringPos ++;
        }else{
          //got what we need here! We can disconnect now
          startRead = false;
          client.stop();
          client.flush();
          Serial.println("disconnecting.");
          return inString;

        }

      }

  }

}
