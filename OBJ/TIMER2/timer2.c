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
 *  @(#) $Id: timer2.c,v 1.2 2004/03/14 23:07:24 roi Exp $
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
#include "timer2.h"

/*
 *  タイマ用タスク
 */
void
timer_task(VP_INT exinf)
{
	SYSTIM  base_time, current_time;
	TMO     tmout = 0;
	UB      led3 = OFF;

	syslog_1(LOG_INFO, "Sample1 timer task starts (exinf = %d).", exinf);
	get_tim(&base_time);		/* 現在時間の取り出し */
	for(;;){
		tslp_tsk(tmout);		/* TICK待ち */
								/* 奇数秒でLED3オン */
		led3 = (base_time / T_1SEC) & ON;
		set_led(LED3, led3);	/* LED3の設定 */
		base_time += T_TICK;
		get_tim(&current_time);
		tmout = base_time - current_time;
		if(tmout < 0)
			tmout = 0;
	}
}

/*
 *  メインタスク
 *  (スイッチプロセス)
 */
void
entry_task(VP_INT exinf)
{
	UB    sw5, sw;
	UB    led2 = OFF;

	syslog_1(LOG_INFO, "Sample entry task starts (exinf = %d).", exinf);
	initial_key();			/* キーの初期化 */
	initial_led();			/* LEDの初期化 */
	sw5 = get_key(SW5);		/* 現在のSW5の取り込み */
	act_tsk(TIMER_TASK);	/* タイマータスクの起動 */

	for(;;){
		tslp_tsk(100);
		sw = get_key(SW5);
		if(sw5 != sw){
			syslog_1(LOG_INFO, "Change switch5 = 0x%x.", sw);
			if(sw == ON)
				led2 = ON;
			else
				led2 = OFF;
			sw5 = sw;
		}
		set_led(LED2, led2);	/* LED2の設定 */
	}
}

