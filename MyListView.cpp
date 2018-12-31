// MyListView.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Nethief.h"
#include "MainFrm.h"

#include "MyListView.h"
#include "NethiefDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListView

IMPLEMENT_DYNCREATE(CMyListView, CListView)

CMyListView::CMyListView()
{
}

CMyListView::~CMyListView()
{
}

BEGIN_MESSAGE_MAP(CMyListView, CListView)
	//{{AFX_MSG_MAP(CMyListView)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListView drawing

void CMyListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyListView diagnostics

#ifdef _DEBUG
void CMyListView::AssertValid() const
{
	CListView::AssertValid();
}

void CMyListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyListView message handlers

void CMyListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	m_pDoc=(CNethiefDoc*)GetDocument();
	m_pDoc->OnListInitalUpdate();
}

BOOL CMyListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style|=LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_EDITLABELS|LVS_SHOWSELALWAYS;

	return CListView::PreCreateWindow(cs);
}

void CMyListView::OnDestroy() 
{
	// TODO: Add your message handler code here
	m_pDoc->OnListDestroy();

	CListView::OnDestroy();	
}
//*****************************************************************
void CMyListView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_pDoc->OnListEndlabeledit(pDispInfo,pResult);
}

void CMyListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_pDoc->OnListDblclk(pNMListView,pResult);
}
//*****************************************************************
void CMyListView::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UINT nChar=pLVKeyDow->wVKey;
	switch(nChar)
	{
		case VK_RETURN://按下Enter
		{
			m_pDoc->OnMenuFileRun();
			break;
		}
		case VK_BACK://按下了Backspace
		{
			m_pDoc->OnToolbarBack();//调用TreeView的响应函数	
			break;
		}
		case VK_TAB:
		{
			CMainFrame* Frame;
			Frame=(CMainFrame*)::AfxGetMainWnd();
			Frame->m_wndSplitter.SetActivePane(0,0);//激活TreeView
			break;
		}
		case 0x58://Ctrl+X
		{
			if(GetAsyncKeyState(VK_CONTROL)==-32767)
				m_pDoc->OnToolbarCut(FALSE);
			break;
		}
		case 0x43://Ctrl+C
		{
			if(GetAsyncKeyState(VK_CONTROL)==-32767)
				m_pDoc->OnToolbarCopy(FALSE);
			break;
		}
		case 0x56://Ctrl+V
		{
			if(GetAsyncKeyState(VK_CONTROL)==-32767)
				m_pDoc->OnToolbarPaste();
			break;
		}
	}
}
