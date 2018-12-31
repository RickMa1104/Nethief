//Download by http://www.NewXing.com
#if !defined(AFX_TOOLBARFINDFILEDLG_H__6C0A0421_C5E7_11D4_89D9_685604C10000__INCLUDED_)
#define AFX_TOOLBARFINDFILEDLG_H__6C0A0421_C5E7_11D4_89D9_685604C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ToolBarFindFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarFindFileDlg dialog

class CToolBarFindFileDlg : public CDialog
{
// Construction
public:
	CToolBarFindFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolBarFindFileDlg)
	enum { IDD = IDD_TOOLBARFINDFILE };
	BOOL	m_bCheck;
	CString	m_strEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarFindFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolBarFindFileDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARFINDFILEDLG_H__6C0A0421_C5E7_11D4_89D9_685604C10000__INCLUDED_)
