#include <PS4Controller.h>

#define MOTOR1_A 27 // Pins for Motor 1
#define MOTOR1_B 14
#define MOTOR2_A 25 // Pins for Motor 2
#define MOTOR2_B 26
#define MOTOR3_A 32 // Pins for Motor 3
#define MOTOR3_B 33

float Vx = 0.0;
float Vy = 0.0;
float w = 0.0;

float v1 = 0.0;
float v2 = 0.0;
float v3 = 0.0;

void setup() {
  Serial.begin(115200);

  // Initialize motors
  pinMode(MOTOR1_A, OUTPUT);
  pinMode(MOTOR1_B, OUTPUT);
  pinMode(MOTOR2_A, OUTPUT);
  pinMode(MOTOR2_B, OUTPUT);
  pinMode(MOTOR3_A, OUTPUT);
  pinMode(MOTOR3_B, OUTPUT);

  PS4.begin("ec:64:c9:5e:28:92"); // Replace with your PS4 MAC address
  Serial.println("PS4 ready to connect!");
}

void loop() {
  if (PS4.isConnected()) {
    // Map left stick for Vx, Vy and right stick for w
    Vx = map(PS4.LStickX(), -128, 127, -255, 255);
    Vy = map(PS4.LStickY(), -128, 127, 255, -255);
    w = map(PS4.RStickX(), -128, 127, -180, 180);

    // Motor speed calculation
    v1 = Vx + w / 2;
    v2 = -Vx * cos(60 * PI / 180) - Vy * cos(30 * PI / 180) + w / 2;
    v3 = -Vx * cos(60 * PI / 180) + Vy * cos(30 * PI / 180) + w / 2;

    Serial.printf("Vx: %.2f   \tVy: %.2f   \tw: %.2f    \t||\t  v1: %.2f\tv2: %.2f\tv3: %.2f\t", Vx, Vy, w, v1, v2, v3);
    Serial.printf("Battery: %d", PS4.Battery());
    if (PS4.Charging()) Serial.print(" (Charging)");
    Serial.println();

    // Control motors
    controlMotor(v1, MOTOR1_A, MOTOR1_B);
    controlMotor(v2, MOTOR2_A, MOTOR2_B);
    controlMotor(v3, MOTOR3_A, MOTOR3_B);
  } 
}

// Function to control motor based on v_target
void controlMotor(float v_target, int in1, int in2) {
  int pwmVal = (int) fabs(v_target);
  if (pwmVal > 255) pwmVal = 255;

  int dir = (v_target >= 0) ? 1 : -1;

  if (dir == 1) {
    analogWrite(in1, pwmVal);
    analogWrite(in2, LOW);
  } else {
    analogWrite(in1, LOW);
    analogWrite(in2, pwmVal);
  }
}
