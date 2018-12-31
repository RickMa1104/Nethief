//Download by http://www.NewXing.com
// WizardPage1.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "WizardPage1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardPage1 property page

IMPLEMENT_DYNCREATE(CWizardPage1, CPropertyPage)

CWizardPage1::CWizardPage1() : CPropertyPage(CWizardPage1::IDD)
{
	//{{AFX_DATA_INIT(CWizardPage1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWizardPage1::~CWizardPage1()
{
}

void CWizardPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPage1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardPage1, CPropertyPage)
	//{{AFX_MSG_MAP(CWizardPage1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardPage1 message handlers

BOOL CWizardPage1::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet* pParent=(CPropertySheet*)GetParent();
	pParent->SetWizardButtons(PSWIZB_NEXT);
	
	return CPropertyPage::OnSetActive();
}

BOOL CWizardPage1::OnQueryCancel() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(MessageBox("网络神偷设置向导未完成设置过程。\r\n\r\n是否想退出本向导？",
		"网络神偷 设置向导",MB_ICONQUESTION|MB_YESNO)==IDNO)
		return FALSE;
	
	return CPropertyPage::OnQueryCancel();
}
