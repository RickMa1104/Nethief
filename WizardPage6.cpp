//Download by http://www.NewXing.com
// WizardPage6.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "WizardPage6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardPage6 property page

IMPLEMENT_DYNCREATE(CWizardPage6, CPropertyPage)

CWizardPage6::CWizardPage6() : CPropertyPage(CWizardPage6::IDD)
{
	//{{AFX_DATA_INIT(CWizardPage6)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWizardPage6::~CWizardPage6()
{
}

void CWizardPage6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPage6)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardPage6, CPropertyPage)
	//{{AFX_MSG_MAP(CWizardPage6)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardPage6 message handlers

BOOL CWizardPage6::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet* pParent=(CPropertySheet*)GetParent();
	pParent->SetWizardButtons(PSWIZB_FINISH);
	CancelToClose();
	
	return CPropertyPage::OnSetActive();
}
