#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <FS.h>
#include <SPIFFS.h>


#include <Adafruit_NeoPixel.h>

void blinkLedStrip(Adafruit_NeoPixel *strip, int n) {
  Serial.println("[INFO] Blinking LED strip");
  int i = 0;
  strip->fill();
  strip->show();
  strip->setBrightness(50);
  while (i < n) {
    Serial.println("blink!");
    i++;
    strip->fill(strip->Color(255, 255, 255));
    strip->show();
    delay(100);
    strip->fill();
    strip->show();
    delay(100);
  }
}

// Shelf LEDs
int shelfLedPin = D8;
int shelfLedCount = 120;

Adafruit_NeoPixel shelfStrip = Adafruit_NeoPixel(shelfLedCount, shelfLedPin, NEO_GRB + NEO_KHZ800);

void initializeShelfLights() {
  Serial.println("[INFO] Initializing shelf lights...");

  shelfStrip.begin();
  shelfStrip.show();
  shelfStrip.setBrightness(50);
  blinkLedStrip(&shelfStrip, 3);
}




// wifi info
const char* wifi_ssid = "Hotswag Manor";
const char* wifi_pass = "xoxo<3!!";

// server info
const int serverPort = 80;
ESP8266WebServer server(serverPort);

String header;

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

String getFilePath(String path) {
  if (path.endsWith("/")) return path + "index.html";
  if (path.indexOf(".") == -1) return path + ".html";
  return path;
}

String readFile(String path) {
  String filePath = getFilePath(path);
  Serial.println("[INFO] Servfing file " + filePath + " for route " +  path);
  if (SPIFFS.exists(filePath)) {
    File file = SPIFFS.open(path, "");
    String data = file.readString();
    file.close();
    return data;
  }
  Serial.println("[ERROR] File Not Found");
  return "";
}

void initializeServer() {
  Serial.println("[INFO] Initializaing server...");
  
  server.on("/", [](){
    String path = server.arg(0);
    Serial.println(path);
    Serial.println("[INFO] GET /");
    blinkLedStrip(&shelfStrip, 1);
    String data = readFile("index.html");
    server.send(200, "text/html", data);
  });

  server.on("/blink", [](){
    Serial.println("[INFO] GET /blink");
    blinkLedStrip(&shelfStrip, 2);
    String data = readFile("/blink.html");
    server.send(200, "text/html", data);
  });

  // server.on("/temperature", [](){
  //   Serial.println("[INFO] GET /temperature");
  //   server.sendHeader("Location", "/temperature/", true);
  //   server.send(302, "text/plain", "");
  // });  

  // server.on("/temperature/", [](){
  //   Serial.println("[INFO] GET /temperature/");
  //   server.send(200, "application/json", String(temperature));
  // });
  
  server.begin();
  
  Serial.print("[INFO] Listening on port ");
  Serial.println(serverPort);
}

void setup() {
  Serial.begin(9600);

  initializeWifi();
  initializeServer();

  initializeShelfLights();
  
  server.begin();
}

void loop() {
  server.handleClient();
}
