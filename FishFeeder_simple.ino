
#include <Wire.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <DS1307RTC.h>

int stepPin = 2;
int directionPin = 3;
int enablePin = 4;
int a = 0;
int time = 0;

void setup() 
{                
  pinMode(stepPin, OUTPUT);
  pinMode(directionPin, OUTPUT);       
  pinMode(enablePin, OUTPUT);       
  time = 60*60*1000;
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("Fish Feeder Command Line Mode");
  getTime();
  Alarm.alarmRepeat(8,00,0, FeedAlarm);  // 8:30am every day
  Alarm.alarmRepeat(12,00,0, FeedAlarm);  // 8:30am every day
  Alarm.alarmRepeat(16,00,0, FeedAlarm);  // 8:30am every day
  Alarm.alarmRepeat(18,00,0, FeedAlarm);  // 8:30am every day
}


void loop() 
{
  Alarm.delay(1000);
}

void FeedAlarm() {
  feedFish();
}

void feedFish() {
  Serial.println("Feeding Fish");
  makeStep(192*4); 
}

void makeStep(int numberOfSteps) {
  digitalWrite(enablePin, LOW);
  while (a <  numberOfSteps)   {
    digitalWrite(directionPin, HIGH);
    a++;
    digitalWrite(stepPin, HIGH);  
    delay(10);               
    digitalWrite(stepPin, LOW);  
    delay(10);
   }
   digitalWrite(enablePin, HIGH);
   a=0;
}

void getTime() {
  tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.println("Time OK");
    setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tm.Year);
  } else {
    Serial.println("DS1307 read error!  Replace Device!");
  }
  
}
