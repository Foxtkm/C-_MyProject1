/******************************************************************************
* File Name : gsinput.h                          Ver : 1.00  Date : 2016-04-16
*
* Description :
*
*       ���̓f�o�C�X�V�X�e�� �w�b�_�t�@�C���D
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSINPUT_H_
#define		_GSINPUT_H_

#include	"ginput.h"
#include	"GStype.h"

/****** �萔�^�錾 ***********************************************************/

#ifdef __cplusplus
extern "C" {
#endif

extern const GSushort GS_XBOX_PAD_UP;
extern const GSushort GS_XBOX_PAD_DOWN;
extern const GSushort GS_XBOX_PAD_LEFT;
extern const GSushort GS_XBOX_PAD_RIGHT;
extern const GSushort GS_XBOX_PAD_START;
extern const GSushort GS_XBOX_PAD_BACK;
extern const GSushort GS_XBOX_PAD_LEFT_THUMB;
extern const GSushort GS_XBOX_PAD_RIGHT_THUMB;
extern const GSushort GS_XBOX_PAD_LEFT_SHOULDER;
extern const GSushort GS_XBOX_PAD_RIGHT_SHOULDER;
extern const GSushort GS_XBOX_PAD_A;
extern const GSushort GS_XBOX_PAD_B;
extern const GSushort GS_XBOX_PAD_X;
extern const GSushort GS_XBOX_PAD_Y;

/****** �֐��v���g�^�C�v�錾 *************************************************/


/*=============================================================================
*
* Purpose : ���̓f�o�C�X�V�X�e���̏������D
*
* Return  : �������ɐ�������ΐ^�C���s����΋U��Ԃ��D
*
*============================================================================*/
BOOL
gsInitInput
(
	HWND			hwnd			/* �E�B���h�E�n���h��           */
);

/*=============================================================================
*
* Purpose : ���̓f�o�C�X�V�X�e���̏I�������D
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
gsFinishInput
(
	void
);

/*=============================================================================
*
* Purpose : �E�B���h�E���A�N�e�B�u�ɂȂ������̏����D
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
gsActivateInput
(
	BOOL		active				/* �A�N�e�B�u�t���O             */
);

/*=============================================================================
*
* Purpose : ���̓f�o�C�X�̏�Ԃ�ǂݍ��ށD
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
gsReadInput
(
	void
);

/*=============================================================================
*
* Purpose : �L�[�̓��͏�Ԃ𒲂ׂ�D
*
* Return  : �L�[��������Ă���ΐ^�C������Ă��Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
gsGetKeyState
(
	GKEYCODE		key_code		/* �L�[�R�[�h                   */
);

/*=============================================================================
*
* Purpose : �L�[�������ꂽ�����ׂ�D
*
* Return  : �L�[��������Ă���ΐ^�C������Ă��Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
gsGetKeyTrigger
(
	GKEYCODE		key_code		/* �L�[�R�[�h                   */
);

/*=============================================================================
*
* Purpose : �L�[�������ꂽ�����ׂ�D
*
* Return  : �L�[���������ΐ^�C������Ă��Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
gsGetKeyDetach
(
	GKEYCODE		key_code		/* �L�[�R�[�h                   */
);

/*=============================================================================
*
* Purpose : �L�[�̉�����Ă���L�[�𒲂ׂ�D
*
* Return  : ������Ă���L�[�̃L�[�R�[�h��Ԃ��D
*           ����������Ă��Ȃ���΂O��Ԃ��D
*
*============================================================================*/
GKEYCODE
gsGetKey
(
	void
);

