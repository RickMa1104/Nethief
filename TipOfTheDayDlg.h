//Download by http://www.NewXing.com
#if !defined(AFX_TIPOFTHEDAYDLG_H__F18CFFC5_B00D_11D4_89D9_645604C10000__INCLUDED_)
#define AFX_TIPOFTHEDAYDLG_H__F18CFFC5_B00D_11D4_89D9_645604C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TipOfTheDayDlg.h : header file
//
#include "ColorStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CTipOfTheDayDlg dialog

class CTipOfTheDayDlg : public CDialog
{
// Construction
public:
	CTipOfTheDayDlg(CWnd* pParent = NULL);   // standard constructor
	//*****************************************************************
	void ShowTips();
	int GetINIfileInt(LPCTSTR lpSectionName,  //从INI中读取整型值
					  LPCTSTR lpKeyName);
	BOOL WriteINIfileInt(LPCTSTR lpSectionName, //向INI中写入整型值
						 LPCTSTR lpKeyName,
						 INT nValue);
	CString GetINIfileString(LPCTSTR lpSectionName, //从INI中读取字符值
							 LPCTSTR lpKeyName);
	//*****************************************************************

// Dialog Data
	//{{AFX_DATA(CTipOfTheDayDlg)
	enum { IDD = IDD_TIPOFTHEDAY };
	CColorStatic	m_Static_Light;
	CColorStatic	m_Static_Tips;
	CColorStatic	m_Static_Doyouknow;
	CColorStatic	m_Static_Background;
	CButton	m_Check;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipOfTheDayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTipOfTheDayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonReadme();
	afx_msg void OnButtonHomepage();
	afx_msg void OnButtonEmail();
	afx_msg void OnButtonTips();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPOFTHEDAYDLG_H__F18CFFC5_B00D_11D4_89D9_645604C10000__INCLUDED_)
