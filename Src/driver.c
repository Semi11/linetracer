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
