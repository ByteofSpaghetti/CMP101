#include <DS3231.h> //Clock library for real time clock (rtc)
/*real time clock connects to:
  VCC - 3.3V on breadboard
  GND - GND on breadboard
  SDA - SDA/D2 on wemos
  SCL - SCL/D1 on wemos*/
#include <Streaming.h> //library to print to serial monitor and to use endl.
#include <Wire.h> //Library for wiring
#include <TM1638plus.h> //Library for expansion board
/*Expansion board connects to:
  VCC - 3.3V on breadboard
  GND - GND on breadboard
  STB - D5 on wemos
  CLK - D6 on wemos
  DIO - D7 on wemos*/
#include <Servo.h> //Library for servo
/*Servo wires connects to:
  Brown - GND on wemos
  Red - 5v on wemos
  Yellow - D4 on wemos*/
#include <Adafruit_GFX.h> //For OLED display
#include <Adafruit_SSD1306.h> //For OLED display
/*OLED wires connect to:
  GND - GND on breadboard
  VCC - 3.3V on breadboard
  SCL - SCL/D1 on wemos
  SDA - SDA/D2 on wemos*/
#define STROBE_TM D5 // strobe = GPIO connected to strobe line of module
#define CLOCK_TM D6 // clock = GPIO connected to clock line of module
#define DIO_TM D7 // data = GPIO connected to data line of module
#define OLED_RESET -1 //Reset pin
#define OLED_SCREEN_I2C_ADDRESS 0x3C //Gives address for display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(128,64); //OLED display size
bool high_freq = false; //
//Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, high_freq); //constructor 
// DS3231 uses I2C bus - device ID 0x68
DS3231 rtc;
Servo myservo;  // create servo object to control a servo
int buzzer = D8; //Connects buzzer to D8
//Bools for time
bool century = false; 
bool h12Flag;//For 24 hour clock
bool pmFlag;//For 24 hour clock
void setDateAndTime() //Function to set Date and Time
{
 rtc.setClockMode(false); // false = 24hr clock mode
 rtc.setYear(22); //Sets year
 rtc.setMonth(12); //Sets month
 rtc.setDate(13); //Set day
 rtc.setHour(3); //Sets hour
 rtc.setMinute(59); //Sets minutes
 rtc.setSecond(50); //Sets seconds
}
void setup() //Setup function
{
  pinMode(buzzer, OUTPUT); //Sets buzzer as output
  Serial.begin(115200);//Writes to Serial monitor 
 display.begin(SSD1306_SWITCHCAPVCC, OLED_SCREEN_I2C_ADDRESS);//Begins interface between wemos and OLED screen
 display.display(); //Beginning function of display
 delay(2000); //delay function
 display.clearDisplay(); //Clears the display
 display.setCursor(2,2); //Sets x and y coordinates
 display.setTextSize(4); // - a line is 21 chars in this size
 display.setTextColor(WHITE); //Sets display colour
 tm.displayBegin(); // 7 Segment
 Wire.begin(); //Initializes the Wire library
 Serial.begin(115200); //To print to serial monitor
 myservo.attach(D4, 500, 2400);//sets servo pins KEEP THIS IN YOU IDIOT
 setDateAndTime(); // Only need to do this once ever.
 // You should comment this out after you've successfully set the RTC
}
void loop()
{
  int sensor = analogRead(A0); //gives a variable to the position of the potentiometer
  int pos = 0; //variable for servo
  int sec = rtc.getSecond(); //sec variable is equal to return function that gets the seconds
  int min = rtc.getMinute();//min variable is equal to return function that gets the minutes
  int hour = rtc.getHour(h12Flag, pmFlag); //hour variable is equal to return function that gets the hour
  int hourd = rtc.getHour(h12Flag, pmFlag); //hourd variable is equal to return function that gets the hourd
  hour = hour * 100; //Multiplies hour by 100
  int total = hour + min; //total variable equals hour and min added together

  delay(10); //Delay function
  if (sensor > 1020) { //Code to be executed if sensor variable is more than 1020
  tm.displayIntNum(total); //displays total variable to 7 seg 
  display.clearDisplay(); //clears display
  display.setCursor(0,0); //sets x and y coordinate respectively in pixels
  display << rtc.getTemperature() << endl; //displays temperature in the room
  display.display(); //displays the temperature
  delay(20);
  }

  else if(sensor < 1020 && sensor > 500) { //Code executes if sensor between 1020 and 500
    tm.displayIntNum(total);//displays total on 7 seg
    display.setTextSize(4);//Increases text size
    display.clearDisplay();//Clears display
    display.setCursor(0,0);//sets x and y coordinate respectively in pixels
    display << rtc.getHour(h12Flag, pmFlag) << ":" <<  rtc.getMinute() << ":" << rtc.getSecond() << endl;
    display.display(); //Displays hour, miinutes and seconds 
    delay(20);
  }

  else if(sensor < 500 && sensor > 100) {
    tm.displayIntNum(total);//displays total on 7 seg
    display.setTextSize(3);//decreases text size
    display.clearDisplay();//Clears display
    display.setCursor(0,0);//sets x and y coordinate respectively in pixels
    display << rtc.getDate() << "/" << rtc.getMonth(century) << "/" << rtc.getYear() << endl;
    display.display(); //Displays date
    delay(20);
  }

  else if(sensor < 100) {
    hour = hour * 100; //multiplies hour variable by 100 (so by 10000 in total)
    min = min * 100 ; //Mutliplies minutes by 100
    total = hour + min + sec;//new total is hours plus minutes plus seconds
    tm.displayIntNum(total);//displays new total on 7 seg

    display.setTextSize(3);//decreases text size
    display.clearDisplay();//Clears display
    display.setCursor(0,0);//sets x and y coordinate respectively in pixels
    display << rtc.getDate() << "/" << rtc.getMonth(century) << "/" << rtc.getYear() << endl;
    display.display(); //Displays date
    delay(20);
  }

//Serial << rtc.getDate() << "/" << rtc.getMonth(century) << "/" << rtc.getYear() << " " ;
//Serial << rtc.getHour(h12Flag, pmFlag) << ":" << rtc.getMinute() << ":" <<
//rtc.getSecond() << endl;

switch(hourd) {//switch statement that reads other hour variable and displays the hour in binary on the 7 segs LEDs

  case 1: 
  tm.setLEDs(256); //1st LED
  break;

  case 13: 
  tm.setLEDs(256); //1st LED
  break;

  case 2:
  tm.setLEDs(512); //2nd LED
  break;

  case 14:
  tm.setLEDs(512); //2nd LED
  break;

  case 3:
  tm.setLEDs(768); //1st and 2nd LED
  break;

  case 15:
  tm.setLEDs(768); //1st and 2nd LED
  break;

  case 4:
  tm.setLEDs(1024); //3rd LED
  break;

  case 16:
  tm.setLEDs(1024); //3rd LED
  break;

  case 5:
  tm.setLEDs(1280); //1st and 3rd LED
  break;

  case 17:
  tm.setLEDs(1280); //1st and 3rd LED
  break;

  case 6: 
  tm.setLEDs(1536); //2nd and 3rd LED
  break;

  case 18: 
  tm.setLEDs(1536);//2nd and 3rd LED
  break;

  case 7: 
  tm.setLEDs(1792);//1st 2nd and 3rd LEDs
  break;

  case 19: 
  tm.setLEDs(1792);//1st 2nd and 3rd LEDs
  break;
 
  case 8:
  tm.setLEDs(2048);//4th LED
  break;

  case 20:
  tm.setLEDs(2048);//4th LED
  break;

  case 9: 
  tm.setLEDs(2304);//4th and 1st LED
  break;

  case 21: 
  tm.setLEDs(2304);//4th and 1st LED
  break;

  case 10:
  tm.setLEDs(2560);//4th and 2nd LED
  break;

  case 22:
  tm.setLEDs(2560);//4th and 2nd LED
  break;

  case 11:
  tm.setLEDs(2816);//4th 2nd and 1st LED
  break;

  case 23:
  tm.setLEDs(2816);//4th 2nd and 1st LED
  break;


  case 12:
  tm.setLEDs(3072);//4th and 3rd LEd
  break;

  case 0:
  tm.setLEDs(3072);//4th and 3rd LEd
  break;
}
  delay(20);
tm.displayIntNum(total); //displays total variable

  if (min == 0 && sec == 0) { //If minute and sec variable equal 0 this code block is executed
 
    for (pos = 180; pos >= 0; pos -= 1) { //code executes until pos variable equals 0
    myservo.write(pos);//turns the servo to the position pos
    }    
    tone(buzzer, 200); 
    delay(900); 
   noTone(buzzer); //buzzer makes sound at 200Hz
  for (pos = 0; pos <= 180; pos += 1) { //executes below code unitil pos is equal to 180
    myservo.write(pos); //turns the servo to the position pos
    delay(15);
    } 
  }
  
   else {
    tm.displayIntNum(total); //displays total variable on 7 seg
      delay(20);
    }
}   
