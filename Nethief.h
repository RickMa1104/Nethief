//Download by http://www.NewXing.com
// Nethief.h : main header file for the NETHIEF application
//

#if !defined(AFX_NETHIEF_H__2DA6D105_AC2D_11D4_89D9_2C4705C10000__INCLUDED_)
#define AFX_NETHIEF_H__2DA6D105_AC2D_11D4_89D9_2C4705C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "SplashDlg.h"	//��������Ի���
#include "Community.h"	//IP��ַ֪ͨ����
#include "FileSocket.h"	//�ļ�������
/////////////////////////////////////////////////////////////////////////////
// CNethiefApp:
// See Nethief.cpp for the implementation of this class
//

class CNethiefApp : public CWinApp
{
public:
	CNethiefApp();
	~CNethiefApp();

	HANDLE		m_hMutex;		//������,��ֹ�����ظ�����

	CString		m_strVersion;	//����汾
	CString		m_strDate;		//��������

	CCommunity*	pCommunity;		//IP��ַ֪ͨ����
	CFileSocket* pFileSocket;	//�ļ�������

	void		RegisterAccounts();	//������
	BOOL		CheckSum();			//���MD5У����Ƿ���ȷ
	void		prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key);
	void		rc4(unsigned char *buffer_ptr, DWORD buffer_len, rc4_key *key);
	void		EncryptData(LPVOID pVoid,DWORD dwBufLen,CString strKey);

	CString GetINIfileString(LPCTSTR lpSectionName, //��INI�ж�ȡ�ַ�ֵ
							 LPCTSTR lpKeyName);
	int GetINIfileInt(LPCTSTR lpSectionName,  //��INI�ж�ȡ����ֵ
					  LPCTSTR lpKeyName);

	int nEasterEgg;				//����ڲʵ�����

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNethiefApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNethiefApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETHIEF_H__2DA6D105_AC2D_11D4_89D9_2C4705C10000__INCLUDED_)
