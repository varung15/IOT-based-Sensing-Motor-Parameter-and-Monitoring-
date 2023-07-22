#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define WebNme "http://tr9v3tem.000webhostapp.com/uya6d7a8bdai6bd7abiydn7had/updater.php"
#define CSIDN "IQ00 Z7 5G"
#define CPASS "1122334455"

byte RSC = 0;

String PostM(String a) {
  WiFiClient client;
  HTTPClient http;
  
  // Make HTTP POST request to the specified URL
  http.begin(client, WebNme);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  Serial.println(a);
  
  // Send the POST request with the provided data
  RSC = http.POST(a);
  
  String payload = "undefined";
  
  if (RSC > 0) {
    // If the request was successful, get the response payload
    payload = http.getString();
    Serial.println(payload);
  }
  
  http.end();
  return payload;
}

void wifiConnect() {
  Serial.println(CSIDN);
  Serial.println(CPASS);
  
  // Connect to the specified Wi-Fi network
  WiFi.begin(CSIDN, CPASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Waiting for connection...");
  }
  
  Serial.println("Connected...");
  delay(3000);
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wi-Fi Connected");
  }
}

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  wifiConnect();
  
  Serial.println(GetM("2133"));
  
  // put your setup code here, to run once:
}

String GetM(String a) {
  WiFiClient client;
  HTTPClient http;
  
  // Make HTTP GET request to the specified URL
  http.begin(client, WebNme);
  RSC = http.GET();
  
  String payload = "undefined";
  
  if (RSC > 0) {
    // If the request was successful, get the response payload
    payload = http.getString();
  }
  
  http.end();
  return payload;
}

int tempdata = 101;
int currentdata = 202;
int speeddata = 303;
int voltagedata = 404;

String senddata = "";

String receivedData;   // String to store received data
String string1, string2, string3, string4;   // Strings to store parsed values

// Initialize string variables
String stringa;
String stringb;
String stringc;
String stringd;

void loop() {
  delay(4000);
  
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');
    Serial.println("Received data: " + receivedData);
    
    char charArray[receivedData.length() + 1];
    receivedData.toCharArray(charArray, receivedData.length() + 1);

    char *token = strtok(charArray, ",");
    
    while (token != NULL) {
      String value = String(token);

      if (value.startsWith("a")) {
        stringa = value.substring(1);
      } else if (value.startsWith("b")) {
        stringb = value.substring(1);
      } else if (value.startsWith("c")) {
        stringc = value.substring(1);
      } else if (value.startsWith("d")) {
        stringd = value.substring(1);
      }

      token = strtok(NULL, ",");
    }
    
    Serial.println("String a: " + stringa);
    Serial.println("String b: " + stringb);
    Serial.println("String c: " + stringc);
    Serial.println("String d: " + stringd);
  }
   // stringa = temperature !! stringb = current !! stringc = speed !! stringd = voltage
  Serial.println(PostM(senddata + "tem=" + stringa + "&cur=" + stringb + "&spd=" + stringc + "&vol=" + stringd + ""));
  delay(2000); 

  stringa = "0";
  stringb = "0";
  stringc = "0";
  stringd = "0";  
}
