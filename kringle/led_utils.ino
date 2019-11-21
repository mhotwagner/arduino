// Blinks a passed in strip n times
void blinkLedStrip(Adafruit_NeoPixel *strip, int n) {
  Serial.println("[INFO] Blinking LED strip");
  int i = 0;
  strip->fill(); strip->show();
  strip->setBrightness(shelfBrightness);
  while (i < n) {
    i++;
    Serial.println("[INFO] blink!");
    strip->fill(shelfColor);
    strip->show(); delay(100);
    strip->fill(); strip->show(); delay(100);
  }
}
