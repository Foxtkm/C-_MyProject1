/******************************************************************************
* File Name : gwaveres.c                           Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ‚v‚`‚u‚dƒŠƒ\[ƒXŠÇ—D
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gwaveres.h"
#include	"heap.h"

/****** ƒf[ƒ^Œ^éŒ¾ *********************************************************/

/*----- ‚v‚`‚u‚dƒŠƒ\[ƒXŠÇ—\‘¢‘Ì ------------------------------------------*/

typedef	struct
{
	GSOUND		gsnd;				/* ƒTƒEƒ“ƒhƒnƒ“ƒhƒ‰         */
	int			n;					/* ‚v‚`‚u‚dƒŠƒ\[ƒX”       */
	GWAVE**		wave;				/* ‚v‚`‚u‚dƒŠƒ\[ƒX         */

} waveres;

/*=============================================================================
*
* Purpose : ‚v‚`‚u‚dƒŠƒ\[ƒX‚Ì¶¬D
*
* Return  : ‚È‚µD
*
*============================================================================*/
GWAVERESOURCE
GWaveResNew
(
	GSOUND			gsnd			/* ƒTƒEƒ“ƒhƒnƒ“ƒhƒ‰         */
)
{
	waveres*		wres;

	wres = new_object( sizeof ( waveres ) );

	wres->gsnd = gsnd;
	wres->n    = 0;
	wres->wave = NULL;

	return	(GWAVERESOURCE)wres;
}

/*=============================================================================
*
* Purpose : ‚v‚`‚u‚dƒŠƒ\[ƒX‚ÌÁ–ÅD
*
* Return  : ‚È‚µD
*
*============================================================================*/
void
GWaveResDelete
(
	GWAVERESOURCE	hnd				/* ‚v‚`‚u‚dƒŠƒ\[ƒXƒnƒ“ƒhƒ‰ */
)
{
	waveres*		wres = (waveres*)hnd;

	/* ‚·‚×‚Ä‚Ì‚v‚`‚u‚dƒŠƒ\[ƒX‚ð‰ð•ú */
	GWaveResDelAllResource( hnd );

	del_object( wres );
}

/*=============================================================================
*
* Purpose : ‚v‚`‚u‚dƒŠƒ\[ƒX‚Ì‰Šú‰»D
*
* Return  : ‚È‚µD
*
*============================================================================*/
void
GWaveResInit
(
	GWAVERESOURCE	hnd,			/* ‚v‚`‚u‚dƒŠƒ\[ƒXƒnƒ“ƒhƒ‰ */
	int				n				/* ‚v‚`‚u‚dƒŠƒ\[ƒX”       */
)
{
	waveres*		wres = (waveres*)hnd;
	int				i;

	/* ‚·‚×‚Ä‚Ì‚v‚`‚u‚dƒŠƒ\[ƒX‚ð‰ð•ú */
	GWaveResDelAllResource( hnd );

	wres->n    = n;
	wres->wave = NULL;

	if ( wres->n != 0 )
	{
		wres->wave = new_object( sizeof( GWAVE* ) * wres->n );
	}
	for ( i = 0; i < wres->n; i++ )
	{
		wres->wave[i] = NULL;
	}
}

/*=============================================================================
*
* Purpose : ‚v‚`‚u‚dƒŠƒ\[ƒX‚ð‚·‚×‚Ä‰ð•úD
*
* Return  : ‚È‚µD
*
*============================================================================*/
void
GWaveResDelAllResource
(
	GWAVERESOURCE	hnd				/* ‚v‚`‚u‚dƒŠƒ\[ƒXƒnƒ“ƒhƒ‰ */
)
{
	waveres*		wres = (waveres*)hnd;
	int				i;

	if ( wres->wave == NULL )
	{
		return;
	}

	for ( i = 0; i < wres->n; i++ )
	{
		if ( wres->wave[i] != NULL )
		{
			GSoundDeleteWave( wres->gsnd, wres->wave[i] );
		
			wres->wave[i] = NULL;
		}
	}
	del_object( wres->wave );

	wres->n    = 0;
	wres->wave = NULL;
}

/*=============================================================================
*
* Purpose : ‚v‚`‚u‚dƒŠƒ\[ƒX‚Ì“Ç‚Ýž‚ÝD
*
* Return  : “Ç‚Ýž‚Ý‚É¬Œ÷‚µ‚½‚ç^CŽ¸”s‚µ‚½‚ç‹U‚ð•Ô‚·D
*
*============================================================================*/
BOOL
GWaveResLoadResource
(
	GWAVERESOURCE	hnd,			/* ‚v‚`‚u‚dƒŠƒ\[ƒXƒnƒ“ƒhƒ‰ */
	int				id,				/* ‚v‚`‚u‚dƒŠƒ\[ƒX‚h‚c     */
	const char*		wave_fname,		/* ‚v‚`‚u‚dƒtƒ@ƒCƒ‹–¼       */
	int				cnt,			/* “¯ŽžÄ¶”               */
	GWAVEFLAG		flag			/* Ä¶ƒ^ƒCƒvƒtƒ‰ƒO         */
)
{
	waveres*		wres = (waveres*)hnd;
	GWAVE			wave;

	if ( id >= wres->n )
	{
		return	FALSE;
	}

	if ( wres->wave[ id ] != NULL )
	{
		return	FALSE;
	}

	/* ‚v‚`‚u‚dƒtƒ@ƒCƒ‹‚Ì“Ç‚Ýž‚Ý */
	if ( ( wave = GSoundLoadWave( wres->gsnd, wave_fname, cnt, flag ) ) == NULL )
	{
		return	FALSE;
	}
	wres->wave[ id ]= wave;

	return	TRUE;
}

/*=============================================================================
*
* Purpose : ‚v‚`‚u‚dƒŠƒ\[ƒX‚ÌŽæ“¾D
*
* Return  : “Ç‚Ýž‚Ý‚É¬Œ÷‚µ‚½‚ç^CŽ¸”s‚µ‚½‚ç‹U‚ð•Ô‚·D
*
*============================================================================*/
GWAVE
GWaveResGetResource
(
	GWAVERESOURCE	hnd,			/* ‚v‚`‚u‚dƒŠƒ\[ƒXƒnƒ“ƒhƒ‰ */
	int				id				/* ‚v‚`‚u‚dƒŠƒ\[ƒX‚h‚c     */
)
{
	waveres*		wres = (waveres*)hnd;

	if ( id >= wres->n )
	{
		return	NULL;
	}

	return	wres->wave[ id ];
}

/*=============================================================================
*
* Purpose : ‚v‚`‚u‚dƒŠƒ\[ƒX‚Ì‰ð•úD
*
* Return  : ‚È‚µD
*
*============================================================================*/
void
GWaveResDelResource
(
	GWAVERESOURCE	hnd,			/* ‚v‚`‚u‚dƒŠƒ\[ƒXƒnƒ“ƒhƒ‰ */
	int				id				/* ‚v‚`‚u‚dƒŠƒ\[ƒX‚h‚c     */
)
{
	waveres*		wres = (waveres*)hnd;

	if ( id >= wres->n )
	{
		return;
	}
	if ( wres->wave[ id ] == NULL )
	{
		return;
	}

	GSoundDeleteWave( wres->gsnd, wres->wave[ id ] );

	wres->wave[ id ] = NULL;
}

/********** End of File ******************************************************/
