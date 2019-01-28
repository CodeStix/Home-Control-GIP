#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "PollenPatatten";
const char* password = "Ziektes123";

WiFiServer server(80);

String header;

void setup() 
{
  Serial.begin(9600);

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

void loop()
{
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
        Serial.write(c)
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
            
            client.println("<p><a href=\"/47/d0\"><button class=\"button\">Toggle ventilator</button></a></p>"); 
         
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
}
