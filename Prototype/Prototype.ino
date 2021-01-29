//sketch created by Akshay Joseph

// Needs to install teensy library
// Needs to manually include LiquidCrystal_I2C.h into library folder

#include <Encoder.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Encoder myEnc(21, 20);

int oldPosition  = 0;

int fanPercent = 0, rpmSpeed = 0, deltaT = 0, oldDeltaT = 0;;

int fanSensePin = 4;
int fanPwmPin = 5;

unsigned long duration;

byte zeroPerc[8] = {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000};
byte twentyPerc[8] = {0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000};
byte fourtyPerc[8] = {0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000};
byte sixtyPerc[8] = {0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100};
byte eightyPerc[8] = {0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110};
byte hundredPerc[8] = {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};

void setup()
{
  delay(2000);     
  Serial.begin(9600);
  Serial.println("FanControl ProtoType");

  pinMode(fanSensePin, INPUT);
  pinMode(fanPwmPin, OUTPUT);

  analogWrite(fanPwmPin, 255);
  
  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, zeroPerc);
  lcd.createChar(1, twentyPerc);
  lcd.createChar(2, fourtyPerc);
  lcd.createChar(3, sixtyPerc);
  lcd.createChar(4, eightyPerc);
  lcd.createChar(5, hundredPerc);
  lcd.clear();

  
}

void loop() {
  int newPosition = myEnc.read();

  //duration = pulseIn(fanSensePin, LOW);
  //Serial.println(duration);
  
  if (newPosition > 100) newPosition = 100;
  if (newPosition < 0) newPosition = 0;
  
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    
    fanPercent = newPosition;
    lcdPrintFanPercSpeed(fanPercent, 0); // 0 for Fan, 1 for RPM
    
    lcdPrintBarLine(fanPercent);

    int dutyCycle = 255 * newPosition / 100 ;

    Serial.print("Pos: ");
    Serial.println(newPosition);
    Serial.print("Duty: ");
    Serial.println(dutyCycle);
    
    analogWrite(fanPwmPin, dutyCycle);
    
  }
  
 
  
  /*if (%100 == 0){
    
  } else if (%50==0){
    
  }*/


}

void fanDutyCycle(){
  digitalWrite(fanPwmPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(fanPwmPin, LOW);
  delayMicroseconds(1000 - 100);
}

void lcdPrintFanPercSpeed(int value, bool option) {
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  
  if (option == 0){
    lcd.print("Fan: ");
    lcd.print(value);
    lcd.print("%");
  } else {
    lcd.print("Speed: ");
    lcd.print(value);
    lcd.print("RPM");
  }
}

void lcdPrintBarLine(int percent) {
  long barValue = 80 * percent / 100;
  int times = barValue / 5;
  int rest = barValue % 5;
  
  // Sets how many characters will appear with full pixels
  lcd.setCursor(0,1);
  lcd.print("                ");
  
  for (int i = 0; i<times;i++){
    lcd.setCursor(i,1);
    lcd.write(5);
  }

  // Selects pixels of last character
  switch (rest) {
    case 0:
      lcd.setCursor(times,1);
      lcd.write(0);
      break;
    case 1:
      lcd.setCursor(times,1);
      lcd.write(1);
      break;
    case 2:
      lcd.setCursor(times,1);
      lcd.write(2);
      break;
    case 3:
      lcd.setCursor(times,1);
      lcd.write(3);
      break;
    case 4:
      lcd.setCursor(times,1);
      lcd.write(4);
      break;
    case 5:
      lcd.setCursor(times,1);
      lcd.write(5);
      break;
  }
}
