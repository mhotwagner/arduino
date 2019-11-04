#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//radio info
RF24 radio(D3, D4);
const byte address[6] = "00001";


// wifi info
//const char* wifi_ssid = "Hotswag Manor";
//const char* wifi_pass = "xoxo<3!!";

const char* wifi_ssid = "Castle Black";
const char* wifi_pass = "youknownothing";


// server info
const int serverPort = 80;
ESP8266WebServer server(serverPort);

String header;

const int ledPin = D5;

const char onString[] = "on";
const char offString[] = "off";

void blink(int n) {
  for (int i = 0; i < n;  i++) {
    Serial.println("[INFO] Blinking");
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}

void initializeRadio() {
  radio.begin();

  Serial.println("[INFO] Checking radio chip connection");
  
  if (radio.isChipConnected()) {
    Serial.println("[INFO] Chip connected");
    radio.printDetails();
  } else {
    Serial.println("[INFO] Chip NOT connected");
  }

  
  Serial.println("[INFO] Initializing radio...");
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
  Serial.println("[INFO] Radio initialized");
}

void initializeWifi() {
  Serial.print("[INFO] Connecting to ");
  Serial.print(wifi_ssid);
  Serial.println("...");
  WiFi.begin(wifi_ssid, wifi_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("[INFO] Connected");
  Serial.print("[INFO] IP: ");
  Serial.println(WiFi.localIP());
}

void initializeServer() {
  Serial.println("[INFO] Initializaing server...");
  
  server.on("/", [](){
    Serial.println("[SERVER] GET /");
    String data = "{}";
    server.send(200, "application/json", data);
  });

  server.on("/health", [](){
    Serial.println("[SERVER] GET /health");
    server.sendHeader("Location", "/health/", true);
    server.send(302, "text/plain", "");
  });

  server.on("/health/", [](){
    Serial.println("[SERVER] GET /health/");
    server.send(200, "text/plain", "OK");
  });

  server.on("/on", [](){
    Serial.println("[SERVER] GET /on");
    server.sendHeader("Location", "/on/", true);
    server.send(302, "text/plain", "");
  });

  server.on("/on/", [](){
    Serial.println("[SERVER] GET /on/");
    radio.write(&onString, sizeof(onString));
    server.send(200, "application/json", "{'on': true}");
  });

  server.on("/off", [](){
    Serial.println("[SERVER] GET /off");
    server.sendHeader("Location", "/off/", true);
    server.send(302, "text/plain", "");
  });

  server.on("/off/", [](){
    Serial.println("[SERVER] GET /off/");
    
    radio.write(&offString, sizeof(offString));
    server.send(200, "application/json", "{'on': false}");
  });

  server.begin();
  
  Serial.print("[SERVER] Listening on port ");
  Serial.println(serverPort);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  Serial.begin(9600);

  initializeRadio();
  initializeWifi();
  initializeServer();
  
  server.begin();

  //blink(3);
}

void loop() {
  server.handleClient();
//  radio.write(&onString, sizeof(onString));
//  delay(1000);
}
