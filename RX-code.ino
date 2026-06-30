#include <SPI.h>      // SPI library for NRF24L01 communication
#include <RF24.h>     // NRF24L01 library

// NRF24L01 object
// CE connected to A0
// CSN connected to A1
RF24 radio(A0, A1); // CE, CSN on RX

// Communication address (must match transmitter)
const byte address[6] = "00001";

// Structure to receive X, Y and Z values
struct DataPacket {
  int x;
  int y;
  int z;
};

DataPacket data;

// Stores the time when the last packet was received
unsigned long lastReceiveTime = 0;

// ---------------- Motor Driver Pins ----------------

// Motor Driver 1
int IN1 = 2;
int IN2 = 3;
int IN3 = 4;
int IN4 = 5;

// Motor Driver 2
int IN5 = 6;
int IN6 = 7;
int IN7 = 8;
int IN8 = 9;

void setup() {

  // Start Serial Monitor
  Serial.begin(9600);

  // Built-in LED used as connection indicator
  pinMode(LED_BUILTIN, OUTPUT);

  // Configure all motor driver pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);

  // Make sure the car is stopped at startup
  stopCar();

  // ---------------- NRF24L01 Setup ----------------

  radio.begin();                         // Initialize NRF24L01
  radio.openReadingPipe(0, address);     // Open reading pipe
  radio.setPALevel(RF24_PA_MIN);         // Minimum transmission power
  radio.setDataRate(RF24_250KBPS);       // Lower data rate for better range
  radio.setChannel(76);                  // Communication channel
  radio.setCRCLength(RF24_CRC_16);       // Enable 16-bit CRC
  radio.startListening();                // Put NRF into receive mode

  Serial.println("RX Started");
}

void loop() {

  // Check if a new packet has been received
  if (radio.available()) {

    // Read incoming data
    radio.read(&data, sizeof(data));

    // Update connection timer
    lastReceiveTime = millis();

    // Turn ON LED when connected
    digitalWrite(LED_BUILTIN, HIGH);

    // Print received values
    Serial.print("Connected | X: ");
    Serial.print(data.x);
    Serial.print(" Y: ");
    Serial.print(data.y);
    Serial.print(" Z: ");
    Serial.println(data.z);

    // Decide the movement based on received values
    controlCar();
  }

  // If no data is received for more than 500 ms
  if (millis() - lastReceiveTime > 500) {

    // Stop the car for safety
    stopCar();

    // Blink LED to indicate lost connection
    digitalWrite(LED_BUILTIN, millis() / 300 % 2);
  }
}

// --------------------------------------------------
// Determines which movement to perform
// --------------------------------------------------
void controlCar() {

  // Move Forward
  if (data.x > 6000) {
    moveForward();
  }

  // Move Backward
  else if (data.x < -6000) {
    moveBackward();
  }

  // Strafe Right
  else if (data.y > 6000) {
    strafeRight();
  }

  // Strafe Left
  else if (data.y < -6000) {
    strafeLeft();
  }

  // Rotate Clockwise
  else if (data.z > 8000) {
    rotateRight();
  }

  // Rotate Counter-Clockwise
  else if (data.z < -8000) {
    rotateLeft();
  }

  // No gesture detected
  else {
    stopCar();
  }
}

// --------------------------------------------------
// Moves the car Forward
// --------------------------------------------------
void moveForward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);
}

// --------------------------------------------------
// Moves the car Backward
// --------------------------------------------------
void moveBackward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
}

// --------------------------------------------------
// Strafes the car to the Right
// --------------------------------------------------
void strafeRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);  digitalWrite(IN8, HIGH);
}

// --------------------------------------------------
// Strafes the car to the Left
// --------------------------------------------------
void strafeLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
}

// --------------------------------------------------
// Rotates the car Clockwise
// --------------------------------------------------
void rotateRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW);  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
}

// --------------------------------------------------
// Rotates the car Counter-Clockwise
// --------------------------------------------------
void rotateLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);  digitalWrite(IN8, HIGH);
}

// --------------------------------------------------
// Stops all four motors
// --------------------------------------------------
void stopCar() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW); digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW); digitalWrite(IN8, LOW);
}