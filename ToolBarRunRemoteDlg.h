//Download by http://www.NewXing.com
#if !defined(AFX_TOOLBARRUNREMOTEDLG_H__5BF022E4_5F34_11D5_89D9_C49406C10000__INCLUDED_)
#define AFX_TOOLBARRUNREMOTEDLG_H__5BF022E4_5F34_11D5_89D9_C49406C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ToolBarRunRemoteDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarRunRemoteDlg dialog

class CToolBarRunRemoteDlg : public CDialog
{
// Construction
public:
	CToolBarRunRemoteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolBarRunRemoteDlg)
	enum { IDD = IDD_TOOLBARRUN_REMOTE };
	int		m_nComboBox;
	CString	m_strEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarRunRemoteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolBarRunRemoteDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARRUNREMOTEDLG_H__5BF022E4_5F34_11D5_89D9_C49406C10000__INCLUDED_)
