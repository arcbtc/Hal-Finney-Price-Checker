#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include <ArduinoJson.h> //downgrade using library manager to version 5.3.0
#include <WiFi.h>
#include <WiFiClientSecure.h>

//Wifi details
char wifiSSID[] = "YOURWIFI";
char wifiPASS[] = "YOURPASS";
String on_currency = "BTCGBP";
String on_sub_currency = on_currency.substring(3);
char conversion[20];

const uint16_t WAIT_TIME = 1000;


#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW //If your LEDs look odd try replacing ICSTATION_HW with one of these GENERIC_HW, FC16_HW, PAROLA_HW .
    
#define MAX_DEVICES 4
#define CLK_PIN   18
#define DATA_PIN  23
#define CS_PIN    5


MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Scrolling parameters
uint8_t scrollSpeed = 25;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 2000; // in milliseconds

// Global message buffers shared by Serial and Scrolling functions
#define  BUF_SIZE  75
char curMessage[BUF_SIZE] = { "" };
char newMessage[BUF_SIZE] = { "Hello! Enter new message?" };
bool newMessageAvailable = true;


void setup(void)
{  
    Serial.begin(115200);

      //connect to local wifi            
  WiFi.begin(wifiSSID, wifiPASS);   
  while (WiFi.status() != WL_CONNECTED) {}

  P.begin();
  P.setIntensity(0);
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
}
int counta = 0;
void loop(void)
{

   if (P.displayAnimate())
   {if (newMessageAvailable){
    strcpy(curMessage, "The computer can be used as a tool to liberate and protect people, rather than to control them.");
    }P.displayReset();}
  
     if (P.displayAnimate())
   {if (newMessageAvailable){
      on_rates();
      strcpy(curMessage, conversion);
    }P.displayReset();}

    if (P.displayAnimate())
   {if (newMessageAvailable){
     strcpy(curMessage, "Bitcoin seems to be a very promising idea.");
    }P.displayReset();}

     if (P.displayAnimate())
   {if (newMessageAvailable){
     strcpy(curMessage, "Easy come, easy go.");
    }P.displayReset();}
 
     if (P.displayAnimate())
    {if (newMessageAvailable){
     strcpy(curMessage, "Since we're all rich with bitcoins ... we ought to put some of this unearned wealth to good use.");
    }P.displayReset();}
    
    if (P.displayAnimate())
   {if (newMessageAvailable){
     strcpy(curMessage, "It is pretty strange that bitcoins will hit a dollar in the relatively near future.");
    }P.displayReset();}
    
   if (P.displayAnimate())
   {if (newMessageAvailable){
     strcpy(curMessage, "For Bitcoin to succeed and become secure, bitcoins must become vastly more expensive.");
    }P.displayReset();}
 
}


void on_rates(){

     // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;

  if (!client.connect("api.opennode.co", 443)) {

    return;
  }

  String url = "/v1/rates";


  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + "api.opennode.co" + "\r\n" +
               "User-Agent: ESP32\r\n" +
               "Connection: close\r\n\r\n");

  while (client.connected()) {

    
    String line = client.readStringUntil('\n');
    if (line == "\r") {

      break;
    }
  }
  String line = client.readStringUntil('\n');

  
const size_t capacity = 169*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(168) + 3800;
DynamicJsonBuffer jsonBuffer(capacity);

JsonObject& root = jsonBuffer.parseObject(line);

JsonObject& data = root["data"];

float conversionnn = data[on_currency][on_currency.substring(3)]; 
String conversionn = String("$") + String(conversionnn, 3);
conversionn.toCharArray(conversion, conversionn.length());
Serial.println(conversion);
}
