/*********
 * Gorman's Butt Controller
 * 
 * Controls backup sensor (and others?) and
 * relays data to Gorbrain via serial comm (wifi?)
 * 
*********/

const int trigPin = D0;
const int echoPin = D1;
const int reversePin = D2;
const int ledPin = D8;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float d;
float distanceInch;

int ON = 1;
int OFF = 2;
int lastState = OFF;

class Sensor {
  public:
    String _name;
    Sensor(String name) {
      _name = name;
    }
    String data(float data) {
      return _name + ":" + String(data) + ";";
    }
};

Sensor backupSensor = Sensor("BACKUP");

void blink(int n) {
  int delayTime = 100;
  for (int i = 0; i < n; i++) {
    digitalWrite(ledPin, HIGH);
    delay(delayTime);
    digitalWrite(ledPin, LOW);
    if (i != n - 1) delay(delayTime);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(reversePin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  blink(3);
}

float getDistance() {
 // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  return duration * SOUND_VELOCITY/2; 
}

void loop() {
  int rev = digitalRead(reversePin);
  Serial.println(rev);
  
  if (rev == LOW) {  
    if (lastState == OFF) {
      lastState = ON;
      Serial.write("BACKUP:-1;");
      blink(1);
    }
    Serial.write(backupSensor.data(round(getDistance())).c_str());
  } else {
    if (lastState == ON) {
      lastState = OFF;
      Serial.write("BACKUP:-2;");
      blink(2);
    }
  }
  delay(50);
}
