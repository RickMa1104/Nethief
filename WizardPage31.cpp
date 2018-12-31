//Download by http://www.NewXing.com
// WizardPage31.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "WizardPage31.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardPage31 property page

IMPLEMENT_DYNCREATE(CWizardPage31, CPropertyPage)

CWizardPage31::CWizardPage31() : CPropertyPage(CWizardPage31::IDD)
{
	//{{AFX_DATA_INIT(CWizardPage31)
	m_strFilename4 = _T("pic4.bmp");
	//}}AFX_DATA_INIT
}

CWizardPage31::~CWizardPage31()
{
}

void CWizardPage31::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPage31)
	DDX_Text(pDX, IDC_EDIT_FILENAME4, m_strFilename4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardPage31, CPropertyPage)
	//{{AFX_MSG_MAP(CWizardPage31)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardPage31 message handlers

BOOL CWizardPage31::OnQueryCancel() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(MessageBox("网络神偷设置向导未完成设置过程。\r\n\r\n是否想退出本向导？",
		"网络神偷 设置向导",MB_ICONQUESTION|MB_YESNO)==IDNO)
		return FALSE;
	
	return CPropertyPage::OnQueryCancel();
}

LRESULT CWizardPage31::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CString strText="";
	if(m_strFilename4=="")
		strText="请填写“主页空间 临时文件4 的文件名”。";
	if(strText!="")
	{
		MessageBox(strText,"网络神偷 设置向导",MB_ICONWARNING|MB_OK);
		return -1;
	}
	m_strFilename4.TrimLeft();
	m_strFilename4.TrimRight();

	CCommunity* pCommunity=((CNethiefApp*)AfxGetApp())->pCommunity;
	pCommunity->SetFtpServerInfo(2,
		"","","","",
		"","","","",
		m_strFilename4,
		"");
	
	return CPropertyPage::OnWizardNext();
}
