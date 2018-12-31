//Download by http://www.NewXing.com
#if !defined(AFX_TOOLBARPROPERTYDISKDRIVEDLG_H__8C84CC24_CD09_11D4_89D9_685604C10000__INCLUDED_)
#define AFX_TOOLBARPROPERTYDISKDRIVEDLG_H__8C84CC24_CD09_11D4_89D9_685604C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ToolBarPropertyDiskdriveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarPropertyDiskdriveDlg dialog

class CToolBarPropertyDiskdriveDlg : public CDialog
{
// Construction
public:
	CToolBarPropertyDiskdriveDlg(CWnd* pParent = NULL);   // standard constructor

	HICON m_hIcon;

// Dialog Data
	//{{AFX_DATA(CToolBarPropertyDiskdriveDlg)
	enum { IDD = IDD_TOOLBARPROPERTY_DISKDRIVE };
	CStatic	m_Static_Icon;
	CString	m_strName;
	CString	m_strSerialNumber;
	CString	m_strTotal;
	CString	m_strUnused;
	CString	m_strUnusedPercent;
	CString	m_strUsed;
	CString	m_strUsedPercent;
	CString	m_strType;
	CString	m_strHostName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarPropertyDiskdriveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolBarPropertyDiskdriveDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARPROPERTYDISKDRIVEDLG_H__8C84CC24_CD09_11D4_89D9_685604C10000__INCLUDED_)
