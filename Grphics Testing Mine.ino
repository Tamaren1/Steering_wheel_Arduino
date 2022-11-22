
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include <Wire.h>

// These are 'flexible' lines that can be changed
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8 // RST can be set to -1 if you tie it to Arduino's reset
// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

//General
int checksumA = 0;
int checksumB = 0;
int buttonState = 0;
int StartTime;
int CurrentTime;
int ElapsedTime;
int time;
int hour;
int mins;
int secon;

String importvar  = "";

//gauges
int varline = 0;
int oilvar = 0;
int tachvar = 0;
int blkvarC = 0;
int cursorbar = 0;
int tempmap = 0;
int fuelmap = 0;
int oilmap = 0;
int tachmap = 0;
int oldtemps = 0;
int oldo = 0;
int oldf = 0;
int otachs = 0;
int ogear = 0;
int ospeed = 0;

//resist
int analogPin = 0;
int vMeasure = 2;
int raw = 0;
int Vin = 5;
float Vout = 0;
float R1 = 1000;
float R2 = 0;
float buffer = 0;

float VoutB = 0.0;
float VinB = 0.0;
float R1B = 100000.0; // R1 (100k ohm) 
float R2B = 10000.0; // R2 (10k ohm) 
int valB = 0;

//TPMS
int frontLPress = 0;
int frontRPress = 0;
int rearLPress = 0;
int rearRPress = 0;

int frontLTemp = 0;
int frontRTemp = 0;
int rearLTemp = 0;
int rearRTemp = 0;

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

//Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, MOSI, SCK, TFT_RST, MISO);


void setup() {
  tft.begin();

  Serial.begin(9600);

  tft.setRotation(3);
  tft.fillScreen(BLACK);

  unsigned long StartTime = millis();

  Wire.begin(4);                
  Wire.onReceive(receiveEvent); 

}

void receiveEvent() {
  importvar = "";
  while(1 < Wire.available()) // loop  all but last
  {
    char c = Wire.read(); 
    importvar.concat(c);       
  }
  int x = Wire.read();            
}

