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

#define FAN_SENSE_PIN 4
#define FAN_PWM_PIN 5
#define FAN 0
#define RPM 1

#define ENC_PIN_A 21
#define ENC_PIN_B 20
#define ENC_START 10
#define ENC_STEP 1

#define ENC_MIN_VALUE 0
#define ENC_MAX_VALUE 100


LiquidCrystal_I2C lcd(0x27, 16, 2);

Encoder myEnc(ENC_PIN_A, ENC_PIN_B);

int oldPosition  = -1;

int fanPercent = 0, rpmSpeed = 0, deltaT = 0, oldDeltaT = 0;;

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
  Serial.println("Initializing...");

  pinMode(FAN_SENSE_PIN, INPUT);
  pinMode(FAN_PWM_PIN, OUTPUT);

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

  delay(5000);

  myEnc.write(ENC_START);
  
  analogWrite(FAN_PWM_PIN, 255 * ENC_START / 100);
  
  lcdPrintFanPercSpeed(ENC_START, FAN);
  lcdPrintBarLine(ENC_START);
}

void loop() {
  int newPosition = myEnc.read() * ENC_STEP;

  if (newPosition <= 0 || ( newPosition < 10 && oldPosition > newPosition )) myEnc.write(0);
  if (newPosition > 0 && newPosition < 10 && oldPosition < newPosition ) myEnc.write(ENC_START);

  if (newPosition > ENC_MAX_VALUE) myEnc.write((ENC_MAX_VALUE) / ENC_STEP);
  //if (newPosition < ENC_MIN_VALUE) myEnc.write((ENC_MIN_VALUE - ENC_START) / ENC_STEP);
  
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    fanPercent = newPosition;
    
    lcdPrintFanPercSpeed(fanPercent, FAN);
    lcdPrintBarLine(fanPercent);

    long dutyCycle = 255 * newPosition / 100 ;
    //long dutyCycle = newPosition;
    
    Serial.println("----");
    Serial.println(newPosition);
    Serial.println(dutyCycle);
    analogWrite(FAN_PWM_PIN, dutyCycle);    
  }
}

void fanDutyCycle(){
  digitalWrite(FAN_PWM_PIN, HIGH);
  delayMicroseconds(100);
  digitalWrite(FAN_PWM_PIN, LOW);
  delayMicroseconds(1000 - 100);
}

void lcdPrintFanPercSpeed(int value, bool option) {
  lcd.setCursor(0,0);
  lcd.print(BLANK);
  lcd.setCursor(0,0);


  
  if (option == 0){
    lcd.print("Fan: ");
    if (value == 0) {
      lcd.print("OFF");
    } else {
      lcd.print(value);
      lcd.print("%");
    }
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
