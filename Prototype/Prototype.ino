//sketch created by Akshay Joseph

// Needs to install teensy library
// Needs to manually include LiquidCrystal_I2C.h into library folder

#include <Encoder.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Encoder myEnc(21, 20);

long oldPosition  = -999;
int percentage = 0;

char message[6] = "hello";

byte zeroPerc[8] = {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000};
byte twentyPerc[8] = {0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000};
byte fourtyPerc[8] = {0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000};
byte sixtyPerc[8] = {0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100};
byte eightyPerc[8] = {0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110};
byte hundredPerc[8] = {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};

int sixty = 0;
void setup()
{
  delay(2000);     
  Serial.begin(9600);
  Serial.println("FanControl ProtoType");
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
  long newPosition = myEnc.read();
  if (newPosition > 100) newPosition = 100;
  if (newPosition < 0) newPosition = 0;
  percentage = 100*newPosition/90;
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.print("New: ");
    Serial.println(newPosition);
    Serial.print("Perc: ");
    Serial.println(newPosition);
    lcdbar(newPosition);
    
  }
}

void lcdbar(long pos) {
  long barPercentage = 80*pos/100;
  int times = barPercentage/5;
  int rest = barPercentage%5;
  String percentageText = "Fan: " + pos;
  percentageText = percentageText + "%";
  lcd.clear();
  lcd.print(percentageText);
  
  for (int i = 0; i<times;i++){
    lcd.setCursor(i,1);
    lcd.write(5);
  }
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
