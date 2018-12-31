//Download by http://www.NewXing.com
// ToolBarPropertySingleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "ToolBarPropertySingleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarPropertySingleDlg dialog


CToolBarPropertySingleDlg::CToolBarPropertySingleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarPropertySingleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarPropertySingleDlg)
	m_Edit_strFilename = _T("");
	m_strFileSize = _T("");
	m_strDataType = _T("");
	m_strATime = _T("");
	m_strCTime = _T("");
	m_strMTime = _T("");
	m_bIsArchive = FALSE;
	m_bIsHide = FALSE;
	m_bIsReadOnly = FALSE;
	m_bIsSystem = FALSE;
	m_strHostName = _T("");
	//}}AFX_DATA_INIT
}


void CToolBarPropertySingleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarPropertySingleDlg)
	DDX_Control(pDX, IDC_STATIC_ICON, m_Static_Icon);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_Edit_strFilename);
	DDX_Text(pDX, IDC_STATIC_SIZE, m_strFileSize);
	DDX_Text(pDX, IDC_STATIC_TYPE, m_strDataType);
	DDX_Text(pDX, IDC_STATIC_ATIME, m_strATime);
	DDX_Text(pDX, IDC_STATIC_CTIME, m_strCTime);
	DDX_Text(pDX, IDC_STATIC_MTIME, m_strMTime);
	DDX_Check(pDX, IDC_CHECK_ARCHIVE, m_bIsArchive);
	DDX_Check(pDX, IDC_CHECK_HIDE, m_bIsHide);
	DDX_Check(pDX, IDC_CHECK_READONLY, m_bIsReadOnly);
	DDX_Check(pDX, IDC_CHECK_SYSTEM, m_bIsSystem);
	DDX_Text(pDX, IDC_STATIC_HOSTNAME, m_strHostName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolBarPropertySingleDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarPropertySingleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarPropertySingleDlg message handlers

BOOL CToolBarPropertySingleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Static_Icon.SetIcon(m_hIcon);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
