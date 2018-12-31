//Download by http://www.NewXing.com
#if !defined(AFX_WIZARDPAGE31_H__1EB34E62_7316_4B75_A74F_4332143BA4F6__INCLUDED_)
#define AFX_WIZARDPAGE31_H__1EB34E62_7316_4B75_A74F_4332143BA4F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardPage31.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizardPage31 dialog

class CWizardPage31 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizardPage31)

// Construction
public:
	CWizardPage31();
	~CWizardPage31();

// Dialog Data
	//{{AFX_DATA(CWizardPage31)
	enum { IDD = IDD_WIZARD31 };
	CString	m_strFilename4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardPage31)
	public:
	virtual BOOL OnQueryCancel();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardPage31)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDPAGE31_H__1EB34E62_7316_4B75_A74F_4332143BA4F6__INCLUDED_)
