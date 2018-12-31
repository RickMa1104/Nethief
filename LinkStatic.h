//Download by http://www.NewXing.com
#if !defined(AFX_LINKSTATIC_H__A4F7B722_F619_11D4_89D9_645604C10000__INCLUDED_)
#define AFX_LINKSTATIC_H__A4F7B722_F619_11D4_89D9_645604C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LinkStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLinkStatic window

class CLinkStatic : public CStatic
{
// Construction
public:
	CLinkStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinkStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLinkStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLinkStatic)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKSTATIC_H__A4F7B722_F619_11D4_89D9_645604C10000__INCLUDED_)
