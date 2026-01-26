// Basic demo for accelerometer readings from Adafruit MPU6050

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
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  pinMode(buttonPin, INPUT);
  pinMode(joystickButtonPin, INPUT_PULLUP);

  Serial.println("");
  delay(100);
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  // Serial.print("Acceleration X: ");
  // Serial.print(a.acceleration.x);
  // Serial.print(", Y: ");
  // Serial.print(a.acceleration.y);
  // Serial.print(", Z: ");
  // Serial.print(a.acceleration.z);
  // Serial.println(" m/s^2");

  // Serial.print("Rotation X: ");
  // Serial.print(g.gyro.x * 180.0 / M_PI);
  // Serial.print(", Y: ");
  // Serial.print(g.gyro.y * 180.0 / M_PI);
  // Serial.print(", Z: ");
  // Serial.print(g.gyro.z * 180.0 / M_PI);
  // Serial.println(" deg/s");

  // Serial.print("Temperature: ");
  // Serial.print(temp.temperature);
  // Serial.println(" degC");

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
  // Serial.println(rotationVelocityZ);

  // Serial.println("");
  // Serial.print("Rotation: ");
  // Serial.println(rotationVelocityZ);
  // Serial.print("Button state: ");
  // Serial.println(buttonState);
  // Serial.print("Joystick X: ");
  // Serial.println(joystickX);
  // Serial.print("Joystick Y: ");
  // Serial.println(joystickY);
  // Serial.print("Joystick Button State: ");
  // Serial.println(joystickButtonState);

  delay(29);
}