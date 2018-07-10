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


