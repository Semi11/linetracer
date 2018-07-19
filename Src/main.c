#include "h8-3069-iodef.h"
#include "h8-3069-int.h"

#include "ad.h"
#include "timer.h"

#include "driver.h"
#include "control.h"
#include "ad_read.h"

/* タイマ割り込みの時間間隔[μs] */
#define TIMER0 100

/* 割り込み処理で各処理を行う頻度を決める定数 */
#define ADTIME  2
#define PWMTIME 1
#define CONTROLTIME 10


/* 割り込み処理に必要な変数 */
static volatile int adTime, pwmTime, ctlTime;

void int_imia0(void);

int main(void)
{
  /* 初期化 */
  ROMEMU();           /* ROMエミュレーションをON */

  /* 割り込みで使用する大域変数の初期化 */
  pwmTime = 0;     /* PWM制御関連 */
  adTime = 0;                  /* A/D変換関連 */
  ctlTime = 0;

  ad_init();           /* A/Dの初期化 */
  timer_init();        /* タイマの初期化 */
  timer_set(0,TIMER0); /* タイマ0の時間間隔をセット */
  timer_start(0);      /* タイマ0スタート */
  ENINT();             /* 全割り込み受付可 */

  /* bポート初期化 */
  PBDDR = 0x0F;

  while(1);


  /* その他の処理はタイマ割り込みによって自動的に実行されるため  */
  /* タイマ 0 の割り込みハンドラ内から各処理関数を呼び出すことが必要 */

  return 1;
}

#pragma interrupt
void int_imia0(void)
     /* タイマ0(GRA) の割り込みハンドラ　　　　　　　　　　　　　　　 */
     /* 関数の名前はリンカスクリプトで固定している                   */
     /* 関数の直前に割り込みハンドラ指定の #pragama interrupt が必要 */
     /* タイマ割り込みによって各処理の呼出しが行われる               */
     /*   呼出しの頻度は KEYTIME,ADTIME,PWMTIME,CONTROLTIME で決まる */
     /* 全ての処理が終わるまで割り込みはマスクされる                 */
     /* 各処理は基本的に割り込み周期内で終わらなければならない       */
{
  /* PWM処理 */
  pwmTime++;
  if(pwmTime >= PWMTIME){
    pwmProc();
    pwmTime = 0;
  }

  /* A/D変換 */
  adTime++;
  if(adTime >= ADTIME){
    ad_scan(0,1);
    adTime = 0;
  }

  /* モータ制御 */
  ctlTime++;
  if(ctlTime >= CONTROLTIME){
    controlMotor();
    ctlTime = 0;
  }

  timer_intflag_reset(0); /* 割り込みフラグをクリア */
  ENINT();                /* CPUを割り込み許可状態に */
}
