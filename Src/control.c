/* control.c */

#include "control.h"

void controlMotor(void){
  //static volatile int cnt;

  if(readSensor(RIGHT)==1){
    motorParamList[MOTOR_R].duty = 99;
    motorParamList[MOTOR_R].mode = FORWARD;
    motorParamList[MOTOR_L].duty = 99;
    motorParamList[MOTOR_L].mode = BACKWARD;
  }else{
    motorParamList[MOTOR_R].duty = 0;
    motorParamList[MOTOR_R].mode = BRAKE;
    motorParamList[MOTOR_L].duty = 80;
    motorParamList[MOTOR_L].mode = FORWARD;
  }
  //if(++cnt<2)cnt=0;
}
