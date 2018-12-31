//Download by http://www.NewXing.com
#if !defined(AFX_MYSPLITTERWND_H__5F09CB24_AF0D_11D4_89D9_2C4705C10000__INCLUDED_)
#define AFX_MYSPLITTERWND_H__5F09CB24_AF0D_11D4_89D9_2C4705C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MySplitterWnd.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CMySplitterWnd frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMySplitterWnd : public CSplitterWnd//改成派生于CSplitterWnd
{
	DECLARE_DYNCREATE(CMySplitterWnd)
public:	//此处要改成public的
	CMySplitterWnd();           // protected constructor used by dynamic creation

// Attributes
protected:
	CSplitterWnd    m_wndSplitter;
public:
	int				m_nStartup;
	BOOL			m_bIsDragSplitterBar;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySplitterWnd)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL IsAllowDrag(CPoint point);
	virtual ~CMySplitterWnd();

	// Generated message map functions
	//{{AFX_MSG(CMySplitterWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSPLITTERWND_H__5F09CB24_AF0D_11D4_89D9_2C4705C10000__INCLUDED_)
