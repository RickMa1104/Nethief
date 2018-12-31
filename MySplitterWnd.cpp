// MySplitterWnd.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Nethief.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySplitterWnd

IMPLEMENT_DYNCREATE(CMySplitterWnd, CSplitterWnd)

CMySplitterWnd::CMySplitterWnd()
{
	m_nStartup=0;
	m_bIsDragSplitterBar=FALSE;
}

CMySplitterWnd::~CMySplitterWnd()
{
}

BOOL CMySplitterWnd::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	return m_wndSplitter.Create(this,
		2, 2,       // TODO: adjust the number of rows, columns
		CSize(10, 10),  // TODO: adjust the minimum pane size
		pContext);
}


BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CMySplitterWnd)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySplitterWnd message handlers

void CMySplitterWnd::OnSize(UINT nType, int cx, int cy) 
{
	// TODO: Add your message handler code here
	if(m_nStartup<2)
	{
		m_nStartup++;
		CSplitterWnd::OnSize(nType, cx, cy);
		return;
	}
	CSplitterWnd::OnSize(nType, cx, cy);
	if(nType==SIZE_MAXIMIZED||nType==SIZE_RESTORED)
	{
		int cxMin;
		int cxOldColumnPos2;
		GetColumnInfo(1,cxOldColumnPos2,cxMin);
		int cxOldColumnPos3;
		GetColumnInfo(2,cxOldColumnPos3,cxMin);

		CMainFrame* frame=(CMainFrame*)GetParentFrame();
		frame->m_cxOldColumnPos2=cxOldColumnPos2+cxOldColumnPos3;
		if(frame->m_bIsFileManager)
		{
			SetColumnInfo(1,frame->m_cxOldColumnPos2,0);
			RecalcLayout();
		}
	}
}

void CMySplitterWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(IsAllowDrag(point))
	{
		m_bIsDragSplitterBar=TRUE;
		CSplitterWnd::OnLButtonDown(nFlags, point);
	}
	else
		return;
}

void CMySplitterWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nFlags==NULL)
	{
		if(IsAllowDrag(point))
			CSplitterWnd::OnMouseMove(nFlags, point);
		return;
	}
	CSplitterWnd::OnMouseMove(nFlags, point);
}

BOOL CMySplitterWnd::IsAllowDrag(CPoint point)
{
	CMainFrame* frame=(CMainFrame*)GetParentFrame();
	if(frame->m_bIsFileManager)
	{
		int cxMin;
		int cxCurColumnPos1;
		GetColumnInfo(0,cxCurColumnPos1,cxMin);
		int nBound=point.x-cxCurColumnPos1;
		if(nBound>=2&&nBound<=8)
			return TRUE;
	}
	return FALSE;
}

void CMySplitterWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CSplitterWnd::OnLButtonUp(nFlags, point);
	if(m_bIsDragSplitterBar)
	{
		m_bIsDragSplitterBar=FALSE;
		int cxMin;
		int cxOldColumnPos2;
		GetColumnInfo(1,cxOldColumnPos2,cxMin);
		int cxOldColumnPos3;
		GetColumnInfo(2,cxOldColumnPos3,cxMin);

		CMainFrame* frame=(CMainFrame*)GetParentFrame();
		frame->m_cxOldColumnPos2=cxOldColumnPos2+cxOldColumnPos3;
		SetColumnInfo(1,frame->m_cxOldColumnPos2,0);
		RecalcLayout();
	}
}