/*=============================================================================
*
* Purpose : �ڑ�����Ă���W���C�X�e�B�b�N���𒲂ׂ�D
*
* Return  : �ڑ�����Ă���W���C�X�e�B�b�N����Ԃ��D
*
*============================================================================*/
int
gsGetJoyCount
(
	void
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�̓��͏�Ԃ𒲂ׂ�D
*
* Return  : ������Ă���ΐ^�C������Ă��Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
gsGetJoyState
(
	int				joy_no,			/* �W���C�X�e�B�b�N�i���o       */
	GJOYSTATE		state			/* ���ׂ����W���C�X�e�b�N�̏�� */
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�������ꂽ�����ׂ�D
*
* Return  : �����ꂽ��^�C������Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
gsGetJoyTrigger
(
	int				joy_no,			/* �W���C�X�e�B�b�N�i���o       */
	GJOYSTATE		state			/* ���ׂ����W���C�X�e�b�N�̏�� */
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�������ꂽ�����ׂ�D
*
* Return  : �����ꂽ��^�C������Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
gsGetJoyDetach
(
	int				joy_no,			/* �W���C�X�e�B�b�N�i���o       */
	GJOYSTATE		state			/* ���ׂ����W���C�X�e�b�N�̏�� */
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�̎��̏�Ԃ𒲂ׂ�D
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
gsGetJoyAxis
(
	int				joy_no,			/* �W���C�X�e�B�b�N�i���o			  */
	int*			x,				/*-> ���W���C�X�e�b�B�N�̂����̏��   */	
	int*			y,				/*-> ���W���C�X�e�b�B�N�̂����̏��   */
	int*			z				/*-> ���W���C�X�e�b�B�N�̂����̏��   */
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�̍����̏�Ԃ𒲂ׂ�D
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
gsGetJoyLeftAxis
(
	int				joy_no,			/* �W���C�X�e�B�b�N�i���o			  */
	int*			x,				/*-> �W���C�X�e�b�B�N�̍������̏��   */
	int*			y				/*-> �W���C�X�e�b�B�N�̍������̏��   */
);

/*=============================================================================
*
* Purpose : �W���C�X�e�B�b�N�̉E���̏�Ԃ𒲂ׂ�D
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
gsGetJoyRightAxis
(
	int				joy_no,			/* �W���C�X�e�B�b�N�i���o			  */
	int*			x,				/*-> �E�W���C�X�e�b�B�N�̂����̏��   */
	int*			y				/*-> �E�W���C�X�e�b�B�N�̂����̏��   */
);

/*=============================================================================
*
* Purpose : �}�E�X�̈ړ��ʂ��擾�D
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
gsGetMouseVelocity
(
	int*			x,				/*-> �������ړ���               */
	int*			y,				/*-> �������ړ���               */
	int*			z				/*-> �������ړ���               */
);

/*=============================================================================
*
* Purpose : �}�E�X�J�[�\���̈ʒu���擾�D
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
gsGetMouseCursorPosition
(
	int*			x,				/*-> �����W                     */
	int*			y				/*-> �����W                     */
);

/*=============================================================================
*
* Purpose : �}�E�X�̃{�^���̉�����Ԃ��擾�D
*
* Return  : ������Ă���ΐ^�C������Ă��Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
gsGetMouseButtonState
(
	GMOUSTATE		state			/* ���ׂ����}�E�X�̃{�^��       */
);

/*=============================================================================
*
* Purpose : �}�E�X�̃{�^���������ꂽ�����ׂ�D
*
* Return  : �����ꂽ��^�C������Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
gsGetMouseButtonTrigger
(
	GMOUSTATE		state			/* ���ׂ����}�E�X�̃{�^��       */
);

/*=============================================================================
*
* Purpose : �}�E�X�̃{�^���������ꂽ�����ׂ�D
*
* Return  : �����ꂽ��^�C������Ȃ���΋U��Ԃ��D
*
*============================================================================*/
BOOL
gsGetMouseButtonDetach
(
	GMOUSTATE		state			/* ���ׂ����}�E�X�̃{�^��       */
);

/*=============================================================================
*
* Purpose : �}�E�X�J�[�\���̈ʒu��ݒ�D
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
gsSetMouseCursorPosition
(
	int				x,				/* �����W                       */
	int				y				/* �����W                       */
);

/*=============================================================================
*
* Purpose : �}�E�X�J�[�\����\���D
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
gsShowMouseCursor
(
	void
);

/*=============================================================================
*
* Purpose : �}�E�X�J�[�\�����\���D
*
* Return  : �Ȃ��D
*
*============================================================================*/
void
gsHideMouseCursor
(
	void
);

/*=============================================================================
*
* Purpose : XBOX�R���g���[���̃{�^����������Ă��邩���ׂ�D
*
* Return  : ������Ă���ΐ^�C������Ă��Ȃ���΋U��Ԃ��D
*
*============================================================================*/
extern GSboolean
gsXBoxPadButtonState
(
	GSuint				pad_no,		/* �p�b�h�i���o                     */
	GSushort			button		/* ���ׂ����{�^����			�@      */
);

