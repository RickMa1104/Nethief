//Download by http://www.NewXing.com
#if !defined(AFX_WIZARDPAGE2_H__381C3282_EECF_11D4_89D9_0C4205C10000__INCLUDED_)
#define AFX_WIZARDPAGE2_H__381C3282_EECF_11D4_89D9_0C4205C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WizardPage2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizardPage2 dialog

class CWizardPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizardPage2)

// Construction
public:
	CWizardPage2();
	~CWizardPage2();

// Dialog Data
	//{{AFX_DATA(CWizardPage2)
	enum { IDD = IDD_WIZARD2 };
	CString	m_strAddress;
	CString	m_strPassword;
	CString	m_strUsername;
	CString	m_strHomepage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardPage2)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnQueryCancel();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardPage2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDPAGE2_H__381C3282_EECF_11D4_89D9_0C4205C10000__INCLUDED_)
