#include <Servo.h> 

#define SERVOPINV  5 //vertical servo
#define SERVOPINH  6 //horizontal servo
bool operation_flag=true;

// Horizontal servo settings
Servo horizontal;            //horizontal servo
int servoh = 90;             //Initialize angle
int servohLimitHigh = 180;   //The maximum angle of rotation in the horizontal direction
int servohLimitLow = 0;      //The minimum angle of rotation in the horizontal direction

// Vertical Servo Settings
Servo vertical;              //vertical servo
int servov = 120;             //Initialize angle
int servovLimitHigh = 180;   //The maximum angle of rotation in the vertical direction
int servovLimitLow = 90;     //The minimum angle of rotation in the vertical direction


void setup()
{ 
  horizontal.attach(SERVOPINH); 
  vertical.attach(SERVOPINV);
  horizontal.write(servoh);
  vertical.write(servov);
}


void loop() 
{
}
