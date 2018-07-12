/******************************************************************************
* File Name : GSframetimer.c                     Ver : 1.00  Date : 2003-04-17
*
* Description :
*
*		フレームタイマ．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSframetimer.h"
#include	<GL/glut.h>

/****** 定数宣言 *************************************************************/

#define	FPS				60					/* １秒間のフレーム数		*/
#define	FRAME_SKIP_MAX	8					/* 最大フレームスキップ数	*/
#define	FPS_ERROR		0.1f				/* タイマの誤差				*/

/****** グローバル変数宣言 ***************************************************/

/* １秒間あたりのフレーム数 */
static GSfloat	s_FPS = FPS;

/* １フレームの開始時間 */
static GSfloat	s_StartTime = 0;

/* 前フレームの開始時間 */
static GSfloat	s_PrevTime = 0;

/* １フレームの経過時間 */
static GSfloat	s_FrameTime = 0;

/* 処理オーバー時間 */
static GSfloat	s_OverTime = 0;

/* 最低ウェイトタイマ */
static GSfloat	s_WaitTime = 0.25f;

/* フレームスキップカウンタ */
static GSuint	s_FrameSkipCount = 0;

/* 最大フレームスキップ数 */
static GSuint	s_FrameSkipCountMax = FRAME_SKIP_MAX;


/*=============================================================================
*
* Purpose : フレームタイマのリセット．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsFrameTimerReset
(
	void
)
{
	s_StartTime      = 0.0f;
	s_FrameTime      = 0.0f;	
	s_OverTime       = 0.0f;
	s_FrameSkipCount = 0;
	s_PrevTime       = gsTimeGetTime();
}

/*=============================================================================
*
* Purpose : フレームタイマを更新する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsFrameTimerUpdate
(
	void
)
{
	/* １フレームの開始タイマを計測 */
	s_StartTime = gsTimeGetTime();

	/* １フレームあたりの時間を求める */
	s_FrameTime = ( s_StartTime - s_PrevTime ) * s_FPS;

	/* 前回のタイマ値を設定する */
	s_PrevTime = s_StartTime;

	/* ＦＰＳタイマ以上に時間が経過してしまっているか？ */
	s_OverTime = 0.0f;
	if ( s_FrameTime > 1.0f )
	{
		/* オーバーした時間を計算する */
		s_OverTime = s_FrameTime - 1.0f;

		/* タイマを調整する */
		s_FrameTime = 1.0f;

		/* タイマ値の誤差を考慮する */
		if ( s_OverTime <= FPS_ERROR )
		{
			s_OverTime = 0.0f;
		}
	}
}

/*=============================================================================
*
* Purpose : 処理落ちによる描画のスキップが必要か調べる．
*
* Return  : スキップするのであれば真，スキップが必要なければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsFrameTimerIsSkip
(
	void
)
{
	/* 最大スキップ数以上はスキップさせないようにする */
	if( s_FrameSkipCount >= s_FrameSkipCountMax )
	{
		/* スキップ数を初期化する */
		s_FrameSkipCount = 0;

		/* オーバーした時間を初期化する */
		s_OverTime = 0.0f;

		return	GS_FALSE;
	}

	/* 時間オーバーしているか？*/
	if ( s_OverTime > 0 )
	{
		/* ＦＰＳタイマよりもオーバーしているか？ */
		if ( s_OverTime > 1.0f )
		{
			/* オーバーした時間を調整する */
			s_OverTime -= 1.0f;

			/* タイマを調整する */
			s_FrameTime = 1.0f;
		}
		else
		{
			/* タイマを調整する */
			s_FrameTime = s_OverTime;

			/* オーバーした時間分は処理をした事にする */
			s_OverTime = 0.0f;
		}

		/* スキップカウンタを増加させる */
		s_FrameSkipCount++;

		/* 描画をスキップする必要がある */
		return	GS_TRUE;
	}

	/* スキップカウンタを初期化する */
	s_FrameSkipCount = 0;

	/* オーバーした時間を初期化する */
	s_OverTime = 0.0f;

	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : タイマウェイトをする．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsFrameTimerWait
(
	void
)
{
	GSfloat		FrameTime;
	GSfloat		StartTime;
	GSfloat		WaitTime;

	/* １フレームあたりの時間を求める */
	FrameTime = ( gsTimeGetTime() - s_StartTime ) * s_FPS;

	/* 最低ウェイト時間よりも時間が短い？ */
	if ( FrameTime < s_WaitTime )
	{
		/* ウェイトする時間を求める */
		WaitTime = s_WaitTime - FrameTime;

		/* 最低ウェイトタイマ分ウェイトする */
		StartTime = gsTimeGetTime() * s_FPS;
		while ( ( ( ( gsTimeGetTime() * s_FPS ) - StartTime )  ) < WaitTime )
			;
	}
}

/*=============================================================================
*
* Purpose : フレームタイマの取得．
*
* Return  : なし．
*
*============================================================================*/
extern GSfloat
gsFrameTimerGetTime
(
	void
)
{
	return	s_FrameTime;
}

/*=============================================================================
*
* Purpose : 最低ウェイト時間の設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsFrameTimerSetWaitTime
(
	GSfloat		WaitTime			/* 最低ウェイト時間 */
)
{
	s_WaitTime = WaitTime;
}

/*=============================================================================
*
* Purpose : ＦＰＳの設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsFrameTimerSetFPS
(
	GSfloat		fps					/* ＦＰＳの設定 */
)
{
	s_FPS = fps;
}

/*=============================================================================
*
* Purpose : 秒タイマの取得．
*
* Return  : 秒タイマを返す．
*
*============================================================================*/
extern GSfloat
gsTimeGetTime
(
	void
)
{
	LARGE_INTEGER		Time;
	LARGE_INTEGER		Freq;

	/* タイマの取得 */
	QueryPerformanceCounter( &Time );

	/* タイマの分解能を取得 */
	QueryPerformanceFrequency( &Freq );

	/* 秒単位に変換して返す */
	return	(GSfloat)( (double)Time.QuadPart / (double)Freq.QuadPart );
}

/********** End of File ******************************************************/
