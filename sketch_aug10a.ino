// Include the Wire library
#include <Wire.h>

// Define the I2C address of the sensor
#define LSM6DS3TR_C_ADDR 0x6B

// Define the registers for LSM6DS3TR-C
#define WHO_AM_I 0x0F   // Device identification register
#define CTRL1_XL 0x10   // Accelerometer control register
#define CTRL2_G 0x11    // Gyroscope control register
#define OUTX_L_XL 0x28  // Accelerometer X-axis output register (low byte)
#define OUTX_H_XL 0x29  // Accelerometer X-axis output register (high byte)
#define OUTY_L_XL 0x2A  // Accelerometer Y-axis output register (low byte)
#define OUTY_H_XL 0x2B  // Accelerometer Y-axis output register (high byte)
#define OUTZ_L_XL 0x2C  // Accelerometer Z-axis output register (low byte)
#define OUTZ_H_XL 0x2D  // Accelerometer Z-axis output register (high byte)

// Define the scale and sensitivity for accelerometer
#define SCALE_XL 2     // Full scale range of +/- 2g
#define SENS_XL 0.061  // Sensitivity of 0.061 mg/LSB

// Define the scale and sensitivity for gyroscope
#define SCALE_G 245 // Full scale range of +/- 245 dps
#define SENS_G 8.75 // Sensitivity of 8.75 mdps/LSB


// Define a function to write a byte to a register
void writeRegister(byte reg, byte val) {
  Wire.beginTransmission(LSM6DS3TR_C_ADDR);  // Start communication with the sensor
  Wire.write(reg);                           // Send the register address
  Wire.write(val);                           // Send the value to write
  Wire.endTransmission();                    // End communication with the sensor
}

// Define a function to read a byte from a register
byte readRegister(byte reg) {
  byte val;                                  // Variable to store the read value
  Wire.beginTransmission(LSM6DS3TR_C_ADDR);  // Start communication with the sensor
  Wire.write(reg);                           // Send the register address
  Wire.endTransmission(false);               // End communication with the sensor without releasing the bus
  Wire.requestFrom(LSM6DS3TR_C_ADDR, 1);     // Request one byte from the sensor
  if (Wire.available()) {                    // Check if there is data available
    val = Wire.read();                       // Read the value from the sensor
  }
  return val;  // Return the read value
}

// Define a function to read a 16-bit value from two registers
int16_t read16bit(byte regL, byte regH) {
  int16_t val;                                         // Variable to store the read value
  val = readRegister(regH) << 8 | readRegister(regL);  // Read the high byte and shift it left, then read the low byte and combine them
  return val;                                          // Return the read value
}

// Define a function to convert a raw value to mg
float rawToMg(int16_t raw) {
  float mg;                     // Variable to store the converted value
  mg = raw * SENS_XL / 1000.0;  // Multiply the raw value by the sensitivity and divide by 1000 to get mg
  return mg;                    // Return the converted value
}

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 bps
  Wire.begin();        // Initialize I2C communication

  // Check if the device is connected by reading the WHO_AM_I register
  byte whoami = readRegister(WHO_AM_I);

  Serial.print("WHO_AM_I: ");
  Serial.println(whoami, HEX);

  if (whoami == 0x6A) {
    Serial.println("LSM6DS3TR-C is online");
  } else {
    Serial.println("LSM6DS3TR-C is offline");
    while (1)
      ;
  }

  // Configure the accelerometer by writing to CTRL1_XL register

  writeRegister(CTRL1_XL, (SCALE_XL << 2) | 0x40);
  // Set full scale range and enable accelerometer at ODR = 104 Hz

    // Configure the gyroscope by writing to CTRL2_G register

    writeRegister(CTRL2_G, (SCALE_G << 2) | 0x40);
  // Set full scale range and enable gyroscope at ODR = 104 Hz
}

void loop() {

  // Read the accelerometer values from the output registers

  int16_t ax_raw = read16bit(OUTX_L_XL, OUTX_H_XL);
  int16_t ay_raw = read16bit(OUTY_L_XL, OUTY_H_XL);
  int16_t az_raw = read16bit(OUTZ_L_XL, OUTZ_H_XL);

  // Convert the raw values to mg

  float ax_mg = rawToMg(ax_raw) * 2 / 0.98;
  float ay_mg = rawToMg(ay_raw) * 2 / 0.98;
  float az_mg = rawToMg(az_raw) * 2 / 0.98;

  // Print the values to the serial monitor

 // Serial.print("AX: ");
  Serial.print(ax_mg);
  Serial.write('\t'); // Tab character
 // Serial.print(" g,\t");
 // Serial.print("AY: ");
  Serial.print(ay_mg);
  Serial.write('\t'); // Tab character
 // Serial.print(" g,\t");
 // Serial.print("AZ: ");
  Serial.print(az_mg);
  Serial.println("\n");
 // Serial.println(", ");
 // Serial.println(" g");

  // Wait for 100 ms

  delay(10);
}
