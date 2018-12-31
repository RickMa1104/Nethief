//Download by http://www.NewXing.com
// ToolBarPropertyMultiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "ToolBarPropertyMultiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarPropertyMultiDlg dialog


CToolBarPropertyMultiDlg::CToolBarPropertyMultiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarPropertyMultiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarPropertyMultiDlg)
	m_strSize = _T("");
	m_strStatistic = _T("");
	m_strHostName = _T("");
	//}}AFX_DATA_INIT
}


void CToolBarPropertyMultiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarPropertyMultiDlg)
	DDX_Text(pDX, IDC_STATIC_SIZE, m_strSize);
	DDX_Text(pDX, IDC_STATIC_STATISTIC, m_strStatistic);
	DDX_Text(pDX, IDC_STATIC_HOSTNAME, m_strHostName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolBarPropertyMultiDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarPropertyMultiDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarPropertyMultiDlg message handlers
