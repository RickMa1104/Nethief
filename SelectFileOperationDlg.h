//Download by http://www.NewXing.com
#if !defined(AFX_SELECTFILEOPERATIONDLG_H__B2DEFFA6_08DB_4BAD_8EDC_51A886C740AD__INCLUDED_)
#define AFX_SELECTFILEOPERATIONDLG_H__B2DEFFA6_08DB_4BAD_8EDC_51A886C740AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectFileOperationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectFileOperationDlg dialog

class CSelectFileOperationDlg : public CDialog
{
// Construction
public:
	CSelectFileOperationDlg(CWnd* pParent = NULL);   // standard constructor

	HICON m_hSourceIcon;	//源文件图标句柄
	HICON m_hDestIcon;		//目的文件图标句柄
	BOOL m_bDoNotContinue;	//是否不允许断点续传

// Dialog Data
	//{{AFX_DATA(CSelectFileOperationDlg)
	enum { IDD = IDD_SELECTFILEOPERATION };
	CButton	m_Button_Continue;
	CStatic	m_Static_DestIcon;
	CStatic	m_Static_SourceIcon;
	CString	m_strSourceFileInfo;
	CString	m_strDestFileInfo;
	CString	m_strFilename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectFileOperationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectFileOperationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOverlay();
	afx_msg void OnButtonContinue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTFILEOPERATIONDLG_H__B2DEFFA6_08DB_4BAD_8EDC_51A886C740AD__INCLUDED_)
