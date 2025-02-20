// calibrate the gyroscope to correct rotation rates
#include<Wire.h>

#define GYRO_SENSITIVITY 65.5
float RateRoll,RatePitch,RateYaw;   // global variables
float RateCalibrationRoll,RateCalibrationPitch,RateCalibrationYaw;    // declare calibration values
int RateCalibrationNumber;

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
      
      RateRoll=(float)GyroX/GYRO_SENSITIVITY;
      RatePitch=(float)GyroY/GYRO_SENSITIVITY;
      RateYaw=(float)GyroZ/GYRO_SENSITIVITY;

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

//  performing calibration 
// we create a loop[ 2000 measures] each value takes 1 millisecond so 2 seconds for 2000 
// add all the values and take average of all variables 
// then subtract them from the measured values in order to get the correct Physical Values

      for(RateCalibrationNumber=0;RateCalibrationNumber<2000;RateCalibrationNumber++){
                gyro_signals();
                RateCalibrationRoll+=RateRoll;
                RateCalibrationPitch+=RatePitch;
                RateCalibrationYaw+=RateYaw;
                delay(1);
      }
      RateCalibrationRoll/=2000;
      RateCalibrationPitch/=2000;
      RateCalibrationYaw/=2000;

}

    

void loop(){
      gyro_signals();
      RateRoll-=RateCalibrationRoll;
      RatePitch-=RateCalibrationPitch;
      RateYaw-=RateCalibrationYaw;
      Serial.print("Roll rate=");
      Serial.print(RateRoll);
      Serial.print(" Pitch rate="); // No println, so it stays on the same line
      Serial.print(RatePitch);
      Serial.print(" Yaw rate="); // No println, so it stays on the same line
      Serial.println(RateYaw); // println here ensures a new line after the full set
      delay(50);



}