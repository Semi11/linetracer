/*driver.c*/

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
