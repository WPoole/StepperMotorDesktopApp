// Shows how to run three Steppers at once with varying speeds
//
// Requires the Adafruit_Motorshield v2 library
//   https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
// And AccelStepper with AFMotor support
//   https://github.com/adafruit/AccelStepper

// This is for Adafruit Motorshield v2 only!
// Will not work with v1 shields

//Include necessary libraries
#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"


//Adafruit Motorshield object.
Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

//Create 2 stepper motor object. The 1 indicates the motor on M1 and M2
// and the 2 indicates the motor on M3 and M4.
Adafruit_StepperMotor *myStepper1 = AFMStop.getStepper(200, 1);
Adafruit_StepperMotor *myStepper2 = AFMStop.getStepper(200, 2);

//Declare Variables
String stateA;
int intStateA;
String stateB;
int intStateB;
String temp = "Initial";
int tempo;

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!

//Motor 1 move forward.
void forwardstep1() {
  myStepper1->onestep(FORWARD, MICROSTEP);
}

//Motor 1 move backward.
void backwardstep1() {
  myStepper1->onestep(BACKWARD, MICROSTEP);
}
// wrappers for the second motor!

//Motor 2 move forward.
void forwardstep2() {
  myStepper2->onestep(FORWARD, MICROSTEP);
}

//Motor 2 move backward.
void backwardstep2() {
  myStepper2->onestep(BACKWARD, MICROSTEP);
}


// Now we'll wrap the 2 steppers in an AccelStepper object
//Move forward
AccelStepper stepper1F(forwardstep1, forwardstep1);
AccelStepper stepper2F(forwardstep2, forwardstep2);
//Move backward
AccelStepper stepper1B(backwardstep1, backwardstep1);
AccelStepper stepper2B(backwardstep2, backwardstep2);


void setup() {
  //9600 baudrate.
  Serial.begin(9600);
  Serial.println("Stepper test!");
  
  // Start the top shield
  AFMStop.begin(); 

  //Forward
  //Set the max speeds to 10000 for each to make
  //sure the motors can spin quickly enough.
  //Also set the acceleration to be an insanely large
  //number. By doing this, it gives the impression that
  //there is no "gradual" acceleration. 
  stepper1F.setMaxSpeed(10000);
  stepper1F.setAcceleration(10000000000000);
  stepper1F.moveTo(1);

  stepper2F.setMaxSpeed(10000);
  stepper2F.setAcceleration(10000000000000);
  stepper2F.moveTo(1);

  //Backward
  stepper1B.setMaxSpeed(10000);
  stepper1B.setAcceleration(10000000000000);
  stepper1B.moveTo(1);

  stepper2B.setMaxSpeed(10000);
  stepper2B.setAcceleration(10000000000000);
  stepper2B.moveTo(1);

}


