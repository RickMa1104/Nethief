//Download by http://www.NewXing.com
#if !defined(AFX_TOOLBARPROPERTYSINGLEDLG_H__3FE6F841_CBA9_11D4_89D9_685604C10000__INCLUDED_)
#define AFX_TOOLBARPROPERTYSINGLEDLG_H__3FE6F841_CBA9_11D4_89D9_685604C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ToolBarPropertySingleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarPropertySingleDlg dialog

class CToolBarPropertySingleDlg : public CDialog
{
// Construction
public:
	CToolBarPropertySingleDlg(CWnd* pParent = NULL);   // standard constructor

	HICON m_hIcon;

// Dialog Data
	//{{AFX_DATA(CToolBarPropertySingleDlg)
	enum { IDD = IDD_TOOLBARPROPERTY_SINGLE };
	CStatic	m_Static_Icon;
	CString	m_Edit_strFilename;
	CString	m_strFileSize;
	CString	m_strDataType;
	CString	m_strATime;
	CString	m_strCTime;
	CString	m_strMTime;
	BOOL	m_bIsArchive;
	BOOL	m_bIsHide;
	BOOL	m_bIsReadOnly;
	BOOL	m_bIsSystem;
	CString	m_strHostName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarPropertySingleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolBarPropertySingleDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARPROPERTYSINGLEDLG_H__3FE6F841_CBA9_11D4_89D9_685604C10000__INCLUDED_)
