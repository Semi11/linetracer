/* control.c */

#include "control.h"

const char curve[] = { 0, 1, 0, 0, 1, 0, 1 };

void controlMotor(void){

  static char isLeftOffLine, isRightOffLine;
  // 初期のノイズをカットする。
  static int temp = 100;;
  static int offlineCount;
  static char curveCount;

  isLeftOffLine = !readSensor(LEFT);
  isRightOffLine = !readSensor(RIGHT);

  if (isLeftOffLine && isRightOffLine) {
    if (offlineCount != 50) offlineCount++;
    motorParamList[MOTOR_L].duty = 65;
    motorParamList[MOTOR_L].mode = curve[curveCount % 7] ? BACKWARD : FORWARD;
    motorParamList[MOTOR_R].duty = 65;
    motorParamList[MOTOR_R].mode = curve[curveCount % 7] ? FORWARD : BACKWARD;
  } else {
    if (temp != 10) temp++;
    // 初期のノイズをカットする。
    if (temp > 200) {
      offlineCount = 0;
      temp = 0;
    }
    if (temp == 10 && offlineCount == 50) {
      curveCount++;
      temp = 0;
      offlineCount = 0;
    }

    if (isRightOffLine) {
      motorParamList[MOTOR_L].duty = 0;
      motorParamList[MOTOR_L].mode = STOP;
    } else {
      motorParamList[MOTOR_L].duty = 65;
      motorParamList[MOTOR_L].mode = FORWARD;
    }
    if (isLeftOffLine) {
      motorParamList[MOTOR_R].duty = 0;
      motorParamList[MOTOR_R].mode = STOP;
    } else {
      motorParamList[MOTOR_R].duty = 65;
      motorParamList[MOTOR_R].mode = FORWARD;
    }
  }

}
