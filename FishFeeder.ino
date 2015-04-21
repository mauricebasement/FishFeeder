#include <Wire.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <DS1307RTC.h>

int stepPin = 2;
int directionPin = 3;
int enablePin = 4;
int a = 0;
int time = 0;
char c;
tmElements_t tm;

void setup() 
{                
  pinMode(stepPin, OUTPUT);
  pinMode(directionPin, OUTPUT);       
  pinMode(enablePin, OUTPUT);       
  time = 60*60*1000;
  Serial.begin(9600);
  while (!Serial) ;
  delay(200);
  Serial.println("Fish Feeder Command Line Mode");
  getTime();
  Alarm.alarmRepeat(6,00,0, FeedAlarm);
  Alarm.alarmRepeat(9,00,0, FeedAlarm);
  Alarm.alarmRepeat(12,00,0, FeedAlarm);
  Alarm.alarmRepeat(13,30,0, FeedAlarm);
  Alarm.alarmRepeat(15,00,0, FeedAlarm);
  Alarm.alarmRepeat(18,00,0, FeedAlarm);
  Alarm.alarmRepeat(00,00,0, getTime);
}


void loop() 
{
  Alarm.delay(1000);
  checkCommandLine();
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
  if (RTC.read(tm)) {
    Serial.println("Time OK");
    setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tm.Year);
  } else {
    Serial.println("DS1307 read error!  Replace Device!");
  }
}

void printTime() {
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

void checkCommandLine() {
  while (Serial.available()) {
    c = Serial.read();  
  }
  if (c == 't' ) {
    FeedAlarm();
    Serial.println("Manually Feed!");
  }  if (c == 'p' ) {
    printTime();
  } if (c == 'b' ) {
    printBattery();
  }
  c = 'a';
}

void printBattery() {
  int sensorValue = analogRead(A1);
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.print("Battery Voltage:"); 
  Serial.print(voltage); 
  Serial.println("V"); 
}
