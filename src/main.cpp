#include <Arduino.h>
#include <LittleFS.h>
#include <FS.h>
#include <AccelStepper.h>

#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager

#include "ctrlpage.h"

const float stepsRPM = 4096 / 60;

// ULN2003 Motor Driver
#define IN1 12 // D6 is GPIO 12
#define IN2 14 // D5 is GPIO 14
#define IN3 4  // D2 is GPIO 4
#define IN4 5  // D1 is GPIO 5
#define LED 2

AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);
ESP8266WebServer server(80); // server on port 80

String ehost = "Plan_Eclip_Motor"; // for filesystem
float rate = 6.0; //rpm
float spd = rate * stepsRPM;
float onfactor = 0.0;  // off
float dirfactor = 1.0; // clockwise

void docsection(String title) {
  Serial.println();
  Serial.println(title);
}

void docstr(String lbl, String val) {
  Serial.print("  ");
  Serial.print(lbl);
  Serial.print(": ");
  Serial.println(val);
}

void doccstr(String lbl, String val) {
  Serial.print("  ");
  Serial.print(lbl);
  Serial.print(": ");
  Serial.println(val.c_str());
}

void docflt(String lbl, float val, String units) {
  Serial.print("  ");
  Serial.print(lbl);
  Serial.print(": ");
  Serial.print(val);
  Serial.print(" ");
  Serial.print(units);
  Serial.println();
}

void handleServer() {
  String z = ctrlpage;
  server.send(200, "text/html", z); // show control panel
}

void handleRate() {
  String Arg = server.arg("Rate");
  rate = Arg.toFloat();
  spd = rate * stepsRPM;
  stepper.setSpeed(onfactor * dirfactor * spd);
  
  docflt("New Rate: ", rate, "rpm");
  server.send(200, "text/plane","");
}
void handleOnOff() {
  String Arg = server.arg("OnOff");
  docstr("onoff param",Arg);
  if (Arg == "O") {
    onfactor = 1.0;
  } else {
    onfactor = 0.0;
  }
  stepper.setSpeed(onfactor * dirfactor * spd);
  server.send(200, "text/plane","");
}
void handleDir() {
  String Arg = server.arg("Dir");
  docstr("dir param",Arg);
  if (Arg == "L") {
    dirfactor = -1.0;
  } else {
    dirfactor = 1.0;
  }
  stepper.setSpeed(onfactor * dirfactor * spd);
  server.send(200, "text/plane","");
}

//..........................................
// Setup Function
//..........................................

void setup() {
  Serial.begin(115200);
  delay(800); // wait for serial port to be ready
  Serial.println("...");
  Serial.print(ehost);
  Serial.println(" v1.0");
  Serial.println("--- Disconnect previously connected WiFi");
 
  // Configure WiFi
  WiFiManager wifiManager;
 
  // Uncomment and run it once, if you want to erase all the stored information
  //wifiManager.resetSettings();

  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("AutoAPServo");
  
  
   // mDNS Config
    WiFi.setHostname(ehost.c_str());
    if (MDNS.begin(ehost.c_str())) {  //Start mDNS

      Serial.print("MDNS started hostname: ");
      Serial.print(ehost.c_str());
      Serial.println(".lan or .local");
    }

    // init the webserver
    server.on("/",handleServer);
    server.on("/setRate",handleRate);
    server.on("/setOnOff",handleOnOff);
    server.on("/setDir",handleDir);
    server.begin();
    Serial.println("... webserver started we hope");

    stepper.setMaxSpeed(1000.0);
    docflt("Speed", rate, "rpm");
    stepper.setSpeed(onfactor * dirfactor * spd);

  }

// Note that runSpeed does the next step if needed
// handleClient checks for web input
void loop() {
  server.handleClient();
  stepper.runSpeed();
  
}