# include <WiFi.h>

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
}