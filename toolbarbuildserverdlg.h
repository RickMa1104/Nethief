//Download by http://www.NewXing.com
#if !defined(AFX_TOOLBARBUILDSERVERDLG_H__42071D22_E3E7_11D4_89D9_684105C10000__INCLUDED_)
#define AFX_TOOLBARBUILDSERVERDLG_H__42071D22_E3E7_11D4_89D9_684105C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ToolBarBuildServerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarBuildServerDlg dialog

class CToolBarBuildServerDlg : public CDialog
{
// Construction
public:
	CToolBarBuildServerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolBarBuildServerDlg)
	enum { IDD = IDD_TOOLBARBUILDSERVER };
	CEdit	m_Edit_ServerFilename;
	CString	m_strServerFilename;
	CString	m_strKeyName;
	CString	m_strFilename;
	CString	m_strNote;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarBuildServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolBarBuildServerDlg)
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonBuild();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARBUILDSERVERDLG_H__42071D22_E3E7_11D4_89D9_684105C10000__INCLUDED_)
