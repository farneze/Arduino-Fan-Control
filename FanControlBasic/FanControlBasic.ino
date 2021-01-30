#define FAN_SENSE_PIN 0
#define FAN_PWM_PIN 5
#define FAN_START 10
#define FAN_MAX_VALUE 100

unsigned int fanPercent = 0, rpm = 0;

unsigned long timeold = 0;
volatile byte half_revolutions = 0;


void setup()
{
  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(FAN_SENSE_PIN, INPUT_PULLUP); // important to set it with internal pullup resistor
  attachInterrupt(digitalPinToInterrupt(FAN_SENSE_PIN), revolutions, RISING);

  Serial.begin(9600);
  Serial.println("Initializing...");

  analogWrite(FAN_PWM_PIN, 255);
  
}

void loop() {

   if (half_revolutions >= 40) { 
     //Update RPM every 20 counts, increase this for better RPM resolution,
     //decrease for faster update
     rpm = 30*1000/(millis() - timeold)*half_revolutions;
     timeold = millis();
     half_revolutions = 0;
     Serial.print("RPM: ");
     Serial.println(rpm);
   } 
   if (millis() > timeold + 2500 && millis() < timeold + 2600){
     Serial.print("RPM: ");
     Serial.println(0);
   }
   
}

void revolutions()
{
  half_revolutions++;
  //Each rotation, this interrupt function is run twice
}
