/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2005 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: sample1.c,v 1.22 2005/11/12 15:01:44 hiro Exp $
 */

/* 
 *  サンプルプログラム(1)の本体
 *
 *  JSPカーネルの基本的な動作を確認するためのサンプルプログラム．
 *
 *  プログラムの概要:
 *
 *  ユーザインタフェースを受け持つメインタスク（タスクID: MAIN_TASK，
 *  優先度: MAIN_PRIORITY）と，三つの並列実行されるタスク（タスクID:
 *  TASK1〜TASK3，初期優先度: MID_PRIORITY）で構成される．また，起動周
 *  期が2秒の周期ハンドラ（周期ハンドラID: CYCHDR1）を用いる．
 *
 *  並列実行されるタスクは，task_loop 回空ループを実行する度に，タスク
 *  が実行中であることをあらわすメッセージを表示する．
 *
 *  周期ハンドラは，三つの優先度（HIGH_PRIORITY，MID_PRIORITY，
 *  LOW_PRIORITY）のレディキューを回転させる．プログラムの起動直後は，
 *  周期ハンドラは停止状態になっている．
 *
 *  メインタスクは，シリアルI/Oポートからの文字入力を行い（文字入力を
 *  待っている間は，並列実行されるタスクが実行されている），入力された
 *  文字に対応した処理を実行する．入力された文字と処理の関係は次の通り．
 *  Control-C または 'Q' が入力されると，プログラムを終了する．
 *
 *  '1' : 以降のコマンドは TASK1 に対して行う．
 *  '2' : 以降のコマンドは TASK2 に対して行う．
 *  '3' : 以降のコマンドは TASK3 に対して行う．
 *  'a' : タスクを act_tsk により起動する．
 *  'A' : タスクに対する起動要求を can_act によりキャンセルする．
 *  'e' : タスクに ext_tsk を呼び出させ，終了させる．
 *  't' : タスクを ter_tsk により強制終了する．
 *  '>' : タスクの優先度を HIGH_PRIORITY にする．
 *  '=' : タスクの優先度を MID_PRIORITY にする．
 *  '<' : タスクの優先度を LOW_PRIORITY にする．
 *  'G' : タスクの優先度を get_pri で読み出す．
 *  's' : タスクに slp_tsk を呼び出させ，起床待ちにさせる．
 *  'S' : タスクに tslp_tsk(10秒) を呼び出させ，起床待ちにさせる．
 *  'w' : タスクを wup_tsk により起床する．
 *  'W' : タスクに対する起床要求を can_wup によりキャンセルする．
 *  'l' : タスクを rel_wai により強制的に待ち解除にする．
 *  'u' : タスクを sus_tsk により強制待ち状態にする．
 *  'm' : タスクの強制待ち状態を rsm_tsk により解除する．
 *  'M' : タスクの強制待ち状態を frsm_tsk により強制解除する．
 *  'd' : タスクに dly_tsk(10秒) を呼び出させ，時間経過待ちにさせる．
 *  'x' : タスクにパターン 0x0001 の例外処理を要求する．
 *  'X' : タスクにパターン 0x0002 の例外処理を要求する．
 *  'y' : タスクに dis_tex を呼び出させ，タスク例外を禁止する．
 *  'Y' : タスクに ena_tex を呼び出させ，タスク例外を許可する．
 *  'r' : 三つの優先度（HIGH_PRIORITY，MID_PRIORITY，LOW_PRIORITY）の
 *        レディキューを回転させる．
 *  'c' : 周期ハンドラを動作させる．
 *  'C' : 周期ハンドラを停止させる．
 *  'z' : CPU例外を発生させる．
 *  'Z' : CPUロック状態でCPU例外を発生させる（プログラムを終了する）．
 *  'V' : vxget_tim で性能評価用システム時刻を2回読む．
 *  'v' : 発行したシステムコールを表示する（デフォルト）．
 *  'q' : 発行したシステムコールを表示しない．
 */

#include <s1c33.h>
#include <t_services.h>
#include "kernel_id.h"
#include "sample1.h"
#include "keyboard.h"