void loop() {
  
  Serial.println(temp);
  //  This one DOES NOT work (well).
  //  SOLUTION: TO GET AROUND THE INCONSISTENCY, YOU MUST
  //  SIMPLY DOUBLE TYPE THE CHARACTER YOU WANT TO ENTER.
  //  E.G. IF YOU WANT TO INPUT "e", YOU MUST ACTUALLY 
  //  ENTER "ee".
  //  Serial.available check to see if there is any data 
  //  to be read from the serial port.
  
  if (Serial.available() || temp != "Initial") {
    
    // If this block isn't here, you can only change the motor
    // direction by entering letters in DECREASING order.
    // Serial.peek() just looks at the first character available
    // to be read from the serial port at that time. The characters
    // that get "peeked" at always come out in ascii value form.
    if(Serial.available()) {
      tempo = Serial.peek();
      if(tempo = 97) {
        temp = "a";
      }
      else if(tempo = 98) {
        temp = "b";
      }
      else if(tempo = 99) {
        temp = "c";
      }
      else if(tempo = 100) {
        temp = "d";
      }
      else if(tempo = 101) {
        temp = "e";
      }
      else if(tempo = 102) {
        temp = "f";
      }
    }
  
    //If "a" is first in serial port, set the speed
    //of Stepper motor 1 to be the value following the "a".
    if (Serial.peek() == 'a') {
      Serial.println(Serial.read());
      stateA = Serial.readString();
      intStateA = stateA.toInt();
      stepper1F.setMaxSpeed(intStateA);
      stepper1B.setMaxSpeed(intStateA);
//      temp = "AAAAAAAA";
    }
    
    //If "b" is first in serial port, set the speed
    //of Stepper motor 2 to be the value following the "b".
    else if (Serial.peek() == 'b') {
      Serial.read();
      stateB = Serial.readString();
      intStateB = stateB.toInt();
      stepper2F.setMaxSpeed(intStateB);
      stepper2B.setMaxSpeed(intStateB);
//      temp = "BBBBBBBBB";
    }
    
    //M1 FORWARD & M2 FORWARD 
    else if (Serial.peek() == 'c' || temp == "c") {
      
      stepper1F.setMaxSpeed(intStateA);
      stepper1B.setMaxSpeed(intStateA);
      stepper2F.setMaxSpeed(intStateB);
      stepper2B.setMaxSpeed(intStateB);
      
      if (stepper2F.distanceToGo() == 0)
        stepper2F.moveTo(-stepper2F.currentPosition());
      if (stepper1F.distanceToGo() == 0)
        stepper1F.moveTo(-stepper1F.currentPosition());
      stepper1F.run();
      stepper2F.run();
      
      temp = "c";
      if(Serial.available()) {
        Serial.read();
      }
    }
    
    //M1 FORWARD & M2 BACKWARD
    else if (Serial.peek() == 'd' || temp == "d") {
      
      stepper1F.setMaxSpeed(intStateA);
      stepper1B.setMaxSpeed(intStateA);
      stepper2F.setMaxSpeed(intStateB);
      stepper2B.setMaxSpeed(intStateB);
      
      if (stepper2B.distanceToGo() == 0)
        stepper2B.moveTo(-stepper2B.currentPosition());
      if (stepper1F.distanceToGo() == 0)
        stepper1F.moveTo(-stepper1F.currentPosition());
      stepper1F.run();
      stepper2B.run();
      
      temp = "d";
      if(Serial.available()) {
        Serial.read();
      }
    }

    //M1 BACKWARD & M2 FORWARD
    else if (Serial.peek() == 'e' || temp == "e") {
      
      stepper1F.setMaxSpeed(intStateA);
      stepper1B.setMaxSpeed(intStateA);
      stepper2F.setMaxSpeed(intStateB);
      stepper2B.setMaxSpeed(intStateB);
      
      if (stepper2F.distanceToGo() == 0)
        stepper2F.moveTo(-stepper2F.currentPosition());
      if (stepper1B.distanceToGo() == 0)
        stepper1B.moveTo(-stepper1B.currentPosition());
      stepper1B.run();
      stepper2F.run();
      
      temp = "e";
      if(Serial.available()) {
        Serial.read();
      }
    }

    //M1 BACKWARD & M2 BACKWARD
    else if (Serial.peek() == 'f' || temp == "f") {
      
      stepper1F.setMaxSpeed(intStateA);
      stepper1B.setMaxSpeed(intStateA);
      stepper2F.setMaxSpeed(intStateB);
      stepper2B.setMaxSpeed(intStateB);
      
      if (stepper2B.distanceToGo() == 0)
        stepper2B.moveTo(-stepper2B.currentPosition());
      if (stepper1B.distanceToGo() == 0)
        stepper1B.moveTo(-stepper1B.currentPosition());
      stepper1B.run();
      stepper2B.run();
      
      temp = "f";
      if(Serial.available()) {
        Serial.read();
      }
    }  
  }
}



