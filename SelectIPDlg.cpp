//Download by http://www.NewXing.com
// SelectIPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "SelectIPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectIPDlg dialog


CSelectIPDlg::CSelectIPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectIPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectIPDlg)
	m_nCombo = -1;
	//}}AFX_DATA_INIT
}


void CSelectIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectIPDlg)
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nCombo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectIPDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectIPDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectIPDlg message handlers

BOOL CSelectIPDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nCount=m_aryIPAddrList.GetSize();
	for(int i=0;i<nCount;i++)
	{
		m_Combo.AddString(m_aryIPAddrList[i]);
	}
	m_Combo.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