#include <tff.h>
#include <wikilib.h>
#include <guilib.h>
#include <malloc.h>

/*
 *  並行実行されるタスクへのメッセージ領域
 */
char	message[3];

/*
 *  ループ回数
 */
UW	task_loop;		/* タスク内でのループ回数 */
UW	tex_loop;		/* 例外処理ルーチン内でのループ回数 */

static FATFS s_activeFatFs;

/*
 *  並行実行されるタスク
 */
void task(VP_INT exinf)
{
}

/*
 *  並行して実行されるタスク用のタスク例外処理ルーチン
 */
void tex_routine(TEXPTN texptn, VP_INT exinf)
{
	volatile UW	i;
	INT	tskno = (INT) exinf;

	syslog(LOG_NOTICE, "task%d receives exception 0x%04x. ",
					tskno, texptn);
	for (i = 0; i < tex_loop; i++);

	if (texptn & 0x8000) {
		syslog(LOG_INFO, "#%d#ext_tsk()", tskno);
		ext_tsk();
	}
}

/*
 *  CPU例外ハンドラ
 */
#ifdef CPUEXC1

void
cpuexc_handler(VP p_excinf)
{
	ID	tskid;

	syslog(LOG_NOTICE, "CPU exception handler (p_excinf = %08p).",
							p_excinf);
	if (sns_ctx() != TRUE) {
		syslog(LOG_WARNING,
			"sns_ctx() is not TRUE in CPU exception handler.");
	}
	if (sns_dpn() != TRUE) {
		syslog(LOG_WARNING,
			"sns_dpn() is not TRUE in CPU exception handler.");
	}
	syslog(LOG_DEBUG,
		"sns_loc = %d sns_dsp = %d", sns_loc(), sns_dsp());
	syslog(LOG_DEBUG,
		"vxsns_loc = %d vxsns_ctx = %d vxsns_dsp = %d vxsns_dpn = %d",
		vxsns_loc(p_excinf), vxsns_ctx(p_excinf),
		vxsns_dsp(p_excinf), vxsns_dpn(p_excinf));

	if (!vxsns_loc(p_excinf) && !vxsns_ctx(p_excinf)) {
		syscall(iget_tid(&tskid));
		syscall(iras_tex(tskid, 0x8000));
	}
	else {
		syslog(LOG_NOTICE, "Sample program ends with exception.");
		kernel_exit();
	}
}

#endif /* CPUEXC1 */

/*
 *  周期ハンドラ
 *
 *  HIGH_PRIORITY，MID_PRIORITY，LOW_PRIORITY の各優先度のレディキュー
 *  を回転させる．
 */
void cyclic_handler(VP_INT exinf)
{
	irot_rdq(HIGH_PRIORITY);
	irot_rdq(MID_PRIORITY);
	irot_rdq(LOW_PRIORITY);
}

/*
 *  メインタスク
 */
void main_task(VP_INT exinf)
{
	volatile UW	i;
	SYSTIM	stime1, stime2;
#ifndef OMIT_VGET_TIM
	SYSUTIM	utime1, utime2;
#endif /* OMIT_VGET_TIM */

	vmsk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG));
	syslog(LOG_NOTICE, "Wikireader task starts (exinf = %d).", (INT) exinf);

	syscall(serial_ctl_por(TASK_PORTID,
			(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

	/*
	 * init local things
	 */
	f_mount(0, &s_activeFatFs); 
	keyboard_init();

	/*
 	 *  jp: ループ回数の設定
 	 *  en: Set loop frequency
	 */
	task_loop = LOOP_REF;
	get_tim(&stime1);
	for (i = 0; i < task_loop; i++);
	get_tim(&stime2);
	task_loop = LOOP_REF * 400 / (stime2 - stime1);
	tex_loop = task_loop / 5;

	ena_tex();
	malloc_init();
	wikilib_init();
	guilib_init();
	wikilib_run();

	syslog(LOG_NOTICE, "Wikireader task ends.");
	kernel_exit();
}
