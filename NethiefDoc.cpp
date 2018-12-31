//Download by http://www.NewXing.com
// NethiefDoc.cpp : implementation of the CNethiefDoc class
//

#include "stdafx.h"
#include "Nethief.h"
#include "NethiefDoc.h"

#include "ToolBarFindFileDlg.h"
#include "ToolBarFindFileResultDlg.h"
#include "ToolBarRunDlg.h"
#include "ToolBarPropertySingleDlg.h"
#include "ToolBarPropertyMultiDlg.h"
#include "ToolBarPropertyDiskdriveDlg.h"
#include "ToolBarBuildServerDlg.h"
#include "HelpUpdateDlg.h"
#include "ToolBarRunRemoteDlg.h"
#include "OptionDlg.h"
#include "SelectFileOperationDlg.h"
#include "DownloadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNethiefDoc
IMPLEMENT_DYNCREATE(CNethiefDoc, CDocument)

BEGIN_MESSAGE_MAP(CNethiefDoc, CDocument)
	//{{AFX_MSG_MAP(CNethiefDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CNethiefDoc construction/destruction
CNethiefDoc::CNethiefDoc()
{
	// TODO: add one-time construction code here
}

CNethiefDoc::~CNethiefDoc()
{
	CString strFrom=m_strFileCacheDir+"\\*.*";

	int nStrLength=strFrom.GetLength();
	strFrom.GetBufferSetLength(nStrLength+1);
	strFrom.SetAt(nStrLength,'\0');

	SHFILEOPSTRUCT shFileOp;
	ZeroMemory(&shFileOp,sizeof(shFileOp));
	shFileOp.hwnd=NULL;
	shFileOp.wFunc=FO_DELETE;
	shFileOp.pFrom=strFrom;
	shFileOp.fFlags=FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_SILENT;
	SHFileOperation(&shFileOp);
}

BOOL CNethiefDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CNethiefDoc serialization
void CNethiefDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
/////////////////////////////////////////////////////////////////////////////
// CNethiefDoc diagnostics
#ifdef _DEBUG
void CNethiefDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNethiefDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
///////////////////////////////新加入的函数//////////////////////////////////
void CNethiefDoc::OnInitial()
{
	m_ListImageList.Create(16,16,ILC_COLOR8|ILC_MASK,0,1024);

	m_TreeImageList.Create(16,16,ILC_COLOR8|ILC_MASK,13,1);

	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_FILEMANAGER));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_MYCOMPUTER));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_FOLDER));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_FOLDER_OPEN));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_REMOTEHOST));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_REMOTEHOST_DISCONNECT));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_REMOTEHOST_HTTPTUNNEL));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_REMOTEHOST_HTTPTUNNEL_DISCONNECT));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_DRIVE_REMOVABLE));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_DRIVE_FIXED));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_DRIVE_CDROM));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_DRIVE_ZIP));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_DRIVE_REMOTE));
	/////////////////////////////////////////////////////////////////
	m_hIcon_Folder=AfxGetApp()->LoadIcon(IDI_FOLDER);
	m_hIcon_UnknownFile=AfxGetApp()->LoadIcon(IDI_UNKNOWNFILE);
	/////////////////////////////////////////////////////////////////
	SetCurDir("","");
	m_hCutParentItem=NULL;
	m_hConnectHostItem=NULL;
	m_dwFOFlags=NULL;
	m_dwNewClipboardFormat=RegisterClipboardFormat("FileOperationPath");
	m_dwClipboardDataSize=0;
	m_strIndicatorStatus="";
	m_bIsOnline=FALSE;
	m_bLogoning=FALSE;
	m_bFlashIcon=FALSE;
	m_bIsBusy=FALSE;
	m_bLastExpand=FALSE;
	m_bLastSelect=FALSE;
//	m_pCommunity=((CNethiefApp*)AfxGetApp())->pCommunity;
	m_pFileSocket=((CNethiefApp*)AfxGetApp())->pFileSocket;
	m_pFileSocket->Initial(&m_Progress);
	m_pFileSocketThread=NULL;
	m_TM_GetOperationObject_StringArray.SetSize(128,128);

	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('\\'));  //找到
	m_strFileCacheDir=exeFullPath+"\\FileCache";
	/////////////////////////////////////////////////////////////////
	m_strFileViewer=GetINIfileString("Client Setting","FileViewer");
	if(m_strFileViewer=="")
	{
		CString strWindowsDir;
		GetWindowsDirectory(strWindowsDir.GetBufferSetLength(MAX_PATH),MAX_PATH);
		strWindowsDir.ReleaseBuffer();

		m_strFileViewer=strWindowsDir+"\\notepad.exe";
	}
	/////////////////////////////////////////////////////////////////
}

void CNethiefDoc::OnClose()
{
	while(TRUE)//清空列表映象列表
	{
		HICON hIcon=m_ListImageList.ExtractIcon(0);
		if(hIcon==NULL)
			break;
		DestroyIcon(hIcon);
		m_ListImageList.Remove(0);
	}
	m_ListImageList.DeleteImageList();
	////////////////////////////////////////////
	WriteINIfileString("Client Setting","FileViewer",m_strFileViewer);
	//////保存远程主机列表//////////////////////
	int nOldTotal=GetINIfileInt("Remote Host List","Total");

	int nCount=0;
	HTREEITEM hChildItem=m_Tree->GetChildItem(m_Tree->GetRootItem());
	while(hChildItem)
	{
		DWORD dwFlag=m_Tree->GetItemData(hChildItem);
		if(dwFlag&TI_REMOTE)
		{
			CString strKey;
			strKey.Format("Host%d",nCount);
			CString strKey1;
			strKey1.Format("Attribute%d",nCount);

			BOOL bIsHttpTunnel=FALSE;
			int nImage,nSelectedImage;
			m_Tree->GetItemImage(hChildItem,nImage,nSelectedImage);
			if(nImage==ICO_REMOTEHOST_HTTPTUNNEL || nImage==ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT)
			{
				bIsHttpTunnel=TRUE;
			}

			WriteINIfileString("Remote Host List",strKey,GetHostName(hChildItem));
			WriteINIfileInt("Remote Host List",strKey1,bIsHttpTunnel);
			nCount++;
		}
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);
	}
	WriteINIfileInt("Remote Host List","Total",nCount);

	for(int i=nCount;i<nOldTotal;i++)//删除多余的Key
	{
		CString strKey;
		strKey.Format("Host%d",i);
		CString strKey1;
		strKey1.Format("Attribute%d",i);

		WriteINIfileString("Remote Host List",strKey,NULL);
		WriteINIfileString("Remote Host List",strKey1,NULL);
	}
	////////////////////////////////////////////
	NOTIFYICONDATA tnd;
	tnd.cbSize=sizeof(NOTIFYICONDATA);
	tnd.hWnd=AfxGetMainWnd()->m_hWnd;
	tnd.uID=IDR_MAINFRAME;//保证删除的是我们的图标
	Shell_NotifyIcon(NIM_DELETE,&tnd);
	////////////////////////////////////////////
	m_pFileSocket->Close();
	////////////////////////////////////////////
	if(m_pFileSocketThread!=NULL)
		TerminateThread(m_pFileSocketThread->m_hThread,0);
}

void CNethiefDoc::OnTreeInitalUpdate()
{
	m_pFrame=(CMainFrame*)::AfxGetMainWnd();
	m_pTreeView=(CMyTreeView*)m_pFrame->m_wndSplitter.GetPane(0,0);
	m_Tree=&(m_pTreeView->GetTreeCtrl());
	m_pStatusBar=(CStatusBar*)AfxGetMainWnd()->
		GetDescendantWindow(AFX_IDW_STATUS_BAR);
	////////////////////////////////////	//状态栏进度条
	m_Progress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,CRect(0,0,0,0),m_pStatusBar,1);
	LONG WndStyle=GetWindowLong((HWND)m_Progress,GWL_EXSTYLE);
	WndStyle&=~WS_EX_STATICEDGE;
	SetWindowLong((HWND)m_Progress,GWL_EXSTYLE,WndStyle);
	m_Progress.SetRange(0,PROGRESS_RANGE);
	m_Progress.SetStep(1);
	////////////////////////////////////	//状态栏脱机图标
	m_hIcon_Offline=(HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_OFFLINE),IMAGE_ICON,16,16,NULL);
	m_Static_Offline.Create(NULL,WS_VISIBLE|WS_CHILD|SS_ICON|SS_CENTERIMAGE
		,CRect(0,0,0,0),m_pStatusBar);
	////////////////////////////////////	//状态栏沙漏图标
	m_hIcon_Busy=(HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_BUSY),IMAGE_ICON,16,16,NULL);
	m_Static_Busy.Create(NULL,WS_VISIBLE|WS_CHILD|SS_ICON|SS_CENTERIMAGE
		,CRect(0,0,0,0),m_pStatusBar);
	////////////////////////////////////////////////
	NOTIFYICONDATA tnd;//加到任务栏
	tnd.cbSize=sizeof(NOTIFYICONDATA);
	tnd.hWnd=AfxGetMainWnd()->m_hWnd;
	tnd.uID=IDR_MAINFRAME;
	tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
 	tnd.uCallbackMessage=WM_SYSTRAY;
	tnd.hIcon=AfxGetApp()->LoadIcon(IDI_SYSTRAY1);
	strcpy(tnd.szTip,"网络神偷[脱机]");
	Shell_NotifyIcon(NIM_ADD,&tnd);

	SetOfflineSign();//启动后默认为脱机状态
	////////////////////////////////////////////////
	m_Tree->SetImageList(&m_TreeImageList,TVSIL_NORMAL);//设置关联映象列表
	
	HTREEITEM hRootItem=m_Tree->InsertItem("文件管理器",ICO_FILEMANAGER,ICO_FILEMANAGER,TVI_ROOT);
	m_Tree->SetItemData(hRootItem,TI_ROOT);
	HTREEITEM hItem=m_Tree->InsertItem("我的电脑",ICO_MYCOMPUTER,ICO_MYCOMPUTER,hRootItem);
	m_Tree->SetItemData(hItem,TI_HOST);
	m_Tree->Expand(hRootItem,TVE_EXPAND);//展开"文件管理器"节点
	////////////////////////////////////////////////
	AddDiskDriveToTree(hItem);//添加磁盘驱动器到"我的电脑"
	//////读取远程主机列表/////////////////////
	int nTotal=GetINIfileInt("Remote Host List","Total");
	for(int i=0;i<nTotal;i++)
	{
		CString strKey;
		strKey.Format("Host%d",i);
		CString strHostName=GetINIfileString("Remote Host List",strKey);

		CString strAttribute;
		strAttribute.Format("Attribute%d",i);
		BOOL bIsHttpTunnel=GetINIfileInt("Remote Host List",strAttribute);

		if(bIsHttpTunnel)
			hItem=m_Tree->InsertItem(strHostName,
				ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT,ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT,hRootItem);
		else
			hItem=m_Tree->InsertItem(strHostName,
				ICO_REMOTEHOST_DISCONNECT,ICO_REMOTEHOST_DISCONNECT,hRootItem);
		m_Tree->InsertItem("Temp",hItem);//插入一个临时项
		m_Tree->SetItemData(hItem,TI_REMOTE|TI_HOST);
	}
	///////////////////////////////////////////
}

void CNethiefDoc::OnListInitalUpdate()
{
	m_pListView=(CMyListView*)m_pFrame->m_wndSplitter.GetPane(0,1);
	m_List=&(m_pListView->GetListCtrl());
	////////////////////////////////////////////////
	m_List->SetImageList(&m_ListImageList,LVSIL_SMALL);//设置关联映象列表
	/////////////////////////////////////////////////
	int nColumnPos0=GetINIfileInt("ListView Placement","ColumnPos0");
	int nColumnPos1=GetINIfileInt("ListView Placement","ColumnPos1");
	int nColumnPos2=GetINIfileInt("ListView Placement","ColumnPos2");
	int nColumnPos3=GetINIfileInt("ListView Placement","ColumnPos3");
	//从INI中得到并重置列表柱的位置
	m_List->InsertColumn(0,"名称",LVCFMT_LEFT,nColumnPos0,-1);
	m_List->InsertColumn(1,"大小",LVCFMT_RIGHT,nColumnPos1,-1);
	m_List->InsertColumn(2,"类型",LVCFMT_LEFT,nColumnPos2,-1);
	m_List->InsertColumn(3,"修改时间",LVCFMT_LEFT,nColumnPos3,-1);
}

void CNethiefDoc::OnListDestroy()
{
	//保存列表柱的位置到INI
	WriteINIfileInt("ListView Placement","ColumnPos0",m_List->GetColumnWidth(0));
	WriteINIfileInt("ListView Placement","ColumnPos1",m_List->GetColumnWidth(1));
	WriteINIfileInt("ListView Placement","ColumnPos2",m_List->GetColumnWidth(2));
	WriteINIfileInt("ListView Placement","ColumnPos3",m_List->GetColumnWidth(3));
}

///////////////////////////////////////////////////////////////////////
void CNethiefDoc::AddDiskDriveToTree(HTREEITEM hItem)
{
	CString strDiskDriveName;
	HTREEITEM hDiskItem;
	for(int i='A';i<'[';i++)//从"A"到"Z"来一遍
	{
		strDiskDriveName=(CString)i+":\\";
		switch(GetDriveType(strDiskDriveName))
		{
			case DRIVE_REMOVABLE://软盘驱动器
			{
				SHFILEINFO shFileInfo;
				SHGetFileInfo(strDiskDriveName,0,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_DISPLAYNAME);
				////////////////////////////////////////
				CString strDisplayName=(LPCSTR)shFileInfo.szDisplayName;
				strDisplayName=strDisplayName.Right(4);
				if(strDisplayName=="(A:)"||strDisplayName=="(B:)")
				{
					hDiskItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
						ICO_DRIVE_REMOVABLE,ICO_DRIVE_REMOVABLE,hItem);
				}
				else
				{
					hDiskItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
						ICO_DRIVE_ZIP,ICO_DRIVE_ZIP,hItem);
				}
				////////////////////////////////////////
				m_Tree->SetItemData(hDiskItem,TI_DISKDRIVE_REMOVABLE|TI_DISKDRIVE);
				m_Tree->InsertItem("Temp",hDiskItem);//插入临时子项
				break;
			}
			case DRIVE_FIXED://硬盘驱动器
			{
				SHFILEINFO shFileInfo;
				SHGetFileInfo(strDiskDriveName,0,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_DISPLAYNAME);
				hDiskItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
					ICO_DRIVE_FIXED,ICO_DRIVE_FIXED,hItem);
				m_Tree->SetItemData(hDiskItem,TI_DISKDRIVE_FIXED|TI_DISKDRIVE);
				AddDoubleDeckSubdirectoryToDiskDriveItem(strDiskDriveName,hDiskItem);//添加双层子目录到磁盘驱动器
				break;
			}
			case DRIVE_CDROM://光盘驱动器
			{
				SHFILEINFO shFileInfo;
				SHGetFileInfo(strDiskDriveName,0,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_DISPLAYNAME);
				hDiskItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
					ICO_DRIVE_CDROM,ICO_DRIVE_CDROM,hItem);
				m_Tree->InsertItem("Temp",hDiskItem);//无论光驱里有没有盘都先插入一个临时项
				CFileFind f;
				BOOL bExit=f.FindFile(strDiskDriveName+"*.*");
				if(bExit)//如果光驱里有盘
				{
					m_Tree->SetItemData(hDiskItem,TI_DISKDRIVE_CDROM|TI_DISKDRIVE|TI_ONCEEXPAND);
					AddDoubleDeckSubdirectoryToDiskDriveItem(strDiskDriveName,hDiskItem);
				}
				else
				{
					m_Tree->SetItemData(hDiskItem,TI_DISKDRIVE_CDROM|TI_DISKDRIVE);
				}
				break;
			}
			case DRIVE_REMOTE:
			{
				SHFILEINFO shFileInfo;
				SHGetFileInfo(strDiskDriveName,0,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_DISPLAYNAME);
				hDiskItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
					ICO_DRIVE_REMOTE,ICO_DRIVE_REMOTE,hItem);
				m_Tree->SetItemData(hDiskItem,TI_DISKDRIVE_FIXED|TI_DISKDRIVE);
				AddDoubleDeckSubdirectoryToDiskDriveItem(strDiskDriveName,hDiskItem);//添加双层子目录到磁盘驱动器
				break;
			}
		}
	} 
}

BOOL CNethiefDoc::AddDoubleDeckSubdirectoryToDiskDriveItem(CString strDir,HTREEITEM hItem)
{
	if(strDir.Right(1)!="\\")//如果没有"\"就自动加上
		strDir+="\\";
	CFileFind f;
	BOOL bExit=f.FindFile(strDir+"*.*");
	if(GetLastError()==ERROR_NOT_READY)//如果读盘错误
	{
		int nReturn=MessageBox(AfxGetMainWnd()->m_hWnd,"无法访问 "+strDir+" 。\r\n\r\n设备没有准备好。",
			"文件管理器 - "+strDir,MB_ICONSTOP|MB_RETRYCANCEL);
		if(nReturn==IDRETRY)//如果选择"重试"就递归调用自己
		{
			return AddDoubleDeckSubdirectoryToDiskDriveItem(strDir,hItem);
		}
		else//如果选择"取消"就是没有展开过
		{
			DWORD dwFlag=m_Tree->GetItemData(hItem);
			dwFlag&=~TI_ONCEEXPAND;
			m_Tree->SetItemData(hItem,dwFlag);
			return FALSE;
		}
	}
	DWORD dwFlag=m_Tree->GetItemData(hItem);
	if(!(dwFlag&TI_DISKDRIVE_FIXED))//操作成功了,如果不是硬盘
	{
		HTREEITEM hChildItem=m_Tree->GetChildItem(hItem);//删除临时项
		m_Tree->DeleteItem(hChildItem);
	}
	//////////////////////////////////////////
	if(dwFlag&TI_DISKDRIVE_CDROM)//操作成功了,如果是光驱
	{
		SHFILEINFO shFileInfo;
		SHGetFileInfo(strDir,0,&shFileInfo,sizeof(SHFILEINFO),
			SHGFI_DISPLAYNAME);
		m_Tree->SetItemText(hItem,(LPCSTR)shFileInfo.szDisplayName);
	}
	while(bExit)
	{
		bExit=f.FindNextFile();
		if(f.IsDirectory())//当前文件是目录
		{
			if(!f.IsDots())	//目录文件非"."和".."
			{
				CString strFilename=f.GetFileName();
				SHFILEINFO shFileInfo;
				SHGetFileInfo(strDir+strFilename,0,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_DISPLAYNAME);

				HTREEITEM hChildItem;
				hChildItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
					ICO_FOLDER,ICO_FOLDER_OPEN,hItem,TVI_SORT);

				AddSubdirectoryToTree(strDir+strFilename,hChildItem);//再添加子目录到这个文件夹
			}
		}
	}
	return TRUE;
}

void CNethiefDoc::AddSubdirectoryToTree(CString strDir, HTREEITEM hItem)
{
	if(strDir.Right(1)!="\\")
		strDir+="\\";
	CFileFind f;
	BOOL bExit=f.FindFile(strDir+"*.*");
	if(GetLastError()==ERROR_NOT_READY)//如果读盘错误
	{
		int nReturn=MessageBox(AfxGetMainWnd()->m_hWnd,"无法访问 "+strDir+" 。\r\n\r\n设备没有准备好。",
			"文件管理器 - "+strDir,MB_ICONSTOP|MB_RETRYCANCEL);
		if(nReturn==IDRETRY)
		{
			AddSubdirectoryToTree(strDir,hItem);
			return;
		}
		else
		{
			HTREEITEM hParentItem=m_Tree->GetParentItem(hItem);
			DWORD dwFlag=m_Tree->GetItemData(hParentItem);
			dwFlag&=~TI_ONCEEXPAND;
			m_Tree->SetItemData(hParentItem,dwFlag);
			return;
		}
	}
	while(bExit)
	{
		bExit=f.FindNextFile();
		if(f.IsDirectory())//当前文件是目录
		{
			if(!f.IsDots())	//目录文件非"."和".."
			{
				CString strFilename=f.GetFileName();
				SHFILEINFO shFileInfo;
				SHGetFileInfo(strDir+strFilename,0,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_DISPLAYNAME);

				CString strDisplayName=(LPCSTR)shFileInfo.szDisplayName;
				if(strDisplayName=="")
				{
					strDisplayName=strFilename;
				}

				m_Tree->InsertItem(strDisplayName,
					ICO_FOLDER,ICO_FOLDER_OPEN,hItem,TVI_SORT);
			}
		}
	}
}

void CNethiefDoc::AddHost(CString strItemName,BOOL bIsHttpTunnel)
{
	HTREEITEM hItem=m_Tree->GetRootItem();
	if(bIsHttpTunnel)
		hItem=m_Tree->InsertItem(strItemName,ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT,ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT,hItem);
	else
		hItem=m_Tree->InsertItem(strItemName,ICO_REMOTEHOST_DISCONNECT,ICO_REMOTEHOST_DISCONNECT,hItem);
	m_Tree->InsertItem("Temp",hItem);//插入一个临时项
	m_Tree->SetItemData(hItem,TI_REMOTE|TI_HOST);
}

///////////////////////////////////////////////////////////////////////
void CNethiefDoc::OnTreeItemexpanding(NM_TREEVIEW* pNMTreeView,LRESULT* pResult)
{
	if(pNMTreeView->action==TVE_EXPAND)//如果是展开节点
	{
		HTREEITEM hExpandItem=pNMTreeView->itemNew.hItem;
		DWORD dwFlag=m_Tree->GetItemData(hExpandItem);
		if(!(dwFlag&TI_REMOTE))	//没有远程属性
		{
			if((dwFlag&TI_DISKDRIVE_CDROM&&(!(dwFlag&TI_ONCEEXPAND)))||(dwFlag&TI_DISKDRIVE_REMOVABLE&&(!(dwFlag&TI_ONCEEXPAND))))
			{//如果是光驱没有展开过,或软驱没有展开过
				BeginWaitCursor();
				m_Tree->SetItemData(hExpandItem,dwFlag|TI_ONCEEXPAND);//设置为展开过
				CString strDiskDrive=m_Tree->GetItemText(hExpandItem);
				strDiskDrive=strDiskDrive.Right(3);
				strDiskDrive=strDiskDrive.Left(2);//得到驱动器路径,如"E:"
				if(!AddDoubleDeckSubdirectoryToDiskDriveItem(strDiskDrive,hExpandItem))
				{//如果读盘失败,就不展开节点
					*pResult = 1;
					EndWaitCursor();
					return;
				}
				EndWaitCursor();
			}
			if(!(dwFlag&TI_ROOT||dwFlag&TI_HOST||dwFlag&TI_DISKDRIVE||dwFlag&TI_ONCEEXPAND))
			{//除去根节点,我的电脑,磁盘驱动器,展开过的所有节点,即没有展开过的文件夹节点
				BeginWaitCursor();
				m_Tree->SetItemData(hExpandItem,dwFlag|TI_ONCEEXPAND);//设置为展开过
				HTREEITEM hChildItem=m_Tree->GetChildItem(hExpandItem);
				while(hChildItem)//如果有子项
				{
					CString strDir;
					strDir=m_Tree->GetItemText(hChildItem);//得到子项的TEXT
					HTREEITEM hParentItem;
					hParentItem=hChildItem;
					while(1)
					{
						hParentItem=m_Tree->GetParentItem(hParentItem);
						if(m_Tree->GetItemData(hParentItem)&TI_DISKDRIVE)//如果父项是驱动器
						{
							CString strParentText=m_Tree->GetItemText(hParentItem);
							strParentText=strParentText.Right(3);
							strParentText=strParentText.Left(2);
							strDir=strParentText+"\\"+strDir;//得到整个的路径
							break;
						}
						strDir=m_Tree->GetItemText(hParentItem)+"\\"+strDir;//加上父项的路径
					}
					AddSubdirectoryToTree(strDir,hChildItem);//添加子目录到这个子项
					hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//得到下一个子项
				}
				EndWaitCursor(); 
			}
		}
		else	//有远程属性
		{
			if(dwFlag&TI_HOST)//是远程又是主机
			{
				int nImage,nSelectedImage;
				m_Tree->GetItemImage(hExpandItem,nImage,nSelectedImage);
				if(nImage==ICO_REMOTEHOST_DISCONNECT || nImage==ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT)//是未连接主机,就连接它
				{
					ConnectHost(hExpandItem);
					*pResult = 1;
				}
				return;
			}
			if(!(dwFlag&TI_ONCEEXPAND))//是没有展开过的节点(没有展开过的文件夹与驱动器)
			{
				if((dwFlag&TI_DISKDRIVE_REMOVABLE||dwFlag&TI_DISKDRIVE_CDROM)&&(!(dwFlag&TI_ONCEACCESS)))
				{//是软驱或光驱且没有访问过
					CString strDiskDirve=m_Tree->GetItemText(hExpandItem);
					strDiskDirve=strDiskDirve.Right(3);
					strDiskDirve=strDiskDirve.Left(2);
					if(MessageBox(AfxGetMainWnd()->m_hWnd,
						"访问远程驱动器 "+strDiskDirve+"，可能会曝露服务端程序。\r\n\r\n想要继续吗？",
						"文件管理器 - "+strDiskDirve,
						MB_ICONQUESTION|MB_YESNO)==IDNO)
					{
						*pResult = 1;
						return;
					}
				}
				//////////////////////////////////////////////////////
				CString strHostName;
				CString strPath;
				CString strParentText;
				DWORD dwParentFlag;
				HTREEITEM hParentItem=hExpandItem;
				while(1)
				{
					strParentText=m_Tree->GetItemText(hParentItem);
					dwParentFlag=m_Tree->GetItemData(hParentItem);
					if(dwParentFlag&TI_DISKDRIVE)
					{	
						strParentText=strParentText.Right(3);
						strParentText=strParentText.Left(2);
						strPath=strParentText+"\\"+strPath;//得到整个的路径
						break;
					}
					strPath=strParentText+"\\"+strPath;
					hParentItem=m_Tree->GetParentItem(hParentItem);
				}
				strHostName=GetHostName(m_Tree->GetParentItem(hParentItem));
				/*以上的代码为得到全路径,如:"d:\","d:\windows\system\"*/
				Remote_ExpandDirectory(hExpandItem,strHostName,strPath);
				*pResult = 1;
				return;
			}
		}
	}
	*pResult = 0;
}

