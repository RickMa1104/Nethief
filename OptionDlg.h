//Download by http://www.NewXing.com
#if !defined(AFX_OPTIONDLG_H__593FF7A1_6FF6_11D5_89D9_F09906C10000__INCLUDED_)
#define AFX_OPTIONDLG_H__593FF7A1_6FF6_11D5_89D9_F09906C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OptionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog

class COptionDlg : public CDialog
{
// Construction
public:
	COptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionDlg)
	enum { IDD = IDD_OPTION };
	CEdit	m_Edit_ViewerFilename;
	CString	m_strViewerFilename;
	BOOL	m_bSelectIP;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionDlg)
	afx_msg void OnButtonBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONDLG_H__593FF7A1_6FF6_11D5_89D9_F09906C10000__INCLUDED_)
