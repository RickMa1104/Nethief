//Download by http://www.NewXing.com
#if !defined(AFX_TOOLBARRUNDLG_H__2AA64361_C464_11D4_89D9_D85504C10000__INCLUDED_)
#define AFX_TOOLBARRUNDLG_H__2AA64361_C464_11D4_89D9_D85504C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ToolBarRunDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarRunDlg dialog

class CToolBarRunDlg : public CDialog
{
// Construction
public:
	CToolBarRunDlg(CWnd* pParent = NULL);   // standard constructor
	
	CString m_strPathName;
	BOOL	m_bIsRun;
// Dialog Data
	//{{AFX_DATA(CToolBarRunDlg)
	enum { IDD = IDD_TOOLBARRUN };
	CEdit	m_Edit2;
	CEdit	m_Edit1;
	CString	m_strEdit1;
	CString	m_strEdit2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarRunDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolBarRunDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARRUNDLG_H__2AA64361_C464_11D4_89D9_D85504C10000__INCLUDED_)
