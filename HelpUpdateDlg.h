//Download by http://www.NewXing.com
#if !defined(AFX_HELPUPDATEDLG_H__8CE256A2_F6A4_11D4_89D9_645604C10000__INCLUDED_)
#define AFX_HELPUPDATEDLG_H__8CE256A2_F6A4_11D4_89D9_645604C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HelpUpdateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHelpUpdateDlg dialog

class CHelpUpdateDlg : public CDialog
{
// Construction
public:
	CHelpUpdateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHelpUpdateDlg)
	enum { IDD = IDD_HELPUPDATE };
	CButton	m_Button1;
	CString	m_strDate;
	CString	m_strVersion;
	CString	m_strNewDate;
	CString	m_strNewVersion;
	CString	m_strNote;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpUpdateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHelpUpdateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPUPDATEDLG_H__8CE256A2_F6A4_11D4_89D9_645604C10000__INCLUDED_)
