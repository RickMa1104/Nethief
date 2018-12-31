//Download by http://www.NewXing.com
// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "OptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog


COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionDlg)
	m_strViewerFilename = _T("");
	m_bSelectIP = FALSE;
	//}}AFX_DATA_INIT
}


void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDlg)
	DDX_Control(pDX, IDC_EDIT_VIEWERFILENAME, m_Edit_ViewerFilename);
	DDX_Text(pDX, IDC_EDIT_VIEWERFILENAME, m_strViewerFilename);
	DDX_Check(pDX, IDC_CHECK_SELECTIP, m_bSelectIP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	//{{AFX_MSG_MAP(COptionDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg message handlers

void COptionDlg::OnButtonBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
			"程序(*.exe)|*.exe|所有文件(*.*)|*.*||");
	dlg.m_ofn.lpstrTitle="请选择查看程序...";
	if(dlg.DoModal()==IDOK) 
	{
		m_Edit_ViewerFilename.SetWindowText(dlg.GetPathName());
	}				
}
