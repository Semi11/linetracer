/*driver.c*/

#include "driver.h"

/*各モータのパラメータを初期化*/
motorParam_t motorParamList[MOTOR_NUM] = {
  {
    .duty = 0,
    .mode = STOP,
  },
  {
    .duty = 0,
    .mode = STOP,
  },
};

int readSensor(sensorKind_t kind)
{
  
  if(kind == LEFT){
    if(ad_read(1) < 128) return 1;
    else return 0;
  }
  else if(kind == RIGHT){
    if(ad_read(2) < 128) return 1;
    else return 0;
  }else {
    return -1;
  }
}

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

void pwmProc(void)
{
  static int pwmCount;
  int i;

  for (i = 0; i < MOTOR_NUM; i++) {
    if (pwmCount < motorParamList[i].duty * PWM_MAX / 100) {
      driveMotor(motorParamList[i].mode, i);
    } else {
      driveMotor(STOP, i);
    }
  }

  pwmCount++;
  if (pwmCount >= PWM_MAX) {
    pwmCount = 0;
  }
}
