//Download by http://www.NewXing.com
#if !defined(AFX_WIZARDPAGE3_H__5E4F9E24_16C4_4AB3_8C77_864AFCE3887D__INCLUDED_)
#define AFX_WIZARDPAGE3_H__5E4F9E24_16C4_4AB3_8C77_864AFCE3887D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardPage3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizardPage3 dialog

class CWizardPage3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizardPage3)

// Construction
public:
	CWizardPage3();
	~CWizardPage3();

// Dialog Data
	//{{AFX_DATA(CWizardPage3)
	enum { IDD = IDD_WIZARD3 };
	CString	m_strPath;
	CString	m_strFilename1;
	CString	m_strFilename2;
	CString	m_strFilename3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardPage3)
	public:
	virtual BOOL OnQueryCancel();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardPage3)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDPAGE3_H__5E4F9E24_16C4_4AB3_8C77_864AFCE3887D__INCLUDED_)
