// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		データタイプ定義ヘッダファイル
// 
// 				Ver 3.19b
// 
// -------------------------------------------------------------------------------

#ifndef __DXDATATYPE_H__
#define __DXDATATYPE_H__

// インクルード ------------------------------------------------------------------
#include "DxCompileConfig.h"
#include <stdio.h>

#ifdef __WINDOWS__
#include "DxDataTypeWin.h"
#endif

#ifdef __ANDROID__
#include "DxDataTypeAndroid.h"
#endif // __ANDROID__

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
		#include "DxDataTypeiOS.h"
	#endif
#endif // __APPLE__

#ifdef __psp2__
#include "DxDataTypePSVita.h"
#endif // __psp2__

#ifdef __ORBIS__
#include "DxDataTypePS4.h"
#endif // __ORBIS__

#ifdef NN_NINTENDO_SDK
#include "DxDataTypeNSW.h"
#endif // NN_NINTENDO_SDK

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define SETRECT( Dest, Left, Top, Right, Bottom )	\
	( Dest ).left   = Left ;\
	( Dest ).top    = Top ;\
	( Dest ).right  = Right ;\
	( Dest ).bottom = Bottom ;

#ifdef __USE_ULL__
#define ULL_NUM( x )				( x##ULL )
#define LL_NUM( x )					( x##LL  )
#define ULL_PARAM( x )				x##ULL
#define LL_PARAM( x )				x##LL
#else
#define ULL_NUM( x )				( ( ULONGLONG )x )
#define LL_NUM( x )					(  ( LONGLONG )x )
#define ULL_PARAM( x )				x
#define LL_PARAM( x )				x
#endif

// 構造体定義 --------------------------------------------------------------------

// ＲＧＢＡ色構造体
struct RGBCOLOR
{
	unsigned char			Blue, Green, Red, Alpha ;
} ;

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXDATATYPE_H__
