//Download by http://www.NewXing.com
#if !defined(AFX_MYFORMVIEW_H__2AAF5F8E_B0C1_11D4_89D9_645604C10000__INCLUDED_)
#define AFX_MYFORMVIEW_H__2AAF5F8E_B0C1_11D4_89D9_645604C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MyFormView.h : header file
//
class CNethiefDoc;
#include "LinkStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CMyFormView form view
//*************************************
#pragma pack(1) //字节对齐

typedef struct
{
	CCommunity*		pCommunity;
	CNethiefDoc*	pDoc;
	CWnd*			pWnd;
}
ONLINE_THREAD_PARAM,*pONLINE_THREAD_PARAM;

typedef struct
{
	CCommunity*		pCommunity;
	CStatic*		pStatic_HostName;
	CStatic*		pStatic_IPAddress;
	CStatic*		pStatic_Position;
	CStatic*		pStatic_OnlineNum;
	CNethiefDoc*	pDoc;
	CWnd*			pWnd;
}
CONN_FTP_THREAD_PARAM,*pCONN_FTP_THREAD_PARAM;

typedef struct
{
	CCommunity*		pCommunity;
	CNethiefDoc*	pDoc;
	CWnd*			pWnd;
}
KEEP_FTP_SESSION_THREAD_PARAM,*pKEEP_FTP_SESSION_THREAD_PARAM;

typedef struct
{
	CCommunity*		pCommunity;
	CWnd*			pWnd;
	CEdit*			pEdit_Callboard;
}
CALLBOARD_THREAD_PARAM,*pCALLBOARD_THREAD_PARAM;

typedef struct
{
	CCommunity*			pCommunity;
	CNethiefDoc*		pDoc;
	CStatic*			pStatic_OnlineNum;
	CListCtrl*			pList;
	SOCKET				hSocket;
	CMapStringToString*	pHostRemarkMap;
}
RECV_NOTIFY_THREAD_PARAM,*pRECV_NOTIFY_THREAD_PARAM;

typedef struct
{
	CCommunity*		pCommunity;
	CNethiefDoc*	pDoc;
	CStatic*		pStatic_OnlineNum;
	CListCtrl*		pList;
}
LIST_REFRESH_THREAD_PARAM,*pLIST_REFRESH_THREAD_PARAM;

typedef struct
{
	CCommunity*		pCommunity;
	CWnd*			pWnd;
}
ACCEPT_CONNECT_THREAD_PARAM,*pACCEPT_CONNECT_THREAD_PARAM;

typedef struct
{
	CCommunity*		pCommunity;
	CLinkStatic*	pLinkStatic_Chatroom;
}
CHATROOM_THREAD_PARAM,*pCHATROOM_THREAD_PARAM;

#pragma pack()	//使用缺省对齐
//*************************************
UINT IsOnlineProc(LPVOID pParam);
UINT ConnectFtpServerProc(LPVOID pParam);
UINT KeepFtpSessionProc(LPVOID pParam);
UINT GetCallboardTextProc(LPVOID pParam);
UINT RecvNotifyProc(LPVOID pParam);
UINT AcceptTcpConnectProc(LPVOID pParam);
UINT ListRefreshProc(LPVOID pParam);
UINT RefreshChatroomProc(LPVOID pParam);

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMyFormView : public CFormView
{
protected:
	CMyFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyFormView)

// Form Data
public:
	//{{AFX_DATA(CMyFormView)
	enum { IDD = IDD_COMMUNITY };
	CLinkStatic	m_LinkStatic_Chatroom;
	CEdit	m_Edit_Callboard;
	CStatic	m_Static_OnlineNum;
	CStatic	m_Static_Position;
	CStatic	m_Static_HostName;
	CStatic	m_Static_IPAddress;
	CListCtrl	m_List;
	//}}AFX_DATA

// Attributes
public:
	//*****************************************************************
	CImageList			m_ImageList;//映象列表
	CString				m_strClipBoard;
	CMapStringToString	m_HostRemarkMap;

	CNethiefDoc*		m_pDoc;
	CCommunity*			m_pCommunity;

	HCURSOR				m_hCursorHand;  //手形光标

	CWinThread*			m_pIsOnlineThread;
	CWinThread*			m_pConnectFtpServerThread;
	CWinThread*			m_pKeepFtpSessionThread;
	CWinThread*			m_pGetCallboardTextThread;
	CWinThread*			m_pRecvNotifyThread;
	CWinThread*			m_pAcceptTcpConnectThread;
	CWinThread*			m_pListRefreshThread;
	CWinThread*			m_pRefreshChatroomThread;
	//*****************************************************************

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	// Generated message map functions
	//{{AFX_MSG(CMyFormView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSocket(WPARAM wParam,LPARAM lParam);
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupmenuCopy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPopupmenuAdd();
	afx_msg void OnPopupmenuRemark();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFORMVIEW_H__2AAF5F8E_B0C1_11D4_89D9_645604C10000__INCLUDED_)
