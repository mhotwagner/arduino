int refTime;
int offset = 25;
int steps = 16;

void setup() {
  refTime = millis();
  Serial.begin(9600);
}

void loop() {
  int step = (millis() - refTime) / offset;
  
  Serial.println("STEP");
  Serial.println(step);

  int cycle = step / steps;
  Serial.println("CYCLE");
  Serial.println(cycle);

  int rainbowOffset = cycle / 6;
  Serial.println("RAINBOW");
  Serial.println(rainbowOffset);
  delay(100);
  Serial.println("---");
  step++;
}
