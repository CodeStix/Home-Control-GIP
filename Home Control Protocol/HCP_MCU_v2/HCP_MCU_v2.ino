#include "SR_HCP.h"
#include <ESP8266WiFi.h>

#define MY_ADDRESS 1

// We will accept values until this point, higher values will get recognized as a command.
#define VALUE_RANGE_MAX 240
// Commands that will be recognized by the main unit.
#define CMD_OKEY 255
#define CMD_UNKNOWN 254
#define CMD_ERROR 253
// Commands that will be recognized by the controlled units.
#define CMD_PING 255
#define CMD_SET 250

/* Main control unit (H-MCU) */

SR_HCP hcp = SR_HCP(MY_ADDRESS, 2400, 12, 14);
WiFiServer server(80);

// Credentials for the WiFiServer
const char* ssid     = "PollenPatatten";
const char* password = "Ziektes123";

String header = "";
String inString = "";
String argument = "";
int value = 0;
int prop = 0;

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
  if (data == CMD_OKEY) // 255 = OK
  {
    flash(100);
  }
  else if (data == CMD_UNKNOWN) // 254 = UNKNOWN
  {
    flash(100, 5);
  }
  else if (data == CMD_ERROR) // 253 = ERROR
  {
    flash(100, 3);
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  delay(200);

  flash(50, 2);

  Serial.begin(9600);

  Serial.print("H-MCU Startup Address: ");
  Serial.println(MY_ADDRESS);

  delay(200);

  flash(50, 2);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    flash(100, 1);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();

  delay(200);

  flash(50, 4);
}

void loop()
{
  byte fromAddress, data;

  if (hcp.hcpReceive(&fromAddress, &data, false))
    received(fromAddress, data);

  hcp.hcpResendIfNeeded();

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
      else if (inString.startsWith("v"))
      {
        value = inString.substring(1).toInt();

        Serial.println("<<< Value is set to " + String(value));
      }
      else if (inString.startsWith("p"))
      {
        prop = inString.substring(1).toInt();

        Serial.println("<<< Selected property is now " + String(prop));
      }
      else if (inString == "set")
      {
        Serial.println("<<< Setting...");
        hcp.hcpSendSet(argument.toInt(), prop, value);
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

  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        //Serial.write(c);
        header += c;
        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /47/d0") >= 0)
            {
               hcp.hcpSend(47, 0);
            }
            else if (header.indexOf("GET /47/d1") >= 0)
            {
               hcp.hcpSend(47, 1);
            }
            else if (header.indexOf("GET /47/d2") >= 0)
            {
               hcp.hcpSend(47, 2);
            }
            else if (header.indexOf("GET /45/red") >= 0)
            {
                hcp.hcpSendSet(45, 0, 255);
            }
            else if (header.indexOf("GET /45/green") >= 0)
            {
                hcp.hcpSendSet(45, 1, 255);
            }
            else if (header.indexOf("GET /45/blue") >= 0)
            {
                hcp.hcpSendSet(45, 2, 255);
            }
            else if (header.indexOf("GET /45/clear") >= 0)
            {
                hcp.hcpSend(45, 0);
            }
            else if (header.indexOf("GET /46/led1on") >= 0)
            {
                hcp.hcpSendSet(46, 0, 255);
            }
            else if (header.indexOf("GET /46/led1off") >= 0)
            {
                hcp.hcpSendSet(46, 0, 0);
            }
            else if (header.indexOf("GET /46/led2on") >= 0)
            {
                hcp.hcpSendSet(46, 2, 255);
            }
            else if (header.indexOf("GET /46/led2off") >= 0)
            {
                hcp.hcpSendSet(46, 2, 0);
            }

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 { background-color: #77878A;}</style></head>");

            client.println("<body><h1>Home Control</h1>");
            client.println("<p>Control panel</p>");

            client.println("<p>Lampen</p>");
            //client.println("<p><a href=\"/47/d0\"><button class=\"button\">Toggle lamp 1</button></a></p>");
            client.println("<p><a href=\"/47/d1\"><button class=\"button\">Toggle lamp 1</button></a></p>");
            client.println("<p><a href=\"/47/d2\"><button class=\"button\">Toggle lamp 2</button></a></p>");
            client.println("<p>Led strip</p>");
            client.println("<p><a href=\"/45/red\"><button class=\"button\" style=\"background-color: red;\">Rood</button></a></p>");
            client.println("<p><a href=\"/45/green\"><button class=\"button\" style=\"background-color: green;\">Groen</button></a></p>");
            client.println("<p><a href=\"/45/blue\"><button class=\"button\" style=\"background-color: blue;\">Blauw</button></a></p>");
            client.println("<p><a href=\"/45/clear\"><button class=\"button\">Reset</button></a></p>");
            client.println("<p>Test leds</p>");
            client.println("<p><a href=\"/46/led1on\"><button class=\"button\" style=\"background-color: green;\">Groen aan</button></a></p>");
            client.println("<p><a href=\"/46/led1off\"><button class=\"button\" style=\"background-color: darkgreen;\">Groen uit</button></a></p>");
            client.println("<p><a href=\"/46/led2on\"><button class=\"button\" style=\"background-color: yellow;\">Geel aan</button></a></p>");
            client.println("<p><a href=\"/46/led2off\"><button class=\"button\" style=\"background-color: olive;\">Geel uit</button></a></p>");

            client.println("</body></html>");
            client.println();
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }

    header = "";
    
    client.stop();
    
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  delay(10);
}


