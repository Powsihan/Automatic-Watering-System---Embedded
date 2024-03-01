#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,16,2);  

const int soilMoisturePin = A0; 
const int motorPin = 2;
const int wet = 80;
const int dry = 40;
const int balance = 60;
 
int sensorValue = 0;    
int outputValue = 0; 
int sensor;
int lastSensorValue;

char buf[20];

void setup()
{
  Serial.begin(9600);
  pinMode(motorPin, OUTPUT);
  lcd.init();                      
  lcd.setCursor(2,0);
  lcd.print("Soil Moisture");
  Serial.print("Soil Moisture = ");
  Serial.print(outputValue);
  Serial.println(" %");
  statusSensor ();
}


void loop()
{
  sensorValue = analogRead(soilMoisturePin);
  outputValue = map(sensorValue, 0, 1023, 100, 0);
  
  if (sensorValue != lastSensorValue)
  {
    statusSensor ();
    
    lastSensorValue = sensorValue;
  }

  if (outputValue >= balance)
  {    
    digitalWrite(motorPin, LOW);
  }

  delay(10);
}

void statusSensor ()
{
  lcd.setCursor(2,1);
  sprintf(buf, "%3d %%", outputValue);
  lcd.print(buf);
  
  if (outputValue > wet)
  {
    Serial.println("Status: Soil is too wet");
    lcd.setCursor(8,1);
    sprintf(buf, "Wet       ");
    lcd.print(buf);
  }
  
  else if (outputValue <= wet && outputValue >= dry)
  {
    Serial.println("Status: Perfect moisture");
    lcd.setCursor(8,1);
    sprintf(buf, "Perfect   ");
    lcd.print(buf);
  }
  
  else
  {
    Serial.println("Status: Soil too dry [PUMP ON]");
    digitalWrite(motorPin, HIGH);
    lcd.setCursor(8,1);
    sprintf(buf, "Dry      ");
    lcd.print(buf);
  }
}
