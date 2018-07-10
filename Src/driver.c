/*driver.c*/
#include"../Inc/driver.h"
void driveMotor(driveMode_t, int);
/*各モータのパラメータを初期化*/
motorParam_t motorParamList[MOTOR_NUM] = {
  {
    .duty = 0;
    .mode = STOP;
  },
  {
    .duty = 0;
    .mode = STOP;
  },
};

void driveMotor(driveMode_t motor, int move){
  swich(motor){
    case STOP:
    if(move == MOTOR_L){
      PB0 = 0;
      PB1 = 0;
    }else if(move == MOTOR_R){
      PB2 = 0
      PB3 = 0
    }
    break;
    case FORWARD:
    if(move == MOTOR_L){
      PB0 = 1;
      PB1 = 0;
    }else if(move == MOTOR_R){
      PB2 = 1;
      PB3 = 0;
    }
    break;
    case BAKCWARD:
    if(move == MOTOR_L){
      PB0 = 0;
      PB1 = 1;
    }else if(move == MOTOR_R){
      PB2 = 0;
      PB3 = 1;
    }
    break;
    case BRAKE:
    if(move == MOTOR_L){
      PB0 = 1;
      PB1 = 1;
    }else if(move == MOTOR_R){
      PB2 = 1;
      PB3 = 1;
    }
    break;
  }

}
