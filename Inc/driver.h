/*driver.h*/
#ifndef __DRIVER_H
#define __DRIVER_H

#define MOTOR_NUM 2

/* タイマ割り込みの時間間隔[μs] */
#define TIMER0 256

/* 割り込み処理で各処理を行う頻度を決める定数 */
#define ADTIME  2
#define PWMTIME 1
#define CONTROLTIME 10

/* モータの駆動モード変更時に貫通電流対策のために必要な待ち時間[μs] */
#define MODE_CHANGE_TIME 100

/*モータのインデックス*/
#define MOTOR_L 0
#define MOTOR_R 1

#define PWM_MAX 100

#define THRESHOLD_VALUE 200

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

void init(void); /*初期化*/
int readSensor(sensorKind_t); /*指定センサーの値を読み取る*/
void pwmProc(void); /*割り込みハンドラ モータ情報をもとにモータの駆動を指定*/
void driveMotor(driveMode_t, int); /*指定モータを指定モードで駆動させる*/

/*各モータの情報*/
extern motorParam_t motorParamList[MOTOR_NUM];

#endif
