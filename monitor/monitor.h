/*
 *  TOPPERS/JSP Educative Program
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
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
 *  @(#) $Id: monitor.h,v 1.2 2004/01/04 11:25:24 roi Exp $
 */

#ifndef _MONITOR_H_
#define	_MONITOR_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
 *  モニタ・サンプルプログラムのヘッダファイル
 */
#include <t_services.h>

/*
 *  タスク優先度の設定
 */
#define MONITOR_PRIORITY	3

/*
 *  モニタのサイズの設定
 */
#define	MONITOR_STACK_SIZE	512		/* タスクのスタックサイズ */
#define	MAX_COMMAND_LENGTH	80		/* モニタの最大コマンド長 */
#define	NUM_LOG_DISP		3		/* 最大表示アイテム数 */

/*
 *  バージョン情報
 */
#define	TMONITOR_PRVER	0x1000		/* カーネルのバージョン番号 */

/*
 *  キー割り当ての定義
 */
#define	KEY_BS			(8)			/* バックスペース */
#define	KEY_DEL			(127)		/* 削除 */
#define	KEY_NL			(13)		/* 改行 */
#define	KEY_EXT			(1)			/* 終了 */

/*
 *  データタイプ定義
 */
#define	DATA_BYTE		1			/* バイトデータ（１バイト）*/
#define	DATA_HALF		2			/* ハーフデータ（２バイト）*/
#define	DATA_WORD		4			/* ワードデータ（４バイト）*/

/*
 *  領域属性の定義
 */
#define	NONE_AREA		0			/* 領域の割り当てのない領域 */
#define	PORT_AREA		1			/* ハードウェアのポート領域 */
#define	MEMORY_AREA		2			/* メモリ領域　*/

#define	MREAD_ONLY		1			/* 読み込み専用 */
#define	MWRITE_ONLY		2			/* 書き込み専用 */
#define	MREAD_WRITE		(MREAD_ONLY+MWRITE_ONLY)

#ifndef _MACRO_ONLY

/*
 *  関数のプロトタイプ宣言
 */

extern BOOL need_monitor(void);
extern void	monitor(VP_INT exinf);
extern INT  MemoryRead(UW address, VP_INT p, INT type);
extern INT  MemoryWrite(UW address, VP_INT p, INT type);
extern UW   MonAlignAddress(UW address);
extern INT  MemoryRead(UW address, VP_INT p, INT type);
extern INT  MemoryWrite(UW address, VP_INT p, INT type);
extern UW   get_exception_pc(VP p_excinf);
extern void display_registers(ID tskid);
extern UW   display_assembler(UW pc);

extern void _setup_stdio(ID *pport);

#endif /* _MACRO_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* _MONITOR_H_ */