void CNethiefDoc::OnTreeSelchanged(NM_TREEVIEW* pNMTreeView,LRESULT* pResult)
{
	BeginWaitCursor();//显示等待光标
	HTREEITEM hSelectItem=pNMTreeView->itemNew.hItem;//得到打开的文件夹节点
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);//得到节点属性
	while(1)
	{
		if(dwFlag&TI_ROOT||dwFlag&TI_HOST)//如果是根节点或者主机节点
		{
			SetCurDir("","");
			m_List->DeleteAllItems();
			break;
		}
		if(!(dwFlag&TI_REMOTE))	//不是远程节点
		{
			if(dwFlag&TI_DISKDRIVE)//如果是磁盘驱动器
			{
				if((dwFlag&TI_DISKDRIVE_CDROM&&(!(dwFlag&TI_ONCEEXPAND)))||(dwFlag&TI_DISKDRIVE_REMOVABLE&&(!(dwFlag&TI_ONCEEXPAND))))
				{//如果是光驱没有展开过,或软驱没有展开过
					BeginWaitCursor();
					m_Tree->SetItemData(hSelectItem,dwFlag|TI_ONCEEXPAND);//设置为展开过
					CString strDiskDrive=m_Tree->GetItemText(hSelectItem);
					strDiskDrive=strDiskDrive.Right(3);
					strDiskDrive=strDiskDrive.Left(2);//得到驱动器路径,如"E:"
					if(!AddDoubleDeckSubdirectoryToDiskDriveItem(strDiskDrive,hSelectItem))
					{
						SetCurDir("我的电脑",strDiskDrive+"\\");
						m_List->DeleteAllItems();
						break;
					}
				}
				CString strSelectText=m_Tree->GetItemText(hSelectItem);
				strSelectText=strSelectText.Right(3);
				strSelectText=strSelectText.Left(2);
				strSelectText+="\\";//得到整个的路径
				SetCurDir("我的电脑",strSelectText);
				AddFileToList(strSelectText);//添加这个路径的文件到List
				RefreshDirecoryItem(hSelectItem);//刷新目录节点
				break;
			}
			if(1)//如果是文件夹
			{
				CString strDir;
				strDir=m_Tree->GetItemText(hSelectItem);//得到子项的TEXT
				HTREEITEM hParentItem;
				hParentItem=hSelectItem;
				while(1)
				{
					hParentItem=m_Tree->GetParentItem(hParentItem);
					if(m_Tree->GetItemData(hParentItem)&TI_DISKDRIVE)//如果父项是驱动器
					{
						CString strParentText=m_Tree->GetItemText(hParentItem);
						strParentText=strParentText.Right(3);
						strParentText=strParentText.Left(2);
						strDir=strParentText+"\\"+strDir;//得到整个的路径
						break;
					}
					strDir=m_Tree->GetItemText(hParentItem)+"\\"+strDir;//加上父项的路径
				}
				SetCurDir("我的电脑",strDir);
				AddFileToList(strDir);//添加这个路径的文件到List
				RefreshDirecoryItem(hSelectItem);//刷新目录节点
				break;
			}
		}
		else	//是远程节点(就是文件夹或驱动器节点)
		{
			if((dwFlag&TI_DISKDRIVE_REMOVABLE||dwFlag&TI_DISKDRIVE_CDROM)&&(!(dwFlag&TI_ONCEACCESS)))
			{//是软驱或光驱且没有访问过
				CString strDiskDirve=m_Tree->GetItemText(hSelectItem);
				strDiskDirve=strDiskDirve.Right(3);
				strDiskDirve=strDiskDirve.Left(2);
				if(MessageBox(AfxGetMainWnd()->m_hWnd,
					"访问远程驱动器 "+strDiskDirve+"，可能会曝露服务端程序。\r\n\r\n想要继续吗？",
					"文件管理器 - "+strDiskDirve,
					MB_ICONQUESTION|MB_YESNO)==IDNO)
				{
					SetCurDir(GetHostName(m_Tree->GetParentItem(hSelectItem)),strDiskDirve+"\\");
					m_List->DeleteAllItems();
					*pResult = 1;
					return;
				}
			}
			//////////////////////////////////////////////////////
			CString strHostName;
			CString strPath;
			CString strParentText;
			DWORD dwParentFlag;
			HTREEITEM hParentItem=hSelectItem;
			while(1)
			{
				strParentText=m_Tree->GetItemText(hParentItem);
				dwParentFlag=m_Tree->GetItemData(hParentItem);
				if(dwParentFlag&TI_DISKDRIVE)
				{	
					strParentText=strParentText.Right(3);
					strParentText=strParentText.Left(2);
					strPath=strParentText+"\\"+strPath;//得到整个的路径
					break;
				}
				strPath=strParentText+"\\"+strPath;
				hParentItem=m_Tree->GetParentItem(hParentItem);
			}
			strHostName=GetHostName(m_Tree->GetParentItem(hParentItem));
			/*以上的代码为得到全路径,如:"d:\","d:\windows\system\"*/
			Remote_ShowFileList(hSelectItem,strHostName,strPath);
			*pResult = 1;
			return;
		}
	}
	EndWaitCursor();//结束显示等待光标
	*pResult = 0;
}

void CNethiefDoc::OnTreeEndlabeledit(TV_DISPINFO* pTVDispInfo,LRESULT* pResult)
{
	HTREEITEM hEditItem=pTVDispInfo->item.hItem;//得到编辑标签的hItem
	CString strText=pTVDispInfo->item.pszText;//得到编辑后的Text
	CString strDirName=m_Tree->GetItemText(hEditItem);//得到编辑前的TEXT
	if(strText.IsEmpty()||strDirName==strText)//如果没做修改或ESC取消了
	{
		return;
	}
	
	if(strText.Find('\\')!=-1||
		strText.Find('/')!=-1||
		strText.Find(':')!=-1||
		strText.Find('*')!=-1||
		strText.Find('?')!=-1||
		strText.Find('"')!=-1||
		strText.Find('<')!=-1||
		strText.Find('>')!=-1||
		strText.Find('|')!=-1)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd,"文件名不能包含以下字母: \r\n	 \\ / :  * ? \" < > | ",
			"重命名",MB_ICONSTOP|MB_OK);
	}
	else
	{
		CString strHostName=GetCurHostName();
		if(strHostName!="我的电脑")
		{
			CString strPath=GetCurDir();
			strPath=strPath.Left(strPath.ReverseFind('\\')+1);/*如:D:\Download\*/
			Remote_RenameFile(hEditItem,strHostName,strPath,strDirName,strText,TRUE,FALSE,NULL);
			return;
		}
		///////////////////////////////////////////////////
		BeginWaitCursor();//显示等待光标

		CString strFrom=GetCurDir();
		CString strTo=GetCurDir();
		strTo=strTo.Mid(0,strTo.ReverseFind('\\')+1)+strText;
		SetCurDir("我的电脑",strTo);

		int nStrLength=strFrom.GetLength();
		strFrom.GetBufferSetLength(nStrLength+1);
		strFrom.SetAt(nStrLength,'\0');
		nStrLength=strTo.GetLength();
		strTo.GetBufferSetLength(nStrLength+1);
		strTo.SetAt(nStrLength,'\0');

		SHFILEOPSTRUCT shFileOp;
		ZeroMemory(&shFileOp,sizeof(shFileOp));
		shFileOp.hwnd=NULL;
		shFileOp.wFunc=FO_RENAME;
		shFileOp.pFrom=strFrom;
		shFileOp.pTo=strTo;
		shFileOp.fFlags=FOF_ALLOWUNDO;
		if(!SHFileOperation(&shFileOp))
		{
			SHFILEINFO shFileInfo;
			SHGetFileInfo(strTo,0,&shFileInfo,sizeof(SHFILEINFO),
				SHGFI_DISPLAYNAME);

			m_Tree->SetItemText(hEditItem,(LPCSTR)shFileInfo.szDisplayName);
			HTREEITEM hParentItem=m_Tree->GetParentItem(hEditItem);
			m_Tree->SortChildren(hParentItem);//父项重新排序
		}
		EndWaitCursor();//结束显示等待光标
	}
}

void CNethiefDoc::OnListEndlabeledit(LV_DISPINFO* pDispInfo, LRESULT* pResult)
{
	int nEditItem=pDispInfo->item.iItem;//得到要编辑标签的项号
	CString strText=pDispInfo->item.pszText;//得到编辑后的Text
	CString strDirName=m_List->GetItemText(nEditItem,0);//得到编辑前的TEXT
	if (strText.IsEmpty()||strDirName==strText)//如果没做修改或ESC取消了
	{
		return;
	}
	
	if(strText.Find('\\')!=-1||
		strText.Find('/')!=-1||
		strText.Find(':')!=-1||
		strText.Find('*')!=-1||
		strText.Find('?')!=-1||
		strText.Find('"')!=-1||
		strText.Find('<')!=-1||
		strText.Find('>')!=-1||
		strText.Find('|')!=-1)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd,"文件名不能包含以下字母: \r\n	 \\ / :  * ? \" < > | ",
			"重命名",MB_ICONSTOP|MB_OK);
	}
	else
	{
		CString strHostName=GetCurHostName();
		if(strHostName!="我的电脑")
		{
			BOOL bIsDir=FALSE;
			if(m_List->GetItemText(nEditItem,1)=="")
				bIsDir=TRUE;
			Remote_RenameFile(m_Tree->GetSelectedItem(),strHostName,GetCurPath(),strDirName,strText,bIsDir,TRUE,nEditItem);
			return;
		}
		///////////////////////////////////////////////////
		BeginWaitCursor();//显示等待光标

		CString strFrom=GetCurPath()+strDirName;
		CString strTo=GetCurPath()+strText;

		int nStrLength=strFrom.GetLength();
		strFrom.GetBufferSetLength(nStrLength+1);
		strFrom.SetAt(nStrLength,'\0');
		nStrLength=strTo.GetLength();
		strTo.GetBufferSetLength(nStrLength+1);
		strTo.SetAt(nStrLength,'\0');

		SHFILEOPSTRUCT shFileOp;
		ZeroMemory(&shFileOp,sizeof(shFileOp));
		shFileOp.hwnd=NULL;
		shFileOp.wFunc=FO_RENAME;
		shFileOp.pFrom=strFrom;
		shFileOp.pTo=strTo;
		shFileOp.fFlags=FOF_ALLOWUNDO;
		if(!SHFileOperation(&shFileOp))
		{
			SHFILEINFO shFileInfo;
			SHGetFileInfo(strTo,0,&shFileInfo,sizeof(SHFILEINFO),
				SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME);
			int nImageIndex=m_ListImageList.Add(shFileInfo.hIcon);
			DestroyIcon(shFileInfo.hIcon);
			//对于"隐藏已知文件类型的扩展名"的处理//////////
			CString strInput1=strTo;//原始文件名
			CString strInput2=shFileInfo.szDisplayName;//处理后的文件名
			CString strOutput;//正确的可以显示的文件名

			int nPos=strInput1.ReverseFind('\\');
			if(nPos!=-1)//如果原始文件名中有路径,去掉路径只留文件名
				strInput1=strInput1.Mid(nPos+1);

			if(strInput1.GetLength()!=strInput2.GetLength())//用户设置了"隐藏已知文件类型的扩展名"
				strOutput=strInput1;//显示原始文件名
			else
				strOutput=strInput2;//显示处理后的文件名
			////////////////////////////////////////////////
			//修改文件图标和名称/////////////////////
			m_List->SetItem(nEditItem,0,LVIF_IMAGE|LVIF_TEXT,
				strOutput,nImageIndex,
				NULL,NULL,NULL);
			//修改文件类型///////////////////////////
			m_List->SetItem(nEditItem,2,LVIF_TEXT,
				(LPCSTR)shFileInfo.szTypeName,NULL,
				NULL,NULL,NULL);
			//////////////////////////////////////////////
			if(m_List->GetItemText(nEditItem,1)=="")
				ChangeDirectoryItem(strDirName,strOutput);
		}
		EndWaitCursor();//结束显示等待光标
	}	
}

///////////////////////////////////////////////////////////////////////
void CNethiefDoc::RefreshDirecoryItem(HTREEITEM hSelectItem)
{
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_REMOTE)
	{
		if(m_Tree->ItemHasChildren(hSelectItem))
		{
			if(!(dwFlag&TI_ONCEEXPAND))
				return;
		}
	}
	//刷新这个目录下的子目录节点///////////////////////
	HTREEITEM hChildItem=m_Tree->GetChildItem(hSelectItem);//得到它的子项
	HTREEITEM hTempItem;
	while(hChildItem)//如果有子项
	{
		CString strDirName=m_Tree->GetItemText(hChildItem);
		int nReturn=FindDirectory(strDirName);
		hTempItem=hChildItem;
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//得到下一个子项
		if(nReturn==-1)
		{
			m_Tree->DeleteItem(hTempItem);//如果没有此目录了,就删除它
		}
	}
	FindNewDirectory(hSelectItem);//添加新目录
}

int CNethiefDoc::FindDirectory(CString strDirName)
{
	LV_FINDINFO fi;
	fi.flags=LVFI_STRING;
	fi.psz=strDirName;
	int nReturn=m_List->FindItem(&fi,-1);
	if(nReturn!=-1)//如果找到了,就做上标记
	{
		m_List->SetItemData(nReturn,LI_FOUND);
	}
	return nReturn;
}

void CNethiefDoc::FindNewDirectory(HTREEITEM hSelectItem)
{
	int i=-1;
	int nItemCount=m_List->GetItemCount();
	if(nItemCount==0)
		return;
	while(1)
	{
		i++;
		if(i>nItemCount-1)
			break;
		if(m_List->GetItemText(i,1)!="")
			break;
		DWORD dwFlag=m_List->GetItemData(i);
		if(dwFlag==LI_FOUND)
			continue;
		AddDirecoryItem(m_List->GetItemText(i,0),hSelectItem,TRUE);
	}
}

void CNethiefDoc::AddDirecoryItem(CString strDirName,HTREEITEM hSelectItem,BOOL bIsRefresh)
{
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(!(dwFlag&TI_REMOTE))//不是远程
	{
		CString strFilename=GetCurPath()+strDirName;//得到目录的路径
		SHFILEINFO shFileInfo;
		SHGetFileInfo(strFilename,0,&shFileInfo,sizeof(SHFILEINFO),
			SHGFI_DISPLAYNAME);//得到显示名称

		HTREEITEM hNewItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
			ICO_FOLDER,ICO_FOLDER_OPEN,hSelectItem,TVI_SORT);
		if(bIsRefresh)
		{
			if(dwFlag&TI_ONCEEXPAND)
				AddSubdirectoryToTree(strFilename,hNewItem);
		}
	}
	else//是远程
	{
		if(m_Tree->ItemHasChildren(hSelectItem))
		{
			if(!(dwFlag&TI_ONCEEXPAND))
				return;
		}
		SHFILEINFO shFileInfo;
		SHGetFileInfo(strDirName,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
				SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME);

		HTREEITEM hNewItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
			ICO_FOLDER,ICO_FOLDER_OPEN,hSelectItem,TVI_SORT);
		m_Tree->SetItemData(hNewItem,TI_REMOTE);
		
		if(!(dwFlag&TI_ONCEEXPAND))
		{
			m_Tree->SetItemData(hSelectItem,dwFlag|TI_ONCEEXPAND);
		}
	}
}

void CNethiefDoc::AddFileToList(CString strDir)
{
	///////////////////////////////
	if(strDir.Right(1)!="\\")
		strDir+="\\";
	////////////////////////////
	CFileFind f;
	BOOL bExit=f.FindFile(strDir+"*.*");
	if(GetLastError()==ERROR_NOT_READY)//如果读盘错误
	{
		int nReturn=MessageBox(AfxGetMainWnd()->m_hWnd,"无法访问 "+strDir+" 。\r\n\r\n设备没有准备好。",
			"文件管理器 - "+strDir,MB_ICONSTOP|MB_RETRYCANCEL);
		if(nReturn==IDRETRY)
		{
			AddFileToList(strDir);
			return;
		}
		else
		{
			return;
		}
	}
	/////////////////////////////////////////////
	DWORD dwTotalFileSize=0;
	/////////////////////////////////////////////
	int nCount=0;
	m_pListView->SetRedraw(FALSE);
	m_List->DeleteAllItems();
	while(TRUE)//清空列表映象列表
	{
		HICON hIcon=m_ListImageList.ExtractIcon(0);
		if(hIcon==NULL)
			break;
		DestroyIcon(hIcon);
		m_ListImageList.Remove(0);
	}

	while(bExit)
	{
		bExit=f.FindNextFile();
		if(!f.IsDots())	//目录文件非"."和".."
		{
			CString strFilename=f.GetFileName();
			CString strDisplayName;
			CString strTypeName;

			SHFILEINFO shFileInfo;
			if(SHGetFileInfo(strDir+strFilename,0,&shFileInfo,sizeof(SHFILEINFO),
				SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME))
			{
				m_ListImageList.Add(shFileInfo.hIcon);//图标添加到列表映象列表里
				DestroyIcon(shFileInfo.hIcon);

				strTypeName=(LPCSTR)shFileInfo.szTypeName;
				//对于"隐藏已知文件类型的扩展名"的处理//////////
				CString strInput1=strFilename;//原始文件名
				CString strInput2=shFileInfo.szDisplayName;//处理后的文件名

				int nPos=strInput1.ReverseFind('\\');
				if(nPos!=-1)//如果原始文件名中有路径,去掉路径只留文件名
					strInput1=strInput1.Mid(nPos+1);

				if(strInput1.GetLength()!=strInput2.GetLength())//用户设置了"隐藏已知文件类型的扩展名"
					strDisplayName=strInput1;//显示原始文件名
				else
					strDisplayName=strInput2;//显示处理后的文件名
				////////////////////////////////////////////////
			}
			else//是回收站里的文件
			{
				strDisplayName=strFilename;

				if(f.IsDirectory())//是回收站里的文件夹
				{
					m_ListImageList.Add(m_hIcon_Folder);//添加文件夹图标到列表映象列表里
					strTypeName="文件夹";
				}
				else//是回收站里的文件
				{
					m_ListImageList.Add(m_hIcon_UnknownFile);//添加未知文件图标到列表映象列表里
					int nPos=strDisplayName.Find(".");
					if(nPos==-1)
						strTypeName="文件";
					else
						strTypeName=strDisplayName.Mid(nPos+1)+" 文件";
				}
			}
			//加入文件图标和名称/////////////////////
			m_List->InsertItem(nCount,
				strDisplayName,nCount);
			//加入文件类型///////////////////////////
			m_List->SetItemText(nCount,2,strTypeName);
			//加入文件大小///////////////////////////
			if(!f.IsDirectory())
			{
				DWORD dwFileLength=f.GetLength();
				dwTotalFileSize+=dwFileLength;
				if(dwFileLength!=0)
				{	
					DWORD dwTemp=dwFileLength/1024;
					if(dwTemp*1024!=dwFileLength)
						dwFileLength=dwTemp+1;
					else
						dwFileLength=dwTemp;
				}
				m_List->SetItemText(nCount,1,ConverDWORDToCString(dwFileLength)+"KB");
			}
			//加入文件修改时间///////////////////////
			CTime refTime;
			f.GetLastWriteTime(refTime);
			CString strFileTime;
			strFileTime=refTime.Format("%y-%m-%d %H:%M");
			m_List->SetItemText(nCount,3,strFileTime);	
			/////////////////////////////////////////
			m_List->SetItemData(nCount,nCount);
			/////////////////////////////////////////
			nCount++;
		}
	}
	m_List->SortItems(CompareFunc,(DWORD)m_List);//列表项排序
	m_pListView->SetRedraw(TRUE);
	////////////////////////////////////
	CString strIndicatorStatus;
	strIndicatorStatus.Format("，%d 个对象，",nCount);
	strIndicatorStatus="[我的电脑]"+GetCurDir()+
		strIndicatorStatus+ConverDWORDToCString(dwTotalFileSize)+" 字节"+
		" (可用空间: "+
		ConverDWORDToCString(GetDiskSpace(GetCurDir().Left(3),TRUE))+
		" 字节)";
	SetIndicatorStatus(strIndicatorStatus);
}

CString CNethiefDoc::ConverDWORDToCString(DWORDLONG dwSize)
{
	CString strSize;
	sprintf(strSize.GetBuffer(100),"%I64u",dwSize);
	strSize.ReleaseBuffer();

	CString strTemp="";
	int nStrLen=strSize.GetLength();
	while(1)
	{
		if(nStrLen>3)
		{		
			strTemp=","+strSize.Right(3)+strTemp;					
		}
		else
		{
			strTemp=strSize+strTemp;
			break;
		}
		strSize=strSize.Mid(0,nStrLen-3);
		nStrLen=strSize.GetLength();
	}
	return strTemp;
}

DWORDLONG CNethiefDoc::GetDiskSpace(CString strDiskDriver,BOOL bFreeSpace)
{
	ULARGE_INTEGER TotalNumberOfBytes;
	ULARGE_INTEGER TotalNumberOfFreeBytes;
	GetDiskFreeSpaceEx(strDiskDriver, NULL,
		&TotalNumberOfBytes, &TotalNumberOfFreeBytes); 
	if(bFreeSpace)
		return TotalNumberOfFreeBytes.QuadPart;
	else
		return TotalNumberOfBytes.QuadPart;
}

int CALLBACK CNethiefDoc::CompareFunc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort)
{
	CListCtrl* m_List=(CListCtrl*)lParamSort;

	BOOL bIsDirItem1=FALSE;
	BOOL bIsDirItem2=FALSE;
	if(m_List->GetItemText(lParam1,1)=="")
		bIsDirItem1=TRUE;
	if(m_List->GetItemText(lParam2,1)=="")
		bIsDirItem2=TRUE;

	if(bIsDirItem1&&!bIsDirItem2)
		return -1;
	if(!bIsDirItem1&&bIsDirItem2)
		return 1;

	CString	strItem1 = m_List->GetItemText(lParam1,2);
	CString	strItem2 = m_List->GetItemText(lParam2,2);
	strItem1.MakeUpper();
	strItem2.MakeUpper();
	int nResult=strcmp(strItem1,strItem2);
	if(nResult!=0)
		return nResult;
	else
	{
		strItem1=m_List->GetItemText(lParam1,0);
		strItem2=m_List->GetItemText(lParam2,0);
		strItem1.MakeUpper();
		strItem2.MakeUpper();
		return strcmp(strItem1,strItem2);
	}	
}

void CNethiefDoc::OnTreeBeginlabeledit(TV_DISPINFO* pTVDispInfo,LRESULT* pResult)
{
	HTREEITEM hEditItem=pTVDispInfo->item.hItem;//得到编辑标签的hItem
	DWORD dwFlag=m_Tree->GetItemData(hEditItem);//得到编辑标签的Flag
	if(dwFlag&TI_ROOT||dwFlag&TI_HOST||dwFlag&TI_DISKDRIVE)
	{//如果要编辑的标签不是文件夹节点
		MessageBeep(0xFFFFFFFF);//响一声
		*pResult=1;				//不出现Edit框
	}
}

void CNethiefDoc::ChangeDirectoryItem(CString strDirName,CString strNewDirName)
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	HTREEITEM hChildItem=m_Tree->GetChildItem(hSelectItem);//得到它的子项
	while(hChildItem)//如果有子项
	{
		if(m_Tree->GetItemText(hChildItem)==strDirName)//得到子项的Text
		{
			m_Tree->SetItemText(hChildItem,strNewDirName);//如果是要改的,就修改
			break;
		}
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//得到下一个子项
	}
	m_Tree->SortChildren(hSelectItem);//父项重新排序
}

void CNethiefDoc::OnListDblclk(NM_LISTVIEW* pNMListView,LRESULT* pResult)
{
	int nDblclkItem=pNMListView->iItem;//得到双击的序号
	if(nDblclkItem!=-1)//如果是在列表项上双击
	{
		BeginWaitCursor();//显示等待光标
		if(m_List->GetItemText(nDblclkItem,1)=="")//双击的是目录
		{
			CString strDirName=m_List->GetItemText(nDblclkItem,0);
			SelectDirecoryItem(strDirName);
		}
		else//双击的是文件
		{
			CString strFilename=m_List->GetItemText(nDblclkItem,0);
			CString strPathName=GetCurPath()+strFilename;
			////////////////////////////////////////////////////////////
			CString strHostName=GetCurHostName();
			if(strHostName!="我的电脑")
			{
				Remote_DownloadFile(strHostName,
					strPathName,m_strFileCacheDir+"\\"+strFilename,0,
					1,1,FALSE,1);
				return;
			}
			////////////////////////////////////////////////////////////
			HINSTANCE hInstance=ShellExecute(NULL,NULL,strPathName,NULL,GetCurDir(),SW_SHOWNORMAL);
			if((INT)hInstance<=32)
			{
				int nErrorCode=(INT)hInstance;
				if(nErrorCode==SE_ERR_NOASSOC||nErrorCode==SE_ERR_ASSOCINCOMPLETE)
				{
					CFileDialog dlg(true,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
						"程序(*.exe)|*.exe|所有文件(*.*)|*.*||");
					dlg.m_ofn.lpstrTitle="请选择用于打开此文件的程序...";
					dlg.m_ofn.lpstrInitialDir=GetCurDir();
					if(dlg.DoModal()==IDOK) 
					{
						CString strProgramPath=dlg.GetPathName();
						strPathName="\""+strPathName+"\"";
						CString strDirectory=strProgramPath.Left(strProgramPath.ReverseFind('\\')+1);
						ShellExecute(NULL,NULL,strProgramPath,strPathName,strDirectory,SW_SHOWNORMAL);
					}				
				}
				else
				{
					MessageErrorBox(nErrorCode,"无法执行文件",strPathName);
				}
			}
		}
		EndWaitCursor();//结束显示等待光标
	}
}

void CNethiefDoc::SelectDirecoryItem(CString strDirName)
{
	m_bLastSelect=FALSE;
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_REMOTE && !(dwFlag&TI_ONCEEXPAND))//是没有展开过的远程文件夹或驱动器节点
	{
		m_bLastSelect=TRUE;
		m_strSelectName=strDirName;
		m_Tree->Expand(hSelectItem,TVE_EXPAND);
	}
	else
	{
		HTREEITEM hChildItem=m_Tree->GetChildItem(hSelectItem);//得到它的子项
		while(hChildItem)//如果有子项
		{
			if(m_Tree->GetItemText(hChildItem)==strDirName)//得到子项的Text
			{
				m_Tree->Expand(hSelectItem,TVE_EXPAND);
				m_Tree->SelectItem(hChildItem);
				break;
			}
			hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//得到下一个子项
		}
	}
}
////////////////////////////////////////////////////
void CNethiefDoc::SetCurDir(CString strCurHostName,CString strCurDir)
{
	m_strCurHostName=strCurHostName;
	m_strCurDir=strCurDir;
}

CString CNethiefDoc::GetCurDir()
{
	int nLength=m_strCurDir.GetLength();
	if(nLength==3)
		return m_strCurDir;
	if(m_strCurDir.Right(1)=="\\")
		return m_strCurDir.Left(nLength-1);
	else
		return m_strCurDir;
}

CString CNethiefDoc::GetCurPath()
{
	if(m_strCurDir.Right(1)=="\\")
		return m_strCurDir;
	else
		return m_strCurDir+"\\";
}

CString CNethiefDoc::GetCurHostName()
{
	return m_strCurHostName;
}

