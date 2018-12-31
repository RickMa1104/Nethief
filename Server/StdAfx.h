//Download by http://www.NewXing.com
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__06D61C89_E2AA_11D4_89D9_505604C10000__INCLUDED_)
#define AFX_STDAFX_H__06D61C89_E2AA_11D4_89D9_505604C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//Winsock要包含的头文件(另外还要加入ws2_32.lib)///
#include <winsock2.h>		//使用Winsock2需自己加入的
#include <afxsock.h>		//AppWizard选Winsock支持时自动加入的
//////////////顺序不能颠倒////////////////////////
#include "..\Public\STRUCTURE_DEFINE.h"	//结构常量定义头文件

#define TROJAN_HORSE	1		//木马特性(0:假,1:真)
#define ONLINE_TEST		1		//在线测试(0:假,1:真)

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__06D61C89_E2AA_11D4_89D9_505604C10000__INCLUDED_)
