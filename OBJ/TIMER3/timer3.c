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
 *  @(#) $Id: timer3.c,v 1.1 2004/03/23 17:05:24 roi Exp $
 */

/* 
 *  TOPPERS初級教育用のサンプルプログラム
 *  entry_taskではスイッチ４，５のセンシング、
 *  timer_taskでは時間とLEDの管理を行い、簡単なタイマーを実現する．
 *  通常はLED3が１秒毎に点滅する．
 *  スイッチ５オンでタイマー起動、10秒ごとにLED2の点滅、１分後にLED2が
 *  アラート点滅する。このときスイッチ４をオンするごとにアラート点滅時間が
 *  １分加算され、アラート点滅時間を延長する．
 *  ３分後にアラート点滅させるには、スイッチ５をオン、その後、スイッチ４を
 *  ２回オンにする．
 *  また、ドライバーはPDICに順ずるため、sfrファイルは使用しない．
 *
 */

#include <t_services.h>
#include "kernel_id.h"
#include "sim_device.h"
#include "timer3.h"

/*
 *  タイマプロセスの状態
 */
enum TIMER_STATE {
	STATE_STOP_TIMER,			/* タイマー停止状態 */
	STATE_ACT_TIMER,			/* タイマー実行状態 */
	STATE_TIMEOUT				/* タイムアウト状態 */
};

/*
 * 変数の宣言
 */
static unsigned char event;

/*
 *  タイマ用タスク
 */
void
timer_task(VP_INT exinf)
{
	SYSTIM  base_time, current_time;
	SYSTIM  start_time, last_time;
	SYSTIM  alaem_time = 0;
	TMO     tmout = 0;
	INT     timer_state = STATE_STOP_TIMER;
	UB      led2 = OFF;
	UB      led3 = OFF;
	UB      sled3;

	syslog(LOG_INFO, "Sample1 timer task starts (exinf = %d).", exinf);
	initial_led();			/* LEDの初期化 */
	get_tim(&base_time);	/* 現在時間の取り出し */
	for(;;){
		tslp_tsk(tmout);
		if(event != 0){		/* entry_taskより通知 */
			if(event & EVT_TIMER_START){
				timer_state = STATE_ACT_TIMER;
				start_time = base_time;
				last_time = start_time + 60*T_1SEC;
				syslog(LOG_INFO, "Timer Start !");
			}
			if(event & EVT_TIMER_COUNT){
				if(timer_state != STATE_STOP_TIMER){
					last_time += 60*T_1SEC;
				}
				syslog(LOG_INFO, "Timer Count Up !");
			}
			if(event & EVT_TIMER_STOP){
				timer_state = STATE_STOP_TIMER;
				alaem_time = 0;
				syslog(LOG_INFO, "Timer Stop !");
			}
			event = 0;
		}
		switch(timer_state){	/* タイマー状態の判定 */
		case STATE_ACT_TIMER:	/* タイマー実行中状態 */
			if(last_time <= current_time){
				alaem_time = base_time + 15*T_1SEC;
				timer_state = STATE_TIMEOUT;
			}
			else if(((base_time - start_time) % (10*T_1SEC)) == 0){
				alaem_time = base_time + T_1SEC;
			}
			break;
		case STATE_TIMEOUT:		/* タイムアウト状態 */
			timer_state = STATE_STOP_TIMER;
			syslog(LOG_INFO, "to STOP_TIMER state !!");
			break;
		default:				/* タイマー停止中状態 */
			break;
		}
								/* 奇数秒でLED3オン */
		sled3 = (UB)((base_time / T_1SEC) & ON);
		if(led3 != sled3){
			led3 = sled3;
			if(timer_state == STATE_STOP_TIMER){
				syslog_1(LOG_INFO, "system time=%d", base_time);
				display_time(0, base_time);
			}
			else{
				syslog_2(LOG_INFO, "max time=%d current time=%d", last_time-start_time, base_time-start_time);
				display_time(last_time-start_time, base_time-start_time);
			}
		}
								/* 警告の要求:偶数TICKでLED2オン */
		if(alaem_time > base_time){
			led2 = (UB)(((alaem_time - base_time) / T_TICK) & ON);
			led2 ^= ON;			/* LED2 のオン 、 オフを切り替える */
		}
		else{					/* 警告の要求がなくLED2がオンなら消灯 */
			led2 = OFF;
		}

		set_led(LED2, led2);	/* LED2の設定 */
		set_led(LED3, led3);	/* LED3の設定 */
		get_tim(&current_time);
		if(current_time >= base_time){
			base_time += T_TICK;
		}
		tmout = base_time - current_time;
		if(tmout < 0){
			tmout = 0;
		}
	}
}

/*
 *  メインタスク
 *  (スイッチプロセス)
 */
void
entry_task(VP_INT exinf)
{
	UB    sw4, sw5, sw;

	syslog(LOG_INFO, "Sample entry task starts (exinf = %d).", exinf);
	initial_key();			/* キーの初期化 */
	event = 0;				/* 共有メモリの初期化 */
	sw4 = get_key(SW4);		/* 現在のSW4の取り込み */
	sw5 = get_key(SW5);		/* 現在のSW5の取り込み */
	act_tsk(TIMER_TASK);	/* タイマータスクの起動 */

	for(;;){
		tslp_tsk(100);
		sw = get_key(SW5);
		if(sw5 != sw){
			syslog(LOG_INFO, "Change switch5 = 0x%x.", sw);
			if(sw == ON){
				event |= EVT_TIMER_START;
			}
			else{
				event |= EVT_TIMER_STOP;
			}
			sw5 = sw;
		}
		sw = get_key(SW4);
		if(sw4 != sw){
			syslog(LOG_INFO, "Change switch4 = 0x%x.", sw);
			if(sw == ON){
				event |= EVT_TIMER_COUNT;
			}
			sw4 = sw;
		}
	}
}

