# 3 Omni-wheeled Base🚘
This program controls a 3 omni-wheeled base robot using PS4 controller library (Bluetooth Classic Connenction). My team and I utilized the ESP32 as the microcontroller and the IBT2 motor driver for controlling PG45 motor DC. Since we didn't use the encoders, we opted for PWM as the output instead of motor speed (𝑉𝑖). 

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
   connect each motor driver’s `PWM` and `EN` pins according to the program's specifications.


## Setting Up the Controller🎮
Every Bluetooth controller has a unique MAC address. To connect the controller to your program, you need to specify its MAC address, such as `ec:64:c9:5e:28:xx`. To find your controller's MAC address, use the SixAxis Pair Tool. Here's how:
1. download and install the [SixAxis Pair Tool](https://sixaxispairtool.en.lo4d.com/windows).
2. connect your controller to your laptop using a data cable.
3. launch the tool to view the MAC address of your controller.


## How it works😺❔
1. upload and run the program.
2. the motor speed and should be started in 0.
3. pair the ESP32 with PS4.


## Notes⚠️
- make sure change the pins mode to `DIO` to prevent the ESP32 resetting repeatedly. In Arduino IDE, open `Tools`->`Flash mode`->`DIO`
- sometimes ESP32 wouldn't pairing. To solve this, try to remove paired device in ESP32 first. `Example`->`BluetoothSerial`->`bt_remove_paired_device`
