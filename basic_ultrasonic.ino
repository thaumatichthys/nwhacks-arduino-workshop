/*
  Basic ultrasonic sensor - Print to serial port
*/

#define TRIG_PIN 12
#define ECHO_PIN 13
#define SPEED_OF_SOUND_CM_US (0.0345f) // speed of sound in centimeters per microsecond

void setup() {
  // initialize serial port
  Serial.begin(9600);

  // configure the trigger pin to output mode
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT);

  // this is to make a virtual power pin. this is just to simplify wiring; it is not recommended in an actual application!
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);
}

void loop() {
  // transmit a 10us pulse to TRIG_PIN
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

  // don't need to run this too often
  delay(500); // 500 millisecond delay
}
