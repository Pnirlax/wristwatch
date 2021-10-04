#include <LiquidCrystal.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin = 2, TXPin = 3;
static const uint32_t gps_baudrate = 9600;
TinyGPSPlus gps;
SoftwareSerial soft(RXPin, TXPin);
String textMessage;
float Lat, Lon;
void setup()
{
  soft.begin(gps_baudrate);
  Serial.begin(19200);
  pinMode(12,INPUT);
  pinMode(4, OUTPUT);
}
void loop()
{
  int key = digitalRead(12);
  while (soft.available() > 0)
  {
    gps.encode(soft.read());
    if (gps.location.isUpdated())
    {
      Lat = gps.location.lat();
      Lon = gps.location.lng();
    }
    else;
  }
  if(key==1)
  {
    digitalWrite(4,HIGH);
    sendsms();
    digitalWrite(4,LOW);
  }
}
void sendsms()
{
    Serial.print("AT+CMGF=1\r");
    delay(100);
    Serial.println("AT+CMGS =\"+9779808023440\"");
    delay(100);
    Serial.println("Please Help! I am in emergency situation with temperature of : " String("Temp:")" degree and my Location is :" + String("Lat: ") +String(Lat) + " "+String("Lon: ") + String(Lon));
    delay(100);
    Serial.println((char)26);
    delay(100);
    Serial.println();
    delay(5000);
}
