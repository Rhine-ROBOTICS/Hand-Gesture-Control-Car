#include <Wire.h>     // I2C communication library (used for MPU6050)
#include <SPI.h>      // SPI library for NRF24L01
#include <RF24.h>     // NRF24L01 library

// NRF24L01 object
// CE connected to D7
// CSN connected to D8
RF24 radio(7, 8); // CE, CSN on TX

// Communication address (must match receiver)
const byte address[6] = "00001";

// LED indicates connection status
int ledPin = 2;

// Structure to send sensor values
struct DataPacket {
  int x;
  int y;
  int z;
};

DataPacket data;

void setup() {

  // Start Serial Monitor
  Serial.begin(9600);

  // Start I2C communication
  Wire.begin();

  // Configure LED pin as output
  pinMode(ledPin, OUTPUT);

  // ---------------- Wake up MPU6050 ----------------
  // MPU6050 starts in sleep mode by default
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);     // Power Management Register
  Wire.write(0);        // Set to 0 to wake up MPU6050
  Wire.endTransmission(true);

  // ---------------- NRF24L01 Setup ----------------

  radio.begin();                         // Initialize NRF24L01
  radio.openWritingPipe(address);        // Open writing pipe
  radio.setPALevel(RF24_PA_MIN);         // Minimum transmission power
  radio.setDataRate(RF24_250KBPS);       // Lower data rate for better range
  radio.setChannel(76);                  // Communication channel
  radio.setCRCLength(RF24_CRC_16);       // Enable 16-bit CRC
  radio.stopListening();                 // Set NRF as transmitter

  Serial.println("TX Started");
}

void loop() {

  // Read values from MPU6050
  readMPU();

  // Send data to the receiver
  bool sent = radio.write(&data, sizeof(data));

  // Check whether transmission was successful
  if (sent) {

    // Keep LED ON while connected
    digitalWrite(ledPin, HIGH);

    Serial.print("Connected | X: ");

  } else {

    // Blink LED if receiver is not responding
    digitalWrite(ledPin, millis() / 300 % 2);

    Serial.print("Disconnected | X: ");
  }

  // Print transmitted values to Serial Monitor
  Serial.print(data.x);
  Serial.print(" Y: ");
  Serial.print(data.y);
  Serial.print(" Z: ");
  Serial.println(data.z);

  // Small delay for stable transmission
  delay(50);
}

// --------------------------------------------------
// Reads Accelerometer and Gyroscope values
// from the MPU6050
// --------------------------------------------------
void readMPU() {

  int16_t AcX, AcY, AcZ;
  int16_t GyX, GyY, GyZ;

  // Request data starting from register 0x3B
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  // Request 14 bytes from MPU6050
  Wire.requestFrom(0x68, 14, true);

  // Read Accelerometer values
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();

  // Skip Temperature register (2 bytes)
  Wire.read();
  Wire.read();

  // Read Gyroscope values
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();

  // Store required values in the data packet
  // X-axis Accelerometer
  data.x = AcX;

  // Y-axis Accelerometer
  data.y = AcY;

  // Z-axis Gyroscope (used for rotation)
  data.z = GyZ;
}