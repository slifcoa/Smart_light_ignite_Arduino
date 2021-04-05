#include <SoftwareSerial.h>
#include <Servo.h> //Servo Library

SoftwareSerial sw(2, 3); // Rx, Tx

bool ledOn = false;

//RGB Light Pins
const int BLUE_PIN = 4;
const int GREEN_PIN = 5;
const int RED_PIN = 6;

//Servo Pins
const int SERVO_PIN = 13;
Servo servo1;

const char ledOnMsg[3] = "ON";
const char ledOffMsg[4] = "OFF";
const char boxOpenMsg[5] = "OPEN";
const char boxCloseMsg[6] = "CLOSE";

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("Initializing Arduino");

    pinMode(BLUE_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(RED_PIN, OUTPUT);
    sw.begin(115200);

    servo1.attach(SERVO_PIN);
    spinServo(false);

}

void loop() {
  // put your main code here, to run repeatedly:
     if (sw.available() > 0) {
      char bfr[51];
      memset(bfr,0, 51);
      size_t sizey = sw.readBytes(bfr,50);
      Serial.println(bfr);
      Serial.print("Size:");
      Serial.println(sizey);

      if (strcmp(ledOnMsg, bfr) == 0){
        Serial.println("Enabling Light");
        ledOn = true;
      }

      if(strcmp(ledOffMsg, bfr) == 0) { 
        Serial.println("Disabling Light");
        ledOn = false;
       }

      if (strcmp(boxOpenMsg, bfr) == 0){
        Serial.println("Opening Box");
        spinServo(true);
      }

      if(strcmp(boxCloseMsg, bfr) == 0) { 
        Serial.println("Closing Box");
        spinServo(false);
       }
   }
  updateLights();
}

void updateLights() {
      if(ledOn){
    showSpectrum();   
    }else {
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(RED_PIN, LOW);
    }
  }

 void spinServo(bool open) {

    int position;

    if(open) {
        for(position = 20; position < 110; position += 2){
          servo1.write(position);
          delay(20);
      }
    } else {
       for(position = 110; position >= 20; position -=1) {
        servo1.write(position);
        delay(20);
      } 
    }
  }

void showSpectrum()
{
  int x;  // define an integer variable called "x"
  
  // Now we'll use a for() loop to make x count from 0 to 767
  // (Note that there's no semicolon after this line!
  // That's because the for() loop will repeat the next
  // "statement", which in this case is everything within
  // the following brackets {} )

  for (x = 0; x < 768; x++)

  // Each time we loop (with a new value of x), do the following:

  {
    showRGB(x);  // Call RGBspectrum() with our new x
    //delay(10);   // Delay for 10 ms (1/100th of a second)
  }
}

void showRGB(int color)
{
  int redIntensity;
  int greenIntensity;
  int blueIntensity;

  // Here we'll use an "if / else" statement to determine which
  // of the three (R,G,B) zones x falls into. Each of these zones
  // spans 255 because analogWrite() wants a number from 0 to 255.

  // In each of these zones, we'll calculate the brightness
  // for each of the red, green, and blue LEDs within the RGB LED.

  if (color <= 255)          // zone 1
  {
    redIntensity = 255 - color;    // red goes from on to off
    greenIntensity = color;        // green goes from off to on
    blueIntensity = 0;             // blue is always off
  }
  else if (color <= 511)     // zone 2
  {
    redIntensity = 0;                     // red is always off
    greenIntensity = 255 - (color - 256); // green on to off
    blueIntensity = (color - 256);        // blue off to on
  }
  else // color >= 512       // zone 3
  {
    redIntensity = (color - 512);         // red off to on
    greenIntensity = 0;                   // green is always off
    blueIntensity = 255 - (color - 512);  // blue on to off
  }

  // Now that the brightness values have been set, command the LED
  // to those values

  digitalWrite(RED_PIN, redIntensity);
  digitalWrite(BLUE_PIN, blueIntensity);
  digitalWrite(GREEN_PIN, greenIntensity);
}