/////////显示出错消息框///////////////////////////////////////////////
void CNethiefDoc::MessageErrorBox(int nErrorCode,CString strSubject,CString strFilename)
{
	switch(nErrorCode)
	{
		case ERROR_NOT_CONNECTED:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+": 指定的文件名与现有文件名重名。请指定另一个文件名。",
				"文件操作出错",MB_ICONSTOP|MB_OK);
			break;
		}
		case ERROR_WRITE_PROTECT:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+": 磁盘被写保护。\r\n\r\n请去掉写保护或使用另一张磁盘。",
				"文件操作出错",MB_ICONSTOP|MB_OK);
			break;
		}
		case ERROR_ACCESS_DENIED:
		{
			if(strSubject=="无法执行文件")
			{
				MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n它不是一个合法的 Win32 应用程序。",
					"文件操作出错",MB_ICONSTOP|MB_OK);
			}
			else
			{
				MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+": 访问被拒绝。\r\n\r\n请确定磁盘是否未满或未被写保护\r\n而且文件未被使用。",
					"文件操作出错",MB_ICONSTOP|MB_OK);
			}
			break;
		}
		case ERROR_FILE_NOT_FOUND:
		{
			if(strSubject=="无法执行文件")
			{
				MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n此文件没有找到。",
					"文件操作出错",MB_ICONSTOP|MB_OK);
			}
			else
			{
				MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+": 访问被拒绝。\r\n\r\n请确定磁盘是否未满或未被写保护\r\n而且文件未被使用。",
					"文件操作出错",MB_ICONSTOP|MB_OK);

			}
			break;
		}
		//////////////////////////////////////////
		case 0:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n操作系统没有足够的内存或资源。",
				"文件操作出错",MB_ICONSTOP|MB_OK);
			break;
		}
		case ERROR_PATH_NOT_FOUND:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n此路径没有找到。",
				"文件操作出错",MB_ICONSTOP|MB_OK);
			break;
		}
		case ERROR_BAD_FORMAT:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n它不是一个合法的 Win32 应用程序。",
				"文件操作出错",MB_ICONSTOP|MB_OK);
			break;
		}
		case SE_ERR_DDEBUSY:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n动态数据交换(DDE)处理不能完成，因为其它的动态数据交换处理正在进行。",
				"文件操作出错",MB_ICONSTOP|MB_OK);
			break;
		}
		case SE_ERR_DDEFAIL:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n动态数据交换(DDE)处理失败。",
				"文件操作出错",MB_ICONSTOP|MB_OK);
			break;
		}
		case SE_ERR_DDETIMEOUT:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n动态数据交换(DDE)处理不能完成，因为请求超时。",
				"文件操作出错",MB_ICONSTOP|MB_OK);
			break;
		}
		case SE_ERR_DLLNOTFOUND:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n指定的动态连接库(DLL)无法找到。",
				"文件操作出错",MB_ICONSTOP|MB_OK);
			break;
		}
		case SE_ERR_OOM:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n没有足够的内存完成此操作。",
				"文件操作出错",MB_ICONSTOP|MB_OK);
			break;
		}
		case SE_ERR_SHARE:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n发生共享违例。",
				"文件操作出错",MB_ICONSTOP|MB_OK);
			break;
		}
		//////////////////////////////////////////
		case ERROR_DISK_FULL:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n磁盘上没有足够的空间。",
				"文件操作出错",MB_ICONSTOP|MB_OK);
			break;
		}
		//////////////////////////////////////////
		default:
		{
			CString strErrorCode;
			strErrorCode.Format("%d",nErrorCode);
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+": 不明错误。\r\n\r\n错误代码："+strErrorCode,
				"文件操作出错",MB_ICONSTOP|MB_OK);
		}
	}
}
//////////////////////////////////////////////////////////
void CNethiefDoc::OnToolbarNewfile()
{
	// TODO: Add your command handler code here
	if(GetCurDir()=="")
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	///////////////////////////////////////////////////
	CString strHostName=GetCurHostName();
	if(strHostName!="我的电脑")
	{
		CString strFilename="新建文件";
		int nCount=1;
		CString strExtCount;
		while(FindDirectory(strFilename)!=-1)
		{
			nCount++;
			strExtCount.Format(" (%d)",nCount);
			strFilename="新建文件"+strExtCount;
		}
		Remote_NewFile(m_Tree->GetSelectedItem(),strHostName,GetCurPath(),strFilename);
		return;
	}
	///////////////////////////////////////////////////
	CString strFilename="新建文件";
	CString strPathName=GetCurPath()+strFilename;
	int nCount=1;
	CString strExtCount;
	CFileFind f;
	while(f.FindFile(strPathName))
	{
		nCount++;
		strExtCount.Format(" (%d)",nCount);
		strPathName=GetCurPath()+strFilename+strExtCount;
	}
	/////////////////////////////////////////////
	HANDLE hFile=CreateFile(strPathName,NULL,
		FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		MessageErrorBox(GetLastError(),"无法新建文件",strFilename+strExtCount);
		return;
	}
	CloseHandle(hFile);
	/////////////////////////////////////////////
	SHFILEINFO shFileInfo;
	SHGetFileInfo(strPathName,0,&shFileInfo,sizeof(SHFILEINFO),
		SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME);
	int nImageIndex=m_ListImageList.Add(shFileInfo.hIcon);
	DestroyIcon(shFileInfo.hIcon);
	//加入文件图标和名称/////////////////////
	int nItemCount=m_List->GetItemCount();
	m_List->InsertItem(nItemCount,
		(LPCSTR)shFileInfo.szDisplayName,nImageIndex);
	//加入文件类型///////////////////////////
	m_List->SetItemText(nItemCount,2,(LPCSTR)shFileInfo.szTypeName);
	//加入文件大小///////////////////////////
	m_List->SetItemText(nItemCount,1,"0KB");
	//加入文件修改时间///////////////////////
	CTime refTime;
	f.FindFile(strPathName);
	f.FindNextFile();		
	f.GetLastWriteTime(refTime);
	CString strFileTime;
	strFileTime=refTime.Format("%y-%m-%d %H:%M");
	m_List->SetItemText(nItemCount,3,strFileTime);
	///////////////////////////////////////////
	m_List->EditLabel(nItemCount);
}

void CNethiefDoc::OnToolbarNewfolder()
{
	if(GetCurDir()=="")
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	///////////////////////////////////////////////////
	CString strHostName=GetCurHostName();
	if(strHostName!="我的电脑")
	{
		CString strFilename="新建文件夹";
		int nCount=1;
		CString strExtCount;
		while(FindDirectory(strFilename)!=-1)
		{
			nCount++;
			strExtCount.Format(" (%d)",nCount);
			strFilename="新建文件夹"+strExtCount;
		}
		Remote_NewFolder(m_Tree->GetSelectedItem(),strHostName,GetCurPath(),strFilename);
		return;
	}
	///////////////////////////////////////////////////
	CString strFilename="新建文件夹";
	CString strPathName=GetCurPath()+strFilename;
	int nCount=1;
	CString strExtCount;
	CFileFind f;
	while(f.FindFile(strPathName))
	{
		nCount++;
		strExtCount.Format(" (%d)",nCount);
		strPathName=GetCurPath()+strFilename+strExtCount;
	}
	/////////////////////////////////////////////
	BOOL bReturn=CreateDirectory(strPathName,NULL); 
	if(!bReturn)
	{
		MessageErrorBox(GetLastError(),"无法新建文件夹",strFilename+strExtCount);
		return;
	}
	/////////////////////////////////////////////
	SHFILEINFO shFileInfo;
	SHGetFileInfo(strPathName,0,&shFileInfo,sizeof(SHFILEINFO),
		SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME);
	int nImageIndex=m_ListImageList.Add(shFileInfo.hIcon);
	DestroyIcon(shFileInfo.hIcon);
	//加入文件图标和名称/////////////////////
	int nItemCount=m_List->GetItemCount();
	m_List->InsertItem(nItemCount,
		(LPCSTR)shFileInfo.szDisplayName,nImageIndex);
	//加入文件类型///////////////////////////
	m_List->SetItemText(nItemCount,2,(LPCSTR)shFileInfo.szTypeName);
	//加入文件修改时间///////////////////////
	CTime refTime;
	f.FindFile(strPathName);
	f.FindNextFile();		
	f.GetLastWriteTime(refTime);
	CString strFileTime;
	strFileTime=refTime.Format("%y-%m-%d %H:%M");
	m_List->SetItemText(nItemCount,3,strFileTime);
	///////////////////////////////////////////
	AddDirecoryItem(strFilename+strExtCount,m_Tree->GetSelectedItem());
	///////////////////////////////////////////
	m_List->EditLabel(nItemCount);
}

void CNethiefDoc::OnToolbarBack()
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(!(dwFlag&TI_ROOT))//如果不是根节点
	{
		HTREEITEM hParentItem=m_Tree->GetParentItem(hSelectItem);
		m_Tree->SelectItem(hParentItem);
	}
	else
	{
		MessageBeep(0xFFFFFFFF);
	}
}

void CNethiefDoc::OnToolbarRefresh()
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	NM_TREEVIEW NMTreeView;
	NMTreeView.itemNew.hItem=hSelectItem;
	LRESULT Result=0;
	OnTreeSelchanged(&NMTreeView,&Result);
}

void CNethiefDoc::OnToolbarFindfile()
{
	CString strHostName=GetCurHostName();
	if(strHostName=="")
		return;
	CToolBarFindFileDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		CString strHostName=GetCurHostName();
		if(strHostName!="我的电脑")
		{
			Remote_FindFile(strHostName,GetCurPath(),dlg.m_strEdit,dlg.m_bCheck);
			return;
		}

		BeginWaitCursor();
		///////////////////////////////
		CToolBarFindFileResultDlg dlg1;
		dlg1.m_StringArray.SetSize(0);
		SearchFile(GetCurPath(),dlg.m_strEdit,dlg.m_bCheck,
			&dlg1.m_StringArray);
		dlg1.DoModal();
		///////////////////////////////
		EndWaitCursor();
	}
}

void CNethiefDoc::SearchFile(CString strFindDir,CString strFindFile,BOOL bSearchSubdir,
							 CStringArray* pStringArray)
{
	if(bSearchSubdir)
	{
		CFileFind f;
		BOOL bExit=f.FindFile(strFindDir+"*.*");
		while(bExit)
		{
			bExit=f.FindNextFile();
			if(f.IsDirectory())
			{
				if(!f.IsDots())
				{
					SearchFile(f.GetFilePath()+"\\",strFindFile,
						bSearchSubdir,pStringArray); //目录文件非"."和"..",则用全路径调用自身
				}
			}
		}
		f.Close();
	}
	//////////////////////////////////////////////////////
	CFileFind f;
	BOOL bExit=f.FindFile(strFindDir+strFindFile);
	while(bExit)
	{
		bExit=f.FindNextFile();
		if(!f.IsDots())
		{
			pStringArray->Add(f.GetFilePath());//加入文件名称
		}
	}
	f.Close();
}

////////////复制,剪切,粘贴操作函数开始/////////////////////////////////////////////
void CNethiefDoc::OnToolbarCut(BOOL bIsTree)
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_ROOT||dwFlag&TI_HOST)
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	//////////////////////////////////////////
	CString strHostName=GetCurHostName();
	if(strHostName=="")
		return;
	if(CopyPathToClipboard(bIsTree))
	{
		m_dwFOFlags=FO_MOVE;
		m_strFOHostName=strHostName;
		UnsignCutDirecoryItem(bIsTree);//标记剪贴目录项
		if(bIsTree)
		{
			SignCutDirecoryItem("");
		}
		else
		{
			int nCount=m_List->GetSelectedCount();
			if(nCount==0)
				return;
			int nSelectItem=-1;
			for(int i=0;i<nCount;i++)
			{
				nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_SELECTED);
				if(m_List->GetItemText(nSelectItem,1)=="")
				{
					CString strFilename=m_List->GetItemText(nSelectItem,0);
					SignCutDirecoryItem(strFilename);
				}
			}
		}
	}
}

void CNethiefDoc::UnsignCutDirecoryItem(BOOL bIsTree)
{
	if(m_hCutParentItem!=NULL)
	{
		HTREEITEM hChildItem=m_Tree->GetChildItem(m_hCutParentItem);//得到它的子项
		while(hChildItem)//如果有子项
		{
			DWORD dwFlags=m_Tree->GetItemData(hChildItem);
			if(dwFlags&TI_CUT)//得到子项的Text
			{
				dwFlags&=~TI_CUT;
				m_Tree->SetItemData(hChildItem,dwFlags);
			}
			hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//得到下一个子项
		}
	}
	/////////////////////////////////////
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();
	if(bIsTree)
		m_hCutParentItem=m_Tree->GetParentItem(hSelectItem);
	else
		m_hCutParentItem=hSelectItem;

	DWORD dwFlags=m_Tree->GetItemData(m_hCutParentItem);
	if(dwFlags&TI_ROOT||dwFlags&TI_HOST)
	{
		m_hCutParentItem=NULL;
	}
}

void CNethiefDoc::SignCutDirecoryItem(CString strDirName)
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	if(strDirName=="")
	{
		DWORD dwFlags=m_Tree->GetItemData(hSelectItem);
		dwFlags|=TI_CUT;
		m_Tree->SetItemData(hSelectItem,dwFlags);
		return;
	}
	HTREEITEM hChildItem=m_Tree->GetChildItem(hSelectItem);//得到它的子项
	while(hChildItem)//如果有子项
	{
		if(m_Tree->GetItemText(hChildItem)==strDirName)//得到子项的Text
		{
			DWORD dwFlags=m_Tree->GetItemData(hChildItem);
			dwFlags|=TI_CUT;
			m_Tree->SetItemData(hChildItem,dwFlags);
			break;
		}
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//得到下一个子项
	}
}

BOOL CNethiefDoc::CopyPathToClipboard(BOOL bIsTree)
{
	CString strFrom;
	if(bIsTree)
	{
		strFrom=GetCurDir();
		int nStrLength=strFrom.GetLength();
		strFrom.GetBufferSetLength(nStrLength+1);
		strFrom.SetAt(nStrLength,'\0');
	}
	else
	{
		SetFileOperationPath(&strFrom);
	}
	if(strFrom=="")
		return FALSE;
	/////////////////////////////////////////
	if(AfxGetMainWnd()->OpenClipboard()) 
	{
		EmptyClipboard();
		int i=strFrom.GetLength();
		m_dwClipboardDataSize=i;
		HGLOBAL hGlobalMem = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,i);
		memcpy((LPSTR)GlobalLock(hGlobalMem),(LPCSTR)strFrom,i);
		GlobalUnlock(hGlobalMem);
		if(!SetClipboardData(m_dwNewClipboardFormat,hGlobalMem)) 
			GlobalFree(hGlobalMem);
		CloseClipboard();
		return TRUE;
	}
	return FALSE;
}

void CNethiefDoc::SetFileOperationPath(CString* pstrPath)
{
	int nCount=m_List->GetSelectedCount();
	if(nCount==0)
		return;
	int nSelectItem=-1;
	for(int i=0;i<nCount;i++)
	{
		nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_SELECTED);
		CString strFilename=m_List->GetItemText(nSelectItem,0);
		*pstrPath=*pstrPath+GetCurPath()+strFilename;

		int nStrLength=pstrPath->GetLength();
		pstrPath->GetBufferSetLength(nStrLength+1);
		pstrPath->SetAt(nStrLength,'\0');
	}
}

void CNethiefDoc::OnToolbarCopy(BOOL bIsTree)
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_ROOT||dwFlag&TI_HOST)//如果不是根节点
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	//////////////////////////////////////////
	CString strHostName=GetCurHostName();
	if(strHostName=="")
		return;
	if(CopyPathToClipboard(bIsTree))
	{
		m_dwFOFlags=FO_COPY;
		m_strFOHostName=strHostName;
	}
}


void CNethiefDoc::OnToolbarPaste()
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_ROOT||dwFlag&TI_HOST)//如果不是根节点
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	//////////////////////////////////////////
	CString strHostName=GetCurHostName();
	if(strHostName=="")
		return;
	if(m_dwFOFlags==NULL)
		return;
	if(!IsClipboardFormatAvailable(m_dwNewClipboardFormat))
		return;
	CString strFrom;
	if(AfxGetMainWnd()->OpenClipboard())
	{
		HGLOBAL hData = GetClipboardData(m_dwNewClipboardFormat);
		if(hData != NULL)
		{
			memcpy(strFrom.GetBufferSetLength(m_dwClipboardDataSize),
				GlobalLock(hData),m_dwClipboardDataSize);
			GlobalUnlock(hData);
		}
		CloseClipboard();
	}
	CString strTo=GetCurDir();
	int nStrLength=strTo.GetLength();
	strTo.GetBufferSetLength(nStrLength+1);
	strTo.SetAt(nStrLength,'\0');
	//////////////////////////////////////////////////////////////////
	//可能是:我的电脑->我的电脑,我的电脑->远程主机,远程主机->我的电脑,
	//同远程主机->同远程主机.
	//////////////////////////////////////////////////////////////////
	if(m_strFOHostName==strHostName && strHostName=="我的电脑")//我的电脑->我的电脑(本地操作)
	{
		SHFILEOPSTRUCT shFileOp;
		ZeroMemory(&shFileOp,sizeof(shFileOp));
		shFileOp.hwnd=NULL;
		shFileOp.wFunc=m_dwFOFlags;
		shFileOp.pFrom=strFrom;
		shFileOp.pTo=strTo;
		shFileOp.fFlags=FOF_ALLOWUNDO;
		if(!SHFileOperation(&shFileOp))
		{
			if(!shFileOp.fAnyOperationsAborted)
			{
				if(m_dwFOFlags==FO_MOVE)
				{
					DeleteCutDirecoryItem();
					m_dwFOFlags=NULL;
					if(AfxGetMainWnd()->OpenClipboard())
					{
						EmptyClipboard();
						CloseClipboard();
					}
				}
				OnToolbarRefresh();
			}
		}
		return;
	}
	if(m_strFOHostName=="我的电脑" && strHostName!="我的电脑")//我的电脑->远程主机(上传)
	{
		GetOperationObject(hSelectItem,strHostName,strFrom,GetCurPath(),m_dwFOFlags);
		return;
	}
	if(m_strFOHostName!="我的电脑" && strHostName=="我的电脑")//远程主机->我的电脑(下载)
	{
		Remote_GetOperationObject(hSelectItem,m_strFOHostName,strFrom,GetCurPath(),m_dwFOFlags);
		return;
	}
	if(m_strFOHostName==strHostName && strHostName!="我的电脑")//同远程主机->同远程主机(远程操作)
	{
		Remote_RemotePaste(m_strFOHostName,strFrom,strTo,m_dwFOFlags);
		return;
	}
}

void CNethiefDoc::DeleteCutDirecoryItem()
{
	if(m_hCutParentItem==NULL)
		return;
	HTREEITEM hChildItem=m_Tree->GetChildItem(m_hCutParentItem);//得到它的子项
	HTREEITEM hTempItem;
	while(hChildItem)//如果有子项
	{
		DWORD dwFlags=m_Tree->GetItemData(hChildItem);
		hTempItem=hChildItem;
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//得到下一个子项
		if(dwFlags&TI_CUT)//得到子项的Text
		{
			m_Tree->DeleteItem(hTempItem);
		}
	}
}

void CNethiefDoc::GetOperationObject(HTREEITEM hSelectItem,CString strHostName,CString strFrom,
									 CString strPath2,DWORD dwFOFlags)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_GetOperationObject_hSelectItem=hSelectItem;
	m_TM_GetOperationObject_strRemoteHostName=strHostName;
	CString strPath=(LPCSTR)strFrom;
	m_TM_GetOperationObject_strPath=strPath.Left(strPath.ReverseFind('\\')+1);
	m_TM_GetOperationObject_strPath2=strPath2;
	if(dwFOFlags==FO_COPY)
		m_TM_GetOperationObject_bIsCut=FALSE;
	else
		m_TM_GetOperationObject_bIsCut=TRUE;
	//////////////////////////////////////////////////////////////
	int nPathLen=m_TM_GetOperationObject_strPath.GetLength();
	DWORD dwParamSize=strFrom.GetLength();

	m_pFileSocket->m_ArrayBuffer.SetSize(0);
	DWORD dwPoint=0;
	LPCSTR szFilename;
	while(dwPoint<dwParamSize)
	{
		szFilename=(LPCSTR)strFrom+dwPoint;
		CFileFind f;
		BOOL bExist=f.FindFile(szFilename);
		if(bExist)
		{
			f.FindNextFile();
			if(f.IsDirectory())
			{
				SearchFile_GetOperationObject(szFilename,nPathLen,&m_pFileSocket->m_ArrayBuffer);
			}
			else
			{
				int nFilenameLen=strlen(szFilename);
				nFilenameLen-=nPathLen;
				int nPostion=m_pFileSocket->m_ArrayBuffer.GetSize();
				m_pFileSocket->m_ArrayBuffer.SetSize(nPostion+nFilenameLen+1);
				memcpy(&m_pFileSocket->m_ArrayBuffer[nPostion],szFilename+nPathLen,nFilenameLen+1);
			}
		}
		f.Close();
		dwPoint+=strlen(szFilename)+1;
	}
	//////////////////////////////////////////////////////////////
	DWORD dwSize=m_pFileSocket->m_ArrayBuffer.GetSize();
	if(dwSize==0)
		return;
	m_TM_GetOperationObject_nIndex=0;
	m_TM_GetOperationObject_StringArray.SetSize(0);

	dwPoint=0;
	LPCSTR szFrom=(LPCSTR)m_pFileSocket->m_ArrayBuffer.GetData();
	while(dwPoint<dwSize)
	{
		szFilename=szFrom+dwPoint;
		m_TM_GetOperationObject_StringArray.Add((LPCSTR)szFilename);
		dwPoint+=strlen(szFilename)+1;
	}
	//////////////////////////////////////////////////////////////
	//看看目的文件夹中有没有同名文件
	Remote_GetFileInfo(m_TM_GetOperationObject_strRemoteHostName,
		m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
		FALSE);//TRUE:下载,FALSE:上传
}

void CNethiefDoc::SearchFile_GetOperationObject(CString strFindDir,int nPathLen,
												CByteArray* pArrayBuffer)
{
	CFileFind f;
	BOOL bExist=f.FindFile(strFindDir+"\\*.*");
	while(bExist)
	{
		bExist=f.FindNextFile();
		CString strPathName=f.GetFilePath();
		if(!f.IsDots())
		{
			if(f.IsDirectory())
			{
				SearchFile_GetOperationObject(strPathName,nPathLen,pArrayBuffer);
			}
			else
			{
				LPCSTR szFilename=strPathName;
				int nFilenameLen=strlen(szFilename);
				nFilenameLen-=nPathLen;
				int nPostion=pArrayBuffer->GetSize();
				pArrayBuffer->SetSize(nPostion+nFilenameLen+1);
				memcpy(&(*pArrayBuffer)[nPostion],szFilename+nPathLen,nFilenameLen+1);
			}
		}
	}
}

////////////复制,剪切,粘贴操作函数结束/////////////////////////////////////////////
void CNethiefDoc::OnMenuFileRun()
{
	int nSelectItem=-1;
	nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_SELECTED);
	if(nSelectItem==-1)//如果没有选择项
		return;
	/////////////////////////////////////////////
	NM_LISTVIEW NMListView;
	NMListView.iItem=nSelectItem;
	LRESULT Result=0;
	OnListDblclk(&NMListView,&Result);//等于双击
}

void CNethiefDoc::OnToolbarRun()
{
	CString strPathName="";
	int nCount=m_List->GetSelectedCount();
	if(nCount!=0)
	{
		int nSelectItem=m_List->GetNextItem(-1,LVNI_SELECTED);
		if(m_List->GetItemText(nSelectItem,1)!="")//如果选的不是文件夹
		{
			CString strFilename=m_List->GetItemText(nSelectItem,0);
			strPathName=GetCurPath()+strFilename;
			///////////////////////////////////////////////////
			CString strHostName=GetCurHostName();
			if(strHostName!="我的电脑" && strHostName!="")
			{
				CToolBarRunRemoteDlg dlg;
				if(dlg.DoModal()==IDOK)
				{
					int nShowCmd;
					switch(dlg.m_nComboBox)
					{
						case 0://正常
						{
							nShowCmd=SW_SHOWNORMAL;
							break;
						}
						case 1://隐藏
						{
							nShowCmd=SW_HIDE;
							break;
						}
						case 2://最大化
						{
							nShowCmd=SW_SHOWMAXIMIZED;
							break;
						}
						case 3://最小化
						{
							nShowCmd=SW_SHOWMINNOACTIVE;
							break;
						}
					}
					Remote_RemoteRun(strHostName,strPathName,dlg.m_strEdit,nShowCmd);
				}
				return;
			}
		}
	}
	///////////////////////////////////////////////
	CToolBarRunDlg dlg;

	dlg.m_strPathName=strPathName;
	if(strPathName!="")
	{
		CString strExtName=strPathName.Right(4);
		strExtName.MakeUpper();
		if(strExtName==".EXE"||strExtName==".COM")
		{
			dlg.m_bIsRun=TRUE;
			dlg.m_strEdit1=strPathName;
		}
		else
		{
			dlg.m_bIsRun=FALSE;
			dlg.m_strEdit2=strPathName;
		}
	}
	if(dlg.DoModal()==IDOK)
	{
		CString strProgramPath=dlg.m_strEdit1;
		CString strParameter=dlg.m_strEdit2;
		CString strDirectory=strProgramPath.Left(strProgramPath.ReverseFind('\\')+1);

		ShellExecute(NULL,NULL,strProgramPath,strParameter,strDirectory,SW_SHOWNORMAL);
	}
}

void CNethiefDoc::OnTreeMenuFileRename()
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);//得到编辑标签的Flag
	if(dwFlag&TI_ROOT||dwFlag&TI_HOST||dwFlag&TI_DISKDRIVE)
	{//如果要编辑的标签不是文件夹节点
		MessageBeep(0xFFFFFFFF);//响一声
		return;
	}
	m_Tree->EditLabel(hSelectItem);
}

void CNethiefDoc::OnListMenuFileRename()
{
	int nSelectItem=-1;
	nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_SELECTED);
	if(nSelectItem==-1)//如果没有选择项
		return;
	m_List->EditLabel(nSelectItem);//编辑所选项的标签
}

void CNethiefDoc::OnToolbarView()
{
	int nCount=m_List->GetSelectedCount();
	if(nCount==0)//如果没有选择
		return;
	int nSelectItem=-1;
	for(int i=0;i<nCount;i++)
	{
		nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_SELECTED);
		if(m_List->GetItemText(nSelectItem,1)=="")//如果选的是文件夹
			return;
		CString strFilename=m_List->GetItemText(nSelectItem,0);
		CString strPathName=GetCurPath()+strFilename;
		////////////////////////////////////////////////////////////
		CString strHostName=GetCurHostName();
		if(strHostName!="我的电脑")
		{
			Remote_DownloadFile(strHostName,
				strPathName,m_strFileCacheDir+"\\"+strFilename,0,
				1,1,FALSE,2);
			return;
		}
		////////////////////////////////////////////////////////////
		strPathName="\""+strPathName+"\"";
		CString strDirectory=m_strFileViewer.Left(m_strFileViewer.ReverseFind('\\')+1);
		ShellExecute(NULL,NULL,m_strFileViewer,strPathName,strDirectory,SW_SHOWNORMAL);
	}
}

void CNethiefDoc::DeleteDirecoryItem(CString strDirName)
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	//////////////////////////////////////////////
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_REMOTE)
	{
		if(m_Tree->ItemHasChildren(hSelectItem))
		{
			if(!(dwFlag&TI_ONCEEXPAND))
				return;
		}
	}
	//////////////////////////////////////////////
	HTREEITEM hChildItem=m_Tree->GetChildItem(hSelectItem);//得到它的子项
	while(hChildItem)//如果有子项
	{
		if(m_Tree->GetItemText(hChildItem)==strDirName)//得到子项的Text
		{
			m_Tree->DeleteItem(hChildItem);
			break;
		}
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//得到下一个子项
	}
}

