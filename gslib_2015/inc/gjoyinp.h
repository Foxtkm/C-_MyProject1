/******************************************************************************
* File Name : gjoyinp.h                            Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       �Q�[���V�X�e���p �W���C�X�e�B�b�N���� �w�b�_�t�@�C���D
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GJOYINP_H_
#define		_GJOYINP_H_

#include	"type.h"

/****** �萔�錾 *************************************************************/

#define	GJOYSTICK_MAX		8		/* �ő�W���C�X�e�B�b�N��       */
#define	GJOY_BUTTON_MAX		10		/* �ő�W���C�X�e�B�b�N�{�^���� */

/****** �f�[�^�^�錾 *********************************************************/

typedef	void*		GJOYINP;		/* �W���C�X�e�B�b�N�n���h�� */

/*----- �W���C�X�e�B�b�N���͏�� --------------------------------------------*/

typedef	enum
{
	GJOY_LEFT 		=	0x8000U,	/* �W���C�X�e�B�b�N����         */
	GJOY_RIGHT		=	0x4000U,	/* �W���C�X�e�B�b�N���E         */
	GJOY_UP			=	0x2000U,	/* �W���C�X�e�B�b�N����         */
	GJOY_DOWN		=	0x1000U,	/* �W���C�X�e�B�b�N����         */
	GJOY_BUTTON_1	=	0x0001U,	/* �W���C�X�e�B�b�N�{�^��1      */
	GJOY_BUTTON_2	=	0x0002U,	/* �W���C�X�e�B�b�N�{�^��2      */
	GJOY_BUTTON_3	=	0x0004U,	/* �W���C�X�e�B�b�N�{�^��3      */
	GJOY_BUTTON_4	=	0x0008U,	/* �W���C�X�e�B�b�N�{�^��4      */
	GJOY_BUTTON_5	=	0x0010U,	/* �W���C�X�e�B�b�N�{�^��5      */
	GJOY_BUTTON_6	=	0x0020U,	/* �W���C�X�e�B�b�N�{�^��6      */
	GJOY_BUTTON_7	=	0x0040U,	/* �W���C�X�e�B�b�N�{�^��7      */
	GJOY_BUTTON_8	=	0x0080U,	/* �W���C�X�e�B�b�N�{�^��8      */
	GJOY_BUTTON_9	=	0x0100U,	/* �W���C�X�e�B�b�N�{�^��9      */
	GJOY_BUTTON_10	=	0x0200U		/* �W���C�X�e�B�b�N�{�^��10     */

} GJOYSTATE;

/****** �֐��v���g�^�C�v�錾 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�̐����D
*
* Return  : �W���C�X�e�B�b�N�n���h����Ԃ��D
*
*============================================================================*/
GJOYINP
GJoyInpNew
(
	HINSTANCE			hinst,		/* �C���X�^���X�n���h��         */
	HWND				hwnd		/* �E�B���h�E�n���h��           */
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�̏��ŁD
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
GJoyInpDelete
(
	GJOYINP				hnd			/* �W���C�X�e�B�b�N�n���h��     */
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�̓ǂݍ��݁D
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
GJoyInpReadDevice
(
	GJOYINP				hnd			/* �W���C�X�e�B�b�N�n���h��     */
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�̓��͏�Ԃ𒲂ׂ�D
*
* Return  : ������Ă���ΐ^�C������Ă��Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
GJoyInpGetState
(
	GJOYINP				hnd,		/* �W���C�X�e�B�b�N�n���h��     */
	int					joy_no,		/* �W���C�X�e�B�b�N�i���o       */
	GJOYSTATE			state		/* ���ׂ����W���C�X�e�b�N�̏�� */
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�������ꂽ�����ׂ�D
*
* Return  : �����ꂽ��^�C������Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
GJoyInpGetTrigger
(
	GJOYINP				hnd,		/* �W���C�X�e�B�b�N�n���h��     */
	int					joy_no,		/* �W���C�X�e�B�b�N�i���o       */
	GJOYSTATE			state		/* ���ׂ����W���C�X�e�b�N�̏�� */
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�������ꂽ�����ׂ�D
*
* Return  : �����ꂽ��^�C������Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
GJoyInpGetDetach
(
	GJOYINP				hnd,		/* �W���C�X�e�B�b�N�n���h��     */
	int					joy_no,		/* �W���C�X�e�B�b�N�i���o       */
	GJOYSTATE			state		/* ���ׂ����W���C�X�e�b�N�̏�� */
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�̎��̏�Ԃ�ǂݍ��ށD
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
GJoyInpGetAxis
(
	GJOYINP				hnd,		/* �W���C�X�e�B�b�N�n���h��         */
	int					joy_no,		/* �W���C�X�e�B�b�N�i���o           */
	int*				lx,			/*-> ���W���C�X�e�b�B�N�̂����̏��   */
	int*				ly,			/*-> ���W���C�X�e�b�B�N�̂����̏��   */
	int*				rx,			/*-> �E�W���C�X�e�b�B�N�̂����̏��   */
	int*				ry			/*-> �E�W���C�X�e�b�B�N�̂����̏��   */
);

/*=============================================================================
*
* Purpose : �ڑ�����Ă���W���C�X�e�B�b�N���𒲂ׂ�D
*
* Return  : �ڑ�����Ă���W���C�X�e�B�b�N����Ԃ��D
*
*============================================================================*/
int
GJoyInpGetCount
(
	GJOYINP				hnd			/* �W���C�X�e�B�b�N�n���h��     */
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�̃A�N�Z�X���̍Ď擾�D
*
* Return  : �擾�ł���ΐ^�C�擾�ł��Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
GJoyInpReacquire
(
	GJOYINP				hnd			/* �W���C�X�e�B�b�N�n���h��     */
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/