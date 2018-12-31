//Download by http://www.NewXing.com
// RemarkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "RemarkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemarkDlg dialog


CRemarkDlg::CRemarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemarkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemarkDlg)
	m_strRemark = _T("");
	//}}AFX_DATA_INIT
}


void CRemarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemarkDlg)
	DDX_Text(pDX, IDC_EDIT, m_strRemark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemarkDlg, CDialog)
	//{{AFX_MSG_MAP(CRemarkDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemarkDlg message handlers
