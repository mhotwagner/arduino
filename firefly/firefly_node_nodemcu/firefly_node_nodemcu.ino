
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(D3, D4); // CE, CSN
const byte address[6] = "00001";

const int ledPin = D5;

void initializeRadio() {
  radio.begin();
  
  if (radio.isChipConnected()) {
    Serial.println("[INFO] Chip connected");
    radio.printDetails();
    Serial.println("[INFO] Initializing radio...");
    radio.openReadingPipe(1, address);
    radio.setPALevel(RF24_PA_HIGH);
    radio.startListening();
//    delay(3000);
    Serial.println("[INFO] Radio initialized");
  } else {
    Serial.println("[ERROR] Chip NOT connected");
    Serial.println("[ERROR] Radio NOT intialized");
  }
}

void setup() {
  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT);
  
  initializeRadio();

  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  
  if (radio.isChipConnected()) {
    Serial.println("[INFO] Listening...");
  }
}

void loop() {
  if (radio.available()) {
    Serial.println(".");
    char text[32] = "";
    radio.read(&text, sizeof(text));

    if (text != "") {
      Serial.println(".");
      Serial.print("|");
      Serial.print(text);
      Serial.print("|");
      Serial.println(".");
    }

    if (text == "off") {
      digitalWrite(ledPin, LOW);
    } else {
      digitalWrite(ledPin, HIGH);
    }
  }
  delay(1000);
}
