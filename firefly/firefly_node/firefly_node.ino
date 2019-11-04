
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <printf.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

const int ledPin = 9;

void initializeRadio() {
  radio.begin();
  
  if (radio.isChipConnected()) {
    Serial.println("[INFO] Chip connected");
    radio.printDetails();
    delay(3000);
  } else {
    Serial.println("[INFO] Chip NOT connected");
  }

  
  Serial.println("[INFO] Initializing radio...");
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
  radio.printDetails();

  Serial.println("[INFO] Radio initialized");
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
  
  Serial.println("[INFO] Listening...");
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
