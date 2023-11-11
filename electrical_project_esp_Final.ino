#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#define WebNme "http://tr9v3tem.000webhostapp.com/uya6d7a8bdai6bd7abiydn7had/updater.php"
#define CSIDN "IQ00 Z7 5G"
#define CPASS "1122334455"
byte RSC = 0;

String string1;
String string2;
String string3;
String string4;

float voltage;
float current;
float rpm;
float temp;

String PostM(String a) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, WebNme);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  Serial.println(a);
  RSC = http.POST(a);
  String payload = "undefined";
  if (RSC > 0) {
    payload = http.getString();
    Serial.println(payload);
  }
  http.end();
  return payload;
}


void wifiConnect() {
  Serial.println(CSIDN);
  Serial.println(CPASS);
  WiFi.begin(CSIDN, CPASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("??? Waiting for connection ???");
  }
  Serial.println("Connected...");
  delay(3000);
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("*** Wi-Fi Connected ***");
  }
}
void setup() {
  Serial.begin(9600);
  wifiConnect();
  Serial.println(GetM("2133"));
  // put your setup code here, to run once:
}

String GetM(String a) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, WebNme );
  // RES = 0;
  RSC = http.GET();
  String payload = "undefined";
  if (RSC > 0) {
    payload = http.getString();
    //RES = 1;
  }
  http.end();
  return payload;
}

int tempdata = 10;
int currentdata = 10;
int speeddata = 10 ;
int voltagedata = 10;

String data1 = "";
float field1;
float field2;
float field3;
float field4;
void loop() {

if (Serial.available() > 0) {
    // Read the data from Serial
    String dataString = Serial.readStringUntil('\n');
    
    // Parse the data based on labels, but only store the values
    float voltage, current, rpm, temp;
    if (sscanf(dataString.c_str(), "a:%f,b:%f,c:%f,d:%f", &voltage, &current, &rpm, &temp) == 4) {
      field1 = voltage;
      field2 = current;
      field3 = rpm;
      field4 = temp;

      // Print the received values for debugging
      Serial.print("Field 1 (a): ");
      Serial.println(field1, 2); // Print with 2 decimal places
      Serial.print("Field 2 (b): ");
      Serial.println(field2, 2);
      Serial.print("Field 3 (c): ");
      Serial.println(field3, 2);
      Serial.print("Field 4 (d): ");
      Serial.println(field4, 2);
    } else {
      // Data did not match the expected format
      Serial.println("Invalid data format");
    }
  }
  
  delay(3000);

  // Serial.println(PostM(data1 + "tem=" + String(tempdata) + "&cur=" + String(currentdata) + "&spd=" + String(speeddata) + "&vol=" + String(voltagedata) + ""));
Serial.println(PostM(data1 + "tem=" + String(field4,2) + "&cur=" + String(field2,2) + "&spd=" + String(field3,2) + "&vol=" + String(field1,2) + ""));

 // Serial.println(PostM(data1 + "tem=" + string4 + "&cur=" + string2 + "&spd=" + string3 + "&vol=" + string1 + ""));


}
