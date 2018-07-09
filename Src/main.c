#include "h8-3052-iodef.h"
#include "h8-3052-int.h"

#include "lcd.h"
#include "ad.h"
#include "timer.h"
#include "key.h"

#include "driver.h"
#include "control.h"

/* タイマ割り込みの時間間隔[μs] */
#define TIMER0 1000

/* 割り込み処理で各処理を行う頻度を決める定数 */
#define ADTIME  2
#define PWMTIME 1
#define CONTROLTIME 10

/* A/D変換関連 */
/* A/D変換のチャネル数とバッファサイズ */
#define ADCHNUM   4
#define ADBUFSIZE 8
/* 平均化するときのデータ個数 */
#define ADAVRNUM 4
/* チャネル指定エラー時に返す値 */
#define ADCHNONE -1


/* 割り込み処理に必要な変数 */
static volatile int adTime, pwmTime;

/* A/D変換関係 */
static volatile unsigned char adbuf[ADCHNUM][ADBUFSIZE];
static volatile int adbufdp;

void int_imia0(void);
void int_adi(void);
int  ad_read(int ch);

int main(void)
{
  /* 初期化 */
  ROMEMU();           /* ROMエミュレーションをON */

  /* 割り込みで使用する大域変数の初期化 */
  pwmTime = 0;     /* PWM制御関連 */
  adTime = 0;                  /* A/D変換関連 */
  /* ここまで */
  adbufdp = 0;         /* A/D変換データバッファポインタの初期化 */
  ad_init();           /* A/Dの初期化 */
  timer_init();        /* タイマの初期化 */
  timer_set(0,TIMER0); /* タイマ0の時間間隔をセット */
  timer_start(0);      /* タイマ0スタート */
  ENINT();             /* 全割り込み受付可 */

  while(1);


  /* その他の処理はタイマ割り込みによって自動的に実行されるため  */
  /* タイマ 0 の割り込みハンドラ内から各処理関数を呼び出すことが必要 */
  }
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

#pragma interrupt
void int_adi(void)
     /* A/D変換終了の割り込みハンドラ                               */
     /* 関数の名前はリンカスクリプトで固定している                   */
     /* 関数の直前に割り込みハンドラ指定の #pragma interrupt が必要  */
{
  ad_stop();    /* A/D変換の停止と変換終了フラグのクリア */

  /* ここでバッファポインタの更新を行う */
  /* 　但し、バッファの境界に注意して更新すること */
  adbufdp++;
  if(adbufdp >= ADBUFSIZE) adbufdp = 0;

  /* ここでバッファにA/Dの各チャネルの変換データを入れる */
  /* スキャングループ 0 を指定した場合は */
  /*   A/D ch0〜3 (信号線ではAN0〜3)の値が ADDRAH〜ADDRDH に格納される */
  /* スキャングループ 1 を指定した場合は */
  /*   A/D ch4〜7 (信号線ではAN4〜7)の値が ADDRAH〜ADDRDH に格納される */
  adbuf[0][adbufdp] = ADDRAH;
  adbuf[1][adbufdp] = ADDRBH;
  adbuf[2][adbufdp] = ADDRCH;
  adbuf[3][adbufdp] = ADDRDH;

  ENINT();      /* 割り込みの許可 */
}

int ad_read(int ch)
     /* A/Dチャネル番号を引数で与えると, 指定チャネルの平均化した値を返す関数 */
     /* チャネル番号は，0〜ADCHNUM の範囲 　　　　　　　　　　　             */
     /* 戻り値は, 指定チャネルの平均化した値 (チャネル指定エラー時はADCHNONE) */
{
  int i,ad,bp;

  if ((ch > ADCHNUM) || (ch < 0)) ad = ADCHNONE; /* チャネル範囲のチェック */
  else {

    /* ここで指定チャネルのデータをバッファからADAVRNUM個取り出して平均する */
    /* データを取り出すときに、バッファの境界に注意すること */
    /* 平均した値が戻り値となる */
    ad = 0;
    bp = adbufdp;

    for(i = 0;i < ADAVRNUM;i++){
      ad += adbuf[ch][bp];
      bp--;
      if(bp <= 0)bp = ADBUFSIZE-1;
    }
    ad /= ADAVRNUM;
  }
  return ad; /* データの平均値を返す */
}

