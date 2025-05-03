/*
 *  TOPPERS/JSP Educative Program
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2003-2004 by Ryosuke Takeuchi
 *               Platform Development Center RICOH COMPANY,LTD. JAPAN
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
 *  @(#) $Id: oaks16_device.c,v 1.1 2004/03/14 23:20:20 roi Exp $
 */

#include <t_services.h>
#ifdef S_DEBUG
#ifdef DEVICE_ENABLE
#include <device.h>
#else
#include <simsil.h>
#endif
#else
#include <monsil.h>
#endif
#ifdef OAKS16
#include "oaks16.h"
#else
#include "oaks16mini.h"
#endif
#include "sim_device.h"

#ifdef OAKS16
/*
 *  ポート７設定
 */
#define	P7_LED1					0x10			/* LED1ビット定義 */
#define	P7_LED2					0x20			/* LED2ビット定義 */

/*
 *  ポート８設定
 */
#define	P8_SW1					0x01			/* SW1ビット定義 */
#define	P8_SW2					0x02			/* SW2ビット定義 */

#endif

#define TINTLVL0        5
#define TIMER_CLOCK		625

volatile static UB cled;

/*
 *  スイッチの初期化関数
 */
void
initial_key()
{
	sil_wrb_mem((VP)TADR_SFR_PD8, 0x00);	/* ポート8入力設定		*/
	sil_wrb_mem((VP)TADR_SFR_PUR2, 0x01);	/* P80からP83プルアップ	*/
}

/*
 *  LEDの初期化関数
 */
void
initial_led()
{
	cled = 0xff;
	sil_wrb_mem((VP)TADR_SFR_P7, cled);		/* ポート7データ初期化	*/
	sil_wrb_mem((VP)TADR_SFR_PD7, 0xff);	/* ポート7出力設定		*/
}

/*
 *  スイッチの状態を取り出す
 *  arg1:SW1またはSW2
 */
int
get_key(int sw)
{
	UB key;
	int         result = OFF;

	key = sil_reb_mem((VP)TADR_SFR_P8);
	switch(sw){
	case SW4:
		if((key & P8_SW2) == 0)
			result = ON;
		break;
	case SW5:
		if((key & P8_SW1) == 0)
			result = ON;
		break;
	default:
		break;
	}
	return result;
}

/*
 *  LEDの設定を行う
 *  arg1:LED1またはLED2
 *  arg2:ONまたはOFF
 */
void
set_led(int led, int req)
{
	UB rled = cled;

	switch(led){
	case LED2:
		rled &= ~P7_LED2;
		if(req == OFF)
			rled |= P7_LED2;
		break;
	case LED3:
		rled &= ~P7_LED1;
		if(req == OFF)
			rled |= P7_LED1;
		break;
	default:
		break;
	}
	if(cled != rled){
		cled = rled;
		sil_wrb_mem((VP)TADR_SFR_P7, cled);
	}
}

/*
 * タイマーの状態を表示する
 * arg1:タイマー最大時間
 * arg2:現在時間
 */
void
display_time(UW maxtime, UW currenttime)
{
	/* 時間表示デバイスなし */
}

