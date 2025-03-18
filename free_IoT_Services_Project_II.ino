         /////////////////////////////////////////////  
        //     IoT RGB Color Detector via          //
       //     TheAmplituhedron IoT Services       //
      //          ---------------------          //
     //           NodeMCU (ESP-12E)             //           
    //           by Kutluhan Aktar             // 
   //                                         //
  /////////////////////////////////////////////

// By merely subscribing to TheAmplituhedron, you will be able get your data packets from NodeMCU, or any other device that can make an HTTP GET Request, to your IoT Dashboard on your account page.
// TheAmplituhedron IoT Services is an available system for TheAmplituhedron.com subscribers only, and simple to use.
// Follow the steps down below to create and communicate your connection path on which you will display your data packets in every 2 seconds.
// 1) Go to your Dashboard.
// 2) Click Iot Services under Available Systems.
// 3) Read the given instructions and click Create+
// 4) Copy the url to use in the souce code.
// After creating your unique connection path, you can send data packets through NodeMCU by entering your WiFi settings and required information down below.
//
// As a reminder, my website has SSL protection so that you need to identify your connection request by entering TheAmplituhedron FingerPrint or ThumbPrint.
// You can learn about it more from the link below.
// https://www.theamplituhedron.com/dashboard/IoTServices/
//
// By taking the following steps, you will learn hoaw to send colors in RGB format from NodeMCU to your IoT Dashboard at TheAmplituhedron.ocm.
// Also, it is the second demonstration project for TheAmplituhedron IoT Serivces, programmed by myself, to give a new perspective on Internet of Things.
// I used a TCS3200 Color Sensor to detect colors in RGB format, and added a control led to show whether WiFi connection is active or not.
//
// Connections
// NodeMCU (ESP-12E) :           
//                                Control Led
// D1  --------------------------- +
//                                TCS3200 Color Sensor
// D2  --------------------------- S0
// D3  --------------------------- S1
// D4  --------------------------- S2
// D5  --------------------------- S3
// D6  --------------------------- OUT


// Include required libraries to send data to a website, in this case TheAmplituhedron.com.
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

// Define your WiFi settings.
const char *ssid = "WiFi_SSID";
const char *password = "WiFi_PASSWORD";

// Define a control pin for WiFi test.
#define control D1

// Define TCS3200 input and output pins to collect color code data.
#define S0 D2
#define S1 D3
#define S2 D4
#define S3 D5
#define OutPin D6

// Create data holders to send data packets.
int colorCode[3];
String connectionPath, USERNAME, HEDRON, data_name_1, data_name_2, data_name_3, data_1, data_2, data_3;

void setup() {

  // Activate control led.
  pinMode(control, OUTPUT);
  digitalWrite(control, LOW);

  // Activate TCS3200 input and output pins.
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OutPin, INPUT);

  // Set frequency scaling for specific development board. In this demonstration, it is set to 100%.
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);
  
  // Wait until connected.
  delay(1000);
  Serial.begin(115200);
  // It is just for assuring connection is alive.
  WiFi.mode(WIFI_OFF);
  delay(1000);
  // This mode allows NodeMCU to connect any WiFi directly.
  WiFi.mode(WIFI_STA);        
  // Connect NodeMCU to your WiFi.
  WiFi.begin(ssid, password);
  
  Serial.print("\n\n");
  Serial.print("Try to connect to WiFi. Please wait! ");
  Serial.print("\n\n");
  // Halt the code until connected to WiFi.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }

  // If connection is successful, turn control led on and write WiFi SSID to serial monitor along with assigned IPAddress.
  digitalWrite(control, HIGH);
  Serial.print("\n\n");
  Serial.print("-------------------------------------");
  Serial.print("\n\n");
  Serial.print("Connection is successful!");
  Serial.print("\n\n");
  Serial.print("Connected WiFi SSID : ");
  Serial.print(ssid);
  Serial.print("\n\n");
  Serial.println("Connected IPAddress : ");
  Serial.println(WiFi.localIP());
  Serial.print("\n\n");

}

void loop() {
  // Get Color Code in array format to send it to IoT Dashboard.
  getRGBColorCode(50);
  
  // Create HTTP object to make a request.
  HTTPClient http;    
 
  // Enter your connection path variables and define a name and a value for each data packet below - up to 6 entry.
  USERNAME = "Account_Username";
  HEDRON = "Account_Hedron";
  // You can change these names if you want.
  data_name_1 = "Red";
  data_name_2 = "Green";
  data_name_3 = "Blue";
  data_1 = String(colorCode[0]);
  data_2 = String(colorCode[1]);
  data_3 = String(colorCode[2]);
  
  connectionPath = "https://www.theamplituhedron.com/dashboard/IoTServices/" + USERNAME + "-" + HEDRON + ".php?dataNameFirst=" + data_name_1 + "&dataNameSecond=" + data_name_2 + "&dataNameThird=" + data_name_3 + "&data1=" + data_1 + "&data2=" + data_2 + "&data3=" + data_3;
  
  http.begin(connectionPath, "xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx");
  
  int httpCode = http.GET();           
  String payload = http.getString();    
 
  Serial.println(httpCode); 

  // End HTTP Get Request.
  http.end(); 
  
  // Send data packets every 2 seconds to TheAmplituhedron.
  delay(2000);
}

// Get RBG Color Code from TCS3200 Color Sensor in red, blue and green as array.
void getRGBColorCode(int Delay){
  // Activate Red Filters on the sensor, and save this data in colorCode[0].
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Read the frequency and save it.
  colorCode[0] = pulseIn(OutPin, LOW);
  // Represent the frequency in the RGB Model according to your sensor's frequency values at the given scaling value, and the brightness as well.
  colorCode[0] = map(colorCode[0], 275, 24, 255, 0);
  delay(Delay);

   // Activate Green Filters on the sensor, and save this data in colorCode[1].
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Read the frequency and save it.
  colorCode[1] = pulseIn(OutPin, LOW);
  // Represent the frequency in the RGB Model according to your sensor's frequency values at the given scaling value, and the brightness as well.
  colorCode[1] = map(colorCode[1], 396, 80, 255, 0);
  delay(Delay);

   // Activate Blue Filters on the sensor, and save this data in colorCode[2].
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Read the frequency and save it.
  colorCode[2] = pulseIn(OutPin, LOW);
  // Represent the frequency in the RGB Model according to your sensor's frequency values at the given scaling value, and the brightness as well.
  colorCode[2] = map(colorCode[2], 300, 37, 255, 0);
  delay(Delay);

  }


