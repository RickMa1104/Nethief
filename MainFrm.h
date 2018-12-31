//Download by http://www.NewXing.com
// MainFrm.h : interface of the CMainFrame class
//
#include "MySplitterWnd.h"

#include "MyTreeView.h"
#include "MyListView.h"
#include "MyFormView.h"
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_MAINFRM_H__2DA6D109_AC2D_11D4_89D9_2C4705C10000__INCLUDED_)
#define AFX_MAINFRM_H__2DA6D109_AC2D_11D4_89D9_2C4705C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	//*****************************************************************
	////////////////////////成员变量/////////////////////////////
	CMySplitterWnd	m_wndSplitter;	//分割窗口
	CStatusBar		m_wndStatusBar;	//状态栏

	CToolBar		m_wndConsoleToolBar;		//控制台工具栏
	CImageList		m_ConsoleToolBarImageList;
	CImageList		m_ConsoleToolBarHotImageList;
	CToolBarCtrl*	m_pWndConsoleToolBarCtrl;	//控制台工具栏控件

	CToolBar		m_wndFileToolBar;	//文件操作工具栏
	CImageList		m_FileToolBarImageList;
	CImageList		m_FileToolBarHotImageList;

	CToolBar		m_wndNetToolBar;	//网络操作工具栏
	CImageList		m_NetToolBarImageList;
	CImageList		m_NetToolBarHotImageList;

	CNethiefDoc*	m_pDoc;			//文档类
	CMyTreeView*	m_pTreeView;	//TreeView类
	CMyListView*	m_pListView;	//ListView类
	CMyFormView*	m_pFormView;	//FormView类

	HCURSOR			m_hCursor;
	/////////////////////////////////////////////////
	int				m_cxOldColumnPos1;	//第一分割栏当前位置
	int				m_cxOldColumnPos2;	//第二分割栏当前位置
	
	BOOL			m_bIsFileManager;	//窗口状态,是显示文件管理器还是IP地址通知器
	int				m_nStartup;			//是启动时产生的WM_SIZE
	BOOL			m_bIsHideIcon;		//隐藏任务栏图标

	////////////////////////成员函数/////////////////////////////
	BOOL IsActivePane(int nRow,int nCol);//窗格是否活动
	void SetActivePane(int nRow,int nCol);//激活窗格
	/////////////////////////////////////////////////
	int GetINIfileInt(LPCTSTR lpSectionName,  //从INI中读取整型值
					  LPCTSTR lpKeyName);
	BOOL WriteINIfileInt(LPCTSTR lpSectionName, //向INI中写入整型值
						 LPCTSTR lpKeyName,
						 INT nValue);
	//*****************************************************************
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnHelpTipoftheday();
	afx_msg void OnToolbarFilemanager();
	afx_msg void OnToolbarCommunity();
	afx_msg void OnToolbarView();
	afx_msg void OnToolbarRefresh();
	afx_msg void OnToolbarPaste();
	afx_msg void OnToolbarNewfolder();
	afx_msg void OnToolbarFindfile();
	afx_msg void OnToolbarDelete();
	afx_msg void OnToolbarCut();
	afx_msg void OnToolbarCopy();
	afx_msg void OnToolbarBack();
	afx_msg void OnToolbarNewfile();
	afx_msg void OnToolbarRun();
	afx_msg void OnMenuFileRun();
	afx_msg void OnMenuFileRename();
	afx_msg void OnMenuConsoleTab();
	afx_msg void OnUpdateIndicatorProgress(CCmdUI *pCmdUI);
	afx_msg void OnToolbarSelectall();
	afx_msg void OnToolbarReverseselect();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSystray(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSystraymenuWindow();
	afx_msg void OnHotKey(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMenuConsoleLogon();
	afx_msg void OnUpdateSystraymenuHide(CCmdUI* pCmdUI);
	afx_msg void OnSystraymenuHide();
	afx_msg void OnToolbarDeletehost();
	afx_msg void OnToolbarBuildserver();
	afx_msg void OnHelpReadme();
	afx_msg void OnHelpHomepage();
	afx_msg void OnHelpEmail();
	afx_msg void OnHelpUpdate();
	afx_msg void OnToolbarDisconnect();
	afx_msg void OnToolbarStop();
	afx_msg void OnMenuNetOption();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnWorkThread(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSocket(WPARAM wParam,LPARAM lParam);
	afx_msg void OnToolbarProperty();
	afx_msg void OnMenuNetUninstall();
	afx_msg void OnMenuNetNote();
	afx_msg void OnMenuNetDownload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__2DA6D109_AC2D_11D4_89D9_2C4705C10000__INCLUDED_)
