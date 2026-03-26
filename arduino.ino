#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
const int buttonPin = 2; // D2
const int joystickPinX = 0; // A0
const int joystickPinY = 1; // A1
const int joystickButtonPin = 4; // D4

typedef int16_t BufferElement;

void setup(void) {
  Serial.begin(115200);
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(buttonPin, INPUT);
  pinMode(joystickButtonPin, INPUT_PULLUP);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  BufferElement header = 0xFFFF;
  BufferElement buffer[6] = { 0 };
  buffer[0] = header;

  float gyroZ = g.gyro.z;
  BufferElement rotationVelocityZ = (BufferElement)(gyroZ * 180.0 / M_PI);
  buffer[1] = rotationVelocityZ;

  BufferElement buttonState = digitalRead(buttonPin);
  buffer[2] = buttonState;

  BufferElement joystickX = analogRead(joystickPinX);
  buffer[3] = joystickX;

  BufferElement joystickY = analogRead(joystickPinY);
  buffer[4] = joystickY;

  BufferElement joystickButtonState = !digitalRead(joystickButtonPin);
  buffer[5] = joystickButtonState;

  Serial.write((uint8_t *)buffer, 6 * sizeof(BufferElement));
  Serial.flush();

  delay(29);
}