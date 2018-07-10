/*driver.h*/
#ifndef __DRIVER_H
#define __DRIVER_H

#define MOTOR_NUM 2

/*モータのインデックス*/
#define MOTOR_L 0
#define MOTOR_R 1

#define PWM_MAX 1000

/*センサー指定用*/
typedef enum{
  LEFT, RIGHT,
}sensorKind_t;

/*駆動モード*/
typedef enum{
  STOP,FORWARD,BACKWARD,BRAKE,
}driveMode_t;

/*モータのパラメータ(現在のモータの情報ではない)*/
typedef struct{
  int duty;
  driveMode_t mode;
}motorParam_t;

int readSensor(sensorKind_t); /*指定センサーの値を読み取る*/
void pwmProc(void); /*割り込みハンドラ モータ情報をもとにモータの駆動を指定*/
void driveMotor(driveMode_t, int); /*指定モータを指定モードで駆動させる*/

/*各モータの情報*/
extern motorParam_t motorParamList[MOTOR_NUM];

#endif
