/* control.c */

#include "control.h"

void controlMotor(void){
  int i;

  for(i=0;i<MOTOR_NUM;i++){
    motorParamList(i)->duty = 30;
    motorParamList(i)->mode = FORWARD;
  }
}
