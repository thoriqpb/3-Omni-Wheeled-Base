// Comunnication between 2 ESP32 using UART
// UART1 TX (GPIO 10) RX (GPIO 9)
// UART2 TX (GPIO 17) RX (GPIO 16)
// TX (Transmit) pin of ESP32 A to RX (Receive) pin of ESP32 B
// RX (Receive) pin of ESP32 A to TX (Transmit) pin of ESP32 B

#include <PS4Controller.h>
#include <math.h>

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 16, 17);
  PS4.begin("08:d1:f9:e0:ab:16");
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

    Serial.printf("Vx: %d   \tVy: %d   \tw: %d    \t||\t  v1: %.2f\tv2: %.2f\tv3: %.2f\t", Vx, Vy, w, v1, v2, v3);

    Serial1.write((byte*)&v1, sizeof(v1));
    Serial1.write((byte*)&v2, sizeof(v2));
    Serial1.write((byte*)&v3, sizeof(v3));

    Serial.printf("Battery: %d", PS4.Battery());
    if (PS4.Charging()) Serial.print(" (Charging)");
    Serial.println();

    delay(100);
  }
}
