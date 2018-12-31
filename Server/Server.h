//Download by http://www.NewXing.com
// Server.h : main header file for the SERVER application
//

#if !defined(AFX_SERVER_H__06D61C85_E2AA_11D4_89D9_505604C10000__INCLUDED_)
#define AFX_SERVER_H__06D61C85_E2AA_11D4_89D9_505604C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Community.h"		//神偷服务器类
#include "FileSocket.h"
/////////////////////////////////////////////////////////////////////////////
// CServerApp:
// See Server.cpp for the implementation of this class
//

class CServerApp : public CWinApp
{
public:
	CServerApp();
	~CServerApp();

	HANDLE			m_hMutex;
	//********************************************
	CCommunity*		pCommunity;		//神偷服务器类
	CFileSocket*	pFileSocket;
	//********************************************
	BOOL FirstRun();
	void RegWriteValue(HKEY hKey,CString sKey,
		CString sChildKey,CString strChildKeyValue);
	CString RegReadValue(HKEY hKey,CString sKey,CString sChildKey);
	void RegDeleteValue(HKEY hKey,CString sKey,CString sChildKey);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVER_H__06D61C85_E2AA_11D4_89D9_505604C10000__INCLUDED_)
