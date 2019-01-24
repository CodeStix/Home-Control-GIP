#include "SR_HCP.h"
#include <ESP8266WiFi.h>
#define MY_ADDRESS 1

// Valid address ranges for each device type.
#define MCU_ADDRESS_RANGE_MIN 0
#define MCU_ADDRESS_RANGE_MAX 15
#define CU_ADDRESS_RANGE_MIN 16
#define CU_ADDRESS_RANGE_MAX 255
// Identifiers(error, info, commands, requests, ...) that will be recognized by the main unit.
#define ID_MCU_OKEY 255
#define ID_MCU_UNKNOWN 254
#define ID_MCU_ERROR 253
// Identifiers(error, info, commands, requests, ...) that will be recognized  by the controlled units (this).
#define ID_CU_PING 255

const char* ssid = "PollenPatatten";
const char* password = "Ziektes123";

/* Main control unit */

WiFiServer server(80);
SR_HCP hcp = SR_HCP(MY_ADDRESS, 2400, 12, 14);

long waitingTime = 0;

String inString = "";
String argument = "";

String header = "";
String currentLine = "";

void flash(int mil, int cycles = 1)
{
  for (int i = 0; i < cycles; i++)
  {
    digitalWrite(LED_BUILTIN, true);
    delay(mil / 2);
    digitalWrite(LED_BUILTIN, false);
    delay(mil / 2);
  }
}

void received(int fromAddress, int data)
{
  if (data == ID_MCU_OKEY) // 255 = OK
  {
    flash(100);
  }
  else if (data == ID_MCU_UNKNOWN) // 254 = UNKNOWN
  {
    flash(100, 5);
  }
  else if (data == ID_MCU_ERROR) // 253 = ERROR
  {
    flash(100, 3);
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  Serial.print("H-MCU Startup Address: ");
  Serial.println(MY_ADDRESS);

  delay(100);

  flash(50, 10);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void connect()
{


}

void loop()
{
  int fromAddress, data;

  if (hcp.hcpReceive(&fromAddress, &data, false))
    received(fromAddress, data);

  hcp.hcpResendIfNeeded();

  // Command line
  while (Serial.available() > 0)
  {
    int inChar = Serial.read();
    if (isAlphaNumeric(inChar))
      inString += (char)inChar;
    
    if (inChar == '\n')
    {
      Serial.print(">>> ");
      Serial.println(inString);

      if (inString == "rndto")
      {
        Serial.println("<<< Sending random data to " + argument.toInt());
        hcp.hcpSend(argument.toInt(), random(0, 4));
      }
      else if (inString.startsWith("d"))
      {
        int data = inString.substring(1).toInt();
        
        Serial.println("<<< Sending " + String(data)  + " to " + argument.toInt());
        hcp.hcpSend(argument.toInt(), data);
      }
      else if (inString == "ok")
      {
        Serial.println("<<< OK");
      }
      else
      {
        argument = inString;
                
        Serial.print("<<< Argument = ");
        Serial.println(argument);
      }

      inString = "";
    }
  }

  // HTTP Server
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) 
  {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) 
    {            // loop while the client's connected
      if (client.available()) 
      {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') 
        {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) 
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /47/output0") >= 0)
            {
               hcp.hcpSend(47, 0);
            } 
            else if (header.indexOf("GET /47/output1") >= 0) 
            {
              hcp.hcpSend(47, 1);
            } 
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Home Control</h1>");
                
            client.println("<p><a href=\"/47/output0\"><button class=\"button\">47: Output 0</button></a></p>");
            client.println("<p><a href=\"/47/output1\"><button class=\"button\">47: Output 1</button></a></p>");
               
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  
  delay(10);
}


