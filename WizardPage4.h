//Download by http://www.NewXing.com
#if !defined(AFX_WIZARDPAGE4_H__DBDFF542_3967_11D5_89D9_289406C10000__INCLUDED_)
#define AFX_WIZARDPAGE4_H__DBDFF542_3967_11D5_89D9_289406C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WizardPage4.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizardPage4 dialog

class CWizardPage4 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizardPage4)

// Construction
public:
	CWizardPage4();
	~CWizardPage4();

// Dialog Data
	//{{AFX_DATA(CWizardPage4)
	enum { IDD = IDD_WIZARD4 };
	CString	m_strPassword;
	CString	m_strPassword2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardPage4)
	public:
	virtual BOOL OnQueryCancel();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardPage4)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDPAGE4_H__DBDFF542_3967_11D5_89D9_289406C10000__INCLUDED_)
