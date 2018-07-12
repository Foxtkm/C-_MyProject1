/******************************************************************************
* File Name : gmidires.c                           Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ‚l‚h‚c‚hƒŠƒ\[ƒXŠÇ—D
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gmidires.h"
#include	"heap.h"

/****** ƒf[ƒ^Œ^éŒ¾ *********************************************************/

/*----- ‚l‚h‚c‚hƒŠƒ\[ƒXŠÇ—\‘¢‘Ì ------------------------------------------*/

typedef	struct
{
	GSOUND		gsnd;				/* ƒTƒEƒ“ƒhƒnƒ“ƒhƒ‰         */
	int			n;					/* ‚l‚h‚c‚hƒŠƒ\[ƒX”       */
	GMIDI**		midi;				/* ‚l‚h‚c‚hƒŠƒ\[ƒX”       */

} midires;

/*=============================================================================
*
* Purpose : ‚l‚h‚c‚hƒŠƒ\[ƒX‚Ì¶¬D
*
* Return  : ‚l‚h‚c‚hƒŠƒ\[ƒXƒnƒ“ƒhƒ‰‚ð•Ô‚·D
*
*============================================================================*/
GMIDIRESOURCE
GMidiResNew
(
	GSOUND			gsnd			/* ƒTƒEƒ“ƒhƒnƒ“ƒhƒ‰         */
)
{
	midires*		mres;

	mres = new_object( sizeof( midires ) );

	mres->gsnd = gsnd;
	mres->n    = 0;
	mres->midi = NULL;

	return	(GMIDIRESOURCE)mres;
}

/*=============================================================================
*
* Purpose : ‚l‚h‚c‚hƒŠƒ\[ƒX‚ÌÁ–ÅD
*
* Return  : ‚È‚µD
*
*============================================================================*/
void
GMidiResDelete
(
	GMIDIRESOURCE	hnd				/* ‚l‚h‚c‚hƒŠƒ\[ƒXƒnƒ“ƒhƒ‰ */
)
{
	midires*		mres = (midires*)hnd;

	/* ‚·‚×‚Ä‚Ì‚l‚h‚c‚hƒŠƒ\[ƒX‚ð‰ð•ú */
	GMidiResDelAllResource( hnd );

	del_object( mres );
}

/*=============================================================================
*
* Purpose : ‚l‚h‚c‚hƒŠƒ\[ƒX‚Ì‰Šú‰»D
*
* Return  : ‚È‚µD
*
*============================================================================*/
void
GMidiResInit
(
	GMIDIRESOURCE	hnd,			/* ‚l‚h‚c‚hƒŠƒ\[ƒXƒnƒ“ƒhƒ‰ */
	int				n				/* ‚l‚h‚c‚hƒŠƒ\[ƒX”       */
)
{
	midires*		mres = (midires*)hnd;
	int				i;

	/* ‚·‚×‚Ä‚Ì‚l‚h‚c‚hƒŠƒ\[ƒX‚ð‰ð•ú */
	GMidiResDelAllResource( hnd );

	mres->n    = n;
	mres->midi = NULL;

	if ( mres->n != 0 )
	{
		mres->midi = new_object( sizeof( GMIDI* ) * mres->n );
	}
	for ( i = 0; i < mres->n; i++ )
	{
		mres->midi[i] = NULL;
	}
}

/*=============================================================================
*
* Purpose : ‚l‚h‚c‚hƒŠƒ\[ƒX‚ð‚·‚×‚Ä‰ð•úD
*
* Return  : ‚È‚µD
*
*============================================================================*/
void
GMidiResDelAllResource
(
	GMIDIRESOURCE	hnd				/* ‚l‚h‚c‚hƒŠƒ\[ƒXƒnƒ“ƒhƒ‰ */
)
{
	midires*		mres = (midires*)hnd;
	int				i;

	if ( mres->midi == NULL )
	{
		return;
	}

	for ( i = 0; i < mres->n; i++ )
	{
		if ( mres->midi[i] != NULL )
		{
			GSoundDeleteMidi( mres->gsnd, mres->midi[i] );
		
			mres->midi[i] = NULL;
		}
	}
	del_object( mres->midi );

	mres->n    = 0;
	mres->midi = NULL;
}

/*=============================================================================
*
* Purpose : ‚l‚h‚c‚hƒŠƒ\[ƒX‚Ì“Ç‚Ýž‚ÝD
*
* Return  : “Ç‚Ýž‚Ý‚É¬Œ÷‚µ‚½‚ç^CŽ¸”s‚µ‚½‚ç‹U‚ð•Ô‚·D
*
*============================================================================*/
BOOL
GMidiResLoadResource
(
	GMIDIRESOURCE	hnd,			/* ‚l‚h‚c‚hƒŠƒ\[ƒXƒnƒ“ƒhƒ‰ */
	int				id,				/* ‚l‚h‚c‚hƒŠƒ\[ƒX‚h‚c     */
	const char*		midi_fname,		/* ‚l‚h‚c‚hƒtƒ@ƒCƒ‹–¼       */
	BOOL			flag			/* ƒ‹[ƒvŽw’èƒtƒ‰ƒO         */
)
{
	midires*		mres = (midires*)hnd;
	GMIDI			midi;

	if ( id >= mres->n )
	{
		return	FALSE;
	}

	if ( mres->midi[ id ] != NULL )
	{
		return	FALSE;
	}

	/* ‚l‚h‚c‚hƒtƒ@ƒCƒ‹‚Ì“Ç‚Ýž‚Ý */
	if ( ( midi = GSoundLoadMidi( mres->gsnd, midi_fname, flag ) ) == NULL )
	{
		return	FALSE;
	}
	mres->midi[ id ]= midi;

	return	TRUE;
}

/*=============================================================================
*
* Purpose : ‚l‚h‚c‚hƒŠƒ\[ƒX‚ÌŽæ“¾D
*
* Return  : “Ç‚Ýž‚Ý‚É¬Œ÷‚µ‚½‚ç^CŽ¸”s‚µ‚½‚ç‹U‚ð•Ô‚·D
*
*============================================================================*/
GMIDI
GMidiResGetResource
(
	GMIDIRESOURCE	hnd,			/* ‚l‚h‚c‚hƒŠƒ\[ƒXƒnƒ“ƒhƒ‰ */
	int				id				/* ‚l‚h‚c‚hƒŠƒ\[ƒX‚h‚c     */
)
{
	midires*		mres = (midires*)hnd;

	if ( id >= mres->n )
	{
		return	NULL;
	}

	return	mres->midi[ id ];
}

/*=============================================================================
*
* Purpose : ‚l‚h‚c‚hƒŠƒ\[ƒX‚Ì‰ð•úD
*
* Return  : ‚È‚µD
*
*============================================================================*/
void
GMidiResDelResource
(
	GMIDIRESOURCE	hnd,			/* ‚l‚h‚c‚hƒŠƒ\[ƒXƒnƒ“ƒhƒ‰ */
	int				id				/* ‚l‚h‚c‚hƒŠƒ\[ƒX‚h‚c     */
)
{
	midires*		mres = (midires*)hnd;

	if ( id >= mres->n )
	{
		return;
	}
	if ( mres->midi[ id ] == NULL )
	{
		return;
	}

	GSoundDeleteMidi( mres->gsnd, mres->midi[ id ] );

	mres->midi[ id ] = NULL;
}

/********** End of File ******************************************************/
