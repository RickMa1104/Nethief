//Download by http://www.NewXing.com
// ToolBarFindFileResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "ToolBarFindFileResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarFindFileResultDlg dialog


CToolBarFindFileResultDlg::CToolBarFindFileResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarFindFileResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarFindFileResultDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_StringArray.SetSize(128,128);
}


void CToolBarFindFileResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarFindFileResultDlg)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolBarFindFileResultDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarFindFileResultDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarFindFileResultDlg message handlers

BOOL CToolBarFindFileResultDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_List.InsertColumn(0,"文件路径名",LVCFMT_LEFT,480,-1);

	int nCount=m_StringArray.GetSize();
	for(int i=0;i<nCount;i++)
	{
		m_List.InsertItem(i,m_StringArray[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
