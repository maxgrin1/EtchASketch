/* Joystick control for a etch a sketch using bresenhams line algorithm. 
   by Max van Grinsven 
   Using: 2 NEMA 17 1.5A stepper motors, 2 DRV8825 drivers, a 12v 4.2A Powersupply, Arduino Uno/Mega, a cheap joystick and a bunch of cables.
   Should work with every steps/rot, higher step/rot means more accuracy but less speed
    */
//Joystick pins
#define joyX A0
#define joyY A1
//Motor pins
#define stepA 3
#define stepB 9
#define dirA 5
#define dirB 6
//Delays for driving the motors.
int pulseWidthDelay = 2; //See data sheet DRV8825, probably unneccesary but just to be sure.
int speedDelayA = 3; //Tunable but don't lower it to much
int speedDelayB = 3;
//A bunch of variables used for calculation and/or positioning.
int valX, valY;
boolean dirX, dirY;
float pdirx = 0, pdiry = 0;
int currentX = 0, currentY = 0;

void setup() { 
  //Initialize pins used.
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
  pinMode(stepA, OUTPUT);
  pinMode(stepB, OUTPUT);
  pinMode(dirA, OUTPUT);
  pinMode(dirB, OUTPUT);
}

void loop() {
  //Read raw values from the joystick.
  valX = analogRead(joyX);
  valY = analogRead(joyY);

  //Get the X directions and magnitude of this direction.
  if(valX < 500){
    pdirx = map(valX, 0, 499, -100, 0); 
    pdirx = pdirx/100; //devide by hundred because the map() function cannot generate decimal values
  } else if(valX > 524){
    pdirx = map(valX, 525, 1023, 0, 100);
    pdirx = pdirx/100;
  } else if(valX >= 500 && valX <= 524){ //'dead-zone' removes most of the jittering.
    pdirx = 0;
  } 
  //Get the Y directions and magnitude of this direction.
  if(valY < 500){
    pdiry = map(valY, 0, 499, -100, 0);
    pdiry = pdiry/100;
  } else if(valY > 524){
    pdiry = map(valY, 525, 1023, 0, 100);
    pdiry = pdiry/100;
  } else if(valY >= 500 && valY <=524){
    pdiry = 0;
  }
  //Generate the points to draw to, the 10 is variable, play with it to find what works the best.
  int toX = round(currentX + 10*pdirx);
  int toY = round(currentY + 10*pdiry);

  //the only thing you see
  drawLine(toX, toY);
  
}

void drawLine(int toX, int toY){
  //Using Bresenhams Line algorithm from http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C.
  int dx = abs(toX-currentX);
  int sx = currentX<toX ? 1 : -1;
  int dy = abs(toY-currentY);
  int sy = currentY<toY ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2;
  int e2;
  //Determine the direction
  if(sx == 1){ dirX = 0;} else {dirX=1;}
  if(sy == 1){ dirY = 0;} else {dirY=1;}
    while(1==1){
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      steps(1, dirX, 1);
      currentX += sx;
      }
    if (e2 < dy) {
      err = err + dx; 
      steps(1, dirY, 0);
      currentY = currentY + sy;
      }
      if(currentX == toX && currentY == toY){
        break;
      }
  }}
//Custom step function
void steps(int numSteps, boolean dir, boolean AorB){
  //Example: steps(800, 0, 0); for 800 steps, clockwise and steppermotor 0.
  if(AorB == 0){
    if(dir == 0){
      digitalWrite(dirA, LOW);
    } else digitalWrite(dirA, HIGH);
    for(int c = 0; c < numSteps; c++){
      digitalWrite(stepA, HIGH);
      delayMicroseconds(pulseWidthDelay);
      digitalWrite(stepA, LOW);
      delayMicroseconds(pulseWidthDelay);
      delay(speedDelayA);
    }
  }
  if(AorB == 1){
    if(dir == 0){
      digitalWrite(dirB, LOW);
    } else digitalWrite(dirB, HIGH);
    for(int c = 0; c < numSteps; c++){
      digitalWrite(stepB, HIGH);
      delayMicroseconds(pulseWidthDelay);
      digitalWrite(stepB, LOW);
      delayMicroseconds(pulseWidthDelay);
      delay(speedDelayB);
    }
  }
}
