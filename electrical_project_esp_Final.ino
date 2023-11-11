#include <math.h>
#include <LiquidCrystal_I2C.h>
#include <ZMPT101B.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define SENSITIVITY 500.0f
const int thermistor_output = A0;

ZMPT101B voltageSensor(A1, 50.0);

float voltage1;
double sensorValue1 = 0;
double sensorValue2 = 0;
int crosscount = 0;
int climb_flag = 0;
int val[100];
int max_v = 0;
double VmaxD = 0;
double VeffD = 0;
double Veff = 0;

const int irPin = 2;
volatile unsigned int counter = 0;
unsigned long lastTime = 0;
unsigned int rpm = 0;
String SendData = "";


const int sensorIn = A2;
int mVperAmp = 185;

double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;
bool mot = true;
#define relaymotor 8
#define vtglampov  13
#define vtglampun 11
#define currlamp 12
#define templamp 9
#define rpmlamp 10
#define tripping 7
int tripp = 0;

void setup() {
  Serial.begin(9600);
  voltageSensor.setSensitivity(SENSITIVITY);
  pinMode(irPin, INPUT);
  lcd.init();
  lcd.backlight();
  attachInterrupt(digitalPinToInterrupt(irPin), countInterrupt, RISING);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Motor Monitering");
  lcd.setCursor(0, 1);
  lcd.print(" System");
  delay(1000);
  pinMode(tripping, INPUT);
  pinMode(relaymotor, OUTPUT);
  pinMode(vtglampov, OUTPUT);
  pinMode(vtglampun, OUTPUT);
  pinMode(templamp, OUTPUT);
  pinMode(rpmlamp, OUTPUT);
  pinMode(currlamp, OUTPUT);
  digitalWrite(relaymotor, HIGH);

  digitalWrite(vtglampov, HIGH);
  digitalWrite(vtglampun, HIGH);
  digitalWrite(templamp, HIGH);
  digitalWrite(rpmlamp, HIGH);
  digitalWrite(currlamp, HIGH);

  delay(2000);

  digitalWrite(vtglampov, LOW);
  digitalWrite(vtglampun, LOW);
  digitalWrite(templamp, LOW);
  digitalWrite(rpmlamp, LOW);
  digitalWrite(currlamp, LOW);
  delay(2000);

  digitalWrite(vtglampov, HIGH);
  digitalWrite(vtglampun, HIGH);
  digitalWrite(templamp, HIGH);
  digitalWrite(rpmlamp, HIGH);
  digitalWrite(currlamp, HIGH);

  lcd.clear();

}


double getTemp() {
  // Thermistor code
  int thermistor_adc_val; double output_voltage;
  double thermistor_resistance, therm_res_ln, temperature;
  thermistor_adc_val = analogRead(thermistor_output);
  output_voltage = ((thermistor_adc_val * 5.0) / 1023.0);
  thermistor_resistance = ((5 * (12.0 / output_voltage)) - 10);
  thermistor_resistance = thermistor_resistance * 1000;
  therm_res_ln = log(thermistor_resistance);
  temperature = (1 / (0.001129148 + (0.000234125 * therm_res_ln) + (0.0000000876741 * therm_res_ln * therm_res_ln * therm_res_ln)));
  temperature = temperature - 273.15; return temperature;
}

double getVolt() {
  voltage1 = voltageSensor.getRmsVoltage();

}


double getAmp() {
  Voltage = getVPP();
  VRMS = (Voltage / 2.0) * 0.707;
  AmpsRMS = (VRMS * 1000) / mVperAmp; return AmpsRMS;
}

