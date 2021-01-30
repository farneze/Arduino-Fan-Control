//sketch created by Akshay Joseph

// Needs to:
// - Install teensy library
// - Manually include LiquidCrystal_I2C.h into library folder
// - Include Paul's enconder

#include <Encoder.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define ledPin 17
#define BLANK "                "

#define FAN_SENSE_PIN 0
#define FAN_PWM_PIN 5
#define FAN 0
#define RPM 1

#define ENC_PIN_A 21
#define ENC_PIN_B 20
#define ENC_STEP 5

#define FAN_START 10
#define FAN_MAX_VALUE 100

LiquidCrystal_I2C lcd(0x27, 16, 2);

Encoder myEnc(ENC_PIN_A, ENC_PIN_B);

int oldPosition  = 0;

unsigned int fanPercent = 0, rpm = 0;

unsigned long timeold = 0;
volatile byte half_revolutions = 0;

byte zeroPerc[8] = {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000};
byte twentyPerc[8] = {0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000};
byte fourtyPerc[8] = {0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000};
byte sixtyPerc[8] = {0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100};
byte eightyPerc[8] = {0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110};
byte hundredPerc[8] = {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};

void setup()
{
  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(FAN_SENSE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FAN_SENSE_PIN), revolutions, RISING);

  delay(2000);     
  Serial.begin(9600);
  Serial.println("Initializing...");

  

  analogWrite(FAN_PWM_PIN, 255);
  
  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, zeroPerc);
  lcd.createChar(1, twentyPerc);
  lcd.createChar(2, fourtyPerc);
  lcd.createChar(3, sixtyPerc);
  lcd.createChar(4, eightyPerc);
  lcd.createChar(5, hundredPerc);
  lcd.clear();
  lcd.print("Initializing...");

  delay(2500);

  myEnc.write(FAN_START);
  
  analogWrite(FAN_PWM_PIN, 255 * FAN_START / 100);
  
  lcdPrintFanPercSpeed(FAN_START, FAN);
  lcdPrintBarLine(FAN_START);
}

void loop() {

  int newPosition = myEnc.read() * ENC_STEP;

  if (newPosition <= 0 || ( newPosition < 10 && oldPosition > newPosition )) myEnc.write(0);
  if (newPosition > 0 && newPosition < 10 && oldPosition < newPosition ) myEnc.write(FAN_START / ENC_STEP);

  if (newPosition > FAN_MAX_VALUE) myEnc.write(FAN_MAX_VALUE / ENC_STEP);

  
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    fanPercent = newPosition;
    
    lcdPrintFanPercSpeed(fanPercent, rpm);
    
    lcdPrintBarLine(fanPercent);

    long dutyCycle = 255 * fanPercent / 100 ;
    analogWrite(FAN_PWM_PIN, dutyCycle);    

    //if (newPosition == 0) lcdPrintFanPercSpeed(0, rpm);
  }
  
   if (half_revolutions >= 40) { 
     //Update RPM every 20 counts, increase this for better RPM resolution,
     //decrease for faster update
     rpm = 30*1000/(millis() - timeold)*half_revolutions;
     timeold = millis();
     half_revolutions = 0;
     lcdPrintFanPercSpeed(fanPercent, rpm);
   } 
   if (millis() > timeold + 2500 && millis() < timeold + 2600){
     lcdPrintFanPercSpeed(0, 0);
   }
   
}

void revolutions()
{
  half_revolutions++;
  //Each rotation, this interrupt function is run twice
}

void lcdPrintFanPercSpeed(unsigned int fanPercent, unsigned int rpm) {
  lcd.setCursor(0,0);
  lcd.print(BLANK);
  lcd.setCursor(1,0);

  if (fanPercent <= 0) {
    lcd.print("OFF");
  } else if (fanPercent >= 100) {
    lcd.setCursor(0,0);
    lcd.print(fanPercent);
    lcd.print("%");
  } else {
    lcd.print(fanPercent);
    lcd.print("%");
  }
  
  if (rpm >= 1000) {
    lcd.setCursor(8,0);
    lcd.print(rpm);
    lcd.print(" RPM");
  } else if ( rpm >= 100) {
    lcd.setCursor(9,0);
    lcd.print(rpm);
    lcd.print(" RPM");
  } else {
    lcd.setCursor(11,0);
    lcd.print("0");
    lcd.print("RPM");
  }
}

void lcdPrintBarLine(int percent) {
  long barValue = 80 * percent / 100; // 80 horizontal pixels in total, 16 chars * 5 pixels each
  int times = barValue / 5;
  int rest = barValue % 5;
  
  // Sets how many characters will appear with full pixels
  lcd.setCursor(0,1);
  lcd.print(BLANK);
  
  for (int i = 0; i < times; i++){
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
