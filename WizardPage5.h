//Download by http://www.NewXing.com
#if !defined(AFX_WIZARDPAGE5_H__DBDFF543_3967_11D5_89D9_289406C10000__INCLUDED_)
#define AFX_WIZARDPAGE5_H__DBDFF543_3967_11D5_89D9_289406C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WizardPage5.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizardPage5 dialog
//*************************************
#pragma pack(1) //字节对齐

typedef struct
{
	CCommunity*	pCommunity;
	CEdit*		pEdit_Test;
	BOOL*		pbTestSucceed;
}
HOMEPAGE_TEST_THREAD_PARAM,*pHOMEPAGE_TEST_THREAD_PARAM;

#pragma pack()	//使用缺省对齐
//*************************************
UINT HomepageTestProc(LPVOID pParam);


class CWizardPage5 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizardPage5)

// Construction
public:
	CWizardPage5();
	~CWizardPage5();

	CWinThread* m_pHomepageTestThread;
	BOOL		m_bTestSucceed;

// Dialog Data
	//{{AFX_DATA(CWizardPage5)
	enum { IDD = IDD_WIZARD5 };
	CEdit	m_Edit_Test;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardPage5)
	public:
	virtual BOOL OnQueryCancel();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardPage5)
	afx_msg void OnButtonTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDPAGE5_H__DBDFF543_3967_11D5_89D9_289406C10000__INCLUDED_)
