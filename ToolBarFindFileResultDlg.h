//Download by http://www.NewXing.com
#if !defined(AFX_TOOLBARFINDFILERESULTDLG_H__706F0FE4_6EF4_11D5_89D9_F09906C10000__INCLUDED_)
#define AFX_TOOLBARFINDFILERESULTDLG_H__706F0FE4_6EF4_11D5_89D9_F09906C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ToolBarFindFileResultDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarFindFileResultDlg dialog

class CToolBarFindFileResultDlg : public CDialog
{
// Construction
public:
	CToolBarFindFileResultDlg(CWnd* pParent = NULL);   // standard constructor

	CStringArray m_StringArray;

// Dialog Data
	//{{AFX_DATA(CToolBarFindFileResultDlg)
	enum { IDD = IDD_TOOLBARFINDFILE_RESULT };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarFindFileResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolBarFindFileResultDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARFINDFILERESULTDLG_H__706F0FE4_6EF4_11D5_89D9_F09906C10000__INCLUDED_)