void CNethiefDoc::OnTreeToolbarDelete()
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_HOST&&dwFlag&TI_REMOTE)//如果是远程主机节点
	{
		OnToolbarDeletehost();//删除主机
		return;
	}
	if(dwFlag&TI_ROOT||dwFlag&TI_HOST||dwFlag&TI_DISKDRIVE)//如果不是根节点
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	//////////////////////////////////////////
	CString strFrom=GetCurDir();

	int nStrLength=strFrom.GetLength();
	strFrom.GetBufferSetLength(nStrLength+1);
	strFrom.SetAt(nStrLength,'\0');
	///////////////////////////////////////////////////
	CString strHostName=GetCurHostName();
	if(strHostName!="我的电脑")
	{
		Remote_DeleteFile(hSelectItem,strHostName,strFrom,FALSE);
		return;
	}
	///////////////////////////////////////////////////
	SHFILEOPSTRUCT shFileOp;
	ZeroMemory(&shFileOp,sizeof(shFileOp));
	int nFlags;
	int nReturn=GetAsyncKeyState(VK_SHIFT);
	if(nReturn!=-32767)//没按Shift
		nFlags=FOF_ALLOWUNDO;
	else//按了Shift
		nFlags=NULL;
	/////////////////////////////
	shFileOp.hwnd=NULL;
	shFileOp.wFunc=FO_DELETE;
	shFileOp.pFrom=strFrom;
	shFileOp.fFlags=nFlags;
	if(!SHFileOperation(&shFileOp))
	{
		if(!shFileOp.fAnyOperationsAborted)
			m_Tree->DeleteItem(hSelectItem);
	}
}

void CNethiefDoc::OnListToolbarDelete()
{
	int nCount=m_List->GetSelectedCount();
	if(nCount==0)//如果没有选择项
		return;
	////////////////////////////////////////
	CString strFrom;
	SetFileOperationPath(&strFrom);
	///////////////////////////////////////////////////
	CString strHostName=GetCurHostName();
	if(strHostName!="我的电脑")
	{
		Remote_DeleteFile(m_Tree->GetSelectedItem(),strHostName,strFrom,TRUE);
		return;
	}
	///////////////////////////////////////////////////
	SHFILEOPSTRUCT shFileOp;
	ZeroMemory(&shFileOp,sizeof(shFileOp));
	int nFlags;
	int nReturn=GetAsyncKeyState(VK_SHIFT);
	if(nReturn!=-32767)//没按Shift
		nFlags=FOF_ALLOWUNDO;
	else//按了Shift
		nFlags=NULL;
	/////////////////////////////
	shFileOp.hwnd=NULL;
	shFileOp.wFunc=FO_DELETE;
	shFileOp.pFrom=strFrom;
	shFileOp.fFlags=nFlags;
	if(!SHFileOperation(&shFileOp))
	{
		if(!shFileOp.fAnyOperationsAborted)
		{
			int nItemCount=m_List->GetItemCount();
			BOOL bLastItemDelete=FALSE;
			/////////////////////////////////////
			int nSelectItem=nItemCount-2;
			nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_BELOW|LVNI_SELECTED);
			if(nSelectItem!=-1)
			{
				bLastItemDelete=TRUE;
				nCount--;
			}
			/////////////////////////////////////
			nSelectItem=nItemCount-1;
			for(int i=0;i<nCount;i++)
			{
				nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_ABOVE|LVNI_SELECTED);
				////////////////////////////////////
				if(m_List->GetItemText(nSelectItem,1)=="")//删除的是文件夹
				{
					DeleteDirecoryItem(m_List->GetItemText(nSelectItem,0));
				}
				////////////////////////////////////
				m_List->DeleteItem(nSelectItem);
			}
			int nFocusItem=nSelectItem;
			if(bLastItemDelete)
			{
				nSelectItem=m_List->GetItemCount()-1;
				////////////////////////////////////
				if(m_List->GetItemText(nSelectItem,1)=="")//删除的是文件夹
				{
					DeleteDirecoryItem(m_List->GetItemText(nSelectItem,0));
				}
				////////////////////////////////////
				m_List->DeleteItem(nSelectItem);
			}
			nItemCount=m_List->GetItemCount();
			if(nFocusItem>nItemCount-1)
				nFocusItem=nItemCount-1;
			m_List->SetItemState(nFocusItem,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
		}
	}
}

void CNethiefDoc::OnToolbarProperty(BOOL bIsTree)
{
	if(bIsTree)
	{
		HTREEITEM hSelectItem=m_Tree->GetSelectedItem();
		DWORD dwFlag=m_Tree->GetItemData(hSelectItem);//得到编辑标签的Flag
		if(dwFlag&TI_ROOT||dwFlag&TI_HOST)
		{//如果要编辑的标签不是文件夹节点
			MessageBeep(0xFFFFFFFF);//响一声
			return;
		}

		BeginWaitCursor();
		if(GetCurDir().GetLength()==3)
		{
			CString strDiskDrive=GetCurDir();
			CString strHostName=GetCurHostName();
			if(strHostName!="我的电脑")
			{
				Remote_ShowProperty_Diskdrive(hSelectItem,strHostName,strDiskDrive);
				return;
			}
			///////////////////////////////////////////////////////
			CToolBarPropertyDiskdriveDlg dlg;

			CString strVolumeNameBuffer;     //得到名字
			DWORD dwVolumeSerialNumber;      //得到序列号
			if(!GetVolumeInformation(strDiskDrive,
				strVolumeNameBuffer.GetBufferSetLength(100),100,&dwVolumeSerialNumber,NULL,NULL,NULL,NULL))
			{
				MessageBox(AfxGetMainWnd()->m_hWnd,"无法显示驱动器属性 "+strDiskDrive+"。",
					"显示驱动器属性出错",MB_ICONSTOP|MB_OK);
				return;
			}
			strVolumeNameBuffer.MakeUpper();
			dlg.m_strName=strVolumeNameBuffer;
			dlg.m_strSerialNumber.Format("%04X-%04X",HIWORD(dwVolumeSerialNumber),LOWORD(dwVolumeSerialNumber));
			/////////////////////////////////////////
			DWORDLONG dwlTotalNumberOfBytes;
			DWORDLONG dwlTotalNumberOfUnusedBytes;
			DWORDLONG dwlTotalNumberOfUsedBytes;

			dwlTotalNumberOfBytes=GetDiskSpace(strDiskDrive,FALSE);
			dwlTotalNumberOfUnusedBytes=GetDiskSpace(strDiskDrive,TRUE);
			dwlTotalNumberOfUsedBytes=dwlTotalNumberOfBytes-dwlTotalNumberOfUnusedBytes;

			dlg.m_strTotal=ConverDWORDToCString(dwlTotalNumberOfBytes)+" 字节";
			dlg.m_strUnused=ConverDWORDToCString(dwlTotalNumberOfUnusedBytes)+" 字节";
			dlg.m_strUsed=ConverDWORDToCString(dwlTotalNumberOfUsedBytes)+" 字节";

			int nUsed=(INT)(dwlTotalNumberOfUsedBytes*1000/dwlTotalNumberOfBytes);
			int nUnused=1000-nUsed;
			CString strUsed;
			CString strUnused;
			strUsed.Format("%02d",nUsed);
			strUnused.Format("%02d",nUnused);

			dlg.m_strUsedPercent="."+strUsed.Right(1)+"%";
			dlg.m_strUsedPercent=strUsed.Mid(0,strUsed.GetLength()-1)+dlg.m_strUsedPercent;
			dlg.m_strUnusedPercent="."+strUnused.Right(1)+"%";
			dlg.m_strUnusedPercent=strUnused.Mid(0,strUnused.GetLength()-1)+dlg.m_strUnusedPercent;

			dlg.m_strHostName="我的电脑";

			int nImage,nSelectedImage;
			m_Tree->GetItemImage(hSelectItem,nImage,nSelectedImage);
			/////////////////////////////////////////////
			if(nImage==ICO_DRIVE_REMOVABLE)	//软盘驱动器
			{
				dlg.m_strType="软盘驱动器";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_REMOVABLE);
			}
			if(nImage==ICO_DRIVE_FIXED)		//硬盘驱动器
			{
				dlg.m_strType="硬盘驱动器";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_FIXED);
			}
			if(nImage==ICO_DRIVE_CDROM)		//光盘驱动器
			{
				dlg.m_strType="光盘驱动器";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_CDROM);
			}
			if(nImage==ICO_DRIVE_ZIP)	//可移动磁盘驱动器
			{
				dlg.m_strType="可移动磁盘驱动器";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_ZIP);
			}
			if(nImage==ICO_DRIVE_REMOTE)	//网络驱动器
			{
				dlg.m_strType="网络驱动器";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_REMOTE);
			}
			/////////////////////////////////////////////
			if(dlg.DoModal()==IDOK) 
			{
				if(strVolumeNameBuffer!=dlg.m_strName)
				{
					if(SetVolumeLabel(strDiskDrive,dlg.m_strName))
					{
						int nImage,nSelectedImage;
						m_Tree->GetItemImage(hSelectItem,nImage,nSelectedImage);
						if(nImage!=ICO_DRIVE_REMOVABLE && nImage!=ICO_DRIVE_ZIP)
						{
							CString strText=m_Tree->GetItemText(hSelectItem);
							strText=dlg.m_strName+" "+strText.Right(4);
							m_Tree->SetItemText(hSelectItem,strText);
						}
					}
					else
					{
						MessageBox(AfxGetMainWnd()->m_hWnd,"无法修改驱动器属性 "+strDiskDrive+"。",
							"修改驱动器属性出错",MB_ICONSTOP|MB_OK);
					}
				}
			}
		}
		else
		{
			CString strHostName=GetCurHostName();
			if(strHostName!="我的电脑")
			{
				CString strPath=GetCurDir();
				int nStrLength=strPath.GetLength();
				strPath.GetBufferSetLength(nStrLength+1);
				strPath.SetAt(nStrLength,'\0');

				Remote_ShowProperty_Multi(strHostName,strPath);
				return;
			}
			////////////////////////////////////////////
			int nFileCount=0;
			int nFolderCount=0;
			DWORD dwTotalSize=0;
			FolderStatistic(GetCurPath(),&nFileCount,
							 &nFolderCount,&dwTotalSize);
			CString strSize=ConverDWORDToCString(dwTotalSize)+" 字节";
			CString strStatistic;
			strStatistic.Format("%d 个文件，%d 个文件夹",nFileCount,nFolderCount);
			CToolBarPropertyMultiDlg dlg;
			dlg.m_strSize=strSize;
			dlg.m_strStatistic=strStatistic;
			dlg.m_strHostName="我的电脑";
			dlg.DoModal();
		}
		EndWaitCursor();
		return;
	}
	///////////////////////////////////////////
	int nCount=m_List->GetSelectedCount();
	if(nCount==0)//没选就返回
		return;
	if(nCount==1)
	{
		int nSelectItem=m_List->GetNextItem(-1,LVNI_SELECTED);
		if(m_List->GetItemText(nSelectItem,1)!="")//只选了一个项目并是文件
		{
			CString strFilename=GetCurPath()+m_List->GetItemText(nSelectItem,0);
			CString strHostName=GetCurHostName();
			if(strHostName!="我的电脑")
			{
				Remote_ShowProperty_Single(strHostName,strFilename);
				return;
			}
			///////////////////////////////////////////////////////
			CToolBarPropertySingleDlg dlg;

			SHFILEINFO shFileInfo;
			SHGetFileInfo(strFilename,0,&shFileInfo,sizeof(SHFILEINFO),
				SHGFI_ICON|SHGFI_LARGEICON);
			dlg.m_hIcon=shFileInfo.hIcon;

			dlg.m_Edit_strFilename=strFilename;
			dlg.m_strHostName="我的电脑";

			CFileStatus status;
			CFile::GetStatus(strFilename,status);
			dlg.m_strFileSize=ConverDWORDToCString(status.m_size)+" 字节";

			DWORD dwFileType;
			dwFileType=SHGetFileInfo(strFilename,NULL,NULL,NULL,SHGFI_EXETYPE);
			WORD wFileType;
			wFileType=LOWORD(dwFileType);
			CString strFileType=LOBYTE(wFileType);
			strFileType+=HIBYTE(wFileType);

			if(strFileType=="")
				dlg.m_strDataType="非可执行文件";
			if(strFileType=="NE")
				dlg.m_strDataType="Win16 NE 可执行文件";
			if(strFileType=="PE")
				dlg.m_strDataType="Win32 PE 可执行文件";
			if(strFileType=="MZ")
				dlg.m_strDataType="MS-DOS .EXE, .COM 或者 .BAT 文件";
			////////////////////////////////////////////
			dlg.m_strCTime=status.m_ctime.Format("%Y年%m月%d日 %H:%M:%S");
			dlg.m_strMTime=status.m_mtime.Format("%Y年%m月%d日 %H:%M:%S");
			dlg.m_strATime=status.m_atime.Format("%Y年%m月%d日");
			////////////////////////////////////////////
			if(status.m_attribute&FILE_ATTRIBUTE_READONLY)
				dlg.m_bIsReadOnly=TRUE;
			if(status.m_attribute&FILE_ATTRIBUTE_HIDDEN)
				dlg.m_bIsHide=TRUE;
			if(status.m_attribute&FILE_ATTRIBUTE_ARCHIVE)
				dlg.m_bIsArchive=TRUE;
			if(status.m_attribute&FILE_ATTRIBUTE_SYSTEM)
				dlg.m_bIsSystem=TRUE;
			////////////////////////////////////////////
			if(dlg.DoModal()==IDOK) 
			{
				DWORD dwFileAttributes=0;
				if(dlg.m_bIsArchive)
					dwFileAttributes|=FILE_ATTRIBUTE_ARCHIVE;
				if(dlg.m_bIsHide)
					dwFileAttributes|=FILE_ATTRIBUTE_HIDDEN;
				if(dlg.m_bIsReadOnly)
					dwFileAttributes|=FILE_ATTRIBUTE_READONLY;
				if(dlg.m_bIsSystem)
					dwFileAttributes|=FILE_ATTRIBUTE_SYSTEM;
				if(dwFileAttributes!=status.m_attribute)
				{
					if(!SetFileAttributes(strFilename,dwFileAttributes))
					{
						MessageBox(AfxGetMainWnd()->m_hWnd,"无法修改单文件属性 "+strFilename+"。",
							"修改单文件属性出错",MB_ICONSTOP|MB_OK);
					}
				}
			}
			DestroyIcon(shFileInfo.hIcon);
			return;
		}
	}
	/////只选了一个文件夹 or 选了多项///////////////////////////
	CString strHostName=GetCurHostName();
	if(strHostName!="我的电脑")
	{
		CString strPath;
		SetFileOperationPath(&strPath);
		Remote_ShowProperty_Multi(strHostName,strPath);
		return;
	}
	////////////////////////////////////////////
	BeginWaitCursor();

	int nFileCount=0;
	int nFolderCount=0;
	DWORD dwTotalSize=0;
	if(nCount==1)
		nFolderCount--;

	int nSelectItem=-1;
	for(int i=0;i<nCount;i++)
	{
		nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_SELECTED);
		CString strFilename=m_List->GetItemText(nSelectItem,0);
		CString strPathName=GetCurPath()+strFilename;
		if(m_List->GetItemText(nSelectItem,1)=="")//如果选的是文件夹
		{
			nFolderCount++;
			FolderStatistic(strPathName+"\\",&nFileCount,
							 &nFolderCount,&dwTotalSize);
		}
		else
		{
			nFileCount++;
			CFileStatus status;
			CFile::GetStatus(strPathName,status);
			dwTotalSize+=status.m_size;
		}
	}
	///////////////////////////////////////////
	CString strSize=ConverDWORDToCString(dwTotalSize)+" 字节";
	CString strStatistic;
	strStatistic.Format("%d 个文件，%d 个文件夹",nFileCount,nFolderCount);
	CToolBarPropertyMultiDlg dlg;
	dlg.m_strSize=strSize;
	dlg.m_strStatistic=strStatistic;
	dlg.m_strHostName="我的电脑";
	dlg.DoModal();

	EndWaitCursor();
}


void CNethiefDoc::FolderStatistic(CString strFindDir,int* nFileCount,
							 int* nFolderCount,DWORD* dwTotalSize)
{
	BOOL bExit;
	CFileFind f;
	bExit=f.FindFile(strFindDir+"*.*");
	if(GetLastError()==ERROR_NOT_READY)//如果读盘错误
	{
		int nReturn=MessageBox(AfxGetMainWnd()->m_hWnd,"无法访问 "+strFindDir+" 。\r\n\r\n设备没有准备好。",
			"文件管理器 - "+strFindDir,MB_ICONSTOP|MB_RETRYCANCEL);
		if(nReturn==IDRETRY)
		{
			FolderStatistic(strFindDir,nFileCount,nFolderCount,dwTotalSize);
			return;
		}
		else
		{
			return;
		}
	}
	while(bExit)
	{
		bExit=f.FindNextFile();
		CString strPathName=f.GetFilePath();

		if(!f.IsDots())
		{
			if(f.IsDirectory())
			{
				(*nFolderCount)++;
				FolderStatistic(strPathName+"\\",nFileCount,nFolderCount,dwTotalSize); //目录文件非"."和"..",则用全路径调用自身
			}
			else
			{
				(*nFileCount)++;
				(*dwTotalSize)+=f.GetLength();
			}
		}
	}
}

void CNethiefDoc::OnToolbarSelectall()
{
	int nCount=m_List->GetItemCount();
	for(int i=0;i<nCount;i++)
	{
		m_List->SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
	}
}

void CNethiefDoc::OnToolbarReverseselect()
{
	int nCount=m_List->GetItemCount();
	for(int i=0;i<nCount;i++)
	{
		if(m_List->GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
			m_List->SetItemState(i,~LVIS_SELECTED,LVIS_SELECTED);
		else
			m_List->SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
	}
}

void CNethiefDoc::OnUpdateIndicatorProgress(CCmdUI *pCmdUI)
{
	CString strText=m_pStatusBar->GetPaneText(m_pStatusBar->CommandToIndex(ID_SEPARATOR));
	if(strText==" ")
		m_pStatusBar->SetPaneText(m_pStatusBar->CommandToIndex(ID_SEPARATOR),
		m_strIndicatorStatus);
}

void CNethiefDoc::SetIndicatorRect()
{
	RECT ProgressRect;
	m_pStatusBar->GetItemRect(
		m_pStatusBar->CommandToIndex(ID_INDICATOR_PROGRESS),&ProgressRect);
	ProgressRect.top+=2;
	ProgressRect.bottom-=2;
	ProgressRect.left+=2;
	ProgressRect.right-=1;
	m_Progress.SetWindowPos(&CWnd::wndTop,ProgressRect.left,ProgressRect.top,ProgressRect.right-ProgressRect.left,
		ProgressRect.bottom-ProgressRect.top,0);
	//////////////////////////////////
	RECT OfflineRect;
	m_pStatusBar->GetItemRect(
		m_pStatusBar->CommandToIndex(ID_INDICATOR_OFFLINE),&OfflineRect);
	OfflineRect.top+=1;
	OfflineRect.bottom-=1;
	OfflineRect.left+=2;
	OfflineRect.right-=2;
	m_Static_Offline.SetWindowPos(&CWnd::wndTop,OfflineRect.left,OfflineRect.top,OfflineRect.right-OfflineRect.left,
		OfflineRect.bottom-OfflineRect.top,0);
	//////////////////////////////////
	RECT BusyRect;
	m_pStatusBar->GetItemRect(
		m_pStatusBar->CommandToIndex(ID_INDICATOR_BUSY),&BusyRect);
	BusyRect.top+=1;
	BusyRect.bottom-=1;
	BusyRect.left+=2;
	BusyRect.right-=2;
	m_Static_Busy.SetWindowPos(&CWnd::wndTop,BusyRect.left,BusyRect.top,BusyRect.right-BusyRect.left,
		BusyRect.bottom-BusyRect.top,0);
}

void CNethiefDoc::SetOfflineSign()
{
	CString strSubject;
	strSubject=((CNethiefApp*)AfxGetApp())->m_strVersion;

	NOTIFYICONDATA tnd;
	tnd.cbSize=sizeof(NOTIFYICONDATA);
	tnd.hWnd=AfxGetMainWnd()->m_hWnd;
	tnd.uID=IDR_MAINFRAME;
	tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
	tnd.uCallbackMessage=WM_SYSTRAY;
	if(m_bIsOnline)
	{
		m_Static_Offline.SetIcon(NULL);
		strSubject+=" - "+m_strServerNum;
		tnd.hIcon=AfxGetApp()->LoadIcon(IDI_SYSTRAY);
		CString strTip="网络神偷"+m_strServerNum;
		strcpy(tnd.szTip,(LPCSTR)strTip);
	}
	else
	{
		if(m_bLogoning)
		{
			strSubject+=" - [正在登录 FTP 服务器...]";
			tnd.hIcon=AfxGetApp()->LoadIcon(IDI_SYSTRAY);
			strcpy(tnd.szTip,"网络神偷[正在登录...]");
		}
		else
		{
			m_Static_Offline.SetIcon(m_hIcon_Offline);
			strSubject+=" - [脱机](未登录到 FTP 服务器)";
			tnd.hIcon=AfxGetApp()->LoadIcon(IDI_SYSTRAY1);
			strcpy(tnd.szTip,"网络神偷[脱机]");
		}
	}
	AfxGetMainWnd()->SetWindowText(strSubject);
	Shell_NotifyIcon(NIM_MODIFY,&tnd);
}

void CNethiefDoc::FlashIcon()
{
	NOTIFYICONDATA tnd;
	tnd.cbSize=sizeof(NOTIFYICONDATA);
	tnd.hWnd=AfxGetMainWnd()->m_hWnd;
	tnd.uID=IDR_MAINFRAME;
	tnd.uFlags=NIF_ICON;
	if(m_bFlashIcon)
		tnd.hIcon=AfxGetApp()->LoadIcon(IDI_SYSTRAY);
	else
		tnd.hIcon=AfxGetApp()->LoadIcon(IDI_SYSTRAY1);
	m_bFlashIcon=!m_bFlashIcon;
	Shell_NotifyIcon(NIM_MODIFY,&tnd);
}

void CNethiefDoc::SetBusyCursor(BOOL bBusy)
{
	if(bBusy)
	{
		m_Static_Busy.SetIcon(m_hIcon_Busy);
		m_bIsBusy=TRUE;
	}
	else
	{
		m_Static_Busy.SetIcon(NULL);
		m_bIsBusy=FALSE;
	}
}

void CNethiefDoc::SetIndicatorStatus(CString strIndicatorStatus)
{
	m_strIndicatorStatus=strIndicatorStatus;
	m_pStatusBar->SetPaneText(m_pStatusBar->CommandToIndex(ID_SEPARATOR),
		m_strIndicatorStatus);
}

CString CNethiefDoc::CharToCString(char* cText,UINT nSize)
{
	CString strText;
	memcpy(strText.GetBuffer(nSize),cText,nSize);
	strText.ReleaseBuffer();
	return strText;
}

CString CNethiefDoc::GetHostName(HTREEITEM hItem)
{
	CString strText=m_Tree->GetItemText(hItem);
	int nFind=strText.Find("[");
	if(nFind!=-1)
		strText=strText.Left(nFind-1);
	return strText;
}

BOOL CNethiefDoc::FindHost(CString strHostName,BOOL bIsHttpTunnel)
{
	HTREEITEM hChildItem=m_Tree->GetChildItem(m_Tree->GetRootItem());
	while(hChildItem)
	{
		DWORD dwFlag=m_Tree->GetItemData(hChildItem);
		if(dwFlag&TI_REMOTE)//是远程主机
		{
			if(strHostName==GetHostName(hChildItem))
			{
				BOOL bTunnel=FALSE;
				int nImage,nSelectedImage;
				m_Tree->GetItemImage(hChildItem,nImage,nSelectedImage);
				if(nImage==ICO_REMOTEHOST_HTTPTUNNEL || nImage==ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT)
				{
					bTunnel=TRUE;
				}
				if(bIsHttpTunnel==bTunnel)
					return TRUE;//找到指定主机
			}
		}
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);
	}
	return FALSE;//没找到指定主机
}

void CNethiefDoc::OnMenuConsoleLogon()
{
	if(m_bIsOnline)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd,"当前正在登录到 FTP 服务器，请在脱机后再试。","注销当前设置"
			,MB_ICONWARNING|MB_OK);
		return;
	}

	if(MessageBox(AfxGetMainWnd()->m_hWnd,"本命令将注销当前设置，使软件恢复到初始状态，所有数据都将丢失。\r\n\r\n你确定要这么做吗？","注销当前设置"
		,MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;

	((CNethiefApp*)AfxGetApp())->pCommunity->LogonCurrentSetting();

	MessageBox(AfxGetMainWnd()->m_hWnd,"注销完毕，软件已恢复到初始状态，请重新运行本软件。\r\n\r\n单击“确定”退出本软件。","注销当前设置"
		,MB_ICONINFORMATION|MB_OK);
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}

void CNethiefDoc::OnHelpUpdate()//在线升级
{
	CHelpUpdateDlg dlg;
	dlg.DoModal();
}

void CNethiefDoc::OnMenuNetOption()
{
	COptionDlg dlg;
	dlg.m_strViewerFilename=m_strFileViewer;
	dlg.m_bSelectIP=GetINIfileInt("Client Setting","IsSelectIP");
	if(dlg.DoModal()==IDOK)
	{
		m_strFileViewer=dlg.m_strViewerFilename;
		WriteINIfileInt("Client Setting","IsSelectIP",dlg.m_bSelectIP);
	}
}

void CNethiefDoc::OnMenuNetDownload()
{
	CDownloadDlg dlg;
	dlg.m_strURL=GetINIfileString("Client Setting","DownloadURL");
	dlg.m_strFilename=GetINIfileString("Client Setting","DownloadFilename");
	if(dlg.DoModal()==IDOK)
	{
		WriteINIfileString("Client Setting","DownloadURL",dlg.m_strURL);
		WriteINIfileString("Client Setting","DownloadFilename",dlg.m_strFilename);
	}
}

//////////////////INI文件操作函数/////////////////
int CNethiefDoc::GetINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //找到
	exeFullPath=exeFullPath+".INI";  //源文件名
	return GetPrivateProfileInt(lpSectionName,lpKeyName,-1,
		exeFullPath.GetBufferSetLength(MAX_PATH));
}

BOOL CNethiefDoc::WriteINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName, INT nValue)
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

CString CNethiefDoc::GetINIfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,	 //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //找到
	exeFullPath=exeFullPath+".INI";  //源文件名
	int nLength=400;
	///////////////////////////////////////////////////////////
	CString strString;           //用于保存读出的字符串
	GetPrivateProfileString(lpSectionName,lpKeyName,"",
		strString.GetBufferSetLength(nLength),nLength,
		exeFullPath.GetBufferSetLength(MAX_PATH));
	strString.ReleaseBuffer();
	return strString;
}

BOOL CNethiefDoc::WriteINIfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //找到
	exeFullPath=exeFullPath+".INI";  //源文件名
	return WritePrivateProfileString(lpSectionName,lpKeyName,lpString,
		exeFullPath.GetBufferSetLength(MAX_PATH));
}
//////////////////INI文件操作函数结束/////////////////////////
//////////////////网络工具栏消息响应//////////////////////////
void CNethiefDoc::OnToolbarBuildserver()//生成服务端
{
	CToolBarBuildServerDlg dlg;
	////////////////////////////////////////
	dlg.m_strKeyName=GetINIfileString("Server Setting","KeyName");
	if(dlg.m_strKeyName=="")
		dlg.m_strKeyName="Internet Explorer";
	dlg.m_strFilename=GetINIfileString("Server Setting","Filename");
	if(dlg.m_strFilename=="")
		dlg.m_strFilename="IExplorer.exe";
	dlg.m_strNote=GetINIfileString("Server Setting","Note");
	if(dlg.m_strNote=="")
		dlg.m_strNote="网络神偷服务端";
	dlg.m_strServerFilename=GetINIfileString("Server Setting","ServerFilename");
	if(dlg.m_strServerFilename=="")
	{
		CString exeFullPath;      //用于保存当前执行程序路径
		GetModuleFileName(NULL,   //得到路径
			exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
		exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('\\')+1);  //找到
		exeFullPath.ReleaseBuffer();
		dlg.m_strServerFilename=exeFullPath+"Nethief_Server.exe";
	}
	dlg.DoModal();
	////////////////////////////////////////
	WriteINIfileString("Server Setting","KeyName",dlg.m_strKeyName);
	WriteINIfileString("Server Setting","Filename",dlg.m_strFilename);
	WriteINIfileString("Server Setting","Note",dlg.m_strNote);
	WriteINIfileString("Server Setting","ServerFilename",dlg.m_strServerFilename);
}

