// TX (Transmit) pin of ESP32 A to RX (Receive) pin of ESP32 B
// RX (Receive) pin of ESP32 A to TX (Transmit) pin of ESP32 B

#define MOTOR1_A 27
#define MOTOR1_B 14
#define MOTOR2_A 25
#define MOTOR2_B 26
#define MOTOR3_A 32
#define MOTOR3_B 33

void setup() {
  Serial.begin(115200);  // For Serial Monitor
  Serial1.begin(9600, SERIAL_8N1, 16, 17);  // UART1 
  Serial.println("Ready to receive data from ESP32 A");

  pinMode(MOTOR1_A, OUTPUT);
  pinMode(MOTOR1_B, OUTPUT);
  pinMode(MOTOR2_A, OUTPUT);
  pinMode(MOTOR2_B, OUTPUT);
  pinMode(MOTOR3_A, OUTPUT);
  pinMode(MOTOR3_B, OUTPUT);
}

void loop() {
  if (Serial1.available() >= sizeof(float) * 3) {  // Ensure there are enough bytes for 3 floats (3 * 4 bytes = 12 bytes)
    float v1, v2, v3;
    
    // Read the 3 float values from the incoming UART data
    Serial1.readBytes((byte*)&v1, sizeof(v1));  // Read v1
    Serial1.readBytes((byte*)&v2, sizeof(v2));  // Read v2
    Serial1.readBytes((byte*)&v3, sizeof(v3));  // Read v3
    
    // Print the received values in sideways format
    Serial.printf("Received v1: %.2f\tv2: %.2f\tv3: %.2f\n", v1, v2, v3);
  
  controlMotor(v1, MOTOR1_A, MOTOR1_B);
  controlMotor(v2, MOTOR2_A, MOTOR2_B);
  controlMotor(v3, MOTOR3_A, MOTOR3_B);
  }
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