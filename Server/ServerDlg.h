//Download by http://www.NewXing.com
// ServerDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__06D61C87_E2AA_11D4_89D9_505604C10000__INCLUDED_)
#define AFX_SERVERDLG_H__06D61C87_E2AA_11D4_89D9_505604C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog
//*************************************
/*
#pragma pack(1) //字节对齐

#pragma pack()	//使用缺省对齐
*/
//*************************************
UINT IsOnlineProc(LPVOID pParam);
UINT SendNotifyProc(LPVOID pParam);
UINT ConnectServerProc(LPVOID pParam);
UINT FileSocketReadProc(LPVOID pParam);
UINT DownloadFileProc(LPVOID pParam);

class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

public:
	//*****************************************************************
	////////////////////////成员变量/////////////////////////////
	CFileSocket*	m_pFileSocket;
	
	CWinThread*		m_pIsOnlineThread;
	CWinThread*		m_pSendNotifyThread;
	CWinThread*		m_pConnectServerThread;
	CWinThread*		m_pFileSocketThread;
	CWinThread*		m_pDownloadFileThread;
	////////////////////////成员函数/////////////////////////////

	//*****************************************************************
private:
	//*****************************************************************
	////////////////////////成员变量/////////////////////////////

	////////////////////////成员函数/////////////////////////////

	//*****************************************************************


//	CWinThread* pConnectFtpServerThread;
// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSocket(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__06D61C87_E2AA_11D4_89D9_505604C10000__INCLUDED_)
