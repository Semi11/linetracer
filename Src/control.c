/* control.c */

#include "control.h"

void controlMotor(void){

  if(readSensor(RIGHT)==1){
    //motorParamList[MOTOR_L].duty = 99;
    motorParamList[MOTOR_L].duty = 50;
    motorParamList[MOTOR_L].mode = FORWARD;
  }else{
    motorParamList[MOTOR_L].duty = 0;
    motorParamList[MOTOR_L].mode = BRAKE;
  }

  if(readSensor(LEFT)==1){
    //motorParamList[MOTOR_R].duty = 99;
    motorParamList[MOTOR_R].duty = 60;
    motorParamList[MOTOR_R].mode = FORWARD;
  }else{
    motorParamList[MOTOR_R].duty = 0;
    motorParamList[MOTOR_R].mode = BRAKE;
  }

  if(readSensor(LEFT)==0 && readSensor(RIGHT)==0){
    motorParamList[MOTOR_L].duty = 99;
    motorParamList[MOTOR_L].mode = FORWARD;
    motorParamList[MOTOR_R].duty = 0;
    motorParamList[MOTOR_R].mode = BRAKE;

  }
}