/*=============================================================================
*
* Purpose : XBOX�R���g���[���̃{�^���������ꂽ�����ׂ�D
*
* Return  : �����ꂽ��^�C������Ȃ���΋U��Ԃ��D
*
*============================================================================*/
extern GSboolean
gsXBoxPadButtonTrigger
(
	GSuint				pad_no,		/* �p�b�h�i���o                     */
	GSushort			button		/* ���ׂ����{�^����			�@      */
);

/*=============================================================================
*
* Purpose : XBOX�R���g���[���̃{�^�����������ꂽ�����ׂ�D
*
* Return  : �����ꂽ��^�C������Ȃ���΋U��Ԃ��D
*
*============================================================================*/
extern GSboolean
gsXBoxPadButtonDetach
(
	GSuint				pad_no,		/* �p�b�h�i���o                     */
	GSushort			button		/* ���ׂ����{�^����			�@      */
);

/*=============================================================================
*
* Purpose : XBOX�R���g���[���̍��g���K�̒l���擾�D
*
* Return  : ���g���K�̒l��Ԃ��D(0.0f �` 1.0f)
*
*============================================================================*/
extern GSfloat
gsXBoxPadGetLeftTrigger
(
	GSuint				pad_no		/* �p�b�h�i���o                     */
);

/*=============================================================================
*
* Purpose : XBOX�R���g���[���̉E�g���K�̒l���擾�D
*
* Return  : �E�g���K�̒l��Ԃ��D(0.0f �` 1.0f���K�������l)
*
*============================================================================*/
extern GSfloat
gsXBoxPadGetRightTrigger
(
	GSuint				pad_no		/* �p�b�h�i���o                     */
);

/*=============================================================================
*
* Purpose : XBOX�R���g���[���̍��A�i���O�X�e�B�b�N�̏�Ԃ��擾�D
*
* Return  : ���A�i���O�X�e�B�b�N�̏�Ԃ�Ԃ��D(-1.0f �` 1.0f�ɐ��K�������l)
*
*============================================================================*/
extern void
gsXBoxPadGetLeftAxis
(
	GSuint				pad_no,		/* �p�b�h�i���o                     */
	GSvector2*			result		/* �A�i���O�X�e�B�b�N�̏��			*/
);

/*=============================================================================
*
* Purpose : XBOX�R���g���[���̉E�A�i���O�X�e�B�b�N�̏�Ԃ��擾�D
*
* Return  : �E�A�i���O�X�e�B�b�N�̏�Ԃ�Ԃ��D(-1.0f �` 1.0f�ɐ��K�������l)
*
*============================================================================*/
extern void
gsXBoxPadGetRightAxis
(
	GSuint				pad_no,		/* �p�b�h�i���o                     */
	GSvector2*			result		/* �A�i���O�X�e�B�b�N�̏��			*/
);

/*=============================================================================
*
* Purpose : XBOX�R���g���[���̐ڑ����𒲂ׂ�D
*
* Return  : �ڑ�����Ă���XBOX�R���g���[������Ԃ��D
*
*============================================================================*/
extern GSuint
gsXBoxGetPadCount
(
	void
);

#ifdef __cplusplus
}
#endif

/****** ���C�u�����ݒ� ********************************************************/

#pragma comment( lib, "dinput.lib"  )
#pragma comment( lib, "dxguid.lib"  )

#ifdef	NDEBUG
# ifdef _MT
#  ifdef _DLL
#   pragma comment( lib, "gsinput_md.lib" )
#   pragma comment( lib, "gsutil_md.lib" )
#  else
#   pragma comment( lib, "gsinput_mt.lib" )
#   pragma comment( lib, "gsutil_mt.lib" )
#  endif
# else
#  pragma comment( lib, "gsinput_ml.lib" )
#  pragma comment( lib, "gsutil_ml.lib" )
# endif
#else
# ifdef _MT
#  ifdef _DLL
#   pragma comment( lib, "gsinput_mdd.lib" )
#   pragma comment( lib, "gsutil_mdd.lib" )
#  else
#   pragma comment( lib, "gsinput_mtd.lib" )
#   pragma comment( lib, "gsutil_mtd.lib" )
#  endif
# else
#  pragma comment( lib, "gsinput_mld.lib" )
#  pragma comment( lib, "gsutil_mld.lib" )
# endif
#endif

#endif

/********** End of File ******************************************************/