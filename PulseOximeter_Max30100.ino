//The Electronic Noobs
//Pulse Oximeter

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
 
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "OakOLED.h"
 
#define REPORTING_PERIOD_MS 1000
OakOLED oled;
 
// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0
PulseOximeter pox;
 
float BPM, SpO2;
uint32_t tsLastReport = 0;
 
const unsigned char bitmap [] PROGMEM=
{
0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x00, 0x0f, 0xe0, 0x7f, 0x00, 0x3f, 0xf9, 0xff, 0xc0,
0x7f, 0xf9, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xf0,
0xff, 0xf7, 0xff, 0xf0, 0xff, 0xe7, 0xff, 0xf0, 0xff, 0xe7, 0xff, 0xf0, 0x7f, 0xdb, 0xff, 0xe0,
0x7f, 0x9b, 0xff, 0xe0, 0x00, 0x3b, 0xc0, 0x00, 0x3f, 0xf9, 0x9f, 0xc0, 0x3f, 0xfd, 0xbf, 0xc0,
0x1f, 0xfd, 0xbf, 0x80, 0x0f, 0xfd, 0x7f, 0x00, 0x017, 0xfe, 0x7e, 0x00, 0x03, 0xfe, 0xfc, 0x00,
0x01, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x3f, 0xc0, 0x00,
0x00, 0x0f, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
 
void onBeatDetected()
{   
    Serial.println("Beat Detected!");
    oled.drawBitmap( 60, 20, bitmap, 28, 28, 1);
    oled.display();
}

void onDisplay()
{
    oled.begin();
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(1);
    oled.setCursor(0,16);
    oled.println("");
   
    oled.setTextSize(2);
    oled.setTextColor(1);
    oled.setCursor(0, 0);
    oled.println("BPM : 87.0");
   
    oled.setTextSize(2);
    oled.setTextColor(1);
    oled.setCursor(0, 16);
    oled.println("Spo2 : 94%");
 
    oled.setTextSize(2);
    oled.setTextColor(1);
    oled.setCursor(0,45);
    oled.println("");
    oled.display();
}
 
void setup()
{   
    Serial.begin(115200);
    oled.begin();
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(1);
    oled.setCursor(0, 0);
 
    oled.println("Initializing pulse oximeter..");
    oled.display();
    
    pinMode(16, OUTPUT);
 
    Serial.print("Initializing Pulse Oximeter..");
 
    if (!pox.begin() )
    {
         Serial.println("FAILED");
         oled.clearDisplay();
         oled.setTextSize(2);
         oled.setTextColor(1);
         oled.setCursor(0, 0);
         oled.println("FAILED");
         oled.display();
         for(;;);
    }
    else
    {    
         oled.clearDisplay();
         oled.setTextSize(2);
         oled.setTextColor(1);
         oled.setCursor(0, 0);
         oled.println("SUCCESS");
         oled.display();
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }
    
    // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
     //pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}
 
void loop()
{
    pox.update();
 
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print(" SpO2:");
        Serial.print(SpO2);
        Serial.println(" %");
        
        
        oled.clearDisplay();
        oled.setTextSize(2);
        oled.setTextColor(1);
        oled.setCursor(0,16);
        oled.println(pox.getHeartRate());
 
        oled.setTextSize(2);
        oled.setTextColor(1);
        oled.setCursor(0, 0);
        oled.println("Heart BPM");
 
        oled.setTextSize(2);
        oled.setTextColor(1);
        oled.setCursor(0, 30);
        oled.println("Spo2");
 
        oled.setTextSize(2);
        oled.setTextColor(1);
        oled.setCursor(0,45);
        oled.println(pox.getSpO2());
        oled.display();
        
 
        tsLastReport = millis();
    }
}
