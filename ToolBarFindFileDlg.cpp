// ToolBarFindFileDlg.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Nethief.h"
#include "ToolBarFindFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarFindFileDlg dialog


CToolBarFindFileDlg::CToolBarFindFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarFindFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarFindFileDlg)
	m_bCheck = TRUE;
	m_strEdit = _T("*.*");
	//}}AFX_DATA_INIT
}


void CToolBarFindFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarFindFileDlg)
	DDX_Check(pDX, IDC_CHECK1, m_bCheck);
	DDX_Text(pDX, IDC_EDIT1, m_strEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolBarFindFileDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarFindFileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarFindFileDlg message handlers
