//********************************************************//
//*  University of Nottingham                            *//
//*  Department of Electrical and Electronic Engineering *//
//*  UoN EEEBot 2023                                     *//
//*                                                      *//
//*  Skeleton Master Code for Use with the               *//
//*  EEEBot_MainboardESP32_Firmware Code                 *//
//*                                                      *//
//*  Nat Dacombe                                         *//
//********************************************************//

// the following code acts as a 'bare bones' template for your own custom master code that works with the firmware code provided
// therefore, the variable names are non-descriptive - you should rename these variables appropriately
// you can either modify this code to be suitable for the project week task, or use the functions as inspiration for your own code

#include <Wire.h>
#define I2C_SLAVE_ADDR 0x04 // 4 in hexadecimal




void setup()
{
  
  Serial.begin(9600);
  Wire.begin();   // join i2c bus (address optional for the master) - on the ESP32 the default I2C pins are 21 (SDA) and 22 (SCL)

  // set input pins

  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);
  pinMode(27, INPUT);
  pinMode(14, INPUT);

}

// LOOPING CODE
void loop()
{

  // read IR pins and assign to wire colour variables

  int yellow = map(analogRead(12), 0, 255, 0, 4095);
  int orange = map(analogRead(14), 0, 255, 0, 4095);
  int white = map(analogRead(27), 0, 255, 0, 4095);
  int brown = map(analogRead(26), 0, 255, 0, 4095);
  int purple = map(analogRead(25), 0, 255, 0, 4095);
  int green = map(analogRead(33), 0, 255, 0, 4095);

  Serial.write(yellow);

  // define distances for each sensor to the reference point in the centre
  int dist_y = 30;
  int dist_o = 20;
  int dist_w = 10;
  int dist_b = 10;
  int dist_p = 20;
  int dist_g = 30;

  int IR_average = (yellow*dist_y + orange*dist_o + white*dist_w + brown*dist_b + purple*dist_p + green*dist_g)/(yellow + orange  + white + brown + purple + green);
  
  float error = 0 - IR_average;
  float previous_error = error;

  float error_count = error_count + error;
  int Kp = 5;
  int Ki = 1;
  int Kd = 1;
  float u = Kp*error + Ki*error_count + Kd*(error - previous_error);

  // three integer values are sent to the slave device x = left, y = right, z = servo
  int base_speed_left = 90;
  int base_speed_right = 90;
  int k = 0.8;
  int centreAngle = 90;
  int x = base_speed_left + k*u;
  int y = base_speed_right - k*u;
  int z = centreAngle + u;
  

  // two 16-bit integer values are requested from the slave
  int16_t a = 0;
  int16_t b = 0;
  uint8_t bytesReceived = Wire.requestFrom(I2C_SLAVE_ADDR, 4);  // 4 indicates the number of bytes that are expected
  uint8_t a16_9 = Wire.read();  // receive bits 16 to 9 of a (one byte)
  uint8_t a8_1 = Wire.read();   // receive bits 8 to 1 of a (one byte)
  uint8_t b16_9 = Wire.read();   // receive bits 16 to 9 of b (one byte)
  uint8_t b8_1 = Wire.read();   // receive bits 8 to 1 of b (one byte)

  a = (a16_9 << 8) | a8_1; // combine the two bytes into a 16 bit number
  b = (b16_9 << 8) | b8_1; // combine the two bytes into a 16 bit number

  Serial.print(a);
  Serial.print("\t");
  Serial.println(b);
  
  Wire.beginTransmission(I2C_SLAVE_ADDR); // transmit to device #4
  /* depending on the microcontroller, the int variable is stored as 32-bits or 16-bits
     if you want to increase the value range, first use a suitable variable type and then modify the code below
     for example; if the variable used to store x and y is 32-bits and you want to use signed values between -2^31 and (2^31)-1
     uncomment the four lines below relating to bits 32-25 and 24-17 for x and y
     for my microcontroller, int is 32-bits hence x and y are AND operated with a 32 bit hexadecimal number - change this if needed

     >> X refers to a shift right operator by X bits
  */
  //Wire.write((byte)((x & 0xFF000000) >> 24)); // bits 32 to 25 of x
  //Wire.write((byte)((x & 0x00FF0000) >> 16)); // bits 24 to 17 of x
  Wire.write((byte)((x & 0x0000FF00) >> 8));    // first byte of x, containing bits 16 to 9
  Wire.write((byte)(x & 0x000000FF));           // second byte of x, containing the 8 LSB - bits 8 to 1
  //Wire.write((byte)((y & 0xFF000000) >> 24)); // bits 32 to 25 of y
  //Wire.write((byte)((y & 0x00FF0000) >> 16)); // bits 24 to 17 of y
  Wire.write((byte)((y & 0x0000FF00) >> 8));    // first byte of y, containing bits 16 to 9
  Wire.write((byte)(y & 0x000000FF));           // second byte of y, containing the 8 LSB - bits 8 to 1
  Wire.write((byte)((z & 0x0000FF00) >> 8));    // first byte of y, containing bits 16 to 9
  Wire.write((byte)(z & 0x000000FF));           // second byte of y, containing the 8 LSB - bits 8 to 1
  Wire.endTransmission();   // stop transmitting
  delay(100);
}