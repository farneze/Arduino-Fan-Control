/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

// Needs to install teensy library

#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(21, 20);
//   avoid using pins with LEDs attached

long encStartValue = 10;

void setup() {
  delay(2000);     
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");
  myEnc.write(encStartValue);
}

long oldPosition  = -1;

void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
}
