/*
* This sketch demonstrates how to set up a simple (local) Web server.
* It will turn an LED ON and OFF using WiFi Connection with commands:
* serverIP/gpio/0 , where serverIP is the IP address of the ESP8266 board and 0 is OFF
* serverIP/gpio/1 , where serverIP is the IP address of the ESP8266 board and 1 is ON
*/
#include <ESP8266WiFi.h>
const char* ssid = "Gourgeous";
const char* password = "Mypreciousone";
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
void setup() {
  Serial.begin(115200);
  delay(10);
  // prepare GPIO2
  pinMode(13, OUTPUT);
  digitalWrite(13, 0);
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
  Serial.println(WiFi.localIP());
}
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Wait until the client sends some data
  Serial.println("new client");
    while(!client.available()){
    delay(1);
  }
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  // Match the request
  int val;
  if (req.indexOf("/gpio/0") != -1)
  val = 0;
  else if (req.indexOf("/gpio/1") != -1)
  val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  // Set GPIO2 according to the request
  digitalWrite(13, val);
  client.flush();
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";
  // Send the response to the client
  client.print(s);
  // buttons to turn the LED ON (/gpio/1) or OFF (/gpio/0)
  client.println("<br><br>");
  client.println("Click <a href=\"/gpio/1\">here</a> turn the LED on pin 13 ON<br>");
  client.println("Click <a href=\"/gpio/0\">here</a> turn the LED on pin 13 OFF<br>");
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
}
