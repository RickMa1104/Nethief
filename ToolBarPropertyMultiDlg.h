//Download by http://www.NewXing.com
#if !defined(AFX_TOOLBARPROPERTYMULTIDLG_H__4D382321_CC58_11D4_89D9_685604C10000__INCLUDED_)
#define AFX_TOOLBARPROPERTYMULTIDLG_H__4D382321_CC58_11D4_89D9_685604C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ToolBarPropertyMultiDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarPropertyMultiDlg dialog

class CToolBarPropertyMultiDlg : public CDialog
{
// Construction
public:
	CToolBarPropertyMultiDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolBarPropertyMultiDlg)
	enum { IDD = IDD_TOOLBARPROPERTY_MULTI };
	CString	m_strSize;
	CString	m_strStatistic;
	CString	m_strHostName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarPropertyMultiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolBarPropertyMultiDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARPROPERTYMULTIDLG_H__4D382321_CC58_11D4_89D9_685604C10000__INCLUDED_)
