//sketch created by Akshay Joseph

// Needs to install teensy library
// Needs to manually include LiquidCrystal_I2C.h into library folder

#include <Encoder.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Encoder myEnc(21, 20);

long oldPosition  = -999;

void setup()
{
  delay(2000);     
  Serial.begin(9600);
  Serial.println("FanControl ProtoType");
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Test");
}

void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
}


/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */





// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

//   avoid using pins with LEDs attached

void setup() {

}


}
