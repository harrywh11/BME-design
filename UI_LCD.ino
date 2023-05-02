#include <Arduino.h>
#include <U8g2lib.h>
#include <AS5600.h>


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_PWMServoDriver.h>
#define FREQUENCY             50
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
uint8_t servonum = 0;
 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  // High speed I2C
AS5600 encoder;

//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 24, /* data=*/ 23, /* reset=*/ U8X8_PIN_NONE);    //Low spped I2C
int location = 0;
float angle1 = 0;
int max_p = 0;
int planed[] = {20, 10, -10, 30, 0, 60, 6, -50, 20 ,10};
bool b1 = true;
bool b2 = true;
bool b3 = true;
int count = 0;

void setup(void) {
  //u8g2.begin();
  Serial.begin(115200);
  pinMode(A0, INPUT);//encoder
  pinMode(A1, INPUT);//button1
  pinMode(A2, INPUT);//button2
  pinMode(A3, INPUT);//button3
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);  // Analog servos run at ~60 Hz updates
  //GPO to +
 // DIR tp -
 // angle1 = analogRead(A0)/675.2*360;
}
 
void loop(void) {  
  int sensorValue3 = analogRead(A1);
  int sensorValue2 = analogRead(A2);
  int sensorValue1 = analogRead(A3);
  //Serial.println(sensorValue3 );
  if (sensorValue1>999){
    if (b1){
      count=count+1;
    }
    b1=false;
  }
  else{
    b1=true;
  }
  if (sensorValue2>999){
    if (b2){
      count=count-1;
    }
    b2=false;
  }
  else{
    b2=true;
  }
  if (sensorValue3>999){
    if (b3){
      location=0;
      max_p=0;
    }
    b3=false;
  }
  else{
    b3=true;
  }
  //float angle1 = analogRead(A0)/675.2*360; // convert voltage readout to angle
  //delay(100);

  
  float angle2 = analogRead(A0)/675.2*360;
  //Serial.println(angle2);
  float diff = angle2-angle1;
  if (angle2-angle1 > 180){
    diff = -(360-angle2+angle1);
  }
  if (angle1-angle2>180){
    diff = (360-angle1+angle2);
  }
  angle1 = angle2;
  
  
  
  location = location - diff;
  //location = location + diff;
  // record max position
  if (location > max_p){
    max_p = location;
  }
  
  display.clearDisplay();
  //testdrawcircle();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  //display.print("Current Location: ");
  display.print("Power Level: ");
  display.setCursor(105,0);
  display.print(count);
  display.setCursor(0,10);
  display.print("Maximum history: ");
  display.setCursor(105,10);
  display.print(max_p);
  display.setCursor(0,20);
  display.print("Current Location: ");
  display.setCursor(105,20);
  display.print(location);
  //display.setCursor(0,40);
  //display.print(angle1);
  //display.setCursor(40,40);
  //display.print(angle2);
  
  int sec = floor(location/20);
  if (sec < 10 && sec >= 0 ){
    display.drawCircle(10, 50, 10, WHITE);
    display.setCursor(6,47);
    display.print(sec);
    display.setCursor(51,47);
    display.print(planed[sec]);
    display.drawCircle(67, 43, 2, WHITE);
  }
  else{
  display.setCursor(6,47);
  display.print("Index out of range!");
  }

  if (diff<1){
    display.drawLine(100, 57, 100, 42, WHITE);
    display.drawLine(100, 57, 103, 52, WHITE);
    display.drawLine(100, 57, 97, 52, WHITE);
  }
  else if (diff>1) {
    display.drawLine(100, 57, 100, 42, WHITE);
    display.drawLine(97, 48, 100, 42, WHITE);
    display.drawLine(103, 48, 100, 42, WHITE);
  }
  else{
    display.drawLine(95, 50, 85, 50, WHITE);
  }
  
  display.display();
  int i = 2.5+count/10;
  pwm.setPWM(0, 0, i); //0-4
  delay(100);
  
}
