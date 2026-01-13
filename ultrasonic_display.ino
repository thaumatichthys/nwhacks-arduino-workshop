/*
  Ultrasonic sensor with Grove RGB LCD display

  Install the Grove-LCD RGB Backlight library from the library manager.
 */


#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;


#define TRIG_PIN 12
#define ECHO_PIN 13
#define SPEED_OF_SOUND_CM_US (0.0345f) // speed of sound in centimeters per microsecond

void setup() {
  // initialize serial port
  Serial.begin(9600);

  // initialize the grove lcd
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("LCD + ultrasonic");

  // configure the trigger pin to output mode
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT);

  // this is to make a virtual power pin. this is just to simplify wiring; it is not recommended in an actual application!
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);
}

void setLCDHue(float distance_cm) {
  // note: this is a block of gpt so dont worry about understanding it
  // map distance to hue 

  float hue = (distance_cm / 20);
  if (hue > 1.f) hue = 1.f;

  float t = 6.28318530718f * hue;
  
  float rf = 0.5f + 0.5f * cos(t);
  float gf = 0.5f + 0.5f * cos(t - 2.09439510239f); // -120°
  float bf = 0.5f + 0.5f * cos(t + 2.09439510239f); // +120°
  
  // scale to 0..255 with clipping
  int ri = (int)(rf * 255.0f + 0.5f);
  int gi = (int)(gf * 255.0f + 0.5f);
  int bi = (int)(bf * 255.0f + 0.5f);
  
  if (ri < 0) ri = 0; else if (ri > 255) ri = 255;
  if (gi < 0) gi = 0; else if (gi > 255) gi = 255;
  if (bi < 0) bi = 0; else if (bi > 255) bi = 255;

  lcd.setRGB(ri, gi, bi);
}

void loop() {
  // transmit a 10us pulse to TRIG_PINar
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // record the pulse length of the echo pin
  float length_us = pulseIn(ECHO_PIN, HIGH);


  // calculate the distance
  float distance_cm = length_us * SPEED_OF_SOUND_CM_US / 2; // divide by two because forward and return path

  // report value via serial port
  Serial.print("Distance in cm: "); // unfortunately Arduino does not natively have printf format specifier
  Serial.println(distance_cm);

  // also report the value on the lcd
  // first clear the lcd
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(distance_cm);

  // set lcd backlight colour
  setLCDHue(distance_cm);

  // don't need to run this too often
  delay(50); // 500 millisecond delay
}
