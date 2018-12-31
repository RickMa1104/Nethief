//Download by http://www.NewXing.com
// ToolBarRunRemoteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "ToolBarRunRemoteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarRunRemoteDlg dialog


CToolBarRunRemoteDlg::CToolBarRunRemoteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarRunRemoteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarRunRemoteDlg)
	m_nComboBox = 0;
	m_strEdit = _T("");
	//}}AFX_DATA_INIT
}


void CToolBarRunRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarRunRemoteDlg)
	DDX_CBIndex(pDX, IDC_COMBO1, m_nComboBox);
	DDX_Text(pDX, IDC_EDIT1, m_strEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolBarRunRemoteDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarRunRemoteDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarRunRemoteDlg message handlers