void CNethiefDoc::OnToolbarDeletehost()//删除主机
{
	BeginWaitCursor();
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_REMOTE && dwFlag&TI_HOST)//必须是远程主机节点
	{
		int nImage,nSelectedImage;
		m_Tree->GetItemImage(hSelectItem,nImage,nSelectedImage);
		if(nImage==ICO_REMOTEHOST_DISCONNECT || nImage==ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT)//必须是未连接主机
		{
			if(hSelectItem!=m_hConnectHostItem)//必须不是正在连接的主机
			{
				m_Tree->DeleteItem(hSelectItem);
				EndWaitCursor();
				return;
			}
		}
	}
	MessageBeep(0xFFFFFFFF);
	EndWaitCursor();
}

void CNethiefDoc::OnToolbarDisconnect()
{
	BeginWaitCursor();
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_REMOTE && dwFlag&TI_HOST)//必须是远程主机节点
	{
		int nImage,nSelectedImage;
		m_Tree->GetItemImage(hSelectItem,nImage,nSelectedImage);
		if(nImage==ICO_REMOTEHOST || nImage==ICO_REMOTEHOST_HTTPTUNNEL)//必须是已连接主机
		{
			m_pFileSocket->DisconnectClient();
			//////////////////////////////////////////////
			m_Tree->Expand(hSelectItem,TVE_COLLAPSE);
			HTREEITEM hChildItem=m_Tree->GetChildItem(hSelectItem);//得到它的子项
			HTREEITEM hTempItem;
			while(hChildItem)//如果有子项
			{
				hTempItem=hChildItem;
				hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//得到下一个子项
				m_Tree->DeleteItem(hTempItem);//删除它
			}

			m_Tree->InsertItem("Temp",hSelectItem);//插入一个临时项
			if(nImage==ICO_REMOTEHOST)
				m_Tree->SetItemImage(hSelectItem,ICO_REMOTEHOST_DISCONNECT,ICO_REMOTEHOST_DISCONNECT);
			else
				m_Tree->SetItemImage(hSelectItem,ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT,ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT);

			CString strText=GetHostName(hSelectItem);
			m_Tree->SetItemText(hSelectItem,strText);
			EndWaitCursor();
			return;
		}
	}
	MessageBeep(0xFFFFFFFF);
	EndWaitCursor();
}

void CNethiefDoc::OnMenuNetUninstall()
{
	BeginWaitCursor();
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_REMOTE && dwFlag&TI_HOST)//必须是远程主机节点
	{
		int nImage,nSelectedImage;
		m_Tree->GetItemImage(hSelectItem,nImage,nSelectedImage);
		if(nImage==ICO_REMOTEHOST || nImage==ICO_REMOTEHOST_HTTPTUNNEL)//必须是连接主机
		{
			CString strHostName=GetHostName(hSelectItem);
			if(MessageBox(AfxGetMainWnd()->m_hWnd,
				"你确定要远程卸载服务端主机 ["+strHostName+"] 的服务端程序吗？",
				"远程卸载服务端",
				MB_ICONQUESTION|MB_YESNO)==IDYES)
			{
				Remote_RemoteUninstall(strHostName);
			}
			return;
		}
	}
	MessageBeep(0xFFFFFFFF);
	EndWaitCursor();
}

void CNethiefDoc::OnMenuNetNote()
{
	BeginWaitCursor();
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//得到当前选择的项
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_REMOTE && dwFlag&TI_HOST)//必须是远程主机节点
	{
		int nImage,nSelectedImage;
		m_Tree->GetItemImage(hSelectItem,nImage,nSelectedImage);
		if(nImage==ICO_REMOTEHOST || nImage==ICO_REMOTEHOST_HTTPTUNNEL)//必须是连接主机
		{
			Remote_ShowNote(GetHostName(hSelectItem));
			return;
		}
	}
	MessageBeep(0xFFFFFFFF);
	EndWaitCursor();
}

void CNethiefDoc::OnToolbarStop()
{
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就终止它
		{
			m_pFileSocket->m_bStop=TRUE;
			return;
		}
	}
	if(m_bIsBusy)//如果线程结束了,但实际却忙着,就...
	{
		AfxGetMainWnd()->KillTimer(1);
		m_pFileSocket->EndListen();
		m_Progress.SetPos(0);
		SetBusyCursor(FALSE);
		SetIndicatorStatus("当前工作被停止");
		m_hConnectHostItem=NULL;
		return;
	}
	MessageBeep(0xFFFFFFFF);
}

//*********************************************************************************//
void CNethiefDoc::ConnectHost(HTREEITEM hExpandItem)//连接远程主机
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	/////////////////////////////////////////////////////
	//客户端同时只能连接一个服务端的判断
	if(!(m_pFileSocket->m_hConnectSocket==NULL || m_pFileSocket->m_hConnectSocket==INVALID_SOCKET))//当前连接的服务端句柄不为空
	{
		SetIndicatorStatus("客户端同时只能连接一台服务端主机，请先断开与服务端主机 ["+m_pFileSocket->m_strHostName+"] 的连接");
		return;
	}
	/////////////////////////////////////////////////////
	m_hConnectHostItem=hExpandItem;	//保存这个项的句柄
	CString strClientName=m_Tree->GetItemText(hExpandItem);

	BOOL bIsHttpTunnel;
	int nImage,nSelectedImage;
	m_Tree->GetItemImage(hExpandItem,nImage,nSelectedImage);
	if(nImage==ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT)
		bIsHttpTunnel=TRUE;
	else
		bIsHttpTunnel=FALSE;
	/////////////////////////////////////////////
	pNOTIFY_CLIENT_CONNECT_THREAD_PARAM pNotifyClientConnectParam=new NOTIFY_CLIENT_CONNECT_THREAD_PARAM;
	pNotifyClientConnectParam->pFileSocket=m_pFileSocket;
	pNotifyClientConnectParam->strClientName=strClientName;
	pNotifyClientConnectParam->bIsHttpTunnel=bIsHttpTunnel;

	m_pFileSocketThread=AfxBeginThread(NotifyClientConnectProc,(LPVOID)pNotifyClientConnectParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在通知服务端主机 ["+strClientName+"] 开始连接本机...");
}

void CNethiefDoc::Remote_ExpandDirectory(HTREEITEM hExpandItem,CString strHostName,CString strPath)
{
	if(m_bIsBusy)
	{
		if(hExpandItem==m_hSelectItem)
		{
			m_bLastExpand=TRUE;
		}
		return;
	}
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_hExpandItem=hExpandItem;	//保存这个项的句柄
	m_strExpandHostName=strHostName;
	m_strExpandPath=strPath;

	CString strDir;
	int nLength=strPath.GetLength();
	if(nLength!=3)
		strDir=strPath.Left(nLength-1);
	else
		strDir=strPath;
	/////////////////////////////////////////////
	pEXPAND_DIRECTORY_THREAD_PARAM pExpandDirectoryThreadParam=new EXPAND_DIRECTORY_THREAD_PARAM;
	pExpandDirectoryThreadParam->pFileSocket=m_pFileSocket;
	pExpandDirectoryThreadParam->strHostName=strHostName;
	pExpandDirectoryThreadParam->strPath=strPath;

	m_pFileSocketThread=AfxBeginThread(Command_ExpandDirectoryProc,(LPVOID)pExpandDirectoryThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在展开文件夹 ["+strHostName+"]"+strDir+"...");
}

void CNethiefDoc::Remote_ShowFileList(HTREEITEM hSelectItem,CString strHostName,CString strPath)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_hSelectItem=hSelectItem;	//保存这个项的句柄
	SetCurDir(strHostName,strPath);
	/////////////////////////////////////////////
	pSHOW_FILELIST_THREAD_PARAM pShowFileListThreadParam=new SHOW_FILELIST_THREAD_PARAM;
	pShowFileListThreadParam->pFileSocket=m_pFileSocket;
	pShowFileListThreadParam->strHostName=strHostName;
	pShowFileListThreadParam->strPath=strPath;

	m_pFileSocketThread=AfxBeginThread(Command_ShowFileListProc,(LPVOID)pShowFileListThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在显示文件列表 ["+strHostName+"]"+GetCurDir()+"...");
}

void CNethiefDoc::Remote_RenameFile(HTREEITEM hSelectItem,CString strHostName,
									CString strPath,CString strFilename1,CString strFilename2,
									BOOL bIsDir,BOOL bIsList,int nEditItem)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}

	m_TM_RenameFile_hEditItem=hSelectItem;
	m_TM_RenameFile_strFullPath="["+strHostName+"]"+strPath+strFilename1;
	m_TM_RenameFile_strFilename2=strFilename2;
	m_TM_RenameFile_bIsDir=bIsDir;
	m_TM_RenameFile_bIsList=bIsList;
	if(bIsList)
		m_TM_RenameFile_nEditItem=nEditItem;
	/////////////////////////////////////////////
	pRENAME_FILE_THREAD_PARAM pRenameFileThreadParam=new RENAME_FILE_THREAD_PARAM;
	pRenameFileThreadParam->pFileSocket=m_pFileSocket;
	pRenameFileThreadParam->strHostName=strHostName;
	pRenameFileThreadParam->strPath=strPath;
	pRenameFileThreadParam->strFilename1=strFilename1;
	pRenameFileThreadParam->strFilename2=strFilename2;

	m_pFileSocketThread=AfxBeginThread(Command_RenameFileProc,(LPVOID)pRenameFileThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在重命名文件 ["+strHostName+"]"+strPath+strFilename1+" -> "+strFilename2+"...");
}

void CNethiefDoc::Remote_NewFile(HTREEITEM hSelectItem,CString strHostName,
		CString strPath,CString strFilename)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_NewFile_hSelectItem=hSelectItem;
	m_TM_NewFile_strFullPath="["+strHostName+"]"+strPath+strFilename;
	m_TM_NewFile_strFilename=strFilename;
	/////////////////////////////////////////////
	pNEW_FILE_THREAD_PARAM pNewFileThreadParam=new NEW_FILE_THREAD_PARAM;
	pNewFileThreadParam->pFileSocket=m_pFileSocket;
	pNewFileThreadParam->strHostName=strHostName;
	pNewFileThreadParam->strFullFilename=strPath+strFilename;

	m_pFileSocketThread=AfxBeginThread(Command_NewFileProc,(LPVOID)pNewFileThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在新建文件 ["+strHostName+"]"+strPath+strFilename+"...");
}

void CNethiefDoc::Remote_NewFolder(HTREEITEM hSelectItem,CString strHostName,
		CString strPath,CString strFilename)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_NewFolder_hSelectItem=hSelectItem;
	m_TM_NewFolder_strFullPath="["+strHostName+"]"+strPath+strFilename;
	m_TM_NewFolder_strFilename=strFilename;
	/////////////////////////////////////////////
	pNEW_FOLDER_THREAD_PARAM pNewFolderThreadParam=new NEW_FOLDER_THREAD_PARAM;
	pNewFolderThreadParam->pFileSocket=m_pFileSocket;
	pNewFolderThreadParam->strHostName=strHostName;
	pNewFolderThreadParam->strFullFilename=strPath+strFilename;

	m_pFileSocketThread=AfxBeginThread(Command_NewFolderProc,(LPVOID)pNewFolderThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在新建文件夹 ["+strHostName+"]"+strPath+strFilename+"...");
}

void CNethiefDoc::Remote_DeleteFile(HTREEITEM hSelectItem,CString strHostName,
									CString strFrom,BOOL bIsList)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_DeleteFile_hSelectItem=hSelectItem;
	m_TM_DeleteFile_strFullPath="["+strHostName+"]"+(LPCSTR)strFrom;
	m_TM_DeleteFile_bIsList=bIsList;
	/////////////////////////////////////////////
	pDELETE_FILE_THREAD_PARAM pDeleteFileThreadParam=new DELETE_FILE_THREAD_PARAM;
	pDeleteFileThreadParam->pFileSocket=m_pFileSocket;
	pDeleteFileThreadParam->strHostName=strHostName;
	pDeleteFileThreadParam->strFrom=strFrom;

	m_pFileSocketThread=AfxBeginThread(Command_DeleteFileProc,(LPVOID)pDeleteFileThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在删除文件 ["+strHostName+"]"+(LPCSTR)strFrom+"...");
}

void CNethiefDoc::Remote_RemotePaste(CString strHostName,
									 CString strFrom,CString strTo,DWORD dwFOFlags)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_RemotePaste_strFullPath="["+strHostName+"]"+(LPCSTR)strFrom;
	/////////////////////////////////////////////
	pREMOTE_PASTE_THREAD_PARAM pRemotePasteThreadParam=new REMOTE_PASTE_THREAD_PARAM;
	pRemotePasteThreadParam->pFileSocket=m_pFileSocket;
	pRemotePasteThreadParam->strHostName=strHostName;
	pRemotePasteThreadParam->strFrom=strFrom;
	pRemotePasteThreadParam->strTo=strTo;
	pRemotePasteThreadParam->dwFOFlags=dwFOFlags;

	m_pFileSocketThread=AfxBeginThread(Command_RemotePasteProc,(LPVOID)pRemotePasteThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	CString strText;
	if(dwFOFlags==FO_COPY)
		strText="复制";
	else
		strText="移动";
	SetIndicatorStatus("正在"+strText+"文件 ["+strHostName+"]"+(LPCSTR)strFrom+" -> ["+strHostName+"]"+(LPCSTR)strTo+"...");
}

void CNethiefDoc::Remote_GetOperationObject(HTREEITEM hSelectItem,CString strHostName,CString strFrom,
											CString strPath2,DWORD dwFOFlags)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_GetOperationObject_hSelectItem=hSelectItem;
	m_TM_GetOperationObject_strRemoteHostName=strHostName;
	CString strPath=(LPCSTR)strFrom;
	m_TM_GetOperationObject_strPath=strPath.Left(strPath.ReverseFind('\\')+1);
	m_TM_GetOperationObject_strPath2=strPath2;
	if(dwFOFlags==FO_COPY)
		m_TM_GetOperationObject_bIsCut=FALSE;
	else
		m_TM_GetOperationObject_bIsCut=TRUE;
	m_TM_GetOperationObject_strFullPath="["+strHostName+"]"+(LPCSTR)strFrom;
	/////////////////////////////////////////////
	pGET_OPERATION_OBJECT_THREAD_PARAM pGetOperationObjectThreadParam=new GET_OPERATION_OBJECT_THREAD_PARAM;
	pGetOperationObjectThreadParam->pFileSocket=m_pFileSocket;
	pGetOperationObjectThreadParam->strHostName=strHostName;
	pGetOperationObjectThreadParam->strFrom=strFrom;

	m_pFileSocketThread=AfxBeginThread(Command_GetOperationObjectProc,(LPVOID)pGetOperationObjectThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在获得操作对象 ["+strHostName+"]"+(LPCSTR)strFrom+"...");
}

void CNethiefDoc::Remote_DownloadFile(CString strHostName,
									  CString strFilename1,CString strFilename2,DWORD dwStartPosition,
									  int nCurNum,int nTotalNum,BOOL bIsCut,int nControlCode)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_DownloadFile_strFilename2=strFilename2;
	m_TM_DownloadFile_bIsCut=bIsCut;
	m_TM_DownloadFile_nControlCode=nControlCode;
	/////////////////////////////////////////////
	pDOWNLOAD_FILE_THREAD_PARAM pDownloadFileThreadParam=new DOWNLOAD_FILE_THREAD_PARAM;
	pDownloadFileThreadParam->pFileSocket=m_pFileSocket;
	pDownloadFileThreadParam->strHostName=strHostName;
	pDownloadFileThreadParam->strFilename1=strFilename1;
	pDownloadFileThreadParam->strFilename2=strFilename2;
	pDownloadFileThreadParam->dwStartPosition=dwStartPosition;
	pDownloadFileThreadParam->bIsCut=bIsCut;

	m_pFileSocketThread=AfxBeginThread(Command_DownloadFileProc,(LPVOID)pDownloadFileThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);

	CString strText;
	strText.Format("(%d/%d)",nCurNum,nTotalNum);
	CString strText2;
	if(bIsCut)
		strText2="移动";
	else
		strText2="复制";
	SetIndicatorStatus("0% "+strText+"正在"+strText2+"文件 ["+strHostName+"]"+strFilename1+" -> [我的电脑]"+strFilename2+"...");
}

void CNethiefDoc::Remote_UploadFile(CString strHostName,
									  CString strFilename1,CString strFilename2,DWORD dwStartPosition,
									  int nCurNum,int nTotalNum,BOOL bIsCut)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_UploadFile_bIsCut=bIsCut;
	/////////////////////////////////////////////
	pUPLOAD_FILE_THREAD_PARAM pUploadFileThreadParam=new UPLOAD_FILE_THREAD_PARAM;
	pUploadFileThreadParam->pFileSocket=m_pFileSocket;
	pUploadFileThreadParam->strHostName=strHostName;
	pUploadFileThreadParam->strFilename1=strFilename1;
	pUploadFileThreadParam->strFilename2=strFilename2;
	pUploadFileThreadParam->dwStartPosition=dwStartPosition;
	pUploadFileThreadParam->bIsCut=bIsCut;

	m_pFileSocketThread=AfxBeginThread(Command_UploadFileProc,(LPVOID)pUploadFileThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);

	CString strText;
	strText.Format("(%d/%d)",nCurNum,nTotalNum);
	CString strText2;
	if(bIsCut)
		strText2="移动";
	else
		strText2="复制";
	SetIndicatorStatus("0% "+strText+"正在"+strText2+"文件 [我的电脑]"+strFilename1+" -> ["+strHostName+"]"+strFilename2+"...");
}

void CNethiefDoc::Remote_ShowProperty_Multi(CString strHostName,CString strPath)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_ShowPropertyMulti_strHostName=strHostName;
	/////////////////////////////////////////////
	pSHOW_PROPERTY_MULTI_THREAD_PARAM pShowPropertyMultiThreadParam=new SHOW_PROPERTY_MULTI_THREAD_PARAM;
	pShowPropertyMultiThreadParam->pFileSocket=m_pFileSocket;
	pShowPropertyMultiThreadParam->strHostName=strHostName;
	pShowPropertyMultiThreadParam->strPath=strPath;

	m_pFileSocketThread=AfxBeginThread(Command_ShowPropertyMultiProc,(LPVOID)pShowPropertyMultiThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在显示多文件属性 ["+strHostName+"]"+(LPCSTR)strPath+"...");
}

void CNethiefDoc::Remote_ShowProperty_Single(CString strHostName,CString strFilename)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_ShowPropertySingle_strHostName=strHostName;
	m_TM_ShowPropertySingle_strFilename=strFilename;
	/////////////////////////////////////////////
	pSHOW_PROPERTY_SINGLE_THREAD_PARAM pShowPropertySingleThreadParam=new SHOW_PROPERTY_SINGLE_THREAD_PARAM;
	pShowPropertySingleThreadParam->pFileSocket=m_pFileSocket;
	pShowPropertySingleThreadParam->strHostName=strHostName;
	pShowPropertySingleThreadParam->strFilename=strFilename;

	m_pFileSocketThread=AfxBeginThread(Command_ShowPropertySingleProc,(LPVOID)pShowPropertySingleThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在显示单文件属性 ["+strHostName+"]"+strFilename+"...");
}

void CNethiefDoc::Remote_ModifyProperty_Single(CString strHostName,
											   CString strFilename,DWORD dwFileAttributes)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_ModifyPropertySingle_strFullPath="["+strHostName+"]"+strFilename;
	/////////////////////////////////////////////
	pMODIFY_PROPERTY_SINGLE_THREAD_PARAM pModifyPropertySingleThreadParam=new MODIFY_PROPERTY_SINGLE_THREAD_PARAM;
	pModifyPropertySingleThreadParam->pFileSocket=m_pFileSocket;
	pModifyPropertySingleThreadParam->strHostName=strHostName;
	pModifyPropertySingleThreadParam->strFilename=strFilename;
	pModifyPropertySingleThreadParam->dwFileAttributes=dwFileAttributes;

	m_pFileSocketThread=AfxBeginThread(Command_ModifyPropertySingleProc,(LPVOID)pModifyPropertySingleThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在修改单文件属性 ["+strHostName+"]"+strFilename+"...");
}

void CNethiefDoc::Remote_ShowProperty_Diskdrive(HTREEITEM hSelectItem,CString strHostName,CString strDiskDrive)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_ShowPropertyDiskdrive_hSelectItem=hSelectItem;
	m_TM_ShowPropertyDiskdrive_strHostName=strHostName;
	m_TM_ShowPropertyDiskdrive_strFullPath="["+strHostName+"]"+strDiskDrive;
	/////////////////////////////////////////////
	pSHOW_PROPERTY_DISKDRIVE_THREAD_PARAM pShowPropertyDiskdriveThreadParam=new SHOW_PROPERTY_DISKDRIVE_THREAD_PARAM;
	pShowPropertyDiskdriveThreadParam->pFileSocket=m_pFileSocket;
	pShowPropertyDiskdriveThreadParam->strHostName=strHostName;
	pShowPropertyDiskdriveThreadParam->strDiskDrive=strDiskDrive;

	m_pFileSocketThread=AfxBeginThread(Command_ShowPropertyDiskdriveProc,(LPVOID)pShowPropertyDiskdriveThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在显示驱动器属性 ["+strHostName+"]"+strDiskDrive+"...");
}

void CNethiefDoc::Remote_ModifyProperty_Diskdrive(HTREEITEM hSelectItem,CString strHostName,
												  CString strDiskDrive,CString strVolumeName)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_ModifyPropertyDiskdrive_hSelectItem=hSelectItem;
	m_TM_ModifyPropertyDiskdrive_strVolumeName=strVolumeName;
	m_TM_ModifyPropertyDiskdrive_strFullPath="["+strHostName+"]"+strDiskDrive;
	/////////////////////////////////////////////
	pMODIFY_PROPERTY_DISKDRIVE_THREAD_PARAM pModifyPropertyDiskdriveThreadParam=new MODIFY_PROPERTY_DISKDRIVE_THREAD_PARAM;
	pModifyPropertyDiskdriveThreadParam->pFileSocket=m_pFileSocket;
	pModifyPropertyDiskdriveThreadParam->strHostName=strHostName;
	pModifyPropertyDiskdriveThreadParam->strDiskDrive=strDiskDrive;
	pModifyPropertyDiskdriveThreadParam->strVolumeName=strVolumeName;

	m_pFileSocketThread=AfxBeginThread(Command_ModifyPropertyDiskdriveProc,(LPVOID)pModifyPropertyDiskdriveThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在修改驱动器属性 ["+strHostName+"]"+strDiskDrive+"...");
}

void CNethiefDoc::Remote_RemoteRun(CString strHostName,
								   CString strFilename,CString strParam,int nShowCmd)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_RemoteRun_strFullPath="["+strHostName+"]"+strFilename;
	/////////////////////////////////////////////
	pREMOTE_RUN_THREAD_PARAM pRemoteRunThreadParam=new REMOTE_RUN_THREAD_PARAM;
	pRemoteRunThreadParam->pFileSocket=m_pFileSocket;
	pRemoteRunThreadParam->strHostName=strHostName;
	pRemoteRunThreadParam->strFilename=strFilename;
	pRemoteRunThreadParam->strParam=strParam;
	pRemoteRunThreadParam->nShowCmd=nShowCmd;

	m_pFileSocketThread=AfxBeginThread(Command_RemoteRunProc,(LPVOID)pRemoteRunThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在远程运行文件 ["+strHostName+"]"+strFilename+"...");
}

void CNethiefDoc::Remote_RemoteUninstall(CString strHostName)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	/////////////////////////////////////////////
	pREMOTE_UNINSTALL_THREAD_PARAM pRemoteUninstallThreadParam=new REMOTE_UNINSTALL_THREAD_PARAM;
	pRemoteUninstallThreadParam->pFileSocket=m_pFileSocket;
	pRemoteUninstallThreadParam->strHostName=strHostName;

	m_pFileSocketThread=AfxBeginThread(Command_RemoteUninstallProc,(LPVOID)pRemoteUninstallThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在远程卸载服务端主机 ["+strHostName+"] 的服务端程序...");
}

void CNethiefDoc::Remote_FindFile(CString strHostName,CString strFindDir,
								  CString strFindFile,BOOL bSearchSubdir)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	/////////////////////////////////////////////
	pFIND_FILE_THREAD_PARAM pFindFileThreadParam=new FIND_FILE_THREAD_PARAM;
	pFindFileThreadParam->pFileSocket=m_pFileSocket;
	pFindFileThreadParam->strHostName=strHostName;
	pFindFileThreadParam->strFindDir=strFindDir;
	pFindFileThreadParam->strFindFile=strFindFile;
	pFindFileThreadParam->bSearchSubdir=bSearchSubdir;

	m_pFileSocketThread=AfxBeginThread(Command_FindFileProc,(LPVOID)pFindFileThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在查找文件 ["+strHostName+"]"+strFindDir+strFindFile+"...");
}

void CNethiefDoc::Remote_ShowNote(CString strHostName)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	/////////////////////////////////////////////
	pSHOW_NOTE_THREAD_PARAM pShowNoteThreadParam=new SHOW_NOTE_THREAD_PARAM;
	pShowNoteThreadParam->pFileSocket=m_pFileSocket;
	pShowNoteThreadParam->strHostName=strHostName;

	m_pFileSocketThread=AfxBeginThread(Command_ShowNoteProc,(LPVOID)pShowNoteThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在显示服务端主机 ["+strHostName+"] 的服务端注释...");
}

void CNethiefDoc::Remote_GetFileInfo(CString strHostName,
									 CString strFilename,BOOL bIsDownloadFile)
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_TM_GetFileInfo_bIsDownloadFile=bIsDownloadFile;//是下载还是上传
	/////////////////////////////////////////////
	pGET_FILE_INFO_THREAD_PARAM pGetFileInfoThreadParam=new GET_FILE_INFO_THREAD_PARAM;
	pGetFileInfoThreadParam->pFileSocket=m_pFileSocket;
	pGetFileInfoThreadParam->strHostName=strHostName;
	pGetFileInfoThreadParam->strFilename=strFilename;

	m_pFileSocketThread=AfxBeginThread(Command_GetFileInfoProc,(LPVOID)pGetFileInfoThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("正在得到文件信息 ["+strHostName+"]"+strFilename+"...");
}

