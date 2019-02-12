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

/* Main control unit */

SR_HCP hcp = SR_HCP(MY_ADDRESS, 2400, 12, 14);
WiFiServer server(80);

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

  delay(100);

  flash(50, 10);
}

void loop()
{
  int fromAddress, data;

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
        Serial.write(c);
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

            if (header.indexOf("GET /47/d1") >= 0)
            {
               hcp.hcpSend(47, 1);
            }

            if (header.indexOf("GET /47/d2") >= 0)
            {
               hcp.hcpSend(47, 2);
            }

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            client.println("<body><h1>Home Control</h1>");
            client.println("<p>Control panel</p>");

            client.println("<p><a href=\"/47/d0\"><button class=\"button\">Toggle lamp 1</button></a></p>");
            client.println("<p><a href=\"/47/d1\"><button class=\"button\">Toggle lamp 2</button></a></p>");
            client.println("<p><a href=\"/47/d2\"><button class=\"button\">Toggle lamp 3</button></a></p>");

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


