// ToolBarRunDlg.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Nethief.h"
#include "ToolBarRunDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarRunDlg dialog


CToolBarRunDlg::CToolBarRunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarRunDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarRunDlg)
	m_strEdit1 = _T("");
	m_strEdit2 = _T("");
	//}}AFX_DATA_INIT

	m_bIsRun=TRUE;
}


void CToolBarRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarRunDlg)
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Text(pDX, IDC_EDIT1, m_strEdit1);
	DDX_Text(pDX, IDC_EDIT2, m_strEdit2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolBarRunDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarRunDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarRunDlg message handlers

BOOL CToolBarRunDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_bIsRun && m_strPathName!="")
	{
		GotoDlgCtrl(&m_Edit2);
	}
	else
	{
		GotoDlgCtrl(&m_Edit1);
	}

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CToolBarRunDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(true,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
			"����(*.exe)|*.exe|�����ļ�(*.*)|*.*||");
	if(m_bIsRun)
		dlg.m_ofn.lpstrTitle="��ѡ��Ҫ���еĳ���...";
	else
		dlg.m_ofn.lpstrTitle="��ѡ�����ڴ򿪴��ļ��ĳ���...";
	CString strInitalDir=m_strPathName.Mid(0,m_strPathName.ReverseFind('\\')+1);
	dlg.m_ofn.lpstrInitialDir=strInitalDir;
	if(dlg.DoModal()==IDOK) 
	{
		m_Edit1.SetWindowText(dlg.GetPathName());
	}				
}

void CToolBarRunDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(true,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		"�����ļ�(*.*)|*.*||");
	if(m_bIsRun)
		dlg.m_ofn.lpstrTitle="��ѡ����Ϊ�������ļ���...";
	else
		dlg.m_ofn.lpstrTitle="��ѡ�񱻴򿪵��ļ�...";
	CString strInitalDir=m_strPathName.Mid(0,m_strPathName.ReverseFind('\\')+1);
	dlg.m_ofn.lpstrInitialDir=strInitalDir;
	if(dlg.DoModal()==IDOK) 
	{
		m_Edit2.SetWindowText(dlg.GetPathName());
	}				
}
