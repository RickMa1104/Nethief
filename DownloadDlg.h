//Download by http://www.NewXing.com
#if !defined(AFX_DOWNLOADDLG_H__76B17D0F_7234_4C9D_8E1A_FEBD9B738B6F__INCLUDED_)
#define AFX_DOWNLOADDLG_H__76B17D0F_7234_4C9D_8E1A_FEBD9B738B6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownloadDlg dialog

class CDownloadDlg : public CDialog
{
// Construction
public:
	CDownloadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDownloadDlg)
	enum { IDD = IDD_DOWNLOAD };
	CEdit	m_Edit_Filename;
	CString	m_strURL;
	CString	m_strFilename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownloadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDownloadDlg)
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADDLG_H__76B17D0F_7234_4C9D_8E1A_FEBD9B738B6F__INCLUDED_)
