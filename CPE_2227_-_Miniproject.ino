#include <ESP8266WiFi.h>
 
const char* ssid = "`ARM";
const char* password = "armza489";

//ตัวรดน้ำต้นไม้

const int analogInPin = A0;
const int relay = D2;

int sensorValue = 0;        
int outputValue = 0;  

int UseSensor = 1;

int ledPin = D2;
WiFiServer server(80);
int clientok  = 1;

int value = LOW;
 
void setup() {
  Serial.begin(9600);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
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
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
     if (clientok != 0) {
        Serial.println("Client not available");
     }
     clientok =0;
     
  } 
   else
  { 
     clientok =1;
     Serial.println("Client Available");
     delay(100);
  }

  if (client.available()){
     Serial.println("client available");
     
     //Read the first line of the request
     
     String request = client.readStringUntil('\r');
     Serial.println(request);
     client.flush();
 
     // Match the request
 
     
     if (request.indexOf("/RELAY=ON") != -1) {
        digitalWrite(ledPin, HIGH);
        value = HIGH;
        Serial.println("Relay ON");
     }
     if (request.indexOf("/RELAY=OFF") != -1){
        digitalWrite(ledPin, LOW);
        value = LOW;
        Serial.println("Relay OFF");
     }

     if (request.indexOf("/MOISTURE=ON") != -1) {
        UseSensor = 1;
        Serial.println("Moisturesensor ON");
     }
     if (request.indexOf("/MOISTURE=OFF") != -1){
        UseSensor = 0;
        Serial.println("Moisturesensor OFF");
     }
     
     // Return the response
     
     client.println("HTTP/1.1 200 OK");
     client.println("Content-Type: text/html");
     client.println(""); //  do not forget this one
     client.println("<!DOCTYPE HTML>");
     client.println("<html>");
 
     client.print("RELAY is now: ");
 
     if (value == HIGH) {
        client.print("On");  
     } else {
        client.print("Off");
     }

     client.println("<br>");
     client.print("MOISTURE is now: ");
     if (UseSensor == 1) {
        client.print("On");  
     } else {
        client.print("Off");
     }
     
     client.println("<br><br>");
     client.println("Click <a href=\"/RELAY=ON\">here</a> Turn relay ON<br>");
     client.println("Click <a href=\"/RELAY=OFF\">here</a> Turn relay OFF<br>");
     client.println("<br>");
     client.println("Click <a href=\"/MOISTURE=ON\">here</a> Turn Moisturesensor ON<br>");
     client.println("Click <a href=\"/MOISTURE=OFF\">here</a> Turn Moisturesensor OFF<br>");
     client.println("</html>");
 
     delay(1);
     Serial.println("Client disconnected");
     Serial.println("");
   } 
   
   Serial.println(UseSensor);
   if (UseSensor == 1) {
     Serial.println("UseMoistureSensor");

     //ตัวรดน้ำต้นไม้

     sensorValue = analogRead(analogInPin);
     outputValue = map(sensorValue, 0, 1023, 100, 0);

     Serial.print("Soil Moisture = ");
     Serial.print(outputValue);
     Serial.println(" %");

     if (outputValue <= 30) {  
        digitalWrite(relay, HIGH);
        Serial.println("High");
        value = HIGH ;
     }
     if (outputValue >= 60) {
        digitalWrite(relay, LOW);
        Serial.println("Low");
        value = LOW ;
        }
     
     delay(1000);
     
   }
   else {
      Serial.println("MoistureSensor Not Use");
     }
}
