#include<Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.


const unsigned long UPDATE_INTERVAL = 1000;  // Update interval in milliseconds

unsigned int pulseCount = 0;  // Number of pulses received
unsigned long lastUpdateTime = 0;  // Last time the count was updated

const int IR_PIN = 12;  // Pin connected to IR sensor
const int relay=13;
const int LM=A0;
const int volt=A1;
const int current=A2;
const int voltout=1;
const int currentout=3;
const int speedout=4;
const int LMOUT=0;

void setup() {
Serial.begin(115200);
lcd.init();
lcd.backlight();

 pinMode(IR_PIN, INPUT_PULLUP);  // Set IR pin as input with internal pull-up resistor

lcd.begin(16,2);
lcd.setCursor(0,0);
lcd.print("Motor Monitering");
lcd.setCursor(0,1);
lcd.print("System");
delay(2000);
lcd.print("Welcome");
lcd.clear();
}

void loop() {
  
  // For Temperature
  float lmvalue=analogRead(LM);
  int tempr=(lmvalue * 500)/1023;
  
  if (tempr > 50){
    TempFun(tempr);
  }
  else {
  digitalWrite(LMOUT,LOW);
}



// for current
  int avgcurrent=0;
  for (int i=0; i<1000; i++){
    avgcurrent = avgcurrent + (0.0264 * analogRead(current) - 13.51)/1000;
  }
  
  if (avgcurrent > 3){
    CurrentFun(avgcurrent);
  }
else {
digitalWrite (currentout,LOW);
}

//for speed
unsigned long currentTime = millis();  // Get current time
  unsigned int currentPulseCount = 0;  // Number of pulses received during the update interval

  while (millis() - currentTime < UPDATE_INTERVAL) {
    if (digitalRead(IR_PIN) == LOW) {
      currentPulseCount++;  // Increment pulse count if a pulse is detected
      while (digitalRead(IR_PIN) == LOW) {
        // Wait until the pulse ends
      }
    }
  }

  float revolutionsPerSecond = currentPulseCount / float(UPDATE_INTERVAL) * 1000.0 / 2.0;  // Calculate revolutions per second (RPS)
  float revolutionsPerMinute = revolutionsPerSecond * 60.0;  // Calculate revolutions per minute (RPM)

  Serial.print("RPM: ");
  Serial.println(revolutionsPerMinute);
  int rpm = revolutionsPerMinute;

  

//for voltage   
float value = analogRead(volt);
int voltage= ((value/204.8)*100)/5;

if (voltage>80){
  VoltFun(voltage);
}
else{
  digitalWrite(voltout,LOW);
}
// To Display all Parameters

  lcd.setCursor(0,0);
   lcd.print("TMP=");
  lcd.setCursor(5,0);
  lcd.print(tempr);
  
  lcd.setCursor(9,0);
  lcd.print("AMP=");
  lcd.setCursor(13,0);
  lcd.print(avgcurrent);
  
  lcd.setCursor(0,1);
  lcd.print("RPM=");
  lcd.setCursor(5,1);
  lcd.print(rpm);
  
  lcd.setCursor(9,1);
  lcd.print("VOLT="); 
  lcd.setCursor(13,1);
  lcd.print(voltage);

voltage = 11;
avgcurrent = 22;
tempr = 33;
rpm = 44; 

   // Create a string with sensor values
  String sensorData = "a" + String(voltage) + "," + "b" + String(avgcurrent) + "," + "c" + String(tempr) + "," + "d" + String(rpm);

  // Send the sensor data to ESP32
  Serial.println(sensorData);  // Use Serial1 for ESP32 communication
  delay(2000);


} 

void TempFun(float tempr){
  //Serial.println(tempr);
  if (tempr > 100){
    digitalWrite(relay,HIGH);
   
  }
  else {
    digitalWrite(relay,LOW);
    digitalWrite(LMOUT,HIGH);
  }
 
}

void CurrentFun(float avgcurrent){
  //Serial.println(avgcurrent);
  if (avgcurrent > 5){
    digitalWrite(relay,HIGH);
    
  }
  else {
    digitalWrite(relay,LOW);
    digitalWrite (currentout,HIGH);
  }
}

void VoltFun(float voltage){
  if (voltage > 100){
    digitalWrite(relay,HIGH);
  }
  else{
    digitalWrite(relay,LOW);
    digitalWrite(voltout,HIGH);
  }
}

void SpeedFun(int rpm){
  if(rpm > 1600){
    digitalWrite(relay,HIGH);
  }
  else{
    digitalWrite(speedout,HIGH);
  }
}
