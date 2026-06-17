🤖 **Hand Gesture Control Car**

An Arduino-based omnidirectional robot car controlled entirely by hand gestures using an MPU6050 accelerometer and gyroscope. The transmitter detects hand movements and sends commands wirelessly through NRF24L01 modules, allowing the robot to move in multiple directions without any physical controller.

---

🚀 **Features**

  📡 Wireless communication using NRF24L01

  🤚 Hand gesture-based control

  ↔️ Omnidirectional movement

  ⚡ Real-time response

  🔧 Built using Arduino Nano and Arduino Uno

  🎯 Simple and low-cost design

---

🎮 **Controls**


 * Hand Gesture - Car Movement

   - Tilt Forward -	Move Forward

   - Tilt Backward -	Reverse

   - Tilt Left -	Strafe Left

   - Tilt Right -	Strafe Right

   - Rotate Hand Left/Right - Rotate Car

---


🛠 **Hardware Used**

 * Transmitter

   - Arduino Nano

   - MPU6050

   - NRF24L01


 * Receiver

   - Arduino Uno

   - NRF24L01

   - 2 × L298N Motor Drivers

   - 4 × DC Motors

   - Omnidirectional (Mecanum) Wheels



⚙️ **How It Works**

  The MPU6050 measures the orientation of the user's hand.
  The Arduino Nano processes the sensor data.
  Movement commands are transmitted wirelessly using the NRF24L01 module.
  The Arduino Uno receives the commands.
  The motor drivers control the motors accordingly.
  The car moves in the desired direction based on the detected gesture.

👨‍💻 **Author**

**Saptaswa Jana**

🎓 **CSE (AI & ML) Student, KIIT**
🤖 **Robotics & Embedded Systems Enthusiast**
🚁 **Drone Builder | Arduino Developer | Maker**

If you found this project interesting, consider giving it a ⭐!
