// HelpUpdateDlg.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Nethief.h"
#include "HelpUpdateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpUpdateDlg dialog


CHelpUpdateDlg::CHelpUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpUpdateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHelpUpdateDlg)
	m_strDate = _T("");
	m_strVersion = _T("");
	m_strNewDate = _T("");
	m_strNewVersion = _T("");
	m_strNote = _T("");
	//}}AFX_DATA_INIT
}


void CHelpUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpUpdateDlg)
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Text(pDX, IDC_STATIC_DATE, m_strDate);
	DDX_Text(pDX, IDC_STATIC_VERSION, m_strVersion);
	DDX_Text(pDX, IDC_STATIC_NEWDATE, m_strNewDate);
	DDX_Text(pDX, IDC_STATIC_NEWVERSION, m_strNewVersion);
	DDX_Text(pDX, IDC_STATIC_NOTE, m_strNote);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpUpdateDlg, CDialog)
	//{{AFX_MSG_MAP(CHelpUpdateDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpUpdateDlg message handlers

BOOL CHelpUpdateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CNethiefApp* app=(CNethiefApp*)AfxGetApp();
	m_strVersion=app->m_strVersion;
	m_strDate=app->m_strDate;

	m_Button1.EnableWindow(FALSE);
	UpdateData(FALSE);
	SetTimer(1,50,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHelpUpdateDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,NULL,"http://www.greenstuffsoft.net/"
		,NULL,NULL,SW_SHOWNORMAL);
}

void CHelpUpdateDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	BeginWaitCursor();

	KillTimer(1);
	//////////////////////////////////////////////////////////////
	((CNethiefApp*)AfxGetApp())->pCommunity->GetNewVersionDateInfo(
		&m_strNewVersion,&m_strNewDate);

	if(m_strNewVersion=="<未知>")
	{
		m_strNote="未连接到 Internet，无法从网上下载最新版本信息，请在上线后再试。";
	}
	else
	{
		if(m_strNewVersion==m_strVersion)
		{
			m_strNote="本软件为最新版本，无需升级。";
		}
		else
		{
			m_strNote="本软件不是最新版本，可以单击“在线升级”进行升级。";
			m_Button1.EnableWindow(TRUE);
		}
	}
	UpdateData(FALSE);

	EndWaitCursor();
}