void loop() {
  double temp = getTemp();
  double volt = getVolt();
  double amp = getAmp();
  // AC Voltage sensor code


  // Tachometer code
  if (millis() - lastTime > 1000) {
    rpm = (counter * 60) / 7;
    counter = 0;
    lastTime = millis();

  }
  VmaxD = 0;


  // ****************************************************************************************

  if (mot) {

    // relay temp the final

    if (temp > 60)
    {
      digitalWrite(relaymotor, LOW);
      mot = false;
      digitalWrite(templamp, LOW);
      delay(100);
    }

    else if (temp < 60 && temp > 40)
    {
      digitalWrite(templamp, LOW);
      delay(100);
    }
    else
    {
      digitalWrite(templamp, HIGH);
    }

    // for rpm

    if (rpm < 300)
    {
      digitalWrite(relaymotor, LOW);
      mot = false;
      digitalWrite(rpmlamp, LOW);
      delay(100);

    }

    else
    {
      digitalWrite(rpmlamp, HIGH);
    }


    // for voltage digitalWrite(vtglampov, HIGH);  digitalWrite(vtglampun, HIGH);
    if (voltage1 > 270)
    {
      digitalWrite(relaymotor, LOW);
      digitalWrite(vtglampov, LOW);
       mot = false;
      delay(100);

    }
    else if (voltage1 < 270 && voltage1 > 220)
    {
      digitalWrite(vtglampov, HIGH);
    }

    else if (voltage1 < 190)
    {
      digitalWrite(relaymotor, LOW);
      mot = false;
      digitalWrite(vtglampun, LOW);
    }
    else if (voltage1 > 190 && voltage1 < 210)
    {
      digitalWrite(vtglampun, LOW);
    }

    else
    {
      digitalWrite(vtglampun, HIGH);
      digitalWrite(vtglampov, HIGH);
    }



    // relay for current
    if (amp > 6)
    {
      digitalWrite(relaymotor, LOW);
      mot = false;
       digitalWrite(currlamp, LOW);
    }
    else if (amp >= 3 && amp < 5)
    {
      digitalWrite(currlamp, LOW);
    }
    else {
      digitalWrite(currlamp, HIGH);
    }


  }
  // ****************************************************************************************

  else if (mot == false)
  {
    lcd.clear();
    while (mot == false)
    {

      tripp = digitalRead(tripping);
      if (tripp == 1)
        mot == true;
      lcd.setCursor(0, 0);
      lcd.print("Motor tripped");
    }
  }


  String tem = String(temp);
  String vol = String(voltage1);
  String cur = String(amp);
  String spd = String(rpm);

  //disp param
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.setCursor(2, 0);
  lcd.print("    ");
  lcd.setCursor(2, 0);
  lcd.print(vol);

  lcd.setCursor(8, 0);
  lcd.print("I:");
  lcd.setCursor(10, 0);
  lcd.print("    ");
  lcd.setCursor(10, 0);
  lcd.print(cur);

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.setCursor(2, 1);
  lcd.print("    ");
  lcd.setCursor(2, 1);
  lcd.print(tem);

  lcd.setCursor(8, 1);
  lcd.print("R:");
  lcd.setCursor(10, 1);
  lcd.print("    ");
  lcd.setCursor(10, 1);
  lcd.print(spd);





  //SendData="{\"tem\":"+tem+",\"vol\":"+vol+",\"cur\":"+cur+",\"spd\":"+spd+"}";
  //Serial.println(SendData);

  String dataString = "a:" + String(voltage1, 2) + ",b:" + String(amp, 2) + ",c:" + String(rpm, 2) + ",d:" + String(temp, 2);

  //String dataString = String(voltage1, 2) + "," + String(amp, 2) + "," + String(rpm, 2) + "," + String(temp, 2);
  Serial.println(dataString);





  //  Serial.println(voltage1);
  //  Serial.println(amp);
  //  Serial.println(rpm);
  //  Serial.println(temp);
  delay(1000);



}

void countInterrupt() {
  counter++;
}

float getVPP() {
  float result;
  int readValue;
  int maxValue = 0;
  int minValue = 1024;

  uint32_t start_time = millis();
  while ((millis() - start_time) < 1000) {
    readValue = analogRead(sensorIn);
    if (readValue > maxValue) {
      maxValue = readValue;
    }
    if (readValue < minValue) {
      minValue = readValue;
    }
  }

  result = ((maxValue - minValue) * 5.0) / 1024.0;
  return result;
}
