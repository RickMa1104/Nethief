//Download by http://www.NewXing.com
#if !defined(AFX_REMARKDLG_H__EDD579EF_BA06_4BA5_940E_19C17242B386__INCLUDED_)
#define AFX_REMARKDLG_H__EDD579EF_BA06_4BA5_940E_19C17242B386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemarkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRemarkDlg dialog

class CRemarkDlg : public CDialog
{
// Construction
public:
	CRemarkDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRemarkDlg)
	enum { IDD = IDD_REMARK };
	CString	m_strRemark;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemarkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRemarkDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMARKDLG_H__EDD579EF_BA06_4BA5_940E_19C17242B386__INCLUDED_)
