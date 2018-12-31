//Download by http://www.NewXing.com
#if !defined(AFX_EASTEREGGDLG_H__48694404_7AC2_11D5_89D9_D49406C10000__INCLUDED_)
#define AFX_EASTEREGGDLG_H__48694404_7AC2_11D5_89D9_D49406C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EasterEggDlg.h : header file
//
#include "ColorStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CEasterEggDlg dialog

class CEasterEggDlg : public CDialog
{
// Construction
public:
	CEasterEggDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEasterEggDlg)
	enum { IDD = IDD_EASTEREGG };
	CColorStatic	m_Static;
	CString	m_strStatic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEasterEggDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEasterEggDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASTEREGGDLG_H__48694404_7AC2_11D5_89D9_D49406C10000__INCLUDED_)
