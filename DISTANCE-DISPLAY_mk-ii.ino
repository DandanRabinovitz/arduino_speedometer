#include <LiquidCrystal.h>
const int lightTrigPin = 9;
const int lightEchoPin = 10;
const int darkTrigPin = 14;
const int darkEchoPin = 15;
const int btnPin = 7;
const int btn2Pin = 8;
const int buzzPin = 13;
int trigPin = lightTrigPin;
int echoPin = lightEchoPin;
int btn1State = 0;
int btn2State = 0;
int lastbtn1 = 0;
int lastbtn2 = 0;
int mode = 1;
bool timer = false;
float duration, distance;
long ms;
double cm = 23.6;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
  // put your setup code here, to run once
  analogWrite(6, 110);
  lcd.begin(16, 2);
  pinMode(lightTrigPin, OUTPUT);
  pinMode(lightEchoPin, INPUT);
  pinMode(darkTrigPin, OUTPUT);
  pinMode(darkEchoPin, INPUT);
  pinMode(btnPin, INPUT);
  pinMode(btn2Pin, INPUT);
  pinMode(buzzPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  lastbtn1 = btn1State;
  lastbtn2 = btn2State;
  btn1State = digitalRead(btnPin);
  btn2State = digitalRead(btn2Pin);
  if (btn2State != lastbtn2) {
    if (btn2State == HIGH) {
      if (mode == 1) {
        //mode 2
        mode++;
        lcd.clear();
        lcd.print("distance (left)");
      } else if (mode == 2) {
        //mode 3
        mode++;
        lcd.clear();
        lcd.print("distance (right)");
      } else if (mode == 3) {
        //mode 1
        mode = 1;
        lcd.clear();
        lcd.print("speed");
      }
    }
  }
  if (btn1State != lastbtn1) {
    if (btn1State == HIGH) {
      if (mode == 2) {
        distance_detector(darkTrigPin);
      } else if (mode == 3) {
        distance_detector(lightTrigPin);
      } else if (mode == 1) {
        speed_detector();
      }
    }
  }
}

void distance_detector(int trig) {
  tone(buzzPin, 1000);
  delay(100);
  noTone(buzzPin);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(trig + 1, HIGH);
  distance = (duration * .0343) / 2;
  if (cm == false) {
    distance /= 2.54;
  }
  lcd.clear();
  if (distance > 1000) {
    lcd.clear();
    lcd.print("invalid reading");
  } else {
    lcd.clear();
    lcd.print(distance);
    lcd.print("cm");
  }
}

void speed_detector() {
  lcd.clear();
  lcd.print("detecting...");
  tone(buzzPin, 1000, 100);
  ms = 0;
  while (true) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration * .0343) / 2;
    if (timer) {
      delay(4);
    }
    if (distance < 50) {
      if (timer == false) {
        timer = true;
        ms = millis();
        trigPin = darkTrigPin;
        echoPin = darkEchoPin;
        lcd.clear();
        lcd.print("S point triggered");
      } else {
        timer = false;
        ms = millis() - ms;
        trigPin = lightTrigPin;
        echoPin = lightEchoPin;
        lcd.clear();
        lcd.print(cm / (ms / 36.0), 2);
        Serial.println(distance);
        lcd.print(" kmh");
        break;
      }
    }
  }
}