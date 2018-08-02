/* control.c */

#include "control.h"

void controlMotor(void){

  if(readSensor(RIGHT)==1){
    motorParamList[MOTOR_L].duty = 0;
    motorParamList[MOTOR_L].mode = BRAKE;
    motorParamList[MOTOR_R].duty = 99;
    motorParamList[MOTOR_R].mode = FORWARD;
  }else{
    motorParamList[MOTOR_L].duty = 99;
    motorParamList[MOTOR_L].mode = FORWARD;
    motorParamList[MOTOR_R].duty = 99;
    motorParamList[MOTOR_R].mode = BACKWARD;
  }

}
