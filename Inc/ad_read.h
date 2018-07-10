/*ad_read.h*/
#ifndef __AD_READ_H
#define __AD_READ_H

/* A/D変換関連 */
/* A/D変換のチャネル数とバッファサイズ */
#define ADCHNUM   4
#define ADBUFSIZE 8
/* 平均化するときのデータ個数 */
#define ADAVRNUM 4
/* チャネル指定エラー時に返す値 */
#define ADCHNONE -1

void int_adi(void);
int  ad_read(int ch);

#endif
