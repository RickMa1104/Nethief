//Download by http://www.NewXing.com
// SelectFileOperationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "SelectFileOperationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectFileOperationDlg dialog


CSelectFileOperationDlg::CSelectFileOperationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectFileOperationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectFileOperationDlg)
	m_strSourceFileInfo = _T("");
	m_strDestFileInfo = _T("");
	m_strFilename = _T("");
	//}}AFX_DATA_INIT
}


void CSelectFileOperationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectFileOperationDlg)
	DDX_Control(pDX, IDC_BUTTON_CONTINUE, m_Button_Continue);
	DDX_Control(pDX, IDC_STATIC_DESTICON, m_Static_DestIcon);
	DDX_Control(pDX, IDC_STATIC_SOURCEICON, m_Static_SourceIcon);
	DDX_Text(pDX, IDC_STATIC_SOURCEFILEINFO, m_strSourceFileInfo);
	DDX_Text(pDX, IDC_STATIC_DESTFILEINFO, m_strDestFileInfo);
	DDX_Text(pDX, IDC_STATIC_FILENAME, m_strFilename);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectFileOperationDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectFileOperationDlg)
	ON_BN_CLICKED(IDC_BUTTON_OVERLAY, OnButtonOverlay)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, OnButtonContinue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectFileOperationDlg message handlers

BOOL CSelectFileOperationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Static_SourceIcon.SetIcon(m_hSourceIcon);//显示源文件图标
	m_Static_DestIcon.SetIcon(m_hDestIcon);//显示目的文件图标

	m_Button_Continue.EnableWindow(!m_bDoNotContinue);//如果不允许断点续传就把按钮灰掉

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectFileOperationDlg::OnButtonOverlay()
{
	// TODO: Add your control notification handler code here
	EndDialog(-1);//覆盖,返回-1
}

void CSelectFileOperationDlg::OnButtonContinue()
{
	// TODO: Add your control notification handler code here
	EndDialog(-2);//断点续传,返回-2
}
