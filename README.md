# 3 Omni-wheeled Base🎮
This program controls a 3 omni-wheeled base robot using PS4 controller (Bluetooth Classic Connenction). My team and I utilized the ESP32 as the microcontroller and the IBT2 motor driver for controlling PG45 motor DC. Since we didn't use the encoders, we opted for PWM as the output instead of motor speed (𝑉𝑖). Our focus was on experimenting with PID control for the BNO055 alongside the 3 omni-wheeled base.
  
The robot rotates at a positive 𝑉𝑖 when 𝑉1, 𝑉2 and 𝑉3 move in a clockwise direction. For 𝑉𝑥, the robot moves to the right, while for 𝑉𝑦, it moves upwards. I calculated the inverse kinematics for 𝑉1, 𝑉, and 𝑉3 using the 𝑉𝑥, 𝑉𝑦 and 𝜔 inputs. While the robot's direction functions correctly, the 𝑉𝑖 is not accurate because I used the transpose instead of the inverse for the matrix calculation. Here are the equations:

  `V1 = Vx + ω`
  
  `V2 = -Vx * cos(60°) - Vy * cos(30°) + ω`
  
  `V3 = -Vx * cos(60°) + Vy * cos(30°) + ω`

This code may be updated for future improvements!

## Wiring💫
1. BNO055 (optional)
   - VCC `3,3V` or `5V`
   - SDA `pin 21`
   - SCL `pin 22`
2. IBT2
   Connect each motor driver’s `PWM` and `EN` pins according to the program's specifications.


## How it works😺❔
1. Upload and run the program.
2. the motor speed and x-angle should be started in 0.
3. Pair the ESP32 with PS4.


## Notes⚠️
- make sure change the pins mode to `DIO` to prevent the ESP32 resetting repeatedly. In Arduino IDE, open `Tools`->`Flash mode`->`DIO`
- sometimes ESP32 wouldn't pairing. To solve this, try to remove paired device in ESP32 first. `Example`->`BluetoothSerial`->`bt_remove_paired_device`
