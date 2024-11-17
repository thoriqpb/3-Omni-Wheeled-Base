// Communication between ESP32 and ESP32 using I2C
// SDA (GPIO 21) and SCL (GPIO 22)

#include <PS4Controller.h>
#include <Wire.h>
#include <math.h>

#define SLAVE_ADDR 9

void setup() {
  Serial.begin(115200);
  PS4.begin("08:d1:f9:e0:ab:16");
  Wire.begin();
  Serial.println("Ready to connect!");
}

void loop() {
  if (PS4.isConnected()) {
    int Vx = map(PS4.LStickX(), -128, 127, -255, 255);
    int Vy = map(PS4.LStickY(), -128, 127, -255, 255);
    int w = map(PS4.RStickX(), -128, 127, -180, 180);

    float v1 = Vx + w / 2.0;
    float v2 = -Vx * cos(60 * PI / 180) - Vy * cos(30 * PI / 180) + w / 2.0;
    float v3 = -Vx * cos(60 * PI / 180) + Vy * cos(30 * PI / 180) + w / 2.0;

    Wire.beginTransmission(SLAVE_ADDR);
    sendFloat(v1);
    sendFloat(v2);
    sendFloat(v3);
    Wire.endTransmission();

    Serial.printf("Vx: %d   \tVy: %d   \tw: %d    \t||\t  v1: %.2f\tv2: %.2f\tv3: %.2f\t", Vx, Vy, w, v1, v2, v3);
    Serial.printf("Battery: %d", PS4.Battery());
    if (PS4.Charging()) Serial.print(" (Charging)");
    Serial.println();
  }
}

void sendFloat(float value) {
  byte *bytePointer = (byte *)&value;
  for (int i = 0; i < 4; i++) {
    Wire.write(bytePointer[i]);
  }
}
