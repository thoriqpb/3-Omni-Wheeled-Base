// ESP32 V4
// Base 3 Omniwhell
// motor dc, Vi = PWM, ga pake encoder
// BNO055, pake PID berdasarkan X-Orientation, SDA pin 21, SCL pin 22
// Input serial monitor (Vx,Vy,targetX) contoh (10,15,30)

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

// Pins for Motor 1
#define MOTOR1_A 27
#define MOTOR1_B 14

// Pins for Motor 2
#define MOTOR2_A 25
#define MOTOR2_B 26

// Pins for Motor 3
#define MOTOR3_A 32
#define MOTOR3_B 33

float Vx = 0.0;     // Initial speed component along x-axis
float Vy = 0.0;     // Initial speed component along y-axis
float w = 0.0;      // Angular velocity (omega) calculated by PID
float targetX = 0.0; // Target roll angle (0°)

float v1 = 0.0;     // Speed for Motor 1
float v2 = 0.0;     // Speed for Motor 2
float v3 = 0.0;     // Speed for Motor 3

// PID Variables
float kp = 1.0;     // Proportional gain
float ki = 0.0;     // Integral gain
float kd = 0.0;     // Derivative gain
float prevError = 0.0;
float integral = 0.0;

String inputString = ""; // Variable to store input string from Serial

void setup() {
  Serial.begin(115200);
  
  // Initialize pins for all motors
  pinMode(MOTOR1_A, OUTPUT);
  pinMode(MOTOR1_B, OUTPUT);
  pinMode(MOTOR2_A, OUTPUT);
  pinMode(MOTOR2_B, OUTPUT);
  pinMode(MOTOR3_A, OUTPUT);
  pinMode(MOTOR3_B, OUTPUT);

  // Initialize BNO055 sensor
  if (!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000);
  bno.setExtCrystalUse(true); // Use external crystal for better accuracy

  // Print instruction for user
  Serial.println("Enter Vx,Vy,targetX separated by commas, e.g., 1.0,0.5,30.0");
}

void loop() {
  // Read orientation from BNO055 sensor
  sensors_event_t event;
  bno.getEvent(&event);

  // Normalize roll angle to -180 to 180 degrees range
  float rollAngle = event.orientation.x;
  if (rollAngle > 180) {
    rollAngle -= 360;  // Bring it back to -180 to 180 range
  } else if (rollAngle < -180) {
    rollAngle += 360;  // Bring it back to -180 to 180 range
  }

  // Calculate the error for the roll (x) angle
  float error = targetX - rollAngle;

  // Handle wrap-around for angles near -180/180 degrees
  if (error > 180) {
    error -= 360;
  } else if (error < -180) {
    error += 360;
  }

  // Calculate the integral and derivative components
  integral += error;
  float derivative = error - prevError;

  // PID output for angular velocity (w)
  w = -((kp * error) + (ki * integral) + (kd * derivative));
  prevError = error;

  // Print PID values and error
  Serial.print("Roll (X): ");
  Serial.print(rollAngle); // Roll
  Serial.print("°, Error: ");
  Serial.print(error);
  Serial.print(", w (PID output): ");
  Serial.println(w);

  // Check for new input from Serial Monitor
  if (Serial.available()) {
    inputString = Serial.readStringUntil('\n');  // Read input until newline character

    // Split the input string by commas and parse Vx, Vy, and targetX
    parseInputString(inputString);
  }

  // Compute the speed for each motor using the given formulas
  v1 = Vx + w;
  v2 = -Vx * cos(60 * PI / 180) - Vy * cos(30 * PI / 180) + w;
  v3 = -Vx * cos(60 * PI / 180) + Vy * cos(30 * PI / 180) + w;

  // Print motor speeds for debugging
  Serial.print("Motor 1 Speed: ");
  Serial.println(v1);
  Serial.print("Motor 2 Speed: ");
  Serial.println(v2);
  Serial.print("Motor 3 Speed: ");
  Serial.println(v3);

  // Control motors
  controlMotor(v1, MOTOR1_A, MOTOR1_B);
  controlMotor(v2, MOTOR2_A, MOTOR2_B);
  controlMotor(v3, MOTOR3_A, MOTOR3_B);

  delay(BNO055_SAMPLERATE_DELAY_MS);  // Delay before updating orientation
}

// Function to control motor based on v_target
void controlMotor(float v_target, int in1, int in2) {
  int pwmVal = (int) fabs(v_target);  // Absolute PWM value (0-255)
  if (pwmVal > 255) pwmVal = 255;     // Limit to max PWM
  
  int dir = (v_target >= 0) ? 1 : -1; // 1 for forward, -1 for reverse

  if (dir == 1) { 
    // Forward direction
    analogWrite(in1, pwmVal);
    analogWrite(in2, LOW);
  } else if (dir == -1) {
    // Reverse direction
    analogWrite(in1, LOW);
    analogWrite(in2, pwmVal);
  } else {
    // Stop motor
    analogWrite(in1, LOW);
    analogWrite(in2, LOW);    
  }
}

// Function to parse input string and update Vx, Vy, and targetX
void parseInputString(String input) {
  input.trim();  // Remove any extra whitespace
  int firstComma = input.indexOf(','); // Find first comma
  int secondComma = input.indexOf(',', firstComma + 1); // Find second comma

  // Extract values and convert to float
  if (firstComma != -1 && secondComma != -1) {
    Vx = input.substring(0, firstComma).toFloat();
    Vy = input.substring(firstComma + 1, secondComma).toFloat();
    targetX = input.substring(secondComma + 1).toFloat();

    // Print parsed values
    Serial.print("Vx: ");
    Serial.print(Vx);
    Serial.print(", Vy: ");
    Serial.print(Vy);
    Serial.print(", targetX: ");
    Serial.println(targetX);
  } else {
    Serial.println("Invalid input. Please enter in the format: Vx,Vy,targetX");
  }
}
