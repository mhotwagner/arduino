#include <LiquidCrystal.h>

const int rs=D6, en=D7, db4=D2, db5=D3, db6=D4, db7=D5;
LiquidCrystal lcd(rs, en, db4, db5, db6, db7);

byte a[8] = {
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000
};
byte b[8] = {
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000
};
byte c[8] = {
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100
};
byte d[8] = {
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b11110
};
byte e[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

String lastMessage;
String message;

const int BACKUP = 1;

int currentSensor;
String currentData;

int parseSensor(String sensorName) {
  if (sensorName == "BACKUP") {
    return 1;
  }
  return 0;
}

void parseMessage(String message) {
  int delim = message.indexOf(":");
  currentSensor = parseSensor(message.substring(0, delim));
  currentData = message.substring(delim + 1, message.length() - 1);
}

void setup()
{ 
  lcd.begin(16, 2);
  lcd.print("starting...");
  delay(1000);
  Serial.begin(9600);
  lcd.createChar(1, a);
  lcd.createChar(2, b);
  lcd.createChar(3, c);
  lcd.createChar(4, d);
  lcd.createChar(5, e);
}



void loop() 
{ 
   // when characters arrive over the serial port...
  if (Serial.available()) {
    lastMessage = message;
    message = String(Serial.readStringUntil(';'));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
    Serial.println(message);
    if (message != lastMessage) {
      parseMessage(message);
      switch (currentSensor) {
        case BACKUP:
          int data = currentData.toInt();
          if (data < 401) {
            int fullChars = data / 25;
            int lastChar = floor((data%25)/5);
//            lcd.setCursor(0, 1);
//            lcd.print(fullChars);
//            lcd.print(lastChar);
            lcd.setCursor(0, 0);
            for (int i = 0; i < fullChars; i++) {
              lcd.write(5);
            }
            if (lastChar > 0) {
              lcd.write(lastChar);
            }

          }
      };

    }
  }
}
