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
  motorr.setSpeed(255);
  motorl.run(FORWARD);
  motorr.run(FORWARD);

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
  
  //while the lux of both sensors above limit calibrated to table surface:
  //move wheels same speed (straight) as robot is on course
  //if during this course, one of the sensors detects lux drop (color absorbs more light)
  //slow down the wheel on the same side, to steer back to course



  //PATH FINDING #2 (Awesome one that will allow us to read tags (black tape communicating spin, end, or region))
  // 1 - Drive forward (state: both sensors see white)
  //   Until - Sensor sees colour (trail) - Go to 2
  //   Until - Sensor sees black (tags) - Go to 3

  // 2a - (state: one sensor on path, veered too far to side, need to reallign)
  //   Lock wheel of sensor (so sensor stays still), move other wheel forward until other sensor on line - Go to 2b
  //   (state: now very skewed on line, we know how skewed)
  // 2b - (state: now very skewed on line, we know how skewed)
  //   Preprogrammed movement to reallign facing along trail. This would be same movement of wheel every time since we were in known messy state
  //   If came from 1 (no tags), go to 1
  //   If came from 3 (tags), go to 4
  // 3 - (state: just saw black of a tag)
  //   Continue forward (or slight turning, would need to try some things) so sensor finds line quickly. Go to 2 (with var set so goes to 4 after)
  //   (The point: Reallign so nicely centered on line for reading tags)
  // 4 - (state: just realligned, maybe on tags, want to read tags)
  //   Drive back a bit (to make sure behind tags we want to read)
  //   Drive forward reading tags
  //   Go to 1 (or do something based on tag)






  


    // Now turn off motor
    motorl.run(RELEASE);
    motorr.run(RELEASE);
    delay(1000);
}