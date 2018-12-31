//Download by http://www.NewXing.com
// ToolBarPropertyDiskdriveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "ToolBarPropertyDiskdriveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarPropertyDiskdriveDlg dialog


CToolBarPropertyDiskdriveDlg::CToolBarPropertyDiskdriveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarPropertyDiskdriveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarPropertyDiskdriveDlg)
	m_strName = _T("");
	m_strSerialNumber = _T("");
	m_strTotal = _T("");
	m_strUnused = _T("");
	m_strUnusedPercent = _T("");
	m_strUsed = _T("");
	m_strUsedPercent = _T("");
	m_strType = _T("");
	m_strHostName = _T("");
	//}}AFX_DATA_INIT
}


void CToolBarPropertyDiskdriveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarPropertyDiskdriveDlg)
	DDX_Control(pDX, IDC_STATIC_ICON, m_Static_Icon);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_STATIC_SERIALNUMBER, m_strSerialNumber);
	DDX_Text(pDX, IDC_STATIC_TOTAL, m_strTotal);
	DDX_Text(pDX, IDC_STATIC_UNUSED, m_strUnused);
	DDX_Text(pDX, IDC_STATIC_UNUSED_PERCENT, m_strUnusedPercent);
	DDX_Text(pDX, IDC_STATIC_USED, m_strUsed);
	DDX_Text(pDX, IDC_STATIC_USED_PERCENT, m_strUsedPercent);
	DDX_Text(pDX, IDC_STATIC_TYPE, m_strType);
	DDX_Text(pDX, IDC_STATIC_HOSTNAME, m_strHostName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolBarPropertyDiskdriveDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarPropertyDiskdriveDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarPropertyDiskdriveDlg message handlers

BOOL CToolBarPropertyDiskdriveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Static_Icon.SetIcon(m_hIcon);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
