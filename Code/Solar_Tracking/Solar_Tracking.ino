#include <Servo.h> 

#define SERVOPINV  5 //vertical servo
#define SERVOPINH  6 //horizontal servo

int mode=0;
int axe=0;
int buttonState1 = 0;    
int buttonState2   = 0;   
int prevButtonState1 = 0;
int prevButtonState2 = 0; 

int tol=100;      //The response range of illuminance, the smaller the value, the more sensitive the response, otherwise it is slow 
                        //(the value is 10~100, the sensitivity is different depending on the ambient light intensity, the indoor light source changes greatly, and the change is smaller under the sun)

int dtime=100;    //delay parameter. The smaller the value, the faster the response speed. 
                        //On the contrary, the larger the value, the slower the response speed. Unit: milliseconds General value (10~100) 
            
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

// 4 connection ports for photoresistor modules   
const int ldrlt = A0;   //top left
const int ldrrt = A1;   //top right
const int ldrld = A2;   //down left
const int ldrrd = A3;   //down right

void setup()
{
  pinMode(2, INPUT);              //Mode switch Button
  pinMode(3, INPUT);              //Axis switch
  pinMode(A4, INPUT);            //Potentiometer for right-left movement and for up-down movement
  horizontal.attach(SERVOPINH); 
  vertical.attach(SERVOPINV);
  horizontal.write(servoh);
  vertical.write(servov);
  delay(100);
  Serial.begin(9600);
}

void loop() 
{
  char   Mode;
  buttonState1 = digitalRead(2);
  if (buttonState1 != prevButtonState1)   {
    if (buttonState1 == HIGH) {
      //Change mode and ligh up the correct   indicator  
      if (mode == 1) {
        mode = 0;
      } else {
         mode = 1;
      }
    }
  }
  prevButtonState1 = buttonState1;
  delay(50); // Wait for 50 millisecond(s)
  
  if (mode == 0) {
    Mode='M';
    Serial.println(Mode);   //send Mode "Manual" to serial port    
    manualsolartracker();
   } else { // mode automatic
    Mode = 'A';
    Serial.println(Mode);      
    automaticsolartracker(); //send Mode "Automatic" to serial port
   } 
}
void manualsolartracker(){
  buttonState2   = digitalRead(3);
  if (buttonState2 != prevButtonState2) {
    if (buttonState2   == HIGH) {
      //Change mode and ligh up the correct indicator  
      if   (axe == 1) {
        axe = 0;
      } else {
        axe = 1;
      }
     }
  }
  prevButtonState2 = buttonState2;
  delay(50); // Wait for 50   millisecond(s)
  if (axe == 0) {     //control right-left movement
    horizontal.write(map(analogRead(A4),   0, 1023, 0, 180));
  } else { // //control up-down movement
    vertical.write(map(analogRead(A4),   0, 1023, 0, 180));
  } 
}
void   automaticsolartracker(){
  //Read the illuminance values ​​of 4 photoresistor modules respectively
  int lt = analogRead(ldrlt); //upper left
  int rt = analogRead(ldrrt); //top right
  int ld = analogRead(ldrld); //down left
  int rd = analogRead(ldrrd); //down right
  Serial.print("lt:");Serial.print(lt);Serial.print(" ");
  Serial.print("rt:");Serial.print(rt);Serial.print(" ");
  Serial.print("ld:");Serial.print(ld);Serial.print(" ");
  Serial.print("rd:");Serial.println(rd);
  delay(50); // Wait for 50   millisecond(s)
  //Average readings from adjacent photoresistor modules    
  int avt = (lt + rt) / 2; 
  int avd = (ld + rd) / 2; 
  int avl = (lt + ld) / 2; 
  int avr = (rt + rd) / 2;

  int veg=(avt + avd + avl + avr) / 4;

 //Adjust response parameters according to different light intensities
 if(0<veg&&veg<300)
 { tol=map(veg,10,300,5,100);
   dtime=map(veg,10,300,100,50);
 }
 else
 { tol=50;
   dtime=50;
 }

  Serial.print("veg= ");Serial.println(veg);
  Serial.print("tol= ");Serial.println(tol);
  Serial.print("dtime= ");Serial.println(dtime);
  delay(50); // Wait for 50   millisecond(s)
  
  //Then calculate the difference between the upper and lower rows and the average value of the left and right rows
  int dvert = avt - avd;    //upper and lower rows 
  int dhoriz = avl - avr;   //left and right rows

   //Check if the difference is within tolerance, otherwise change the vertical angle    
   if (-1*tol > dvert || dvert > tol) 
   {
      if (avt > avd)
      {
        servov = ++servov;
         if (servov > servovLimitHigh) 
         { 
           servov = servovLimitHigh;
         }
      }
      else if (avt < avd)
      {
        servov= --servov;
        if (servov < servovLimitLow)
        {
          servov = servovLimitLow;
        }
      }
      vertical.write(servov); //If the rotation angle of the servo is opposite to the light, use (180- servov) or (servov) to change the direction
    }

  //Check if the difference is within tolerance, otherwise change the horizontal angle 
  if (-1*tol > dhoriz || dhoriz > tol) 
  {
    if (avl < avr)
    {
      servoh = --servoh;
      if (servoh < servohLimitLow)
      {
        servoh = servohLimitLow;
      }
    }
    else if (avl > avr)
    {
       servoh = ++servoh;
       if (servoh > servohLimitHigh)
       {
         servoh = servohLimitHigh;
       }
    }
    horizontal.write(servoh); //If the rotation angle of the servo is opposite to the light, use (180- servoh) or (servoh) to change the direction
  }
   delay(dtime);
}

