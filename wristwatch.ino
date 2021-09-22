
#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiServer.h>
#include <LWiFiClient.h>

#define WIFI_AP "TP-LINK_6CF268"
#define WIFI_PASSWORD "password"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP according to your WiFi AP configuration


int serverPort = 80;
LWiFiServer server(serverPort);
int LED = 13;

#include <Wire.h>

#include <rgb_lcd.h>

rgb_lcd lcd;


int ButtonPin = 7;
int BuzzerPin = 2;

int buttonState;


void setup()
{

  Serial.begin(9600);
lcd.begin(16,2);
 
  pinMode(ButtonPin,INPUT);
  pinMode(BuzzerPin,OUTPUT);
  
  pinMode(LED, OUTPUT);
  LWiFi.begin();

  // keep retrying until connected to AP
  Serial.println("Connecting to AP");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
  {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(600);

  }
  digitalWrite(LED, HIGH);
  printWifiStatus();
  Serial.println("Start Server");
  server.begin();
  Serial.println("Server Started");
  digitalWrite(LED, LOW);
}

int loopCount = 0;

void loop()
{

  
  // put your main code here, to run repeatedly:
  String str = "";
  String url = "";
  int i;
  delay(500);
  loopCount++;
  LWiFiClient client = server.available();
  if (client)
  {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected())
    {
      if (client.available())
      {
        // we basically ignores client request, but wait for HTTP request end
        char c = client.read();
        Serial.print(c);
        if(c != '\n')
          str += c;
        if(c == '\n')
        {
          //Serial.println(str);
          if(str.startsWith("GET "))
          {
            url = str.substring(4, str.lastIndexOf(" "));
            Serial.print("URL:");
            Serial.print(url);
            Serial.println(":");
          }
          str = "";
        }

        if (c == '\n' && currentLineIsBlank)
        {
          Serial.println("send response");
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          if(url != String("favicon.ico"))
          {
            client.println("<!DOCTYPE HTML>");
            client.println("<html>\n<head>\n<title>LED Control</title>\n</head>");
            IPAddress ip = LWiFi.localIP();
            client.println("<body><center>");
            client.println("<form action='");
            client.println("' method='GET'>Tell your device what to do!<br><br><input type='radio' name='q' value='on'>Turn the LED on.<br><input type='radio' name='q' value='off'>Turn the LED off.<br><br><input type='submit' value='Do it!'></form>");
            //i = digitalRead(LED);
            url.toLowerCase();
            if(url == String("/?q=on"))
            {

              lcd.clear();
              lcd.println("Lat:13.4259765");
              lcd.setCursor(0,1);
              lcd.println("Lon:74.7721086");
     
              Serial.println(" Latitude : 13.4259765 , Longitude : 74.7721086 \n ALERT!!! LINK : http://www.latlong.net/c/?lat=13.4259765&long=74.7721086");
      
          
 
              digitalWrite(BuzzerPin,HIGH);
              delay(2000);
              digitalWrite(BuzzerPin,LOW);
              lcd.clear();

    
              digitalWrite(LED, HIGH);
              client.println("LED on<br>");
            }
            else if(url == String("/?q=off"))
            {
              digitalWrite(LED, LOW);
              client.println("LED off<br>");

              digitalWrite(BuzzerPin,LOW);
              lcd.clear();
            }
            else
            {
              client.println("Doing nothing<br>");

              digitalWrite(BuzzerPin,LOW);
              lcd.clear();
            }
            
            client.println("</center></body>\n</html>");
            client.println();
            break;
          }
        }
        if (c == '\n')
        {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r')
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(50);

    // close the connection:
    Serial.println("close connection");
    client.stop();
    Serial.println("client disconnected");
  }


  buttonState = digitalRead(ButtonPin);

  if(buttonState == HIGH){
  
    lcd.clear();
    lcd.println("Lat:13.4259765");
    lcd.setCursor(0,1);
    lcd.println("Lon:74.7721086");
     
    Serial.println(" Latitude : 13.4259765 , Longitude : 74.7721086 \n ALERT!!! LINK : http://www.latlong.net/c/?lat=13.4259765&long=74.7721086");
    
          
 
    digitalWrite(BuzzerPin,HIGH);
    delay(2000);
    digitalWrite(BuzzerPin,LOW);
    lcd.clear();

     
    
  }

  else 
  {
    digitalWrite(BuzzerPin,LOW);
    lcd.clear();
    
  }


  
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(LWiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = LWiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.print("subnet mask: ");
  Serial.println(LWiFi.subnetMask());

  Serial.print("gateway IP: ");
  Serial.println(LWiFi.gatewayIP());

  // print the received signal strength:
  long rssi = LWiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


