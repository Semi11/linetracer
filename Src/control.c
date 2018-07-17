/* control.c */

#include "control.h"

void controlMotor(void){

  if(readSensor(LEFT)){
    motorParamList[MOTOR_L].duty = 60;
    motorParamList[MOTOR_L].mode = FORWARD;
  }else{
    motorParamList[MOTOR_L].duty = 0;
    motorParamList[MOTOR_L].mode = STOP;
  }

  if(readSensor(RIGHT)){
    motorParamList[MOTOR_R].duty = 60;
    motorParamList[MOTOR_R].mode = FORWARD;
  }else{
    motorParamList[MOTOR_R].duty = 0;
    motorParamList[MOTOR_R].mode = STOP;
  }

}
