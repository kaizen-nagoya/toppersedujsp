/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: task_expansion.h,v 1.2 2004/01/04 11:38:45 roi Exp $
 */

/*
 *	モニタ用タスク用拡張定義
 */

#ifndef _TASK_EXPANSION_H_
#define _TASK_EXPANSION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"
#include "wait.h"
#include "time_event.h"
#include "kernel_debug.h"
#include <t_services.h>
/*
 *  タスク状態（簡易版）のパケット形式
 */

typedef struct t_rtst{
	STAT tskstat;				/* タスク状態 */
	STAT tskwait;				/* 待ち要因 */
	PRI  tskpri;				/* タスクの現在の優先度 */
	FP   tskpc;					/* タスクの現在のプログラムカウンタ */
	VP   tsksp;					/* タスクの現在のスタックポインタ */
	VP   inistk;				/* 初期設定のスタックアドレス */
	SIZE inistksz;				/* 初期設定のスタックサイズ */
}T_RTST;

#define	TTW_OTHR	0x0040		/* その他の待ち状態 */
					/* TOPPERS待ち状態TTW_SEM,TTW_FLG, TTW_SDTQ */
					/* TTW_MBX, TTW_MPFの待ち状態区別ではない */

/*
 *  タスクの実行状態構造体定義
 */
#define	MAX_TASK_LOG	8		/* タスクログテーブルの数 */
#define	ANA_STIC		10		/* サブチック値 */

typedef struct t_tlog{
	INT		runcount;			/* タスクの実行回数 */
	SYSTIM	runtimes;			/* タスクの実行時間(MS) */
}T_TLOG;

/*
 *  タスクのログ情報取り出し構造体定義
 */

#define	NUM_LDSP		6		/* 表示用のログ情報取り出し領域の最大数 */

typedef struct t_tprm{
	SYSTIM  pervtime;			/* 過去の参照時間 */
	SYSTIM  currtime;			/* 今回の参照時間 */
	T_TLOG	tlog[NUM_LDSP];		/* 取り出したログ情報 */
}T_TPRM;

/*
 *  デバイスポート判定テーブルの構造体定義
 */

#define	NUM_PCHK		4		/* デバイスポート判定テーブルのアイテム数 */

typedef struct t_pchk{
	UB		logtype;			/* ログ表示判定情報 */
	UW		portaddress;		/* ポートアドレス */
}T_PCHK;

/*
 *  プロトタイプ宣言
 */

extern SYSCALL ER isus_tsk(ID tskid);
extern SYSCALL ER ref_tst(ID tskid, T_RTST *pk_rtst);
extern void iana_tsk(void);
extern void ana_rdv(UW address, UW data, INT size);
extern void ana_wdv(UW address, UW data, INT size);
extern INT  get_tsklog(T_TPRM *pprm);
extern T_PCHK *get_device_log(UW no);

#ifdef __cplusplus
}
#endif

#endif /* _TASK_EXPANSION_H_ */

