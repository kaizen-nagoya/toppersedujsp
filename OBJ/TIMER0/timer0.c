/*
 *  TOPPERS/JSP Educative Program
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2003-2004 by Platform Development Center
 *                              RICOH COMPANY,LTD. JAPAN
 *
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)～(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: timer0.c,v 1.0 2004/02/06 14:55:24 roi Exp $
 */

/* 
 *  TOPPERS初級教育用のサンプルプログラム
 *  RTOSを使わないケース、mainは２つのプロセスを一定時間で起動する．
 *  switch_processはスイッチの監視を行い．全体の制御を行う．
 *  timer_processでは時間とLEDの管理を行い、簡単なタイマーを実現する．
 *  通常はLED3が１秒毎に点滅する．
 *  スイッチ５オンでタイマー起動、10秒ごとにLED2の点滅、１分後にLED2が
 *  アラート点滅する。このときスイッチ４をオンするごとにアラート点滅時間が
 *  １分加算され、アラート点滅時間を延長する．
 *  ３分後にアラート点滅させるには、スイッチ５をオン、その後、スイッチ４を
 *  ２回オンにする．
 *  また、ドライバーはPDICに順ずるため、sfrは使用しない．
 *
 */
#include <t_services.h>
#include "oaks16mini.h"
#include "sim_device.h"

/*
 *  プロトタイプ宣言
 */
void	main(void);					/* メイン関数	*/
void	switch_process(void);		/* スイッチプロセス */
void	timer_process(void);		/* タイマープロセス */
void	timer_handler_entry(void);	/* 割込み関数	*/
#pragma	INTERRUPT	timer_handler_entry

/*
 *  タイマプロセスの状態
 */
enum TIMER_STATE {
	STATE_STOP_TIMER,		/* タイマー停止状態 */
	STATE_ACT_TIMER,		/* タイマー実行状態 */
	STATE_TIMEOUT			/* タイムアウト状態 */
};

/*
 *  イベントの定義
 */
#define	EVT_TIMER_START		0x01
#define	EVT_TIMER_STOP		0x02
#define	EVT_TIMER_COUNT		0x04

/*
 *  変数の宣言
 */
static unsigned long base_time;
static unsigned char event;
static unsigned char sw4;
static unsigned char sw5;
static unsigned long max_time;
static unsigned long current_time;
static unsigned char timer_state;
static unsigned char sec;
static unsigned char alaem;
static unsigned char led2;
static unsigned char led3;

/*
 *  カップラーメンタイマーのメイン関数
 *  ハードウェアとデータの初期化後
 *  switch_processとtimer_processを250ms毎に
 *  起動する．
 */
void
main(void)
{
	unsigned long timer250;

	initial_key();					/* スイッチの初期化 */
	initial_led();					/* LEDの初期化 */
	initial_timer();				/* タイマーの初期化 */
	base_time = 0;
	event = 0;
	timer250 = 250;

	_asm( "\tFSET	I");			/* 割り込み許可	*/

	sw4 = get_key(SW4);				/* 現在のSW4の取り込み */
	sw5 = get_key(SW5);				/* 現在のSW5の取り込み */
	timer_state = STATE_STOP_TIMER;	/* timer_process初期化状態 */
	sec = 0;
	alaem = 0;
	led2 = OFF;
	led3 = OFF;

	for(;;){					/* 永久ループ */
		if(timer250 <= base_time){
			switch_process();
			timer_process();
			timer250 += 250;
		}
	}
}

/*
 *  タイマ用プロセス
 */
void
timer_process(void)
{
	if(event != 0){		/* switch_processより通知 */
		if(event & EVT_TIMER_START){	/* 開始 */
			timer_state = STATE_ACT_TIMER;
			current_time = 0;
			max_time = 60*1000;
		}
		if(event & EVT_TIMER_STOP){		/* 停止 */
			timer_state = STATE_STOP_TIMER;
			alaem = 0;
		}
		if(event & EVT_TIMER_COUNT){	/* 時間アップ */
			if(timer_state == STATE_ACT_TIMER){
				max_time += 60*1000;
			}
		}
		event = 0;
	}
	switch(timer_state){	/* タイマー状態の判定 */
	case STATE_ACT_TIMER:	/* タイマー実行中状態 */
		if(current_time >= max_time){
			alaem = 15*4;
			timer_state = STATE_TIMEOUT;
		}
		else if((current_time % (10*1000)) == 0){
			alaem = 1*4;
		}
		current_time += 250;/* タイムアップ */
		break;
	case STATE_TIMEOUT:		/* タイムアウト状態 */
		timer_state = STATE_STOP_TIMER;
		break;
	default:				/* タイマー停止中状態 */
		break;
	}
	if(++sec > 3){			/* 時間の表示：1秒経過 */
		led3 ^= ON;			/* LED3 のオン 、 オフを切り替える */
		sec = 0;
	}
	if(alaem > 0){			/* 警告の要求：250ms経過 */
		led2 ^= ON;			/* LED2 のオン 、 オフを切り替える */
		alaem--;
	}
	else					/* 警告の要求がなくLED2がオンなら消灯 */
		led2 = OFF;

	set_led(LED2, led2);	/* LED2の設定 */
	set_led(LED3, led3);	/* LED3の設定 */
}

/*
 *  スイッチプロセス
 */
void
switch_process(void)
{
	unsigned char sw;

	sw = get_key(SW5);
	if(sw5 != sw){
		if(sw == ON)
			event |= EVT_TIMER_START;
		else
			event |= EVT_TIMER_STOP;
		sw5 = sw;
	}
	sw = get_key(SW4);
	if(sw4 != sw){
		if(sw == ON)
			event |= EVT_TIMER_COUNT;
		sw4 = sw;
	}
}

/*
 *  タイマーA0割込み関数
 *  1ms単位に起動される
 *  base_timeは起動からのミリ秒のカウントを持つ
 */
void
timer_handler_entry(void)
{
	base_time++;
}