//*********************************************************************************//
void CNethiefDoc::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==1)//等待服务端连接超时计数
	{
		m_nCount++;//计数器加一
		if(m_nCount>=70)//如果大于70秒,超时了
		{
			AfxGetMainWnd()->KillTimer(1);

			m_pFileSocket->EndListen();

			PlaySound(MAKEINTRESOURCE(IDR_SYSTEM),
				AfxGetInstanceHandle(),SND_RESOURCE|SND_ASYNC);
			m_Progress.SetPos(0);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("连接失败，没有监听到服务端主机 ["+m_pFileSocket->m_strClientName+"] 的连接，可能对方当前没有在线");
			m_hConnectHostItem=NULL;
		}
		else
		{
			m_Progress.SetPos((FLOAT)m_nCount/(FLOAT)70*PROGRESS_RANGE);
		}
		return;
	}
	if(nIDEvent==2)//身份验证超时计数
	{
		m_nCount2++;//计数器加一
		if(m_nCount2>=30)//等待服务端的身份验证信息超过了30秒,超时了
		{
			if(m_pFileSocketThread!=NULL)//如果线程对象不为空
			{
				unsigned long ExitCode;
				GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
				if(ExitCode==STILL_ACTIVE)//如果线程没结束就终止它
				{
					TerminateThread(m_pFileSocketThread->m_hThread,0);//终止身份验证线程
					AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_IDVERIFY,0);//发身份验证失败的消息
				}
			}
		}
		return;
	}
	if(nIDEvent==3)//下载驱动器信息超时计数
	{
		m_nCount3++;//计数器加一
		if(m_nCount3>=30)//下载驱动器信息超过了30秒,超时了
		{
			if(m_pFileSocketThread!=NULL)//如果线程对象不为空
			{
				unsigned long ExitCode;
				GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
				if(ExitCode==STILL_ACTIVE)//如果线程没结束就终止它
				{
					TerminateThread(m_pFileSocketThread->m_hThread,0);//终止下载驱动器信息线程
					AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_DOWNLOADDISKDRIVEINFO,0);//发下载驱动器信息失败的消息
				}
			}
		}
		return;
	}
}

void CNethiefDoc::OnSocketAccept()
{
	AfxGetMainWnd()->KillTimer(1);

	m_pFileSocket->OnSocketAccept();
	m_pFileSocket->EndListen();

	m_Progress.SetPos(0);
	////////////////////////////////////////////////////////
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	m_pFileSocketThread=AfxBeginThread(Command_IDVerifyProc,(LPVOID)NULL);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetIndicatorStatus("正在验证身份...");
	m_nCount2=0;//身份验证超时计数器清零
	AfxGetMainWnd()->SetTimer(2,1000,NULL);//开始计算身份验证超时
}

