#include <Wire.h>
#include "Adafruit_TCS34725softi2c.h"
#include <AFMotor.h>
// You can use any digital pin for emulate SDA / SCL
#define SDApinl 10 //Servo 1
#define SCLpinl 9 //Servo 2
#define SDApinr A0 //Yellow
#define SCLpinr 13 //Orange

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();
/* Initialise with specific int time and gain values */
Adafruit_TCS34725softi2c tcsl  = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X, SDApinl, SCLpinl);
Adafruit_TCS34725softi2c tcsr  = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X, SDApinr, SCLpinr);
// Adafruit_TCS34725 tcs_r = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
const int LED_Debug = A1;
int LED_counter = 0; // Switches between 0 and 1

AF_DCMotor motorl = AF_DCMotor(4, MOTOR12_64KHZ);

AF_DCMotor motorr = AF_DCMotor(3, MOTOR12_64KHZ);




void setup(void) {
  Serial.begin(9600);
  if (tcsr.begin()) {
    Serial.println("Found right sensor");
  } else {
    Serial.println("No TCS34725 right found ... check your connections");
    while (1);
  }
  pinMode(LED_Debug, OUTPUT);
  if (tcsl.begin()) {
    Serial.println("Found left sensor");
  } else {
    Serial.println("No TCS34725 left found ... check your connections");
    while (1);
  }

  motorl.setSpeed(255);
  motorl.run(RELEASE);
  Serial.println("Left motor running");

  motorr.setSpeed(255);
  motorr.run(RELEASE);
  Serial.println("Right motor running");

  // Now we're ready to get readings!
}

void loop(void) {

  if (LED_counter == 0) {
    digitalWrite(LED_Debug, HIGH);
    LED_counter = 1;
    Serial.println("HIGH");
  } else {
    digitalWrite(LED_Debug, LOW);
    LED_counter = 0;
    Serial.println("LOW");
  }

  uint16_t rl, gl, bl, cl, colorTempl, luxl;
  uint16_t rr, gr, br, cr, colorTempr, luxr;
  tcsl.getRawData(&rl, &gl, &bl, &cl);
  tcsr.getRawData(&rr, &gr, &br, &cr);
  
  luxl = tcsl.calculateLux(rl, gl, bl);
  luxr = tcsr.calculateLux(rr, gr, br);

  Serial.println(" \n Left:");
  Serial.print("Lux: "); Serial.print(luxl, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(rl, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(gl, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(bl, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(cl, DEC); Serial.print(" ");
  Serial.println(" \n Right:");
  Serial.print("Lux: "); Serial.print(luxl, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(rr, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(gr, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(br, DEC); Serial.print(" ");

  //motor stuff
  uint8_t i;

    // Turn on motor
    motorl.run(FORWARD);
    motorr.run(FORWARD);

    // Accelerate from zero to maximum speed
    for (i=0; i<255; i++)
    {
       motorl.setSpeed(i); 
       motorr.setSpeed(i);
       delay(10);
    }

   // Decelerate from maximum speed to zero
    for (i=255; i!=0; i--)
    {
       motorl.setSpeed(i); 
       motorr.setSpeed(i);
       delay(10);
    }
    // Now turn off motor
    motorl.run(RELEASE);
    motorr.run(RELEASE);
    delay(1000);
}