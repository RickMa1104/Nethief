//Download by http://www.NewXing.com
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2DA6D107_AC2D_11D4_89D9_2C4705C10000__INCLUDED_)
#define AFX_STDAFX_H__2DA6D107_AC2D_11D4_89D9_2C4705C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//WinsockҪ������ͷ�ļ�(���⻹Ҫ����ws2_32.lib)///
#include <winsock2.h>		//ʹ��Winsock2���Լ������
#include <afxsock.h>		//AppWizardѡWinsock֧��ʱ�Զ������
//////////////˳���ܵߵ�////////////////////////
#include <mmsystem.h>
#include ".\Public\STRUCTURE_DEFINE.h"	//�ṹ��������ͷ�ļ�

#define ONLINE_TEST		1		//���߲���(0:��,1:��)
#define CHECK_SUM		0		//���У���(0:��,1:��)

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2DA6D107_AC2D_11D4_89D9_2C4705C10000__INCLUDED_)