/////////////////////////////////////////////////////////////////
UINT NotifyClientConnectProc(LPVOID pParam)
{
	pNOTIFY_CLIENT_CONNECT_THREAD_PARAM pNotifyClientConnectParam=(pNOTIFY_CLIENT_CONNECT_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pNotifyClientConnectParam->pFileSocket;
	CString			strClientName=pNotifyClientConnectParam->strClientName;
	BOOL			bIsHttpTunnel=pNotifyClientConnectParam->bIsHttpTunnel;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->NotifyClientConnect(strClientName,bIsHttpTunnel);
	return 0;
}

UINT Command_IDVerifyProc(LPVOID pParam)
{
	((CNethiefApp*)AfxGetApp())->pFileSocket->Command_IDVerify();
	return 0;
}

UINT Command_DownloadDiskDriveInfoProc(LPVOID pParam)
{
	((CNethiefApp*)AfxGetApp())->pFileSocket->Command_DownloadDiskDriveInfo();
	return 0;
}

UINT Command_ExpandDirectoryProc(LPVOID pParam)
{
	pEXPAND_DIRECTORY_THREAD_PARAM pExpandDirectoryThreadParam=(pEXPAND_DIRECTORY_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pExpandDirectoryThreadParam->pFileSocket;
	CString			strHostName=pExpandDirectoryThreadParam->strHostName;
	CString			strPath=pExpandDirectoryThreadParam->strPath;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_ExpandDirectory(strHostName,strPath);
	return 0;
}

UINT Command_ShowFileListProc(LPVOID pParam)
{
	pSHOW_FILELIST_THREAD_PARAM pShowFileListThreadParam=(pSHOW_FILELIST_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pShowFileListThreadParam->pFileSocket;
	CString			strHostName=pShowFileListThreadParam->strHostName;
	CString			strPath=pShowFileListThreadParam->strPath;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_ShowFileList(strHostName,strPath);
	return 0;
}

UINT Command_RenameFileProc(LPVOID pParam)
{
	pRENAME_FILE_THREAD_PARAM pRenameFileThreadParam=(pRENAME_FILE_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pRenameFileThreadParam->pFileSocket;
	CString			strHostName=pRenameFileThreadParam->strHostName;
	CString			strPath=pRenameFileThreadParam->strPath;
	CString			strFilename1=pRenameFileThreadParam->strFilename1;
	CString			strFilename2=pRenameFileThreadParam->strFilename2;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_RenameFile(strHostName,strPath,strFilename1,strFilename2);
	return 0;
}

UINT Command_NewFileProc(LPVOID pParam)
{
	pNEW_FILE_THREAD_PARAM pNewFileThreadParam=(pNEW_FILE_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pNewFileThreadParam->pFileSocket;
	CString			strHostName=pNewFileThreadParam->strHostName;
	CString			strFullFilename=pNewFileThreadParam->strFullFilename;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_NewFile(strHostName,strFullFilename);
	return 0;
}

UINT Command_NewFolderProc(LPVOID pParam)
{
	pNEW_FOLDER_THREAD_PARAM pNewFolderThreadParam=(pNEW_FOLDER_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pNewFolderThreadParam->pFileSocket;
	CString			strHostName=pNewFolderThreadParam->strHostName;
	CString			strFullFilename=pNewFolderThreadParam->strFullFilename;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_NewFolder(strHostName,strFullFilename);
	return 0;
}

UINT Command_DeleteFileProc(LPVOID pParam)
{
	pDELETE_FILE_THREAD_PARAM pDeleteFileThreadParam=(pDELETE_FILE_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pDeleteFileThreadParam->pFileSocket;
	CString			strHostName=pDeleteFileThreadParam->strHostName;
	CString			strFrom=pDeleteFileThreadParam->strFrom;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_DeleteFile(strHostName,strFrom);
	return 0;
}

UINT Command_RemotePasteProc(LPVOID pParam)
{
	pREMOTE_PASTE_THREAD_PARAM pRemotePasteThreadParam=(pREMOTE_PASTE_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pRemotePasteThreadParam->pFileSocket;
	CString			strHostName=pRemotePasteThreadParam->strHostName;
	CString			strFrom=pRemotePasteThreadParam->strFrom;
	CString			strTo=pRemotePasteThreadParam->strTo;
	DWORD			dwFOFlags=pRemotePasteThreadParam->dwFOFlags;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_RemotePaste(strHostName,strFrom,strTo,dwFOFlags);
	return 0;
}

UINT Command_GetOperationObjectProc(LPVOID pParam)
{
	pGET_OPERATION_OBJECT_THREAD_PARAM pGetOperationObjectThreadParam=(pGET_OPERATION_OBJECT_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pGetOperationObjectThreadParam->pFileSocket;
	CString			strHostName=pGetOperationObjectThreadParam->strHostName;
	CString			strFrom=pGetOperationObjectThreadParam->strFrom;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_GetOperationObject(strHostName,strFrom);
	return 0;
}

UINT Command_DownloadFileProc(LPVOID pParam)
{
	pDOWNLOAD_FILE_THREAD_PARAM pDownloadFileThreadParam=(pDOWNLOAD_FILE_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pDownloadFileThreadParam->pFileSocket;
	CString			strHostName=pDownloadFileThreadParam->strHostName;
	CString			strFilename1=pDownloadFileThreadParam->strFilename1;
	CString			strFilename2=pDownloadFileThreadParam->strFilename2;
	DWORD			dwStartPosition=pDownloadFileThreadParam->dwStartPosition;
	BOOL			bIsCut=pDownloadFileThreadParam->bIsCut;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_DownloadFile(strHostName,strFilename1,strFilename2,
		dwStartPosition,bIsCut);
	return 0;
}

UINT Command_UploadFileProc(LPVOID pParam)
{
	pUPLOAD_FILE_THREAD_PARAM pUploadFileThreadParam=(pUPLOAD_FILE_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pUploadFileThreadParam->pFileSocket;
	CString			strHostName=pUploadFileThreadParam->strHostName;
	CString			strFilename1=pUploadFileThreadParam->strFilename1;
	CString			strFilename2=pUploadFileThreadParam->strFilename2;
	DWORD			dwStartPosition=pUploadFileThreadParam->dwStartPosition;
	BOOL			bIsCut=pUploadFileThreadParam->bIsCut;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_UploadFile(strHostName,strFilename1,strFilename2,
		dwStartPosition,bIsCut);
	return 0;
}

UINT Command_ShowPropertyMultiProc(LPVOID pParam)
{
	pSHOW_PROPERTY_MULTI_THREAD_PARAM pShowPropertyMultiThreadParam=(pSHOW_PROPERTY_MULTI_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pShowPropertyMultiThreadParam->pFileSocket;
	CString			strHostName=pShowPropertyMultiThreadParam->strHostName;
	CString			strPath=pShowPropertyMultiThreadParam->strPath;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_ShowProperty_Multi(strHostName,strPath);
	return 0;
}

UINT Command_ShowPropertySingleProc(LPVOID pParam)
{
	pSHOW_PROPERTY_SINGLE_THREAD_PARAM pShowPropertySingleThreadParam=(pSHOW_PROPERTY_SINGLE_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pShowPropertySingleThreadParam->pFileSocket;
	CString			strHostName=pShowPropertySingleThreadParam->strHostName;
	CString			strFilename=pShowPropertySingleThreadParam->strFilename;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_ShowProperty_Single(strHostName,strFilename);
	return 0;
}

UINT Command_ModifyPropertySingleProc(LPVOID pParam)
{
	pMODIFY_PROPERTY_SINGLE_THREAD_PARAM pModifyPropertySingleThreadParam=(pMODIFY_PROPERTY_SINGLE_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pModifyPropertySingleThreadParam->pFileSocket;
	CString			strHostName=pModifyPropertySingleThreadParam->strHostName;
	CString			strFilename=pModifyPropertySingleThreadParam->strFilename;
	DWORD			dwFileAttributes=pModifyPropertySingleThreadParam->dwFileAttributes;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_ModifyProperty_Single(strHostName,strFilename,dwFileAttributes);
	return 0;
}

UINT Command_ShowPropertyDiskdriveProc(LPVOID pParam)
{
	pSHOW_PROPERTY_DISKDRIVE_THREAD_PARAM pShowPropertyDiskdriveThreadParam=(pSHOW_PROPERTY_DISKDRIVE_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pShowPropertyDiskdriveThreadParam->pFileSocket;
	CString			strHostName=pShowPropertyDiskdriveThreadParam->strHostName;
	CString			strDiskDrive=pShowPropertyDiskdriveThreadParam->strDiskDrive;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_ShowProperty_Diskdrive(strHostName,strDiskDrive);
	return 0;
}

UINT Command_ModifyPropertyDiskdriveProc(LPVOID pParam)
{
	pMODIFY_PROPERTY_DISKDRIVE_THREAD_PARAM pModifyPropertyDiskdriveThreadParam=(pMODIFY_PROPERTY_DISKDRIVE_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pModifyPropertyDiskdriveThreadParam->pFileSocket;
	CString			strHostName=pModifyPropertyDiskdriveThreadParam->strHostName;
	CString			strDiskDrive=pModifyPropertyDiskdriveThreadParam->strDiskDrive;
	CString			strVolumeName=pModifyPropertyDiskdriveThreadParam->strVolumeName;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_ModifyProperty_Diskdrive(strHostName,strDiskDrive,strVolumeName);
	return 0;
}

UINT Command_RemoteRunProc(LPVOID pParam)
{
	pREMOTE_RUN_THREAD_PARAM pRemoteRunThreadParam=(pREMOTE_RUN_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pRemoteRunThreadParam->pFileSocket;
	CString			strHostName=pRemoteRunThreadParam->strHostName;
	CString			strFilename=pRemoteRunThreadParam->strFilename;
	CString			strParam=pRemoteRunThreadParam->strParam;
	int				nShowCmd=pRemoteRunThreadParam->nShowCmd;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_RemoteRun(strHostName,strFilename,strParam,nShowCmd);
	return 0;
}

UINT Command_RemoteUninstallProc(LPVOID pParam)
{
	pREMOTE_UNINSTALL_THREAD_PARAM pRemoteUninstallThreadParam=(pREMOTE_UNINSTALL_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pRemoteUninstallThreadParam->pFileSocket;
	CString			strHostName=pRemoteUninstallThreadParam->strHostName;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_RemoteUninstall(strHostName);
	return 0;
}

UINT Command_FindFileProc(LPVOID pParam)
{
	pFIND_FILE_THREAD_PARAM pFindFileThreadParam=(pFIND_FILE_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pFindFileThreadParam->pFileSocket;
	CString			strHostName=pFindFileThreadParam->strHostName;
	CString			strFindDir=pFindFileThreadParam->strFindDir;
	CString			strFindFile=pFindFileThreadParam->strFindFile;
	BOOL			bSearchSubdir=pFindFileThreadParam->bSearchSubdir;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_FindFile(strHostName,strFindDir,strFindFile,bSearchSubdir);
	return 0;
}

UINT Command_ShowNoteProc(LPVOID pParam)
{
	pSHOW_NOTE_THREAD_PARAM pShowNoteThreadParam=(pSHOW_NOTE_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pShowNoteThreadParam->pFileSocket;
	CString			strHostName=pShowNoteThreadParam->strHostName;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_ShowNote(strHostName);
	return 0;
}

UINT Command_GetFileInfoProc(LPVOID pParam)
{
	pGET_FILE_INFO_THREAD_PARAM pGetFileInfoThreadParam=(pGET_FILE_INFO_THREAD_PARAM)pParam;
	CFileSocket*	pFileSocket=pGetFileInfoThreadParam->pFileSocket;
	CString			strHostName=pGetFileInfoThreadParam->strHostName;
	CString			strFilename=pGetFileInfoThreadParam->strFilename;
	delete pParam;
	//////////////////////////////////////////////
	pFileSocket->Command_GetFileInfo(strHostName,strFilename);
	return 0;
}

/////////////////////////////////////////////////////////////////
void CNethiefDoc::OnTM_NotifyConnect(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("未登录到 FTP 服务器，无法通知服务端主机 ["+m_pFileSocket->m_strClientName+"] 开始连接本机，请在联机后再试");
			m_hConnectHostItem=NULL;
			break;
		}
		case 1://完成
		{
			if(m_pFileSocket->BeginListen())
			{
				m_nCount=0;
				AfxGetMainWnd()->SetTimer(1,1000,NULL);
				SetIndicatorStatus("正在等待服务端主机 ["+m_pFileSocket->m_strClientName+"] 的连接，请稍候...");
			}
			else//如果开始监听本机的TCP 60000端口失败
			{
				SetBusyCursor(FALSE);
				SetIndicatorStatus("绑定本机的TCP 60000端口失败");
				m_hConnectHostItem=NULL;
				MessageBox(AfxGetMainWnd()->m_hWnd,"无法绑定本机的TCP 60000端口。\r\n\r\n详细情况请参见 FAQ.txt(常见问题解答) 之问题15。",
				"绑定端口出错",MB_ICONSTOP|MB_OK);
			}
			break;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			m_hConnectHostItem=NULL;
			break;
		}
	}
}

void CNethiefDoc::OnTM_IDVerify(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			AfxGetMainWnd()->KillTimer(2);//停止计算身份验证超时

			m_pFileSocket->DisconnectClient();
			PlaySound(MAKEINTRESOURCE(IDR_SYSTEM),
				AfxGetInstanceHandle(),SND_RESOURCE|SND_ASYNC);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("由 ["+m_pFileSocket->m_strAddress+"] 下载身份验证信息失败");
			m_hConnectHostItem=NULL;
			break;
		}
		case 1://完成
		{
			AfxGetMainWnd()->KillTimer(2);//停止计算身份验证超时

			RECV_IDVERIFY_INFO RecvIDVerifyInfo;
			memcpy(&RecvIDVerifyInfo,m_pFileSocket->m_ArrayBuffer.GetData(),sizeof(RecvIDVerifyInfo));

			CString strSoftwareName;
			CString strHostName;
			strSoftwareName=CharToCString(RecvIDVerifyInfo.cSoftwareName,sizeof(RecvIDVerifyInfo.cSoftwareName));
			strHostName=CharToCString(RecvIDVerifyInfo.cHostName,sizeof(RecvIDVerifyInfo.cHostName));
			if(strSoftwareName=="Nethief_Server")
			{			
#if ONLINE_TEST
				if(strHostName==m_pFileSocket->m_strClientName)
#endif
				{
					m_Progress.OffsetPos(PROGRESS_RANGE/3);
					if(m_pFileSocketThread!=NULL)
					{
						Sleep(50);
						unsigned long ExitCode;
						GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
						if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
							return;
					}
					m_pFileSocketThread=AfxBeginThread(Command_DownloadDiskDriveInfoProc,(LPVOID)NULL);
					m_pFileSocketThread->m_bAutoDelete=FALSE;
					/////////////////////////////////////////////
					SetIndicatorStatus("正在下载驱动器信息...");
					m_nCount3=0;//下载驱动器信息超时计数器清零
					AfxGetMainWnd()->SetTimer(3,1000,NULL);//开始计算下载驱动器信息超时
					break;
				}
			}
			///////////////////////////////////////////////////
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			m_pFileSocket->DisconnectClient();
			PlaySound(MAKEINTRESOURCE(IDR_SYSTEM),
				AfxGetInstanceHandle(),SND_RESOURCE|SND_ASYNC);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("身份验证错误，对方不是服务端主机 ["+m_pFileSocket->m_strClientName+"]，而是 ["+m_pFileSocket->m_strAddress+"]，连接被拒绝");
			m_hConnectHostItem=NULL;
			break;
		}
		case 2://被停止
		{
			AfxGetMainWnd()->KillTimer(2);//停止计算身份验证超时

			m_pFileSocket->DisconnectClient();
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			m_hConnectHostItem=NULL;
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_DownloadDiskDriveInfo(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			AfxGetMainWnd()->KillTimer(3);//停止计算下载驱动器信息超时

			m_pFileSocket->DisconnectClient();
			PlaySound(MAKEINTRESOURCE(IDR_SYSTEM),
				AfxGetInstanceHandle(),SND_RESOURCE|SND_ASYNC);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("由 ["+m_pFileSocket->m_strAddress+"] 下载驱动器信息失败");
			m_hConnectHostItem=NULL;
			break;
		}
		case 1://完成
		{
			AfxGetMainWnd()->KillTimer(3);//停止计算下载驱动器信息超时

			m_Tree->DeleteItem(m_Tree->GetChildItem(m_hConnectHostItem));

			RECV_DISKDRIVE_INFO RecvDiskDriveInfo;
			DWORD dwSize=m_pFileSocket->m_ArrayBuffer.GetSize();
			int nIndex='A';
			int nIndex2='C';
			for(DWORD i=0;i<dwSize;i+=sizeof(RecvDiskDriveInfo))
			{
				CString strItemName;
				DWORD dwFlag;
				int nImage;

				memcpy(&RecvDiskDriveInfo,&m_pFileSocket->m_ArrayBuffer[i],sizeof(RecvDiskDriveInfo));
				switch(RecvDiskDriveInfo.dwType)
				{
					case DRIVE_REMOVABLE://软盘驱动器
					{
						if(nIndex2=='C')//是软驱还是ZIP驱
						{
							strItemName="3.5 软盘 ("+(CString)nIndex+":)";
							nImage=ICO_DRIVE_REMOVABLE;
							nIndex++;
						}
						else
						{
							strItemName="可移动磁盘 ("+(CString)nIndex2+":)";
							nImage=ICO_DRIVE_ZIP;
							nIndex2++;
						}
						/////////////////////////////////////////////////
						dwFlag=TI_DISKDRIVE_REMOVABLE|TI_DISKDRIVE|TI_REMOTE;
						break;
					}
					case DRIVE_FIXED://硬盘驱动器
					{
						strItemName=CharToCString(RecvDiskDriveInfo.cVolumeName,sizeof(RecvDiskDriveInfo.cVolumeName));
						if(strItemName!="")
						{
							SHFILEINFO shFileInfo;
							SHGetFileInfo(strItemName,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
								SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME);
							strItemName=(LPCSTR)shFileInfo.szDisplayName;
						}
						strItemName=strItemName+" ("+(CString)nIndex2+":)";
						nImage=ICO_DRIVE_FIXED;
						nIndex2++;
						dwFlag=TI_DISKDRIVE_FIXED|TI_DISKDRIVE|TI_REMOTE;
						break;
					}
					case DRIVE_CDROM://光盘驱动器
					{
						strItemName=" ("+(CString)nIndex2+":)";
						nImage=ICO_DRIVE_CDROM;
						nIndex2++;
						dwFlag=TI_DISKDRIVE_CDROM|TI_DISKDRIVE|TI_REMOTE;
						break;
					}
					case DRIVE_REMOTE:
					{
						strItemName=CharToCString(RecvDiskDriveInfo.cVolumeName,sizeof(RecvDiskDriveInfo.cVolumeName));
						nImage=ICO_DRIVE_REMOTE;
						nIndex2++;
						dwFlag=TI_DISKDRIVE_FIXED|TI_DISKDRIVE|TI_REMOTE;
						break;
					}
				}
				HTREEITEM hChildItem=m_Tree->InsertItem(strItemName,
					nImage,nImage,m_hConnectHostItem);
				m_Tree->SetItemData(hChildItem,dwFlag);
				m_Tree->InsertItem("Temp",hChildItem);
			}
			///////////////////////////////////////////////////
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			m_Tree->SetItemText(m_hConnectHostItem,m_Tree->GetItemText(m_hConnectHostItem)+" ["+m_pFileSocket->m_strAddress+"]");

			int nImage,nSelectedImage;
			m_Tree->GetItemImage(m_hConnectHostItem,nImage,nSelectedImage);
			if(nImage==ICO_REMOTEHOST_DISCONNECT)
				m_Tree->SetItemImage(m_hConnectHostItem,ICO_REMOTEHOST,ICO_REMOTEHOST);
			else
				m_Tree->SetItemImage(m_hConnectHostItem,ICO_REMOTEHOST_HTTPTUNNEL,ICO_REMOTEHOST_HTTPTUNNEL);

			m_Tree->Expand(m_hConnectHostItem,TVE_EXPAND);
			
			PlaySound(MAKEINTRESOURCE(IDR_GLOBAL),
				AfxGetInstanceHandle(),SND_RESOURCE|SND_ASYNC);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("服务端主机 ["+m_pFileSocket->m_strClientName+"] 连接成功");
			m_hConnectHostItem=NULL;
			break;
		}
		case 2://被停止
		{
			AfxGetMainWnd()->KillTimer(3);//停止计算下载驱动器信息超时

			m_pFileSocket->DisconnectClient();
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			m_hConnectHostItem=NULL;
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_ExpandDirectory(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("展开文件夹失败");
			break;
		}
		case 1://完成
		{
			DWORD dwFlag=m_Tree->GetItemData(m_hExpandItem);
			if(dwFlag&TI_DISKDRIVE_REMOVABLE||dwFlag&TI_DISKDRIVE_CDROM)
				m_Tree->SetItemData(m_hExpandItem,dwFlag|TI_ONCEACCESS);
			///////////////////////////////////////////////////////////
			DWORD dwSize=m_pFileSocket->m_ArrayBuffer.GetSize();
			LPDWORD pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(dwSize==sizeof(DWORD)&&*pdwErrorCode==0)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("展开文件夹失败");

				int nRet=MessageBox(AfxGetMainWnd()->m_hWnd,"无法访问 ["+m_strExpandHostName+"]"+m_strExpandPath+" 。\r\n\r\n设备没有准备好。",
					"文件管理器 - "+m_strExpandPath,MB_ICONSTOP|MB_RETRYCANCEL);
				if(nRet==IDRETRY)
					m_Tree->Expand(m_hExpandItem,TVE_EXPAND);
				return;
			}
			HTREEITEM hChildItem=m_Tree->GetChildItem(m_hExpandItem);
			if(!hChildItem)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("展开文件夹失败");
				return;
			}
			m_Tree->SetRedraw(FALSE);
			m_Tree->DeleteItem(hChildItem);
			dwFlag=m_Tree->GetItemData(m_hExpandItem);
			m_Tree->SetItemData(m_hExpandItem,dwFlag|TI_ONCEEXPAND);

			DWORD dwPoint=0;
			LPSTR szDirName;
			BYTE byHasChildren;
			while(dwPoint<dwSize)
			{				
				szDirName=(char*)&m_pFileSocket->m_ArrayBuffer[dwPoint];
				dwPoint+=strlen(szDirName)+1;
				memcpy(&byHasChildren,&m_pFileSocket->m_ArrayBuffer[dwPoint],sizeof(byHasChildren));
				dwPoint+=sizeof(byHasChildren);
	
				SHFILEINFO shFileInfo;
				SHGetFileInfo(szDirName,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME);

				HTREEITEM hChildItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
					ICO_FOLDER,ICO_FOLDER_OPEN,m_hExpandItem,TVI_SORT);
				m_Tree->SetItemData(hChildItem,TI_REMOTE);
				if(byHasChildren)
					m_Tree->InsertItem("Temp",hChildItem);
			}
			///////////////////////////////////////////////////
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			m_Tree->Expand(m_hExpandItem,TVE_EXPAND);
			m_Tree->SetRedraw(TRUE);
			SetBusyCursor(FALSE);
			if(m_bLastExpand)
			{
				m_bLastExpand=FALSE;
				SetIndicatorStatus(m_strIndicatorShowFileList);
			}
			else
			{
				SetIndicatorStatus("展开文件夹成功");
			}
			if(m_bLastSelect)
			{
				SelectDirecoryItem(m_strSelectName);
			}
			break;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_ShowFileList(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("显示文件列表失败");
			break;
		}
		case 1://完成
		{
			HTREEITEM hSelectItem=m_Tree->GetSelectedItem();
			if(m_hSelectItem!=hSelectItem)
			{
				SetBusyCursor(FALSE);
				SetIndicatorStatus("当前文件夹改变");
				m_hSelectItem=NULL;
				m_Progress.SetPos(0);
				m_List->DeleteAllItems();
				return;
			}
			DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
			if(dwFlag&TI_DISKDRIVE_REMOVABLE||dwFlag&TI_DISKDRIVE_CDROM)
				m_Tree->SetItemData(hSelectItem,dwFlag|TI_ONCEACCESS);
			///////////////////////////////////////////////////////////////
			DWORD dwSize=m_pFileSocket->m_ArrayBuffer.GetSize();
			LPDWORD pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(dwSize==sizeof(DWORD)&&*pdwErrorCode==0)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("显示文件列表失败");

				int nRet=MessageBox(AfxGetMainWnd()->m_hWnd,"无法访问 ["+GetCurHostName()+"]"+GetCurPath()+" 。\r\n\r\n设备没有准备好。",
					"文件管理器 - "+GetCurPath(),MB_ICONSTOP|MB_RETRYCANCEL);
				if(nRet==IDRETRY)
				{
					NM_TREEVIEW NMTreeView;
					NMTreeView.itemNew.hItem=m_hSelectItem;
					m_hSelectItem=NULL;
					LRESULT Result=0;
					OnTreeSelchanged(&NMTreeView,&Result);
				}
				else
				{
					m_List->DeleteAllItems();
					m_hSelectItem=NULL;
					m_bLastExpand=FALSE;
				}
				return;
			}

			DWORD dwTotalFileSize=0;
			int nCount=0;
			m_pListView->SetRedraw(FALSE);
			m_List->DeleteAllItems();

			DWORD dwPoint=0;
			LPSTR szFilename;
			DWORD dwFileLength;
			FILETIME filetime;
			while(TRUE)//清空列表映象列表
			{
				HICON hIcon=m_ListImageList.ExtractIcon(0);
				if(hIcon==NULL)
					break;
				DestroyIcon(hIcon);
				m_ListImageList.Remove(0);
			}
			///////////////////////////////////////////////////////////////////////////////
			while(dwPoint<dwSize)
			{
				szFilename=(char*)&m_pFileSocket->m_ArrayBuffer[dwPoint];
				dwPoint+=strlen(szFilename)+1;
				memcpy(&dwFileLength,&m_pFileSocket->m_ArrayBuffer[dwPoint],sizeof(dwFileLength));
				dwPoint+=sizeof(dwFileLength);

				SHFILEINFO shFileInfo;
				if(dwFileLength==0xFFFFFFFF)//是文件夹
				{
					SHGetFileInfo(szFilename,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
						SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME);

					//对于"隐藏已知文件类型的扩展名"的处理//////////
					CString strInput1=szFilename;//原始文件名
					CString strInput2=shFileInfo.szDisplayName;//处理后的文件名
					CString strOutput;//正确的可以显示的文件名

					int nPos=strInput1.ReverseFind('\\');
					if(nPos!=-1)//如果原始文件名中有路径,去掉路径只留文件名
						strInput1=strInput1.Mid(nPos+1);

					if(strInput1.GetLength()!=strInput2.GetLength())//用户设置了"隐藏已知文件类型的扩展名"
						strOutput=strInput1;//显示原始文件名
					else
						strOutput=strInput2;//显示处理后的文件名
					////////////////////////////////////////////////
					m_ListImageList.Add(m_hIcon_Folder);//添加文件夹图标到列表映象列表里
					m_List->InsertItem(nCount,strOutput,nCount);
					m_List->SetItemText(nCount,2,"文件夹");
				}
				else//是文件
				{
					SHGetFileInfo(szFilename,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
						SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME);
					CString strDisplayName;
					CString strTypeName;

					if((CString)shFileInfo.szTypeName!="")//不是回收站里的文件
					{
						strTypeName=(LPCSTR)shFileInfo.szTypeName;

						//对于"隐藏已知文件类型的扩展名"的处理//////////
						CString strInput1=szFilename;//原始文件名
						CString strInput2=shFileInfo.szDisplayName;//处理后的文件名

						int nPos=strInput1.ReverseFind('\\');
						if(nPos!=-1)//如果原始文件名中有路径,去掉路径只留文件名
							strInput1=strInput1.Mid(nPos+1);

						if(strInput1.GetLength()!=strInput2.GetLength())//用户设置了"隐藏已知文件类型的扩展名"
							strDisplayName=strInput1;//显示原始文件名
						else
							strDisplayName=strInput2;//显示处理后的文件名
						////////////////////////////////////////////////
					}
					else//是回收站里的文件
					{
						strDisplayName=(LPCSTR)shFileInfo.szDisplayName;

						int nPos=strDisplayName.Find(".");
						if(nPos==-1)
							strTypeName="文件";
						else
							strTypeName=strDisplayName.Mid(nPos+1)+" 文件";
					}
					//////////////////////////////////////////////////////////
					CString strExtension=strDisplayName.Right(4);//得到扩展名
					strExtension.MakeUpper();
					if(strExtension==".EXE")//是EXE文件
					{
						//得到传送Bitmap信息
						pRECV_BITMAP_INFO pstRecvBitmapInfoMask=(pRECV_BITMAP_INFO)&m_pFileSocket->m_ArrayBuffer[dwPoint];
						dwPoint+=sizeof(RECV_BITMAP_INFO);

						//得到Bitmap数据
						BYTE* pbyBitsMask=new BYTE[pstRecvBitmapInfoMask->dwBitsCount];
						memcpy(pbyBitsMask,&m_pFileSocket->m_ArrayBuffer[dwPoint],pstRecvBitmapInfoMask->dwBitsCount);
						dwPoint+=pstRecvBitmapInfoMask->dwBitsCount;

						//赋值Bitmap信息
						BITMAP stBitmapMask;
						stBitmapMask.bmType=pstRecvBitmapInfoMask->bmType;
						stBitmapMask.bmWidth=pstRecvBitmapInfoMask->bmWidth;
						stBitmapMask.bmHeight=pstRecvBitmapInfoMask->bmHeight;
						stBitmapMask.bmWidthBytes=pstRecvBitmapInfoMask->bmWidthBytes;
						stBitmapMask.bmPlanes=pstRecvBitmapInfoMask->bmPlanes;
						stBitmapMask.bmBitsPixel=pstRecvBitmapInfoMask->bmBitsPixel;
						stBitmapMask.bmBits=pbyBitsMask;//指向Bitmap数据

						//生成Bitmap图象
						CBitmap BitmapMask;
						BitmapMask.CreateBitmapIndirect(&stBitmapMask);
						////////////////////////////////////////////
						DWORD dwCountColor=16*16*sizeof(COLORREF);//算出BMP大小

						//得到Bitmap数据
						BYTE* pbyBitsColor=new BYTE[dwCountColor];
						memcpy(pbyBitsColor,&m_pFileSocket->m_ArrayBuffer[dwPoint],dwCountColor);
						dwPoint+=dwCountColor;

						CDC ScreenDC;
						ScreenDC.CreateDC("DISPLAY",NULL,NULL,NULL);//得到屏幕DC

						//得到屏幕的每象素位数(即颜色数,256色=8,增强色=16,真彩色=32)
						int nBitsPixel=ScreenDC.GetDeviceCaps(BITSPIXEL);

						CBitmap BitmapColor;//建立与当前屏幕颜色数相同的Bitmap
						BitmapColor.CreateBitmap(16,16,1,nBitsPixel,NULL);

						CDC MemDC;
						MemDC.CreateCompatibleDC(&ScreenDC);//建立兼容DC
						MemDC.SelectObject(&BitmapColor);//选入Bitmap

						//读取每个象素的颜色
						DWORD dwPos=0;
						for(int i=0;i<16;i++)
						{
							for(int j=0;j<16;j++)
							{
								COLORREF crColor;
								memcpy(&crColor,pbyBitsColor+dwPos,sizeof(crColor));
								MemDC.SetPixel(j,i,crColor);
								dwPos+=sizeof(crColor);
							}
						}
						MemDC.DeleteDC();//删除兼容DC
						ScreenDC.DeleteDC();//删除屏幕DC
						////////////////////////////////////////////
						ICONINFO stIconInfo;//赋值图标信息
						stIconInfo.fIcon=1;
						stIconInfo.xHotspot=8;
						stIconInfo.yHotspot=8;
						stIconInfo.hbmMask=HBITMAP(BitmapMask);//Bitmap图象
						stIconInfo.hbmColor=HBITMAP(BitmapColor);//Bitmap图象

						HICON hExeIcon=CreateIconIndirect(&stIconInfo);//生成EXE图标
						/////////////////////////////////////////////////////////
						m_ListImageList.Add(hExeIcon);//EXE图标添加到列表映象列表

						delete[] pbyBitsMask;//删除指针
						delete[] pbyBitsColor;//删除指针
						BitmapMask.DeleteObject();//删除Bitmap对象
						BitmapColor.DeleteObject();//删除Bitmap对象
						DestroyIcon(hExeIcon);//消毁图标
					}
					else
					{
						m_ListImageList.Add(shFileInfo.hIcon);//图标添加到列表映象列表里
					}
					DestroyIcon(shFileInfo.hIcon);
					//////////////////////////////////////////////////////////
					m_List->InsertItem(nCount,strDisplayName,nCount);
					/////////////////////////////////////
					dwTotalFileSize+=dwFileLength;
					if(dwFileLength!=0)
					{	
						DWORD dwTemp=dwFileLength/1024;
						if(dwTemp*1024!=dwFileLength)
							dwFileLength=dwTemp+1;
						else
							dwFileLength=dwTemp;
					}
					m_List->SetItemText(nCount,1,ConverDWORDToCString(dwFileLength)+"KB");
					m_List->SetItemText(nCount,2,strTypeName);
				}
				//加入文件修改时间///////////////////////
				memcpy(&filetime,&m_pFileSocket->m_ArrayBuffer[dwPoint],sizeof(filetime));
				dwPoint+=sizeof(filetime);

				CTime refTime(filetime);
				CString strFileTime;
				strFileTime=refTime.Format("%y-%m-%d %H:%M");
				m_List->SetItemText(nCount,3,strFileTime);	
				/////////////////////////////////////////
				m_List->SetItemData(nCount,nCount);
				nCount++;
			}
			///////////////////////////////////////////////////////////////////////////////
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			m_List->SortItems(CompareFunc,(DWORD)m_List);//列表项排序
			m_pListView->SetRedraw(TRUE);
			RefreshDirecoryItem(m_hSelectItem);//刷新目录节点

			SetBusyCursor(FALSE);
			CString strIndicatorStatus;
			strIndicatorStatus.Format("，%d 个对象，",nCount);
			strIndicatorStatus="["+GetCurHostName()+"]"+GetCurDir()+
				strIndicatorStatus+ConverDWORDToCString(dwTotalFileSize)+" 字节";

			if(m_bLastExpand)
			{
				m_Progress.SetPos(0);
				m_strIndicatorShowFileList=strIndicatorStatus;
				m_Tree->Expand(m_hSelectItem,TVE_EXPAND);
				m_hSelectItem=NULL;
				return;
			}
			SetIndicatorStatus(strIndicatorStatus);
			break;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_hSelectItem=NULL;
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_RenameFile(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("重命名文件失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);

			LPDWORD pdwErrorCode;
			pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("重命名文件失败");

				MessageBox(AfxGetMainWnd()->m_hWnd,"无法重命名文件 "+m_TM_RenameFile_strFullPath+"。",
					"重命名文件出错",MB_ICONSTOP|MB_OK);
				return;
			}

			if(m_TM_RenameFile_bIsList)
			{
				if(m_Tree->GetSelectedItem()==m_TM_RenameFile_hEditItem)
				{
					if(m_TM_RenameFile_bIsDir)
					{
						SHFILEINFO shFileInfo;
						SHGetFileInfo(m_TM_RenameFile_strFilename2,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
								SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME);

						//对于"隐藏已知文件类型的扩展名"的处理//////////
						CString strInput1=m_TM_RenameFile_strFilename2;//原始文件名
						CString strInput2=shFileInfo.szDisplayName;//处理后的文件名
						CString strOutput;//正确的可以显示的文件名

						int nPos=strInput1.ReverseFind('\\');
						if(nPos!=-1)//如果原始文件名中有路径,去掉路径只留文件名
							strInput1=strInput1.Mid(nPos+1);

						if(strInput1.GetLength()!=strInput2.GetLength())//用户设置了"隐藏已知文件类型的扩展名"
							strOutput=strInput1;//显示原始文件名
						else
							strOutput=strInput2;//显示处理后的文件名
						////////////////////////////////////////////////
						CString strFilename1=m_List->GetItemText(m_TM_RenameFile_nEditItem,0);
						m_List->SetItemText(m_TM_RenameFile_nEditItem,0,strOutput);

						DWORD dwFlag=m_Tree->GetItemData(m_TM_RenameFile_hEditItem);
						if(dwFlag&TI_ONCEEXPAND)
							ChangeDirectoryItem(strFilename1,strOutput);
					}
					else
					{
						SHFILEINFO shFileInfo;
						SHGetFileInfo(m_TM_RenameFile_strFilename2,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
							SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME);
						int nImageIndex=m_ListImageList.Add(shFileInfo.hIcon);
						DestroyIcon(shFileInfo.hIcon);

						//对于"隐藏已知文件类型的扩展名"的处理//////////
						CString strInput1=m_TM_RenameFile_strFilename2;//原始文件名
						CString strInput2=shFileInfo.szDisplayName;//处理后的文件名
						CString strOutput;//正确的可以显示的文件名

						int nPos=strInput1.ReverseFind('\\');
						if(nPos!=-1)//如果原始文件名中有路径,去掉路径只留文件名
							strInput1=strInput1.Mid(nPos+1);

						if(strInput1.GetLength()!=strInput2.GetLength())//用户设置了"隐藏已知文件类型的扩展名"
							strOutput=strInput1;//显示原始文件名
						else
							strOutput=strInput2;//显示处理后的文件名
						////////////////////////////////////////////////
						//修改文件图标和名称/////////////////////
						m_List->SetItem(m_TM_RenameFile_nEditItem,0,LVIF_IMAGE|LVIF_TEXT,
							strOutput,nImageIndex,
							NULL,NULL,NULL);
						//修改文件类型///////////////////////////
						m_List->SetItem(m_TM_RenameFile_nEditItem,2,LVIF_TEXT,
							(LPCSTR)shFileInfo.szTypeName,NULL,
							NULL,NULL,NULL);
					}
				}
			}
			else	//是树改文件名
			{
				SHFILEINFO shFileInfo;
				SHGetFileInfo(m_TM_RenameFile_strFilename2,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
						SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME);
				m_Tree->SetItemText(m_TM_RenameFile_hEditItem,(LPCSTR)shFileInfo.szDisplayName);
				HTREEITEM hParentItem=m_Tree->GetParentItem(m_TM_RenameFile_hEditItem);
				m_Tree->SortChildren(hParentItem);//父项重新排序
			}

			SetBusyCursor(FALSE);
			SetIndicatorStatus("重命名文件成功");
			break;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_NewFile(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("新建文件失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);

			LPDWORD pdwErrorCode;
			pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("新建文件失败");

				MessageBox(AfxGetMainWnd()->m_hWnd,"无法新建文件 "+m_TM_NewFile_strFullPath+"。",
					"新建文件出错",MB_ICONSTOP|MB_OK);
				return;
			}

			if(m_Tree->GetSelectedItem()==m_TM_NewFile_hSelectItem)
			{
				SHFILEINFO shFileInfo;
				SHGetFileInfo(m_TM_NewFile_strFilename,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME);
				int nImageIndex=m_ListImageList.Add(shFileInfo.hIcon);
				DestroyIcon(shFileInfo.hIcon);
				//加入文件图标和名称/////////////////////
				int nItemCount=m_List->GetItemCount();
				m_List->InsertItem(nItemCount,
					(LPCSTR)shFileInfo.szDisplayName,nImageIndex);
				//加入文件类型///////////////////////////
				m_List->SetItemText(nItemCount,2,(LPCSTR)shFileInfo.szTypeName);
				//加入文件大小///////////////////////////
				m_List->SetItemText(nItemCount,1,"0KB");
				//加入文件修改时间///////////////////////
				CTime tmModifyTime=CTime::GetCurrentTime();
				m_List->SetItemText(nItemCount,3,tmModifyTime.Format("%y-%m-%d %H:%M"));
				///////////////////////////////////////////
				m_List->EditLabel(nItemCount);
			}
			SetBusyCursor(FALSE);
			SetIndicatorStatus("新建文件成功");
			break;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_NewFolder(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("新建文件夹失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);

			LPDWORD pdwErrorCode;
			pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("新建文件夹失败");

				MessageBox(AfxGetMainWnd()->m_hWnd,"无法新建文件夹 "+m_TM_NewFolder_strFullPath+"。",
					"新建文件夹出错",MB_ICONSTOP|MB_OK);
				return;
			}

			if(m_Tree->GetSelectedItem()==m_TM_NewFolder_hSelectItem)
			{
				SHFILEINFO shFileInfo;
				SHGetFileInfo(m_TM_NewFolder_strFilename,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
						SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME);
				int nImageIndex=m_ListImageList.Add(m_hIcon_Folder);//添加文件夹图标到列表映象列表里
				//加入文件图标和名称/////////////////////
				int nItemCount=m_List->GetItemCount();
				m_List->InsertItem(nItemCount,
					(LPCSTR)shFileInfo.szDisplayName,nImageIndex);
				//加入文件类型///////////////////////////
				m_List->SetItemText(nItemCount,2,"文件夹");
				//加入文件修改时间///////////////////////
				CTime tmModifyTime=CTime::GetCurrentTime();
				m_List->SetItemText(nItemCount,3,tmModifyTime.Format("%y-%m-%d %H:%M"));
				///////////////////////////////////////////
				m_List->EditLabel(nItemCount);
			}
			AddDirecoryItem(m_TM_NewFolder_strFilename,m_TM_NewFolder_hSelectItem);

			SetBusyCursor(FALSE);
			SetIndicatorStatus("新建文件夹成功");
			break;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_DeleteFile(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("删除文件失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);

			LPDWORD pdwErrorCode;
			pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("删除文件失败");

				MessageBox(AfxGetMainWnd()->m_hWnd,"无法删除文件 "+m_TM_DeleteFile_strFullPath+"。",
					"删除文件出错",MB_ICONSTOP|MB_OK);
				return;
			}

			if(m_TM_DeleteFile_bIsList)
			{
				if(m_Tree->GetSelectedItem()==m_TM_DeleteFile_hSelectItem)
				{
					int nCount=m_List->GetSelectedCount();
					int nItemCount=m_List->GetItemCount();
					BOOL bLastItemDelete=FALSE;
					/////////////////////////////////////
					int nSelectItem=nItemCount-2;
					nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_BELOW|LVNI_SELECTED);
					if(nSelectItem!=-1)
					{
						bLastItemDelete=TRUE;
						nCount--;
					}
					/////////////////////////////////////
					nSelectItem=nItemCount-1;
					for(int i=0;i<nCount;i++)
					{
						nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_ABOVE|LVNI_SELECTED);
						////////////////////////////////////
						if(m_List->GetItemText(nSelectItem,1)=="")//删除的是文件夹
						{
							DeleteDirecoryItem(m_List->GetItemText(nSelectItem,0));
						}
						////////////////////////////////////
						m_List->DeleteItem(nSelectItem);
					}
					int nFocusItem=nSelectItem;
					if(bLastItemDelete)
					{
						nSelectItem=m_List->GetItemCount()-1;
						////////////////////////////////////
						if(m_List->GetItemText(nSelectItem,1)=="")//删除的是文件夹
						{
							DeleteDirecoryItem(m_List->GetItemText(nSelectItem,0));
						}
						////////////////////////////////////
						m_List->DeleteItem(nSelectItem);
					}
					nItemCount=m_List->GetItemCount();
					if(nFocusItem>nItemCount-1)
						nFocusItem=nItemCount-1;
					m_List->SetItemState(nFocusItem,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
				}
			}
			else	//是树
			{
				SetBusyCursor(FALSE);
				SetIndicatorStatus("删除文件成功");
				m_Progress.SetPos(0);
				m_Tree->DeleteItem(m_TM_DeleteFile_hSelectItem);
				return;
			}

			SetBusyCursor(FALSE);
			SetIndicatorStatus("删除文件成功");
			break;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_RemotePaste(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			CString strText;
			if(m_dwFOFlags==FO_COPY)
				strText="复制";
			else
				strText="移动";
			SetIndicatorStatus(strText+"文件失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);

			CString strText;
			if(m_dwFOFlags==FO_COPY)
				strText="复制";
			else
				strText="移动";

			LPDWORD pdwErrorCode;
			pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus(strText+"文件失败");

				MessageBox(AfxGetMainWnd()->m_hWnd,"无法"+strText+"文件 "+m_TM_RemotePaste_strFullPath+"。",
					strText+"文件出错",MB_ICONSTOP|MB_OK);
				return;
			}

			if(m_dwFOFlags==FO_MOVE)
			{
				DeleteCutDirecoryItem();
				m_dwFOFlags=NULL;
				if(AfxGetMainWnd()->OpenClipboard())
				{
					EmptyClipboard();
					CloseClipboard();
				}
			}
			SetBusyCursor(FALSE);
			SetIndicatorStatus(strText+"文件成功");
			m_Progress.SetPos(0);
			OnToolbarRefresh();
			return;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_GetOperationObject(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("获得操作对象失败");
			break;
		}
		case 1://完成
		{
			DWORD dwSize=m_pFileSocket->m_ArrayBuffer.GetSize();
			if(dwSize==0)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("所选文件夹内无操作对象");
				return;
			}
			LPDWORD pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(dwSize==sizeof(DWORD)&&*pdwErrorCode==0)
			{
				LPDWORD pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
				if(*pdwErrorCode==0)
				{
					SetBusyCursor(FALSE);
					m_Progress.SetPos(0);
					SetIndicatorStatus("获得操作对象失败");

					MessageBox(AfxGetMainWnd()->m_hWnd,"无法获得操作对象 "+m_TM_GetOperationObject_strFullPath+"。",
						"获得操作对象出错",MB_ICONSTOP|MB_OK);
					return;
				}
			}
			//////////////////////////////////////////////////////////////
			m_TM_GetOperationObject_nIndex=0;	
			m_TM_GetOperationObject_StringArray.SetSize(0);

			DWORD dwPoint=0;
			LPCSTR szFilename;
			LPCSTR szFrom=(LPCSTR)m_pFileSocket->m_ArrayBuffer.GetData();
			while(dwPoint<dwSize)
			{
				szFilename=szFrom+dwPoint;
				m_TM_GetOperationObject_StringArray.Add((LPCSTR)szFilename);
				dwPoint+=strlen(szFilename)+1;
			}
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("获得操作对象成功");
			m_Progress.SetPos(0);
			//////////////////////////////////////////////////////////////
			//看看目的文件夹中有没有同名文件
			if(IsFileExist(m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex]))
			{	//目的文件夹中有同名文件
				Remote_GetFileInfo(m_TM_GetOperationObject_strRemoteHostName,
					m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
					TRUE);//TRUE:下载,FALSE:上传
			}
			else//目的文件夹中没有同名文件
			{
				Remote_DownloadFile(m_TM_GetOperationObject_strRemoteHostName,
					m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
					m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
					0,//从头开始下载
					m_TM_GetOperationObject_nIndex+1,
					m_TM_GetOperationObject_StringArray.GetSize(),
					m_TM_GetOperationObject_bIsCut,0);
			}
			return;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_DownloadFile(DWORD dwResult)
{
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			CString strText;
			if(m_TM_DownloadFile_bIsCut)
				strText="移动";
			else
				strText="复制";
			SetIndicatorStatus(strText+"文件失败");
			break;
		}
		case 1://完成
		{
			SetBusyCursor(FALSE);
			//////////////////////////////////////////////////////////////
			switch(m_TM_DownloadFile_nControlCode)//是粘贴,本地运行还是查看
			{
				case 0://粘贴操作(下载文件)
				{ 
					m_TM_GetOperationObject_nIndex++;//下一个文件
					//是否都下载完了
					if(m_TM_GetOperationObject_nIndex > m_TM_GetOperationObject_StringArray.GetUpperBound())
					{
						if(m_TM_GetOperationObject_bIsCut)//如果是剪切
						{
							m_dwFOFlags=NULL;
							if(AfxGetMainWnd()->OpenClipboard())//清空剪贴板
							{
								EmptyClipboard();
								CloseClipboard();
							}
						}
						if(m_Tree->GetSelectedItem()==m_TM_GetOperationObject_hSelectItem)
						{
							OnToolbarRefresh();//如果焦点没变,刷新显示
						}
						CString strText;
						if(m_TM_GetOperationObject_bIsCut)
							strText="移动";
						else
							strText="复制";
						SetIndicatorStatus(strText+"文件成功");
					}
					else//没有都下载完
					{
						//看看目的文件夹中有没有同名文件
						if(IsFileExist(m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex]))
						{	//目的文件夹中有同名文件
							Remote_GetFileInfo(m_TM_GetOperationObject_strRemoteHostName,
								m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
								TRUE);//TRUE:下载,FALSE:上传
						}
						else//目的文件夹中没有同名文件
						{
							Remote_DownloadFile(m_TM_GetOperationObject_strRemoteHostName,
								m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
								m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
								0,//从头开始下载
								m_TM_GetOperationObject_nIndex+1,
								m_TM_GetOperationObject_StringArray.GetSize(),
								m_TM_GetOperationObject_bIsCut,0);
						}
					}
					break;
				}
				case 1://本地运行
				{
					SetIndicatorStatus("本地运行文件成功");
					////////////////////////////////////////////////////////////
					CString strDirectory=m_TM_DownloadFile_strFilename2.Left(m_TM_DownloadFile_strFilename2.ReverseFind('\\')+1);
					HINSTANCE hInstance=ShellExecute(NULL,NULL,m_TM_DownloadFile_strFilename2,NULL,strDirectory,SW_SHOWNORMAL);
					if((INT)hInstance<=32)
					{
						int nErrorCode=(INT)hInstance;
						if(nErrorCode==SE_ERR_NOASSOC||nErrorCode==SE_ERR_ASSOCINCOMPLETE)
						{
							CFileDialog dlg(true,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
								"程序(*.exe)|*.exe|所有文件(*.*)|*.*||");
							dlg.m_ofn.lpstrTitle="请选择用于打开此文件的程序...";
							if(dlg.DoModal()==IDOK) 
							{
								CString strProgramPath=dlg.GetPathName();
								m_TM_DownloadFile_strFilename2="\""+m_TM_DownloadFile_strFilename2+"\"";
								CString strDirectory=strProgramPath.Left(strProgramPath.ReverseFind('\\')+1);
								ShellExecute(NULL,NULL,strProgramPath,m_TM_DownloadFile_strFilename2,strDirectory,SW_SHOWNORMAL);
							}				
						}
						else
						{
							MessageErrorBox(nErrorCode,"无法执行文件",m_TM_DownloadFile_strFilename2);
						}
					}
					break;
				}
				case 2://查看
				{
					SetIndicatorStatus("查看文件成功");
					////////////////////////////////////////////////////////////
					m_TM_DownloadFile_strFilename2="\""+m_TM_DownloadFile_strFilename2+"\"";
					CString strDirectory=m_strFileViewer.Left(m_strFileViewer.ReverseFind('\\')+1);
					ShellExecute(NULL,NULL,m_strFileViewer,m_TM_DownloadFile_strFilename2,strDirectory,SW_SHOWNORMAL);
					break;
				}
			}
			return;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
}

void CNethiefDoc::OnTM_UploadFile(DWORD dwResult)
{
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			CString strText;
			if(m_TM_UploadFile_bIsCut)
				strText="移动";
			else
				strText="复制";
			SetIndicatorStatus(strText+"文件失败");
			break;
		}
		case 1://完成
		{
			SetBusyCursor(FALSE);
			//////////////////////////////////////////////////////////////
			m_TM_GetOperationObject_nIndex++;//下一个文件
			//是否都上传完了
			if(m_TM_GetOperationObject_nIndex > m_TM_GetOperationObject_StringArray.GetUpperBound())
			{
				if(m_TM_GetOperationObject_bIsCut)//如果是剪切
				{
					m_dwFOFlags=NULL;
					if(AfxGetMainWnd()->OpenClipboard())//清空剪贴板
					{
						EmptyClipboard();
						CloseClipboard();
					}
				}
				CString strText;
				if(m_TM_GetOperationObject_bIsCut)
					strText="移动";
				else
					strText="复制";
				SetIndicatorStatus(strText+"文件成功");
				if(m_Tree->GetSelectedItem()==m_TM_GetOperationObject_hSelectItem)
				{
					OnToolbarRefresh();//如果焦点没变,刷新显示
				}
			}
			else//没有都上传完
			{
				//看看目的文件夹中有没有同名文件
				Remote_GetFileInfo(m_TM_GetOperationObject_strRemoteHostName,
					m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
					FALSE);//TRUE:下载,FALSE:上传
			}
			return;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
}

void CNethiefDoc::OnTM_ShowProperty_Multi(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("显示多文件属性失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);

			pRECV_PROPERTYMULTI_INFO pRecvPropertyMultiInfo=(pRECV_PROPERTYMULTI_INFO)m_pFileSocket->m_ArrayBuffer.GetData();
			CString strSize=ConverDWORDToCString(pRecvPropertyMultiInfo->dwTotalSize)+" 字节";
			CString strStatistic;
			strStatistic.Format("%d 个文件，%d 个文件夹",
				pRecvPropertyMultiInfo->nFileCount,pRecvPropertyMultiInfo->nFolderCount);

			CToolBarPropertyMultiDlg dlg;
			dlg.m_strSize=strSize;
			dlg.m_strStatistic=strStatistic;
			dlg.m_strHostName=m_TM_ShowPropertyMulti_strHostName;

			SetBusyCursor(FALSE);
			SetIndicatorStatus("显示多文件属性成功");
			m_Progress.SetPos(0);
			///////////////////////////////////////////
			dlg.DoModal();
			return;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_ShowProperty_Single(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("显示单文件属性失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			DWORD dwPoint=0;

			pRECV_PROPERTYSINGLE_INFO pRecvPropertySingleInfo=(pRECV_PROPERTYSINGLE_INFO)&m_pFileSocket->m_ArrayBuffer[dwPoint];
			dwPoint+=sizeof(RECV_PROPERTYSINGLE_INFO);
			/////////////////////////////////////////////////////
			CToolBarPropertySingleDlg dlg;

			SHFILEINFO shFileInfo;
			SHGetFileInfo(m_TM_ShowPropertySingle_strFilename.Right(m_TM_ShowPropertySingle_strFilename.GetLength()-m_TM_ShowPropertySingle_strFilename.ReverseFind('\\')-1),
				FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
				SHGFI_USEFILEATTRIBUTES|SHGFI_ICON|SHGFI_LARGEICON);
			dlg.m_hIcon=shFileInfo.hIcon;

			dlg.m_Edit_strFilename=m_TM_ShowPropertySingle_strFilename;
			dlg.m_strHostName=m_TM_ShowPropertySingle_strHostName;

			dlg.m_strFileSize=ConverDWORDToCString(pRecvPropertySingleInfo->m_size)+" 字节";

			WORD wFileType;
			wFileType=LOWORD(pRecvPropertySingleInfo->dwFileType);
			CString strFileType=LOBYTE(wFileType);
			strFileType+=HIBYTE(wFileType);

			if(strFileType=="")
				dlg.m_strDataType="非可执行文件";
			if(strFileType=="NE")
				dlg.m_strDataType="Win16 NE 可执行文件";
			if(strFileType=="PE")
				dlg.m_strDataType="Win32 PE 可执行文件";
			if(strFileType=="MZ")
				dlg.m_strDataType="MS-DOS .EXE, .COM 或者 .BAT 文件";
			////////////////////////////////////////////
			dlg.m_strCTime=pRecvPropertySingleInfo->m_ctime.Format("%Y年%m月%d日 %H:%M:%S");
			dlg.m_strMTime=pRecvPropertySingleInfo->m_mtime.Format("%Y年%m月%d日 %H:%M:%S");
			dlg.m_strATime=pRecvPropertySingleInfo->m_atime.Format("%Y年%m月%d日");
			////////////////////////////////////////////
			if(pRecvPropertySingleInfo->m_attribute&FILE_ATTRIBUTE_READONLY)
				dlg.m_bIsReadOnly=TRUE;
			if(pRecvPropertySingleInfo->m_attribute&FILE_ATTRIBUTE_HIDDEN)
				dlg.m_bIsHide=TRUE;
			if(pRecvPropertySingleInfo->m_attribute&FILE_ATTRIBUTE_ARCHIVE)
				dlg.m_bIsArchive=TRUE;
			if(pRecvPropertySingleInfo->m_attribute&FILE_ATTRIBUTE_SYSTEM)
				dlg.m_bIsSystem=TRUE;
			////////////////////////////////////////////
			CString strExtension=m_TM_ShowPropertySingle_strFilename.Right(4);//得到扩展名
			strExtension.MakeUpper();
			if(strExtension==".EXE")//是EXE文件
			{
				DestroyIcon(shFileInfo.hIcon);

				//得到传送Bitmap信息
				pRECV_BITMAP_INFO pstRecvBitmapInfoMask=(pRECV_BITMAP_INFO)&m_pFileSocket->m_ArrayBuffer[dwPoint];
				dwPoint+=sizeof(RECV_BITMAP_INFO);

				//得到Bitmap数据
				BYTE* pbyBitsMask=new BYTE[pstRecvBitmapInfoMask->dwBitsCount];
				memcpy(pbyBitsMask,&m_pFileSocket->m_ArrayBuffer[dwPoint],pstRecvBitmapInfoMask->dwBitsCount);
				dwPoint+=pstRecvBitmapInfoMask->dwBitsCount;

				//赋值Bitmap信息
				BITMAP stBitmapMask;
				stBitmapMask.bmType=pstRecvBitmapInfoMask->bmType;
				stBitmapMask.bmWidth=pstRecvBitmapInfoMask->bmWidth;
				stBitmapMask.bmHeight=pstRecvBitmapInfoMask->bmHeight;
				stBitmapMask.bmWidthBytes=pstRecvBitmapInfoMask->bmWidthBytes;
				stBitmapMask.bmPlanes=pstRecvBitmapInfoMask->bmPlanes;
				stBitmapMask.bmBitsPixel=pstRecvBitmapInfoMask->bmBitsPixel;
				stBitmapMask.bmBits=pbyBitsMask;//指向Bitmap数据

				//生成Bitmap图象
				CBitmap BitmapMask;
				BitmapMask.CreateBitmapIndirect(&stBitmapMask);
				////////////////////////////////////////////
				DWORD dwCountColor=32*32*sizeof(COLORREF);//算出BMP大小

				//得到Bitmap数据
				BYTE* pbyBitsColor=new BYTE[dwCountColor];
				memcpy(pbyBitsColor,&m_pFileSocket->m_ArrayBuffer[dwPoint],dwCountColor);
				dwPoint+=dwCountColor;

				CDC ScreenDC;
				ScreenDC.CreateDC("DISPLAY",NULL,NULL,NULL);//得到屏幕DC

				//得到屏幕的每象素位数(即颜色数,256色=8,增强色=16,真彩色=32)
				int nBitsPixel=ScreenDC.GetDeviceCaps(BITSPIXEL);

				CBitmap BitmapColor;//建立与当前屏幕颜色数相同的Bitmap
				BitmapColor.CreateBitmap(32,32,1,nBitsPixel,NULL);

				CDC MemDC;
				MemDC.CreateCompatibleDC(&ScreenDC);//建立兼容DC
				MemDC.SelectObject(&BitmapColor);//选入Bitmap

				//读取每个象素的颜色
				DWORD dwPos=0;
				for(int i=0;i<32;i++)
				{
					for(int j=0;j<32;j++)
					{
						COLORREF crColor;
						memcpy(&crColor,pbyBitsColor+dwPos,sizeof(crColor));
						MemDC.SetPixel(j,i,crColor);
						dwPos+=sizeof(crColor);
					}
				}
				MemDC.DeleteDC();//删除兼容DC
				ScreenDC.DeleteDC();//删除屏幕DC
				////////////////////////////////////////////
				ICONINFO stIconInfo;//赋值图标信息
				stIconInfo.fIcon=1;
				stIconInfo.xHotspot=16;
				stIconInfo.yHotspot=16;
				stIconInfo.hbmMask=HBITMAP(BitmapMask);//Bitmap图象
				stIconInfo.hbmColor=HBITMAP(BitmapColor);//Bitmap图象

				HICON hExeIcon=CreateIconIndirect(&stIconInfo);//生成EXE图标
				/////////////////////////////////////////////////////////
				dlg.m_hIcon=hExeIcon;

				delete[] pbyBitsMask;//删除指针
				delete[] pbyBitsColor;//删除指针
				BitmapMask.DeleteObject();//删除Bitmap对象
				BitmapColor.DeleteObject();//删除Bitmap对象
			}
			////////////////////////////////////////////
			SetBusyCursor(FALSE);
			SetIndicatorStatus("显示单文件属性成功");
			m_Progress.SetPos(0);
			////////////////////////////////////////////////////////////////////////
			if(dlg.DoModal()==IDOK) 
			{
				DWORD dwFileAttributes=0;
				if(dlg.m_bIsArchive)
					dwFileAttributes|=FILE_ATTRIBUTE_ARCHIVE;
				if(dlg.m_bIsHide)
					dwFileAttributes|=FILE_ATTRIBUTE_HIDDEN;
				if(dlg.m_bIsReadOnly)
					dwFileAttributes|=FILE_ATTRIBUTE_READONLY;
				if(dlg.m_bIsSystem)
					dwFileAttributes|=FILE_ATTRIBUTE_SYSTEM;
				if(dwFileAttributes!=pRecvPropertySingleInfo->m_attribute)
				{
					Remote_ModifyProperty_Single(m_TM_ShowPropertySingle_strHostName,
						m_TM_ShowPropertySingle_strFilename,dwFileAttributes);
				}
			}
			DestroyIcon(dlg.m_hIcon);
			return;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_ModifyProperty_Single(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("修改单文件属性失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			/////////////////////////////////////////////
			LPDWORD pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("修改单文件属性失败");

				MessageBox(AfxGetMainWnd()->m_hWnd,"无法修改单文件属性 "+m_TM_ModifyPropertySingle_strFullPath+"。",
					"修改单文件属性出错",MB_ICONSTOP|MB_OK);
				return;
			}
			/////////////////////////////////////////////
			SetBusyCursor(FALSE);
			SetIndicatorStatus("修改单文件属性成功");
			break;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_ShowProperty_Diskdrive(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("显示驱动器属性失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			pRECV_PROPERTYDISKDRIVE_INFO pRecvPropertyDiskdriveInfo=(pRECV_PROPERTYDISKDRIVE_INFO)m_pFileSocket->m_ArrayBuffer.GetData();
			/////////////////////////////////////////////////////
			if(pRecvPropertyDiskdriveInfo->dwlTotalNumberOfBytes==0)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("显示驱动器属性失败");

				MessageBox(AfxGetMainWnd()->m_hWnd,"无法显示驱动器属性 "+m_TM_ShowPropertyDiskdrive_strFullPath+"。",
					"显示驱动器属性出错",MB_ICONSTOP|MB_OK);
				return;
			}
			/////////////////////////////////////////////////////
			CToolBarPropertyDiskdriveDlg dlg;

			CString strVolumeName=CharToCString(pRecvPropertyDiskdriveInfo->cVolumeName,sizeof(pRecvPropertyDiskdriveInfo->cVolumeName));
			strVolumeName.MakeUpper();
			dlg.m_strName=strVolumeName;
			dlg.m_strSerialNumber.Format("%04X-%04X",HIWORD(pRecvPropertyDiskdriveInfo->dwSerialNumber),LOWORD(pRecvPropertyDiskdriveInfo->dwSerialNumber));

			dlg.m_strHostName=m_TM_ShowPropertyDiskdrive_strHostName;

			int nImage,nSelectedImage;
			m_Tree->GetItemImage(m_TM_ShowPropertyDiskdrive_hSelectItem,nImage,nSelectedImage);
			////////////////////////////////////
			if(nImage==ICO_DRIVE_REMOVABLE)	//软盘驱动器
			{
				dlg.m_strType="软盘驱动器";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_REMOVABLE);
			}
			if(nImage==ICO_DRIVE_FIXED)		//硬盘驱动器
			{
				dlg.m_strType="硬盘驱动器";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_FIXED);
			}
			if(nImage==ICO_DRIVE_CDROM)		//光盘驱动器
			{
				dlg.m_strType="光盘驱动器";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_CDROM);
			}
			if(nImage==ICO_DRIVE_ZIP)	//可移动磁盘驱动器
			{
				dlg.m_strType="可移动磁盘驱动器";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_ZIP);
			}
			if(nImage==ICO_DRIVE_REMOTE)	//网络驱动器
			{
				dlg.m_strType="网络驱动器";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_REMOTE);
			}
			/////////////////////////////////////////
			DWORDLONG dwlTotalNumberOfUsedBytes;
			dwlTotalNumberOfUsedBytes=pRecvPropertyDiskdriveInfo->dwlTotalNumberOfBytes-pRecvPropertyDiskdriveInfo->dwlTotalNumberOfUnusedBytes;

			dlg.m_strTotal=ConverDWORDToCString(pRecvPropertyDiskdriveInfo->dwlTotalNumberOfBytes)+" 字节";
			dlg.m_strUnused=ConverDWORDToCString(pRecvPropertyDiskdriveInfo->dwlTotalNumberOfUnusedBytes)+" 字节";
			dlg.m_strUsed=ConverDWORDToCString(dwlTotalNumberOfUsedBytes)+" 字节";

			int nUsed=(INT)(dwlTotalNumberOfUsedBytes*1000/pRecvPropertyDiskdriveInfo->dwlTotalNumberOfBytes);
			int nUnused=1000-nUsed;
			CString strUsed;
			CString strUnused;
			strUsed.Format("%02d",nUsed);
			strUnused.Format("%02d",nUnused);

			dlg.m_strUsedPercent="."+strUsed.Right(1)+"%";
			dlg.m_strUsedPercent=strUsed.Mid(0,strUsed.GetLength()-1)+dlg.m_strUsedPercent;
			dlg.m_strUnusedPercent="."+strUnused.Right(1)+"%";
			dlg.m_strUnusedPercent=strUnused.Mid(0,strUnused.GetLength()-1)+dlg.m_strUnusedPercent;

			SetBusyCursor(FALSE);
			SetIndicatorStatus("显示驱动器属性成功");
			m_Progress.SetPos(0);
			/////////////////////////////////////////////
			if(dlg.DoModal()==IDOK) 
			{
				if(strVolumeName!=dlg.m_strName)
				{
					Remote_ModifyProperty_Diskdrive(m_TM_ShowPropertyDiskdrive_hSelectItem,
						m_TM_ShowPropertyDiskdrive_strHostName,
						m_TM_ShowPropertyDiskdrive_strFullPath.Right(3),dlg.m_strName);
				}
			}
			return;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_ModifyProperty_Diskdrive(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("修改驱动器属性失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			/////////////////////////////////////////////
			LPDWORD pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("修改驱动器属性失败");

				MessageBox(AfxGetMainWnd()->m_hWnd,"无法修改驱动器属性 "+m_TM_ModifyPropertyDiskdrive_strFullPath+"。",
					"修改驱动器属性出错",MB_ICONSTOP|MB_OK);
				return;
			}

			int nImage,nSelectedImage;
			m_Tree->GetItemImage(m_TM_ModifyPropertyDiskdrive_hSelectItem,nImage,nSelectedImage);
			if(nImage!=ICO_DRIVE_REMOVABLE && nImage!=ICO_DRIVE_ZIP)
			{
				CString strText=m_Tree->GetItemText(m_TM_ModifyPropertyDiskdrive_hSelectItem);
				strText=m_TM_ModifyPropertyDiskdrive_strVolumeName+" "+strText.Right(4);
				m_Tree->SetItemText(m_TM_ModifyPropertyDiskdrive_hSelectItem,strText);
			}

			SetBusyCursor(FALSE);
			SetIndicatorStatus("修改驱动器属性成功");
			break;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_RemoteRun(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("远程运行文件失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			/////////////////////////////////////////////
			LPDWORD pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("远程运行文件失败");

				MessageBox(AfxGetMainWnd()->m_hWnd,"无法远程运行文件 "+m_TM_RemoteRun_strFullPath+"。",
					"远程运行文件出错",MB_ICONSTOP|MB_OK);
				return;
			}

			SetBusyCursor(FALSE);
			SetIndicatorStatus("远程运行文件成功");
			break;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_RemoteUninstall(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("远程卸载服务端失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("远程卸载服务端成功");
			break;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_FindFile(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("查找文件失败");
			break;
		}
		case 1://完成
		{
			DWORD dwSize=m_pFileSocket->m_ArrayBuffer.GetSize();
			/////////////////////////////////////////////////////
			CToolBarFindFileResultDlg dlg;
			dlg.m_StringArray.SetSize(0);
			LPDWORD pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(!(dwSize==sizeof(DWORD)&&*pdwErrorCode==0))
			{
				DWORD dwPoint=0;
				LPSTR szPathName;
				LPBYTE pbIsDirectory;
				while(dwPoint<dwSize)
				{
					szPathName=(char*)&m_pFileSocket->m_ArrayBuffer[dwPoint];
					dwPoint+=strlen(szPathName)+1;
					pbIsDirectory=(LPBYTE)&m_pFileSocket->m_ArrayBuffer[dwPoint];
					dwPoint+=sizeof(*pbIsDirectory);

					dlg.m_StringArray.Add(szPathName);
				}
			}
			///////////////////////////////////////////////////
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("查找文件成功");
			m_Progress.SetPos(0);
			dlg.DoModal();
			return;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_ShowNote(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("显示服务端注释失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			///////////////////////////////////////////////////
			SERVER_CONFIG stServerConfig;
			CString strNote=CharToCString((char*)m_pFileSocket->m_ArrayBuffer.GetData(),
				sizeof(stServerConfig.cNote));

			SetBusyCursor(FALSE);
			SetIndicatorStatus("显示服务端注释成功");
			m_Progress.SetPos(0);
			///////////////////////////////////////////
			MessageBox(AfxGetMainWnd()->m_hWnd,strNote,
				"查看服务端注释",MB_ICONINFORMATION|MB_OK);
			return;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_RefreshStatusBar(DWORD dwResult)
{
	CString strText=m_pStatusBar->GetPaneText(m_pStatusBar->CommandToIndex(ID_SEPARATOR));
	int nPos=strText.Find("%");
	if(nPos==-1)//如果没找到百分比，就不修改了
		return;
	strText=strText.Mid(nPos);

	CString strPercent;
	strPercent.Format("%d",dwResult);
	strText=strPercent+strText;//更新状态栏提示

	SetIndicatorStatus(strText);
}

void CNethiefDoc::OnTM_GetFileInfo(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://失败
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("得到文件信息失败");
			break;
		}
		case 1://完成
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			pRECV_GETFILEINFO_INFO pRecvGetFileInfoInfo=(pRECV_GETFILEINFO_INFO)m_pFileSocket->m_ArrayBuffer.GetData();
			/////////////////////////////////////////////////////
			if(m_TM_GetFileInfo_bIsDownloadFile)//是下载文件操作
			{
				CString		strFilenameText;	//文件名显示文字
				HICON		hSourceIcon;		//源文件图标
				CString		strSourceFileInfo;	//源文件文件信息
				HICON		hDestIcon;			//目的文件图标
				CString		strDestFileInfo;	//目的文件文件信息
				BOOL		bDoNotContinue;		//是否允许断点续传

				CString		strFilename;		//文件名
				CString		strSourceFilename;	//源文件全路径
				CString		strDestFilename;	//目的文件全路径

				strSourceFilename=m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex];
				strDestFilename=m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex];
				strFilename=strSourceFilename.Right(strSourceFilename.GetLength()-strSourceFilename.ReverseFind('\\')-1);

				strFilenameText.Format("此文件夹已包含一个名为“%s”的文件。",strFilename);

				SHFILEINFO shFileInfo;
				SHGetFileInfo(strFilename,
					FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_USEFILEATTRIBUTES|SHGFI_ICON|SHGFI_LARGEICON);
				hSourceIcon=shFileInfo.hIcon;

				strSourceFileInfo=ConverDWORDToCString(pRecvGetFileInfoInfo->m_size)+" 字节";
				strSourceFileInfo+="\r\n修改时间: ";
				strSourceFileInfo+=pRecvGetFileInfoInfo->m_mtime.Format("%Y年%m月%d日 %H:%M:%S");

				SHFILEINFO shFileInfo1;
				SHGetFileInfo(strDestFilename,
					0,&shFileInfo1,sizeof(SHFILEINFO),
					SHGFI_ICON|SHGFI_LARGEICON);
				hDestIcon=shFileInfo1.hIcon;
		
				CFileStatus status;
				CFile::GetStatus(strDestFilename,status);
				strDestFileInfo=ConverDWORDToCString(status.m_size)+" 字节";
				strDestFileInfo+="\r\n修改时间: ";
				strDestFileInfo+=status.m_mtime.Format("%Y年%m月%d日 %H:%M:%S");

				if(pRecvGetFileInfoInfo->m_size>status.m_size)//是否允许断点续传
				{
					bDoNotContinue=FALSE;
				}
				else
				{
					bDoNotContinue=TRUE;
				}

				SetBusyCursor(FALSE);
				SetIndicatorStatus("得到文件信息成功");
				m_Progress.SetPos(0);
				/////////////////////////////////////////////
				//显示选择文件操作对话框
				int nRet=ShowSelectFileOperationDialog(strFilenameText,
					hSourceIcon,strSourceFileInfo,hDestIcon,strDestFileInfo,bDoNotContinue);

				DestroyIcon(hSourceIcon);
				DestroyIcon(hDestIcon);

				switch(nRet)//根据用户不同的选择,进行不同的处理
				{
					case -1://覆盖
					{
						Remote_DownloadFile(m_TM_GetOperationObject_strRemoteHostName,
							m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							0,//从头开始下载
							m_TM_GetOperationObject_nIndex+1,
							m_TM_GetOperationObject_StringArray.GetSize(),
							m_TM_GetOperationObject_bIsCut,0);
						break;
					}
					case -2://断点续传
					{
						Remote_DownloadFile(m_TM_GetOperationObject_strRemoteHostName,
							m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							status.m_size,//断点续传
							m_TM_GetOperationObject_nIndex+1,
							m_TM_GetOperationObject_StringArray.GetSize(),
							m_TM_GetOperationObject_bIsCut,0);
						break;
					}
					case IDCANCEL://取消
					{
						m_TM_DownloadFile_nControlCode=0;//是下载文件,而不是查看或本地运行
						OnTM_DownloadFile(1);//与OnTM_DownloadFile完成时的处理相同
						break;
					}
				}
			}
			else//是上传文件
			{
				if(!pRecvGetFileInfoInfo->bIsFileExist)//文件不存在
				{
					SetBusyCursor(FALSE);
					SetIndicatorStatus("得到文件信息成功");
					m_Progress.SetPos(0);

					Remote_UploadFile(m_TM_GetOperationObject_strRemoteHostName,
						m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
						m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
						0,//从头开始上传
						m_TM_GetOperationObject_nIndex+1,
						m_TM_GetOperationObject_StringArray.GetSize(),
						m_TM_GetOperationObject_bIsCut);
					return;
				}

				CString		strFilenameText;	//文件名显示文字
				HICON		hSourceIcon;		//源文件图标
				CString		strSourceFileInfo;	//源文件文件信息
				HICON		hDestIcon;			//目的文件图标
				CString		strDestFileInfo;	//目的文件文件信息
				BOOL		bDoNotContinue;		//是否允许断点续传

				CString		strFilename;		//文件名
				CString		strSourceFilename;	//源文件全路径
				CString		strDestFilename;	//目的文件全路径

				strSourceFilename=m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex];
				strDestFilename=m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex];
				strFilename=strSourceFilename.Right(strSourceFilename.GetLength()-strSourceFilename.ReverseFind('\\')-1);

				strFilenameText.Format("此文件夹已包含一个名为“%s”的文件。",strFilename);

				SHFILEINFO shFileInfo;
				SHGetFileInfo(strSourceFilename,
					0,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_ICON|SHGFI_LARGEICON);
				hSourceIcon=shFileInfo.hIcon;
		
				CFileStatus status;
				CFile::GetStatus(strSourceFilename,status);
				strSourceFileInfo=ConverDWORDToCString(status.m_size)+" 字节";
				strSourceFileInfo+="\r\n修改时间: ";
				strSourceFileInfo+=status.m_mtime.Format("%Y年%m月%d日 %H:%M:%S");

				SHFILEINFO shFileInfo1;
				SHGetFileInfo(strFilename,
					FILE_ATTRIBUTE_NORMAL,&shFileInfo1,sizeof(SHFILEINFO),
					SHGFI_USEFILEATTRIBUTES|SHGFI_ICON|SHGFI_LARGEICON);
				hDestIcon=shFileInfo1.hIcon;

				strDestFileInfo=ConverDWORDToCString(pRecvGetFileInfoInfo->m_size)+" 字节";
				strDestFileInfo+="\r\n修改时间: ";
				strDestFileInfo+=pRecvGetFileInfoInfo->m_mtime.Format("%Y年%m月%d日 %H:%M:%S");

				if(status.m_size>pRecvGetFileInfoInfo->m_size)//是否允许断点续传
				{
					bDoNotContinue=FALSE;
				}
				else
				{
					bDoNotContinue=TRUE;
				}

				SetBusyCursor(FALSE);
				SetIndicatorStatus("得到文件信息成功");
				m_Progress.SetPos(0);
				/////////////////////////////////////////////
				//显示选择文件操作对话框
				int nRet=ShowSelectFileOperationDialog(strFilenameText,
					hSourceIcon,strSourceFileInfo,hDestIcon,strDestFileInfo,bDoNotContinue);

				DestroyIcon(hSourceIcon);
				DestroyIcon(hDestIcon);

				switch(nRet)//根据用户不同的选择,进行不同的处理
				{
					case -1://覆盖
					{
						Remote_UploadFile(m_TM_GetOperationObject_strRemoteHostName,
							m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							0,//从头开始上传
							m_TM_GetOperationObject_nIndex+1,
							m_TM_GetOperationObject_StringArray.GetSize(),
							m_TM_GetOperationObject_bIsCut);
						break;
					}
					case -2://断点续传
					{
						Remote_UploadFile(m_TM_GetOperationObject_strRemoteHostName,
							m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							pRecvGetFileInfoInfo->m_size,//断点续传
							m_TM_GetOperationObject_nIndex+1,
							m_TM_GetOperationObject_StringArray.GetSize(),
							m_TM_GetOperationObject_bIsCut);
						break;
					}
					case IDCANCEL://取消
					{
						OnTM_UploadFile(1);//与OnTM_UploadFile完成时的处理相同
						break;
					}
				}
			}
			return;
		}
		case 2://被停止
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("当前工作被停止");
			break;
		}
	}
	m_Progress.SetPos(0);
}

//private///////////////////////////////////////////////////////////////
//显示选择文件操作对话框
int CNethiefDoc::ShowSelectFileOperationDialog(CString strFilename,
											   HICON hSourceIcon,CString strSourceFileInfo,
											   HICON hDestIcon,CString strDestFileInfo,
											   BOOL bDoNotContinue)
{
	CSelectFileOperationDlg dlg;

	dlg.m_strFilename=strFilename;
	dlg.m_hSourceIcon=hSourceIcon;
	dlg.m_strSourceFileInfo=strSourceFileInfo;
	dlg.m_hDestIcon=hDestIcon;
	dlg.m_strDestFileInfo=strDestFileInfo;
	dlg.m_bDoNotContinue=bDoNotContinue;

	return dlg.DoModal();
}

//查找指定文件是否存在
BOOL CNethiefDoc::IsFileExist(CString strFilename)
{
	CFileFind f;
	return f.FindFile(strFilename);
}

////////////////////////////////////////////////////////////////////////
