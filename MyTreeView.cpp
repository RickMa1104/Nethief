// MyTreeView.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Nethief.h"
#include "MainFrm.h"

#include "MyTreeView.h"
#include "NethiefDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTreeView

IMPLEMENT_DYNCREATE(CMyTreeView, CTreeView)

CMyTreeView::CMyTreeView()
{
}

CMyTreeView::~CMyTreeView()
{
}

BEGIN_MESSAGE_MAP(CMyTreeView, CTreeView)
	//{{AFX_MSG_MAP(CMyTreeView)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTreeView drawing

void CMyTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyTreeView diagnostics

#ifdef _DEBUG
void CMyTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CMyTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyTreeView message handlers
void CMyTreeView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	m_pDoc=(CNethiefDoc*)GetDocument();
	m_pDoc->OnTreeInitalUpdate();
}

BOOL CMyTreeView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style|=TVS_HASLINES|TVS_HASBUTTONS|TVS_EDITLABELS;

	return CTreeView::PreCreateWindow(cs);
}

//*****************************************************************
void CMyTreeView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_pDoc->OnTreeItemexpanding(pNMTreeView,pResult);
}

void CMyTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_pDoc->OnTreeSelchanged(pNMTreeView,pResult);	
}

void CMyTreeView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_pDoc->OnTreeBeginlabeledit(pTVDispInfo,pResult);
}

void CMyTreeView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_pDoc->OnTreeEndlabeledit(pTVDispInfo,pResult);
}
//*****************************************************************
void CMyTreeView::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UINT nChar=pTVKeyDown->wVKey;
	switch(nChar)
	{
		case VK_TAB:
		{
			CMainFrame* Frame;
			Frame=(CMainFrame*)::AfxGetMainWnd();
			Frame->m_wndSplitter.SetActivePane(0,1);//¼¤»îListView
			*pResult = 1;
			break;
		}
		case 0x58://Ctrl+X
		{
			int nReturn=GetAsyncKeyState(VK_CONTROL);
			if(!(nReturn==0||nReturn==1))
			{
				m_pDoc->OnToolbarCut(TRUE);
				*pResult = 1;
			}
			break;
		}
		case 0x43://Ctrl+C
		{
			int nReturn=GetAsyncKeyState(VK_CONTROL);
			if(!(nReturn==0||nReturn==1))
			{
				m_pDoc->OnToolbarCopy(TRUE);
				*pResult = 1;
			}
			break;
		}
		case 0x56://Ctrl+V
		{
			int nReturn=GetAsyncKeyState(VK_CONTROL);
			if(!(nReturn==0||nReturn==1))
			{
				m_pDoc->OnToolbarPaste();
				*pResult = 1;
			}
			break;
		}
	}	
}
