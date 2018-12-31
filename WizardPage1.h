//Download by http://www.NewXing.com
#if !defined(AFX_WIZARDPAGE1_H__381C3281_EECF_11D4_89D9_0C4205C10000__INCLUDED_)
#define AFX_WIZARDPAGE1_H__381C3281_EECF_11D4_89D9_0C4205C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WizardPage1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizardPage1 dialog

class CWizardPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizardPage1)

// Construction
public:
	CWizardPage1();
	~CWizardPage1();

// Dialog Data
	//{{AFX_DATA(CWizardPage1)
	enum { IDD = IDD_WIZARD1 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardPage1)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnQueryCancel();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardPage1)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDPAGE1_H__381C3281_EECF_11D4_89D9_0C4205C10000__INCLUDED_)
