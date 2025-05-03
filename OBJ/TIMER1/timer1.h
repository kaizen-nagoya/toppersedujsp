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
 *  @(#) $Id: timer1.h,v 1.1 2004/03/01 11:15:24 roi Exp $
 */

/* 
 *  モニターテスト用サンプルプログラムのヘッダファイル
 */
#include <t_services.h>

#define TEX_LOOP	500000	/* 例外処理ルーチン内でのループ回数 */


/*
 *  タスク優先度の設定
 */
#define DEFAULT_MAIN_PRIORITY  8
#define TIMER_PRIORITY         9

/*
 *  ターゲットに依存する可能性のある定数の定義
 *  スタックサイズ
 */

#ifdef STACK_SIZE
#undef STACK_SIZE
#endif /* STACK_SIZE */
#define	STACK_SIZE	386		/* タスクのスタックサイズ */

/*
 *  タイマープロセスの時間設定
 */
#define	T_TICK		250		/* 実行周期 250ms */
#define	T_1SEC		1000	/* タイマー上の１秒 */

/*
 * 例外ハンドラの設定
 */
#define	INT_IRG		64		/* イレギュラー割込みハンドラ番号 */
#define EXC_IRG		64		/* イレギュラー例外ハンドラ番号 */

/*
 *  関数のプロトタイプ宣言
 */
#ifndef _MACRO_ONLY

extern void	entry_task(VP_INT exinf);
extern void timer_task(VP_INT exinf);

#endif /* _MACRO_ONLY */
