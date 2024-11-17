// SDA (GPIO 21) and SCL (GPIO 22)

#include <Wire.h>

#define SLAVE_ADDR 9

#define MOTOR1_A 27
#define MOTOR1_B 14
#define MOTOR2_A 25
#define MOTOR2_B 26
#define MOTOR3_A 32
#define MOTOR3_B 33

float v1, v2, v3;

void setup() {
  Serial.begin(115200);
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveData);
  Serial.println("Slave ready.");
  
  pinMode(MOTOR1_A, OUTPUT);
  pinMode(MOTOR1_B, OUTPUT);
  pinMode(MOTOR2_A, OUTPUT);
  pinMode(MOTOR2_B, OUTPUT);
  pinMode(MOTOR3_A, OUTPUT);
  pinMode(MOTOR3_B, OUTPUT);
}

void loop() {
  controlMotor(v1, MOTOR1_A, MOTOR1_B);
  controlMotor(v2, MOTOR2_A, MOTOR2_B);
  controlMotor(v3, MOTOR3_A, MOTOR3_B);
}

void receiveData(int byteCount) {
  if (byteCount == 12) {
    v1 = readFloat();
    v2 = readFloat();
    v3 = readFloat();
    Serial.printf("Received v1: %.2f\tv2: %.2f\tv3: %.2f\n", v1, v2, v3);
  }
}

float readFloat() {
  byte data[4];
  for (int i = 0; i < 4; i++) {
    data[i] = Wire.read();
  }
  float *fPtr = (float *)data;
  return *fPtr;
}

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