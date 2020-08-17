#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <FS.h>

#include <Adafruit_NeoPixel.h>


// Shelf LEDs
int shelfLedPin = D8;
int shelfLedCount = 180;
int shelfBrightness = 50;
int shelfr = 255;
int shelfg = 255;
int shelfb = 255;
int shelfColor = Adafruit_NeoPixel::Color(shelfr, shelfg, shelfb);

Adafruit_NeoPixel shelfStrip = Adafruit_NeoPixel(shelfLedCount, shelfLedPin, NEO_GRB + NEO_KHZ800);

// Start the shelf lights
void initializeShelfLights() {
  Serial.println("[INFO] Initializing shelf lights...");
  shelfStrip.begin();
  shelfStrip.show();
  shelfStrip.setBrightness(shelfBrightness);
  blinkLedStrip(&shelfStrip, 3);
}



class BlinkHandler : public RequestHandler {
  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith( "/blink" );
  }
  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) {    
    int n;
    if (requestUri == "/blink" || requestUri == "/blink/") {
      n = 1;
    } else {
      n = requestUri.substring(7).toInt();
    }
    blinkLedStrip(&shelfStrip, n);
    server.send(200);
    return true;
  }
} blinkHandler;

class APIHandler : public RequestHandler {
  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith("/api/");
  }
  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) {
    bool turnOn = false;
    // Handle queries
    if (server.hasArg("r")) { shelfr = server.arg("r").toInt(); turnOn = true; }
    if (server.hasArg("g")) { shelfg = server.arg("g").toInt(); turnOn = true; }
    if (server.hasArg("b")) { shelfb = server.arg("b").toInt(); turnOn = true; }
    if (server.hasArg("brightness")) { shelfBrightness = server.arg("brightness").toInt(); turnOn = true; }
    
    // Handle arguments
    String argument = requestUri.substring(5);
    if (argument.startsWith("on/")) turnOn = true;
    if (argument.startsWith("off/")) {
      shelfStrip.fill();
      shelfStrip.show();
    }

    if (turnOn) {
      shelfStrip.setBrightness(shelfBrightness);
      shelfStrip.fill(Adafruit_NeoPixel::Color(shelfr, shelfg, shelfb));
      shelfStrip.show();
    }
    
    server.send(200);
    return true;
  }
} apiHandler;



// server info
const int serverPort = 80;
ESP8266WebServer server(serverPort);

String header;

// Read a file from SPIFFS
String readFile(String path) {
  Serial.println("[INFO] Servfing file " + path);
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    String data = file.readString();
    file.close();
    return data;
  }
  Serial.println("[ERROR] File Not Found");
  return "";
}

// Index view
void handleIndex() {
  Serial.println("[INFO] GET /");
  blinkLedStrip(&shelfStrip, 1);
  String data = readFile("/dashboard.html");
  server.send(200, "text/html", data);
}


// Start the server
void initializeServer() {
  Serial.println("[INFO] Initializaing server...");

  server.addHandler(&blinkHandler);
  server.addHandler(&apiHandler);
  
  server.on("/", handleIndex);
//  server.on("/blink", handleBlink);
//  server.on("/dash", handleDash);

  server.begin();
  
  Serial.print("[INFO] Listening on port ");
  Serial.println(serverPort);
}

void setup() {
  Serial.begin(9600);

  initializeFS();

  initializeWifi();
  initializeServer();

  initializeShelfLights();
  
  server.begin();
}
//
//void handleKnob() {
//  Serial.println(knob.read());
//}


void loop() {
  server.handleClient();
//  handleKnob();
}


  // server.on("/temperature", [](){
  //   Serial.println("[INFO] GET /temperature");
  //   server.sendHeader("Location", "/temperature/", true);
  //   server.send(302, "text/plain", "");
  // });  
