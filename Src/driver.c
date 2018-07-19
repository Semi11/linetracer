/*driver.c*/
#include "h8-3069-iodef.h"

#include "driver.h"
#include "ad_read.h"

void init(void){
  ROMEMU();           /* ROMエミュレーションをON */

  ad_init();           /* A/Dの初期化 */
  timer_init();        /* タイマの初期化 */
  timer_set(0,TIMER0); /* タイマ0の時間間隔をセット */
  timer_start(0);      /* タイマ0スタート */
  ENINT();             /* 全割り込み受付可 */

  /* bポート初期化 */
  PBDDR = 0x0F;
}

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
    if(ad_read(1) < THRESHOLD_VALUE) return 1;
    else return 0;
  }
  else if(kind == RIGHT){
    if(ad_read(2) < THRESHOLD_VALUE) return 1;
    else return 0;
  }else {
    return -1;
  }
}

void driveMotor(driveMode_t motor, int move){
  switch(motor){
    case STOP:
    if(move == MOTOR_L){
      PBDR &= 0xfc;
    }else if(move == MOTOR_R){
      PBDR &= 0xf3;
    }
    break;
    case FORWARD:
    if(move == MOTOR_L){
      PBDR &= 0xfd;
      PBDR |= 0x01;
    }else if(move == MOTOR_R){
      PBDR &= 0xf7;
      PBDR |= 0x04;
    }
    break;
    case BACKWARD:
    if(move == MOTOR_L){
      PBDR &= 0xfe;
      PBDR |= 0x02;
    }else if(move == MOTOR_R){
      PBDR &= 0xfb;
      PBDR |= 0x08;
    }
    break;
    case BRAKE:
    if(move == MOTOR_L){
      PBDR |= 0x03;
    }else if(move == MOTOR_R){
      PBDR |= 0x0c;
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
