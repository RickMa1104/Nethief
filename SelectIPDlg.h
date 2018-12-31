//Download by http://www.NewXing.com
#if !defined(AFX_SELECTIPDLG_H__BB20086B_F112_407F_95B6_0E9721EE621A__INCLUDED_)
#define AFX_SELECTIPDLG_H__BB20086B_F112_407F_95B6_0E9721EE621A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectIPDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectIPDlg dialog

class CSelectIPDlg : public CDialog
{
// Construction
public:
	CSelectIPDlg(CWnd* pParent = NULL);   // standard constructor

	CStringArray m_aryIPAddrList;

// Dialog Data
	//{{AFX_DATA(CSelectIPDlg)
	enum { IDD = IDD_SELECTIP };
	CComboBox	m_Combo;
	int		m_nCombo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectIPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectIPDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTIPDLG_H__BB20086B_F112_407F_95B6_0E9721EE621A__INCLUDED_)
