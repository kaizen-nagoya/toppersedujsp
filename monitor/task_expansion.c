/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2003 by Ryosuke Takeuchi
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
 *  @(#) $Id: task_expansion.c,v 1.2 2003/01/04 11:55:25 roi Exp $
 */

/*
 *	タスク管理拡張機能
 */

#include "task_expansion.h"
#include "kernel_id.h"

#define	TS_WAIT_MASK	(TS_WAIT_SLEEP | TS_WAIT_WOBJ | TS_WAIT_WOBJCB)

/*
 *  コンテクスト中からの強制待ち状態への移行
 */
SYSCALL ER
isus_tsk(ID tskid)
{
	TCB	*tcb;
	UINT	tstat;
	ER	ercd;

	LOG_IACT_TSK_ENTER(tskid);
	CHECK_INTCTX_UNL();
	CHECK_TSKID(tskid);
	tcb = get_tcb(tskid);

	i_lock_cpu();
	if (TSTAT_DORMANT(tstat = tcb->tstat)) {
		ercd = E_OBJ;
	}
	else if (TSTAT_RUNNABLE(tstat)) {
		/*
		 *  実行できる状態から強制待ち状態への遷移
		 */
		tcb->tstat = TS_SUSPENDED;
		if (make_non_runnable(tcb)) {
			reqflg = TRUE;
		}
		ercd = E_OK;
	}
	else if (TSTAT_SUSPENDED(tstat)) {
		ercd = E_QOVR;
	}
	else {
		/*
		 *  待ち状態から二重待ち状態への遷移
		 */
		tcb->tstat |= TS_SUSPENDED;
		ercd = E_OK;
	}
	i_unlock_cpu();

    exit:
	LOG_IACT_TSK_LEAVE(ercd);
	return(ercd);
}

/*
 *  タスク状態参照（簡易版）
 */
SYSCALL ER
ref_tst(ID tskid, T_RTST *pk_rtst)
{
	TCB	*tcb;
	UB	tstat;
	ER	ercd;

	LOG_ACT_TSK_ENTER(tskid);
	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	tcb = get_tcb_self(tskid);

	t_lock_cpu();
	pk_rtst->tskwait  = 0;
	pk_rtst->tskpri   = EXT_TSKPRI(tcb->priority);
	pk_rtst->inistk   = (VP)tcb->tinib->stk;
	pk_rtst->inistksz = tcb->tinib->stksz;

	tstat = tcb->tstat;
	if (TSTAT_RUNNABLE(tstat)) {
		if (tcb == runtsk) {
			pk_rtst->tskstat = TTS_RUN;
		}
		else {
			pk_rtst->tskstat = TTS_RDY;
		}
	}
	else if (TSTAT_WAITING(tstat)) {
		if (TSTAT_SUSPENDED(tstat)) {
			pk_rtst->tskstat = TTS_WAS;
		}
		else {
			pk_rtst->tskstat = TTS_WAI;
		}
		if (!(tstat & TS_WAIT_MASK)) {
			pk_rtst->tskwait = TTW_DLY;
		}
		else if ((tstat & TS_WAIT_MASK) == TS_WAIT_SLEEP) {
			pk_rtst->tskwait = TTW_SLP;
		}
		else if ((tstat & TS_WAIT_MASK) == TS_WAIT_WOBJ) {
			pk_rtst->tskwait = TTW_RDTQ;
		}
		else {
			pk_rtst->tskwait = TTW_OTHR;
		}
	}
	else if (TSTAT_SUSPENDED(tstat)) {
		pk_rtst->tskstat = TTS_SUS;
	}
	else {
		pk_rtst->tskstat = TTS_DMT;
	}
	if (pk_rtst->tskstat == TTS_DMT)
		pk_rtst->tskpc = (FP)tcb->tinib->task;
	else
		pk_rtst->tskpc = (FP)tcb->tskctxb.pc;
	pk_rtst->tsksp   = (VP)tcb->tskctxb.sp;
	ercd = E_OK;
	t_unlock_cpu();

    exit:
	LOG_ACT_TSK_LEAVE(ercd);
	return(ercd);
}

/*
 *  タスク状態のロギング格納領域
 */

static T_TLOG tsk_log[MAX_TASK_LOG+1];
static ID     check_tskid;
static SYSTIM check_time;
static SYSTIM pervious_time;

/*
 *  システム時間取り出し関数
 *  割込み中でも使用でき、共通化できるように
 *  インライン関数とする．
 */
Inline SYSTIM get_systime(void)
{
	return systim_offset + next_time;
}

/*
 *  タスク実行領域設定関数
 *  この関数はdispatchのタスク確定時、または、アイドル状態
 *  確定時、CPUロック状態で呼び出す．
 */
#include "hw_timer.h"

void iana_tsk(void)
{
	T_TLOG  *t;
	ID      tskid;
	SYSTIM  time;
#if TIC_DENO != 1
	INT     subtime;
#endif /* TIC_DENO != 1 */
	CLOCK   clock;
	BOOL    ireq;

	/*
	 * 時間の取り出しはコンテキスト中でも行われるので
	 * vxget_timと同等の記述をここにおく。
	 */
#if TIC_DENO != 1
	subtime = (INT) next_subtime;
#endif /* TIC_DENO != 1 */
	clock = hw_timer_get_current();
	ireq = hw_timer_fetch_interrupt();

	time = get_systime() * ANA_STIC;
#if TIC_DENO != 1
	time += subtime * ANA_STIC / TIC_DENO;
#endif /* TIC_DENO != 1 */
	if (!(ireq) || clock >= (TO_CLOCK(TIC_NUME, TIC_DENO)
					- GET_TOLERANCE)) {
		time -= TIC_NUME * ANA_STIC / TIC_DENO;
	}
	time += clock * ANA_STIC / TIMER_CLOCK;

	if(check_tskid < 0)
		t = &tsk_log[0];
	else if(check_tskid < MAX_TASK_LOG)
		t = &tsk_log[check_tskid];
	else
		t = &tsk_log[MAX_TASK_LOG];
	t->runcount++;
	t->runtimes += time - check_time;

	if(runtsk)
		check_tskid = TSKID(runtsk);
	else
		check_tskid = 0;
	check_time = time;
}

/*
 *  タスクログ状態取り出し関数
 */
INT get_tsklog(T_TPRM * pprm)
{
	INT no, num_item;

	get_tim(&pprm->currtime);
	t_lock_cpu();
	pprm->pervtime = pervious_time;
	pervious_time  = pprm->currtime;
	for(no = 0 ; no <= tmax_tskid && no <= MAX_TASK_LOG ; no++){
		if(no < NUM_LDSP){
			pprm->tlog[no] = tsk_log[no];
			num_item = no;
		}
		else{
			pprm->tlog[NUM_LDSP-1].runcount += tsk_log[no].runcount;
			pprm->tlog[NUM_LDSP-1].runtimes += tsk_log[no].runtimes;
			num_item = NUM_LDSP-1;
		}
		tsk_log[no].runcount = 0;
		tsk_log[no].runtimes = 0;
	}
	t_unlock_cpu();
	return num_item;
}


/*
 *  デバイス・ポートのログアウト要求判定用データ領域
 */

static T_PCHK port_log[NUM_PCHK];

/*
 *  デバイスログを行うかどうかの判定
 */
static UINT check_device_log(UW address, UW size)
{
	int    i;
	T_PCHK *p;

	for(i = 0 ; i < NUM_PCHK ; i++){
		p = &port_log[i];
		if(p->portaddress >= address && p->portaddress < (address+size) && p->logtype)
			return (UINT)p->logtype;
	}
	return 0;
}

/*
 *  デバイス・ポートのログ判定データの取り出し
 */
T_PCHK *get_device_log(UW no){
	if(no < NUM_PCHK)
		return &port_log[no];
	else
		return 0;
}

/*
 *  デバイス読み出し表示判定とログアウト関数
 */
void
ana_rdv(UW address, UW data, INT size)
{
	SYSTIM time = get_systime();
	ID     id;
	UINT   logtype = check_device_log(address, size);

	if(logtype == 0)
		return;
	if(sense_context() || !runtsk)
		syslog_3(logtype, "Device Read  time=%09d interrupt port=%08x data=0x%x", time, address, data);
	else{
		id = TSKID(runtsk);
		switch(size){
		case 4:
			syslog_4(logtype, "Device Read  time=%09d task=%04d port=%08x data=%08x", time, id, address, data);
			break;
		case 2:
			syslog_4(logtype, "Device Read  time=%09d task=%04d port=%08x data=%04x", time, id, address, data);
			break;
		default:
			syslog_4(logtype, "Device Read  time=%09d task=%04d port=%08x data=%02x", time, id, address, data);
			break;
		}
	}
}

/*
 *  デバイス書き込み表示判定とログアウト関数
 */
void
ana_wdv(UW address, UW data, INT size)
{
	SYSTIM time = get_systime();
	ID     id;
	UINT   logtype = check_device_log(address, size);

	if(logtype == 0)
		return;
	if(sense_context() || !runtsk)
		syslog_3(logtype, "Device Write  time=%09d interrupt port=%08x data=0x%x", time, address, data);
	else{
		id = TSKID(runtsk);
		switch(size){
		case 4:
			syslog_4(logtype, "Device Write time=%09d task=%04d port=%08x data=%08x", time, id, address, data);
			break;
		case 2:
			syslog_4(logtype, "Device Write time=%09d task=%04d port=%08x data=%04x", time, id, address, data);
			break;
		default:
			syslog_4(logtype, "Device Write time=%09d task=%04d port=%08x data=%02x", time, id, address, data);
			break;
		}
	}
}

