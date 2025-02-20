#include<Wire.h>

// test code for measuring gyroscopic values


float RateRoll,RatePitch,RateYaw;   // global variables

void gyro_signals(void){

      Wire.beginTransmission(0x68);   //Start I2C communication with the gyro
      Wire.write(0x1A);      // switch on the low-pass filter
      Wire.write(0x05);    
      Wire.endTransmission();

      Wire.beginTransmission(0x68);    // set the sensitivity factor
      Wire.write(0x1B);
      Wire.write(0x8);
      Wire.endTransmission();

      Wire.beginTransmission(0x68);
      Wire.write(0x43);          // Access the registers storing gyro measurements
      Wire.endTransmission();
      Wire.requestFrom(0x68,6);

      int16_t GyroX=Wire.read()<<8 | Wire.read();   // Read the gyro measurements around x-Axis
      // similarly for y and z axis
      int16_t GyroY=Wire.read()<<8 | Wire.read();
      int16_t GyroZ=Wire.read()<<8 | Wire.read();

      // converting the measurements units to degree/seconds

      RateRoll=(float)GyroX/65.5;
      RatePitch=(float)GyroY/65.5;
      RateYaw=(float)GyroZ/65.5;

}
void setup(){
      Serial.begin(115200);
      pinMode(13,OUTPUT);
      digitalWrite(13,HIGH);
      
      Wire.setClock(400000);    // set the clock speed of I2C
      Wire.begin();
      delay(250);
      
      Wire.beginTransmission(0x68);   // start the gyro in power mode
      Wire.write(0x6B);
      Wire.write(0x00);
      Wire.endTransmission();
}

    

void loop(){
      gyro_signals();
      Serial.print("Roll rate=");
      Serial.print(RateRoll);
      Serial.print(" Pitch rate="); // No println, so it stays on the same line
      Serial.print(RatePitch);
      Serial.print(" Yaw rate="); // No println, so it stays on the same line
      Serial.println(RateYaw); // println here ensures a new line after the full set
      delay(50);



}