void loop(void) {
  //Resist loop for menu switch
  raw = analogRead(analogPin);
  buffer = raw * Vin;
  Vout = (buffer)/1024.0;
  buffer = (Vin/Vout) - 1;
  R2= R1 * buffer;

  //resist loop for voltage measurement 
   valB = analogRead(vMeasure); //analog input
   VoutB = (valB * 5.0) / 1024; 
   VinB = VoutB / (R2B/(R1B+R2B));

  //TEMPorary!!
  R2 = 220;

  if (R2 > 200 && R2 < 240){
    checksumA = 1;
  if(checksumA != checksumB){
     tft.fillScreen(BLACK);
  }

  //Borders
  //Outside
  tft.fillRect(0, 0, 4, 320, WHITE);
  tft.fillRect(0, 0, 480, 4, WHITE);
  tft.fillRect(476, 0, 4, 316, WHITE);
  tft.fillRect(0, 316, 480, 4, WHITE);

  //Timer Button

  buttonState = digitalRead(2);
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  time = ElapsedTime / 1000;                                                           
  hour = time/3600;                                                        
  mins = (time-hour*3600)/60;                                              
  secon = time-hour*3600-mins*60;                                            
  tft.setCursor(358, 95);
  tft.setTextSize(2);
  tft.setTextColor(WHITE,BLACK);
  if (secon < 10){
    tft.println (String(mins) + ":" + "0" + String(secon));
  }
  else {
    tft.println (String(mins) + ":" + String(secon));
  }
  tft.setCursor(365, 120);
  tft.setTextColor(YELLOW);
  tft.println("Lap");
  tft.setTextColor(WHITE);

     //Gear and Speedo
    tft.fillRect(154, 79, 4, 320, WHITE);
    tft.fillRect(4, 158, 154, 4, WHITE);
    //Tach
    tft.fillRect(0, 79, 480, 4, WHITE);
    //Bars
    tft.fillRect(218, 79, 4, 320, WHITE); //Oil
    tft.fillRect(282, 79, 4, 320, WHITE); //Temp
    tft.fillRect(348, 79, 4, 320, WHITE); //Fuel
    tft.fillRect(412, 79, 4, 320, WHITE); //Fuel

    tft.fillRect(348, 140, 65, 4, WHITE); //time
    tft.fillRect(348, 194, 65, 4, WHITE); //time
    tft.fillRect(348, 248, 65, 4, WHITE); //time
  
//Endborders

  //Oil
    tft.setCursor(236, 300);
    tft.setTextColor(WHITE);    
    tft.setTextSize(2);
    tft.println("Oil");

  //temp
    tft.setCursor(295, 300);
    tft.setTextColor(WHITE);    
    tft.setTextSize(2);
    tft.println("Temp");

  //Fuel
    tft.setCursor(167, 300);
    tft.setTextColor(WHITE);    
    tft.setTextSize(2);
    tft.println("Fuel");

  //Gear
    tft.setCursor(45, 280);
    tft.setTextColor(WHITE);    
    tft.setTextSize(3);
    tft.println("Gear");

  //Tach
    tft.setCursor(20, 30);
    tft.setTextColor(YELLOW);    
    tft.setTextSize(3);
    tft.println("Tach");

  //Speedo

    tft.setCursor(110, 135);
    tft.setTextColor(YELLOW);    
    tft.setTextSize(2);
    tft.println("MPH");

  //Menu Options
    tft.setCursor(420, 95);
    tft.setTextColor(YELLOW);    
    tft.setTextSize(2);
    tft.println("Main");
    tft.setTextColor(WHITE);
    tft.setCursor(420, 120);
    tft.println("Tire");
    tft.setCursor(420, 145);
    tft.println("Time");
    tft.setCursor(420, 170);
     tft.println("Susp");
    tft.setCursor(420, 195);
    tft.println("Eng");
    tft.setCursor(420, 220);
    tft.println("Diag");
    tft.setCursor(420, 245);
    tft.println("Extr");
    tft.setCursor(420, 270);
    tft.println("Extr");
    tft.setCursor(420, 295);
    tft.println("Extr");
  
  //Info Column
    if (VinB < 8.00){
      tft.setTextColor(RED, BLACK);
    }
    else{
      tft.setTextColor(WHITE, BLACK);
    }
    tft.setCursor(358, 150);
    tft.println(VinB,1);
    tft.setCursor(358, 175);
    tft.setTextColor(YELLOW);
    tft.println("Volt");
    tft.setTextColor(WHITE);

    tft.setCursor(365, 205);
    tft.println("+ 4");
    tft.setCursor(358, 230);
    tft.setTextColor(YELLOW);
    tft.println("Bias");
    tft.setTextColor(WHITE);

    tft.setCursor(358, 275);
    tft.println("Race");
    tft.setCursor(358, 300);
    tft.setTextColor(YELLOW);
    tft.println("Mode");
    tft.setTextColor(WHITE);

    Serial.println(String(importvar));
    
  //temp Var
    int temps = importvar.substring(0,3).toInt(); 
    tempmap = map(temps, 50, 250, 280, 90);
    if (temps > 99){
      tft.setCursor(288, tempmap);
    }
    else {
      tft.setCursor(294, tempmap);
    }
    if (temps > 220){
      tft.setTextColor(RED, BLACK);
    }    
    else {
      tft.setTextColor(WHITE, BLACK);
    }
    if (temps != oldtemps){
      tft.fillRect(288, 90, 60, 210, BLACK);
    }
    tft.setTextSize(2);
    tft.print("-");
    tft.print(temps);
    tft.print("-");
    oldtemps = temps;
    delay(10);

  //Fuel Var
    int fuels = importvar.substring(5,7).toInt(); 
    fuelmap = map(fuels, 0, 20, 280, 90);
      tft.setCursor(159, fuelmap);
      if (fuels > 5){
        tft.setTextColor(WHITE, BLACK);
      }    
      else {
        tft.setTextColor(RED, BLACK);
      }  
      if (fuels != oldf){
        tft.fillRect(159, 90, 60, 210, BLACK);
      }
      tft.setTextSize(2);
      tft.print("-");
      if (fuels <= 5){
        tft.print("1/4");
      }
      if (fuels <= 10 && fuels > 5){
        tft.print("1/2");
      }
      if (fuels <= 15 && fuels > 10){
        tft.print("3/4");
      }
      if (fuels > 15){
        tft.print("4/4");
      }
      tft.print("-");
      oldf = fuels;
      delay(10);

    //Oil Var

  int oils = importvar.substring(3,5).toInt(); 
    oilmap = map(oils, 0, 99, 280, 90);
   if (oils > 99){
     tft.setCursor(223, oilmap);
   }
   else {
      tft.setCursor(229, oilmap);
    }
    if (oils > 20){
      tft.setTextColor(WHITE, BLACK);
    }    
    else {
      tft.setTextColor(RED, BLACK);
    }
    if (oils != oldo){
      tft.fillRect(223, 90, 60, 210, BLACK);
    }
    tft.setTextSize(2);
    tft.print("-");
    tft.print(oils);
    tft.print("-");
    oldo = oils;
    delay(10);

  int tachs = importvar.substring(7,11).toInt(); 
  tachmap = map(tachs, 0, 7000, 150, 465);
  if (tachs != otachs){
      tft.fillRect(100, 4, 376, 74, BLACK);
  }
  cursorbar = tachmap - 65;
  tft.fillRect(tachmap,9,5,65,GREEN);
  tft.setCursor(cursorbar, 55);
  tft.setTextColor(WHITE, BLACK);    
  tft.setTextSize(2);
  tft.println(" " + String(tachs));
  delay(500);
  otachs = tachs;

  //Speedo Var
    int speedvar = importvar.substring(11,14).toInt(); 
    tft.setCursor(15, 106);
    if (speedvar != ospeed){
      tft.fillRect(14, 105, 50, 20, BLACK);
    }
    tft.setTextColor(WHITE, BLACK);    
    tft.setTextSize(4);
    tft.println(speedvar);
    ospeed = speedvar;

  //Gear Var
  // NOTE: Here is where to change settings for tire sizes, exc. 
  // Multiply values by 1000
    long tire_size = 25300;
    long fdrive = 3444;
    tft.setCursor(55, 190);
    tft.setTextColor(YELLOW, BLACK);    
    tft.setTextSize(10);
    long GRA = tachs * tire_size;
    long GRB = fdrive * speedvar * 336;
    Serial.println(GRA);
    Serial.println(GRB);
    float GR = float(GRA) / float(GRB);
    Serial.println(GR);
    if (GR != ogear){
      tft.fillRect(54, 189, 50, 40, BLACK);
    }
    if (GR > 2900){
      tft.println("1");
    }
    if (GR > 1.9 && GR < 2.8){
      tft.println("2");
    }
     if (GR > 1.325 && GR < 1.9){
      tft.println("3");
    }
    if (GR > 1.1 && GR < 1.325){
      tft.println("4");
    }
    if (GR > .9 && GR < 1.1){
      tft.println("5");
    }
    if (GR < .9){
      tft.println("6");
    } 
    ogear = GR;

  }

  if (R2 > 400 && R2 < 500) {
    checksumA = 2;
    if(checksumA != checksumB){
      tft.fillScreen(BLACK);
    }
    frontLPress = 30;
    frontRPress = 31;
    rearLPress = 21;
    rearRPress = 30;

    frontLTemp = 95;
    frontRTemp = 99;
    rearLTemp = 91;
    rearRTemp = 92;

  //Car Outline
   //sides
    tft.drawLine(150, 120, 275, 120, WHITE);
    tft.drawLine(150, 205, 275, 205, WHITE);
   //rearsides
    tft.drawLine(315, 135, 420, 135, WHITE);  
    tft.drawLine(315, 190, 420, 190, WHITE); 
   //frontsides
    tft.drawLine(150, 120, 30, 130, WHITE); 
    tft.drawLine(150, 205, 30, 195, WHITE); 
   //Intakes
    tft.drawCircle(330, 155, 4, YELLOW);
    tft.drawCircle(330, 170, 4, YELLOW);
    tft.drawCircle(340, 155, 4, YELLOW);
    tft.drawCircle(340, 170, 4, YELLOW);
    tft.drawCircle(350, 155, 4, YELLOW);
    tft.drawCircle(350, 170, 4, YELLOW);
    tft.drawCircle(360, 155, 4, YELLOW);
    tft.drawCircle(360, 170, 4, YELLOW);
    tft.drawCircle(370, 155, 4, YELLOW);
    tft.drawCircle(370, 170, 4, YELLOW);
    tft.drawCircle(380, 155, 4, YELLOW);
    tft.drawCircle(380, 170, 4, YELLOW);
   //Exhausts
    tft.fillRect(410, 151, 50, 10, WHITE);
    tft.fillRect(410, 166, 50, 10, WHITE);
   //rearangles
    tft.drawLine(275, 120, 315, 135, WHITE);
    tft.drawLine(275, 205, 315, 190, WHITE);
   //Rear
    tft.drawLine(420, 135, 430, 145, WHITE);
    tft.drawLine(420, 190, 430, 180, WHITE);
    tft.drawLine(430, 180, 430, 145, WHITE);
   //Front
    tft.drawLine(30, 195, 25, 190, WHITE);
    tft.drawLine(30, 130, 25, 135, WHITE);
    tft.drawLine(25, 190, 25, 135, WHITE);
   // susrear
    tft.drawLine(310, 133, 380, 85, WHITE);
    tft.drawLine(410, 133, 380, 85, WHITE);
    tft.drawLine(310, 193, 380, 240, WHITE);
    tft.drawLine(410, 193, 380, 240, WHITE);
  //susfront
    tft.drawLine(65, 125, 95, 85, WHITE);
    tft.drawLine(125, 120, 95, 85, WHITE);
    tft.drawLine(65, 200, 95, 240, WHITE);
    tft.drawLine(125, 205, 95, 240, WHITE);

  //Front Right
    tft.setCursor(60, 36);
    if (frontRPress < 25){
      tft.setTextColor(BLACK, RED);
      tft.fillRoundRect(45, 25, 100, 60, 7, RED);
    }
    else{
      tft.setTextColor(WHITE, BLACK);
      tft.drawRoundRect(45, 25, 100, 60, 7, WHITE);
    }    
    tft.setTextSize(2);
    tft.print(frontRPress);
    tft.print(" PSI");
    tft.setCursor(60, 56);
    tft.setTextColor(CYAN); 
    tft.print(frontRTemp);
    tft.print(" F");

    //Front Left
    tft.setCursor(60, 250);
    if (frontLPress < 25){
      tft.setTextColor(BLACK, RED);
      tft.fillRoundRect(45, 240, 100, 60, 7, RED);
    }
    else{
      tft.setTextColor(WHITE, BLACK);
      tft.drawRoundRect(45, 240, 100, 60, 7, WHITE);
    }
    tft.setTextSize(2);
    tft.print(frontLPress);
    tft.print(" PSI");
    tft.setCursor(60, 270);
    tft.setTextColor(CYAN); 
    tft.print(frontLTemp);
    tft.print(" F");

    //Rear Right
    tft.setCursor(345, 36);
    if (rearRPress < 25){
      tft.setTextColor(BLACK, RED);
      tft.fillRoundRect(330, 25, 100, 60, 7, RED);
    }
    else{
      tft.setTextColor(WHITE, BLACK);
      tft.drawRoundRect(330, 25, 100, 60, 7, WHITE);
    }        
    tft.setTextSize(2);
    tft.print(rearRPress);
    tft.print(" PSI");
    tft.setCursor(345, 56);
    tft.setTextColor(CYAN); 
    tft.print(rearRTemp);
    tft.print(" F");

  //Rear Left
    tft.setCursor(345, 250);
    if (rearLPress < 25){
      tft.setTextColor(BLACK, RED);
      tft.fillRoundRect(330, 240, 100, 60, 7, RED);
    }
    else {
      tft.setTextColor(WHITE, BLACK);
      tft.drawRoundRect(330, 240, 100, 60, 7, WHITE);
    }       
    tft.setTextSize(2);
    tft.print(rearLPress);
    tft.print(" PSI");
    tft.setCursor(345, 270);
    tft.setTextColor(CYAN); 
    tft.print(rearLTemp);
    tft.print(" F");
  delay(100);
  }

//Timer
  checksumB = checksumA;
  delay(100);
}


