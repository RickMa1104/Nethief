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
///////////////////////////////�¼���ĺ���//////////////////////////////////
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

	CString exeFullPath;      //���ڱ��浱ǰִ�г���·��
	GetModuleFileName(NULL,exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('\\'));  //�ҵ�
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
	while(TRUE)//����б�ӳ���б�
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
	//////����Զ�������б�//////////////////////
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

	for(int i=nCount;i<nOldTotal;i++)//ɾ�������Key
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
	tnd.uID=IDR_MAINFRAME;//��֤ɾ���������ǵ�ͼ��
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
	////////////////////////////////////	//״̬��������
	m_Progress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,CRect(0,0,0,0),m_pStatusBar,1);
	LONG WndStyle=GetWindowLong((HWND)m_Progress,GWL_EXSTYLE);
	WndStyle&=~WS_EX_STATICEDGE;
	SetWindowLong((HWND)m_Progress,GWL_EXSTYLE,WndStyle);
	m_Progress.SetRange(0,PROGRESS_RANGE);
	m_Progress.SetStep(1);
	////////////////////////////////////	//״̬���ѻ�ͼ��
	m_hIcon_Offline=(HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_OFFLINE),IMAGE_ICON,16,16,NULL);
	m_Static_Offline.Create(NULL,WS_VISIBLE|WS_CHILD|SS_ICON|SS_CENTERIMAGE
		,CRect(0,0,0,0),m_pStatusBar);
	////////////////////////////////////	//״̬��ɳ©ͼ��
	m_hIcon_Busy=(HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_BUSY),IMAGE_ICON,16,16,NULL);
	m_Static_Busy.Create(NULL,WS_VISIBLE|WS_CHILD|SS_ICON|SS_CENTERIMAGE
		,CRect(0,0,0,0),m_pStatusBar);
	////////////////////////////////////////////////
	NOTIFYICONDATA tnd;//�ӵ�������
	tnd.cbSize=sizeof(NOTIFYICONDATA);
	tnd.hWnd=AfxGetMainWnd()->m_hWnd;
	tnd.uID=IDR_MAINFRAME;
	tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
 	tnd.uCallbackMessage=WM_SYSTRAY;
	tnd.hIcon=AfxGetApp()->LoadIcon(IDI_SYSTRAY1);
	strcpy(tnd.szTip,"������͵[�ѻ�]");
	Shell_NotifyIcon(NIM_ADD,&tnd);

	SetOfflineSign();//������Ĭ��Ϊ�ѻ�״̬
	////////////////////////////////////////////////
	m_Tree->SetImageList(&m_TreeImageList,TVSIL_NORMAL);//���ù���ӳ���б�
	
	HTREEITEM hRootItem=m_Tree->InsertItem("�ļ�������",ICO_FILEMANAGER,ICO_FILEMANAGER,TVI_ROOT);
	m_Tree->SetItemData(hRootItem,TI_ROOT);
	HTREEITEM hItem=m_Tree->InsertItem("�ҵĵ���",ICO_MYCOMPUTER,ICO_MYCOMPUTER,hRootItem);
	m_Tree->SetItemData(hItem,TI_HOST);
	m_Tree->Expand(hRootItem,TVE_EXPAND);//չ��"�ļ�������"�ڵ�
	////////////////////////////////////////////////
	AddDiskDriveToTree(hItem);//��Ӵ�����������"�ҵĵ���"
	//////��ȡԶ�������б�/////////////////////
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
		m_Tree->InsertItem("Temp",hItem);//����һ����ʱ��
		m_Tree->SetItemData(hItem,TI_REMOTE|TI_HOST);
	}
	///////////////////////////////////////////
}

void CNethiefDoc::OnListInitalUpdate()
{
	m_pListView=(CMyListView*)m_pFrame->m_wndSplitter.GetPane(0,1);
	m_List=&(m_pListView->GetListCtrl());
	////////////////////////////////////////////////
	m_List->SetImageList(&m_ListImageList,LVSIL_SMALL);//���ù���ӳ���б�
	/////////////////////////////////////////////////
	int nColumnPos0=GetINIfileInt("ListView Placement","ColumnPos0");
	int nColumnPos1=GetINIfileInt("ListView Placement","ColumnPos1");
	int nColumnPos2=GetINIfileInt("ListView Placement","ColumnPos2");
	int nColumnPos3=GetINIfileInt("ListView Placement","ColumnPos3");
	//��INI�еõ��������б�����λ��
	m_List->InsertColumn(0,"����",LVCFMT_LEFT,nColumnPos0,-1);
	m_List->InsertColumn(1,"��С",LVCFMT_RIGHT,nColumnPos1,-1);
	m_List->InsertColumn(2,"����",LVCFMT_LEFT,nColumnPos2,-1);
	m_List->InsertColumn(3,"�޸�ʱ��",LVCFMT_LEFT,nColumnPos3,-1);
}

void CNethiefDoc::OnListDestroy()
{
	//�����б�����λ�õ�INI
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
	for(int i='A';i<'[';i++)//��"A"��"Z"��һ��
	{
		strDiskDriveName=(CString)i+":\\";
		switch(GetDriveType(strDiskDriveName))
		{
			case DRIVE_REMOVABLE://����������
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
				m_Tree->InsertItem("Temp",hDiskItem);//������ʱ����
				break;
			}
			case DRIVE_FIXED://Ӳ��������
			{
				SHFILEINFO shFileInfo;
				SHGetFileInfo(strDiskDriveName,0,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_DISPLAYNAME);
				hDiskItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
					ICO_DRIVE_FIXED,ICO_DRIVE_FIXED,hItem);
				m_Tree->SetItemData(hDiskItem,TI_DISKDRIVE_FIXED|TI_DISKDRIVE);
				AddDoubleDeckSubdirectoryToDiskDriveItem(strDiskDriveName,hDiskItem);//���˫����Ŀ¼������������
				break;
			}
			case DRIVE_CDROM://����������
			{
				SHFILEINFO shFileInfo;
				SHGetFileInfo(strDiskDriveName,0,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_DISPLAYNAME);
				hDiskItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
					ICO_DRIVE_CDROM,ICO_DRIVE_CDROM,hItem);
				m_Tree->InsertItem("Temp",hDiskItem);//���۹�������û���̶��Ȳ���һ����ʱ��
				CFileFind f;
				BOOL bExit=f.FindFile(strDiskDriveName+"*.*");
				if(bExit)//�������������
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
				AddDoubleDeckSubdirectoryToDiskDriveItem(strDiskDriveName,hDiskItem);//���˫����Ŀ¼������������
				break;
			}
		}
	} 
}

BOOL CNethiefDoc::AddDoubleDeckSubdirectoryToDiskDriveItem(CString strDir,HTREEITEM hItem)
{
	if(strDir.Right(1)!="\\")//���û��"\"���Զ�����
		strDir+="\\";
	CFileFind f;
	BOOL bExit=f.FindFile(strDir+"*.*");
	if(GetLastError()==ERROR_NOT_READY)//������̴���
	{
		int nReturn=MessageBox(AfxGetMainWnd()->m_hWnd,"�޷����� "+strDir+" ��\r\n\r\n�豸û��׼���á�",
			"�ļ������� - "+strDir,MB_ICONSTOP|MB_RETRYCANCEL);
		if(nReturn==IDRETRY)//���ѡ��"����"�͵ݹ�����Լ�
		{
			return AddDoubleDeckSubdirectoryToDiskDriveItem(strDir,hItem);
		}
		else//���ѡ��"ȡ��"����û��չ����
		{
			DWORD dwFlag=m_Tree->GetItemData(hItem);
			dwFlag&=~TI_ONCEEXPAND;
			m_Tree->SetItemData(hItem,dwFlag);
			return FALSE;
		}
	}
	DWORD dwFlag=m_Tree->GetItemData(hItem);
	if(!(dwFlag&TI_DISKDRIVE_FIXED))//�����ɹ���,�������Ӳ��
	{
		HTREEITEM hChildItem=m_Tree->GetChildItem(hItem);//ɾ����ʱ��
		m_Tree->DeleteItem(hChildItem);
	}
	//////////////////////////////////////////
	if(dwFlag&TI_DISKDRIVE_CDROM)//�����ɹ���,����ǹ���
	{
		SHFILEINFO shFileInfo;
		SHGetFileInfo(strDir,0,&shFileInfo,sizeof(SHFILEINFO),
			SHGFI_DISPLAYNAME);
		m_Tree->SetItemText(hItem,(LPCSTR)shFileInfo.szDisplayName);
	}
	while(bExit)
	{
		bExit=f.FindNextFile();
		if(f.IsDirectory())//��ǰ�ļ���Ŀ¼
		{
			if(!f.IsDots())	//Ŀ¼�ļ���"."��".."
			{
				CString strFilename=f.GetFileName();
				SHFILEINFO shFileInfo;
				SHGetFileInfo(strDir+strFilename,0,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_DISPLAYNAME);

				HTREEITEM hChildItem;
				hChildItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
					ICO_FOLDER,ICO_FOLDER_OPEN,hItem,TVI_SORT);

				AddSubdirectoryToTree(strDir+strFilename,hChildItem);//�������Ŀ¼������ļ���
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
	if(GetLastError()==ERROR_NOT_READY)//������̴���
	{
		int nReturn=MessageBox(AfxGetMainWnd()->m_hWnd,"�޷����� "+strDir+" ��\r\n\r\n�豸û��׼���á�",
			"�ļ������� - "+strDir,MB_ICONSTOP|MB_RETRYCANCEL);
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
		if(f.IsDirectory())//��ǰ�ļ���Ŀ¼
		{
			if(!f.IsDots())	//Ŀ¼�ļ���"."��".."
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
	m_Tree->InsertItem("Temp",hItem);//����һ����ʱ��
	m_Tree->SetItemData(hItem,TI_REMOTE|TI_HOST);
}

///////////////////////////////////////////////////////////////////////
void CNethiefDoc::OnTreeItemexpanding(NM_TREEVIEW* pNMTreeView,LRESULT* pResult)
{
	if(pNMTreeView->action==TVE_EXPAND)//�����չ���ڵ�
	{
		HTREEITEM hExpandItem=pNMTreeView->itemNew.hItem;
		DWORD dwFlag=m_Tree->GetItemData(hExpandItem);
		if(!(dwFlag&TI_REMOTE))	//û��Զ������
		{
			if((dwFlag&TI_DISKDRIVE_CDROM&&(!(dwFlag&TI_ONCEEXPAND)))||(dwFlag&TI_DISKDRIVE_REMOVABLE&&(!(dwFlag&TI_ONCEEXPAND))))
			{//����ǹ���û��չ����,������û��չ����
				BeginWaitCursor();
				m_Tree->SetItemData(hExpandItem,dwFlag|TI_ONCEEXPAND);//����Ϊչ����
				CString strDiskDrive=m_Tree->GetItemText(hExpandItem);
				strDiskDrive=strDiskDrive.Right(3);
				strDiskDrive=strDiskDrive.Left(2);//�õ�������·��,��"E:"
				if(!AddDoubleDeckSubdirectoryToDiskDriveItem(strDiskDrive,hExpandItem))
				{//�������ʧ��,�Ͳ�չ���ڵ�
					*pResult = 1;
					EndWaitCursor();
					return;
				}
				EndWaitCursor();
			}
			if(!(dwFlag&TI_ROOT||dwFlag&TI_HOST||dwFlag&TI_DISKDRIVE||dwFlag&TI_ONCEEXPAND))
			{//��ȥ���ڵ�,�ҵĵ���,����������,չ���������нڵ�,��û��չ�������ļ��нڵ�
				BeginWaitCursor();
				m_Tree->SetItemData(hExpandItem,dwFlag|TI_ONCEEXPAND);//����Ϊչ����
				HTREEITEM hChildItem=m_Tree->GetChildItem(hExpandItem);
				while(hChildItem)//���������
				{
					CString strDir;
					strDir=m_Tree->GetItemText(hChildItem);//�õ������TEXT
					HTREEITEM hParentItem;
					hParentItem=hChildItem;
					while(1)
					{
						hParentItem=m_Tree->GetParentItem(hParentItem);
						if(m_Tree->GetItemData(hParentItem)&TI_DISKDRIVE)//���������������
						{
							CString strParentText=m_Tree->GetItemText(hParentItem);
							strParentText=strParentText.Right(3);
							strParentText=strParentText.Left(2);
							strDir=strParentText+"\\"+strDir;//�õ�������·��
							break;
						}
						strDir=m_Tree->GetItemText(hParentItem)+"\\"+strDir;//���ϸ����·��
					}
					AddSubdirectoryToTree(strDir,hChildItem);//�����Ŀ¼���������
					hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//�õ���һ������
				}
				EndWaitCursor(); 
			}
		}
		else	//��Զ������
		{
			if(dwFlag&TI_HOST)//��Զ����������
			{
				int nImage,nSelectedImage;
				m_Tree->GetItemImage(hExpandItem,nImage,nSelectedImage);
				if(nImage==ICO_REMOTEHOST_DISCONNECT || nImage==ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT)//��δ��������,��������
				{
					ConnectHost(hExpandItem);
					*pResult = 1;
				}
				return;
			}
			if(!(dwFlag&TI_ONCEEXPAND))//��û��չ�����Ľڵ�(û��չ�������ļ�����������)
			{
				if((dwFlag&TI_DISKDRIVE_REMOVABLE||dwFlag&TI_DISKDRIVE_CDROM)&&(!(dwFlag&TI_ONCEACCESS)))
				{//�������������û�з��ʹ�
					CString strDiskDirve=m_Tree->GetItemText(hExpandItem);
					strDiskDirve=strDiskDirve.Right(3);
					strDiskDirve=strDiskDirve.Left(2);
					if(MessageBox(AfxGetMainWnd()->m_hWnd,
						"����Զ�������� "+strDiskDirve+"�����ܻ���¶����˳���\r\n\r\n��Ҫ������",
						"�ļ������� - "+strDiskDirve,
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
						strPath=strParentText+"\\"+strPath;//�õ�������·��
						break;
					}
					strPath=strParentText+"\\"+strPath;
					hParentItem=m_Tree->GetParentItem(hParentItem);
				}
				strHostName=GetHostName(m_Tree->GetParentItem(hParentItem));
				/*���ϵĴ���Ϊ�õ�ȫ·��,��:"d:\","d:\windows\system\"*/
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
	BeginWaitCursor();//��ʾ�ȴ����
	HTREEITEM hSelectItem=pNMTreeView->itemNew.hItem;//�õ��򿪵��ļ��нڵ�
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);//�õ��ڵ�����
	while(1)
	{
		if(dwFlag&TI_ROOT||dwFlag&TI_HOST)//����Ǹ��ڵ���������ڵ�
		{
			SetCurDir("","");
			m_List->DeleteAllItems();
			break;
		}
		if(!(dwFlag&TI_REMOTE))	//����Զ�̽ڵ�
		{
			if(dwFlag&TI_DISKDRIVE)//����Ǵ���������
			{
				if((dwFlag&TI_DISKDRIVE_CDROM&&(!(dwFlag&TI_ONCEEXPAND)))||(dwFlag&TI_DISKDRIVE_REMOVABLE&&(!(dwFlag&TI_ONCEEXPAND))))
				{//����ǹ���û��չ����,������û��չ����
					BeginWaitCursor();
					m_Tree->SetItemData(hSelectItem,dwFlag|TI_ONCEEXPAND);//����Ϊչ����
					CString strDiskDrive=m_Tree->GetItemText(hSelectItem);
					strDiskDrive=strDiskDrive.Right(3);
					strDiskDrive=strDiskDrive.Left(2);//�õ�������·��,��"E:"
					if(!AddDoubleDeckSubdirectoryToDiskDriveItem(strDiskDrive,hSelectItem))
					{
						SetCurDir("�ҵĵ���",strDiskDrive+"\\");
						m_List->DeleteAllItems();
						break;
					}
				}
				CString strSelectText=m_Tree->GetItemText(hSelectItem);
				strSelectText=strSelectText.Right(3);
				strSelectText=strSelectText.Left(2);
				strSelectText+="\\";//�õ�������·��
				SetCurDir("�ҵĵ���",strSelectText);
				AddFileToList(strSelectText);//������·�����ļ���List
				RefreshDirecoryItem(hSelectItem);//ˢ��Ŀ¼�ڵ�
				break;
			}
			if(1)//������ļ���
			{
				CString strDir;
				strDir=m_Tree->GetItemText(hSelectItem);//�õ������TEXT
				HTREEITEM hParentItem;
				hParentItem=hSelectItem;
				while(1)
				{
					hParentItem=m_Tree->GetParentItem(hParentItem);
					if(m_Tree->GetItemData(hParentItem)&TI_DISKDRIVE)//���������������
					{
						CString strParentText=m_Tree->GetItemText(hParentItem);
						strParentText=strParentText.Right(3);
						strParentText=strParentText.Left(2);
						strDir=strParentText+"\\"+strDir;//�õ�������·��
						break;
					}
					strDir=m_Tree->GetItemText(hParentItem)+"\\"+strDir;//���ϸ����·��
				}
				SetCurDir("�ҵĵ���",strDir);
				AddFileToList(strDir);//������·�����ļ���List
				RefreshDirecoryItem(hSelectItem);//ˢ��Ŀ¼�ڵ�
				break;
			}
		}
		else	//��Զ�̽ڵ�(�����ļ��л��������ڵ�)
		{
			if((dwFlag&TI_DISKDRIVE_REMOVABLE||dwFlag&TI_DISKDRIVE_CDROM)&&(!(dwFlag&TI_ONCEACCESS)))
			{//�������������û�з��ʹ�
				CString strDiskDirve=m_Tree->GetItemText(hSelectItem);
				strDiskDirve=strDiskDirve.Right(3);
				strDiskDirve=strDiskDirve.Left(2);
				if(MessageBox(AfxGetMainWnd()->m_hWnd,
					"����Զ�������� "+strDiskDirve+"�����ܻ���¶����˳���\r\n\r\n��Ҫ������",
					"�ļ������� - "+strDiskDirve,
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
					strPath=strParentText+"\\"+strPath;//�õ�������·��
					break;
				}
				strPath=strParentText+"\\"+strPath;
				hParentItem=m_Tree->GetParentItem(hParentItem);
			}
			strHostName=GetHostName(m_Tree->GetParentItem(hParentItem));
			/*���ϵĴ���Ϊ�õ�ȫ·��,��:"d:\","d:\windows\system\"*/
			Remote_ShowFileList(hSelectItem,strHostName,strPath);
			*pResult = 1;
			return;
		}
	}
	EndWaitCursor();//������ʾ�ȴ����
	*pResult = 0;
}

void CNethiefDoc::OnTreeEndlabeledit(TV_DISPINFO* pTVDispInfo,LRESULT* pResult)
{
	HTREEITEM hEditItem=pTVDispInfo->item.hItem;//�õ��༭��ǩ��hItem
	CString strText=pTVDispInfo->item.pszText;//�õ��༭���Text
	CString strDirName=m_Tree->GetItemText(hEditItem);//�õ��༭ǰ��TEXT
	if(strText.IsEmpty()||strDirName==strText)//���û���޸Ļ�ESCȡ����
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
		MessageBox(AfxGetMainWnd()->m_hWnd,"�ļ������ܰ���������ĸ: \r\n	 \\ / :  * ? \" < > | ",
			"������",MB_ICONSTOP|MB_OK);
	}
	else
	{
		CString strHostName=GetCurHostName();
		if(strHostName!="�ҵĵ���")
		{
			CString strPath=GetCurDir();
			strPath=strPath.Left(strPath.ReverseFind('\\')+1);/*��:D:\Download\*/
			Remote_RenameFile(hEditItem,strHostName,strPath,strDirName,strText,TRUE,FALSE,NULL);
			return;
		}
		///////////////////////////////////////////////////
		BeginWaitCursor();//��ʾ�ȴ����

		CString strFrom=GetCurDir();
		CString strTo=GetCurDir();
		strTo=strTo.Mid(0,strTo.ReverseFind('\\')+1)+strText;
		SetCurDir("�ҵĵ���",strTo);

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
			m_Tree->SortChildren(hParentItem);//������������
		}
		EndWaitCursor();//������ʾ�ȴ����
	}
}

void CNethiefDoc::OnListEndlabeledit(LV_DISPINFO* pDispInfo, LRESULT* pResult)
{
	int nEditItem=pDispInfo->item.iItem;//�õ�Ҫ�༭��ǩ�����
	CString strText=pDispInfo->item.pszText;//�õ��༭���Text
	CString strDirName=m_List->GetItemText(nEditItem,0);//�õ��༭ǰ��TEXT
	if (strText.IsEmpty()||strDirName==strText)//���û���޸Ļ�ESCȡ����
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
		MessageBox(AfxGetMainWnd()->m_hWnd,"�ļ������ܰ���������ĸ: \r\n	 \\ / :  * ? \" < > | ",
			"������",MB_ICONSTOP|MB_OK);
	}
	else
	{
		CString strHostName=GetCurHostName();
		if(strHostName!="�ҵĵ���")
		{
			BOOL bIsDir=FALSE;
			if(m_List->GetItemText(nEditItem,1)=="")
				bIsDir=TRUE;
			Remote_RenameFile(m_Tree->GetSelectedItem(),strHostName,GetCurPath(),strDirName,strText,bIsDir,TRUE,nEditItem);
			return;
		}
		///////////////////////////////////////////////////
		BeginWaitCursor();//��ʾ�ȴ����

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
			//����"������֪�ļ����͵���չ��"�Ĵ���//////////
			CString strInput1=strTo;//ԭʼ�ļ���
			CString strInput2=shFileInfo.szDisplayName;//�������ļ���
			CString strOutput;//��ȷ�Ŀ�����ʾ���ļ���

			int nPos=strInput1.ReverseFind('\\');
			if(nPos!=-1)//���ԭʼ�ļ�������·��,ȥ��·��ֻ���ļ���
				strInput1=strInput1.Mid(nPos+1);

			if(strInput1.GetLength()!=strInput2.GetLength())//�û�������"������֪�ļ����͵���չ��"
				strOutput=strInput1;//��ʾԭʼ�ļ���
			else
				strOutput=strInput2;//��ʾ�������ļ���
			////////////////////////////////////////////////
			//�޸��ļ�ͼ�������/////////////////////
			m_List->SetItem(nEditItem,0,LVIF_IMAGE|LVIF_TEXT,
				strOutput,nImageIndex,
				NULL,NULL,NULL);
			//�޸��ļ�����///////////////////////////
			m_List->SetItem(nEditItem,2,LVIF_TEXT,
				(LPCSTR)shFileInfo.szTypeName,NULL,
				NULL,NULL,NULL);
			//////////////////////////////////////////////
			if(m_List->GetItemText(nEditItem,1)=="")
				ChangeDirectoryItem(strDirName,strOutput);
		}
		EndWaitCursor();//������ʾ�ȴ����
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
	//ˢ�����Ŀ¼�µ���Ŀ¼�ڵ�///////////////////////
	HTREEITEM hChildItem=m_Tree->GetChildItem(hSelectItem);//�õ���������
	HTREEITEM hTempItem;
	while(hChildItem)//���������
	{
		CString strDirName=m_Tree->GetItemText(hChildItem);
		int nReturn=FindDirectory(strDirName);
		hTempItem=hChildItem;
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//�õ���һ������
		if(nReturn==-1)
		{
			m_Tree->DeleteItem(hTempItem);//���û�д�Ŀ¼��,��ɾ����
		}
	}
	FindNewDirectory(hSelectItem);//�����Ŀ¼
}

int CNethiefDoc::FindDirectory(CString strDirName)
{
	LV_FINDINFO fi;
	fi.flags=LVFI_STRING;
	fi.psz=strDirName;
	int nReturn=m_List->FindItem(&fi,-1);
	if(nReturn!=-1)//����ҵ���,�����ϱ��
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
	if(!(dwFlag&TI_REMOTE))//����Զ��
	{
		CString strFilename=GetCurPath()+strDirName;//�õ�Ŀ¼��·��
		SHFILEINFO shFileInfo;
		SHGetFileInfo(strFilename,0,&shFileInfo,sizeof(SHFILEINFO),
			SHGFI_DISPLAYNAME);//�õ���ʾ����

		HTREEITEM hNewItem=m_Tree->InsertItem((LPCSTR)shFileInfo.szDisplayName,
			ICO_FOLDER,ICO_FOLDER_OPEN,hSelectItem,TVI_SORT);
		if(bIsRefresh)
		{
			if(dwFlag&TI_ONCEEXPAND)
				AddSubdirectoryToTree(strFilename,hNewItem);
		}
	}
	else//��Զ��
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
	if(GetLastError()==ERROR_NOT_READY)//������̴���
	{
		int nReturn=MessageBox(AfxGetMainWnd()->m_hWnd,"�޷����� "+strDir+" ��\r\n\r\n�豸û��׼���á�",
			"�ļ������� - "+strDir,MB_ICONSTOP|MB_RETRYCANCEL);
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
	while(TRUE)//����б�ӳ���б�
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
		if(!f.IsDots())	//Ŀ¼�ļ���"."��".."
		{
			CString strFilename=f.GetFileName();
			CString strDisplayName;
			CString strTypeName;

			SHFILEINFO shFileInfo;
			if(SHGetFileInfo(strDir+strFilename,0,&shFileInfo,sizeof(SHFILEINFO),
				SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME))
			{
				m_ListImageList.Add(shFileInfo.hIcon);//ͼ����ӵ��б�ӳ���б���
				DestroyIcon(shFileInfo.hIcon);

				strTypeName=(LPCSTR)shFileInfo.szTypeName;
				//����"������֪�ļ����͵���չ��"�Ĵ���//////////
				CString strInput1=strFilename;//ԭʼ�ļ���
				CString strInput2=shFileInfo.szDisplayName;//�������ļ���

				int nPos=strInput1.ReverseFind('\\');
				if(nPos!=-1)//���ԭʼ�ļ�������·��,ȥ��·��ֻ���ļ���
					strInput1=strInput1.Mid(nPos+1);

				if(strInput1.GetLength()!=strInput2.GetLength())//�û�������"������֪�ļ����͵���չ��"
					strDisplayName=strInput1;//��ʾԭʼ�ļ���
				else
					strDisplayName=strInput2;//��ʾ�������ļ���
				////////////////////////////////////////////////
			}
			else//�ǻ���վ����ļ�
			{
				strDisplayName=strFilename;

				if(f.IsDirectory())//�ǻ���վ����ļ���
				{
					m_ListImageList.Add(m_hIcon_Folder);//����ļ���ͼ�굽�б�ӳ���б���
					strTypeName="�ļ���";
				}
				else//�ǻ���վ����ļ�
				{
					m_ListImageList.Add(m_hIcon_UnknownFile);//���δ֪�ļ�ͼ�굽�б�ӳ���б���
					int nPos=strDisplayName.Find(".");
					if(nPos==-1)
						strTypeName="�ļ�";
					else
						strTypeName=strDisplayName.Mid(nPos+1)+" �ļ�";
				}
			}
			//�����ļ�ͼ�������/////////////////////
			m_List->InsertItem(nCount,
				strDisplayName,nCount);
			//�����ļ�����///////////////////////////
			m_List->SetItemText(nCount,2,strTypeName);
			//�����ļ���С///////////////////////////
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
			//�����ļ��޸�ʱ��///////////////////////
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
	m_List->SortItems(CompareFunc,(DWORD)m_List);//�б�������
	m_pListView->SetRedraw(TRUE);
	////////////////////////////////////
	CString strIndicatorStatus;
	strIndicatorStatus.Format("��%d ������",nCount);
	strIndicatorStatus="[�ҵĵ���]"+GetCurDir()+
		strIndicatorStatus+ConverDWORDToCString(dwTotalFileSize)+" �ֽ�"+
		" (���ÿռ�: "+
		ConverDWORDToCString(GetDiskSpace(GetCurDir().Left(3),TRUE))+
		" �ֽ�)";
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
	HTREEITEM hEditItem=pTVDispInfo->item.hItem;//�õ��༭��ǩ��hItem
	DWORD dwFlag=m_Tree->GetItemData(hEditItem);//�õ��༭��ǩ��Flag
	if(dwFlag&TI_ROOT||dwFlag&TI_HOST||dwFlag&TI_DISKDRIVE)
	{//���Ҫ�༭�ı�ǩ�����ļ��нڵ�
		MessageBeep(0xFFFFFFFF);//��һ��
		*pResult=1;				//������Edit��
	}
}

void CNethiefDoc::ChangeDirectoryItem(CString strDirName,CString strNewDirName)
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
	HTREEITEM hChildItem=m_Tree->GetChildItem(hSelectItem);//�õ���������
	while(hChildItem)//���������
	{
		if(m_Tree->GetItemText(hChildItem)==strDirName)//�õ������Text
		{
			m_Tree->SetItemText(hChildItem,strNewDirName);//�����Ҫ�ĵ�,���޸�
			break;
		}
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//�õ���һ������
	}
	m_Tree->SortChildren(hSelectItem);//������������
}

void CNethiefDoc::OnListDblclk(NM_LISTVIEW* pNMListView,LRESULT* pResult)
{
	int nDblclkItem=pNMListView->iItem;//�õ�˫�������
	if(nDblclkItem!=-1)//��������б�����˫��
	{
		BeginWaitCursor();//��ʾ�ȴ����
		if(m_List->GetItemText(nDblclkItem,1)=="")//˫������Ŀ¼
		{
			CString strDirName=m_List->GetItemText(nDblclkItem,0);
			SelectDirecoryItem(strDirName);
		}
		else//˫�������ļ�
		{
			CString strFilename=m_List->GetItemText(nDblclkItem,0);
			CString strPathName=GetCurPath()+strFilename;
			////////////////////////////////////////////////////////////
			CString strHostName=GetCurHostName();
			if(strHostName!="�ҵĵ���")
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
						"����(*.exe)|*.exe|�����ļ�(*.*)|*.*||");
					dlg.m_ofn.lpstrTitle="��ѡ�����ڴ򿪴��ļ��ĳ���...";
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
					MessageErrorBox(nErrorCode,"�޷�ִ���ļ�",strPathName);
				}
			}
		}
		EndWaitCursor();//������ʾ�ȴ����
	}
}

void CNethiefDoc::SelectDirecoryItem(CString strDirName)
{
	m_bLastSelect=FALSE;
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_REMOTE && !(dwFlag&TI_ONCEEXPAND))//��û��չ������Զ���ļ��л��������ڵ�
	{
		m_bLastSelect=TRUE;
		m_strSelectName=strDirName;
		m_Tree->Expand(hSelectItem,TVE_EXPAND);
	}
	else
	{
		HTREEITEM hChildItem=m_Tree->GetChildItem(hSelectItem);//�õ���������
		while(hChildItem)//���������
		{
			if(m_Tree->GetItemText(hChildItem)==strDirName)//�õ������Text
			{
				m_Tree->Expand(hSelectItem,TVE_EXPAND);
				m_Tree->SelectItem(hChildItem);
				break;
			}
			hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//�õ���һ������
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

/////////��ʾ������Ϣ��///////////////////////////////////////////////
void CNethiefDoc::MessageErrorBox(int nErrorCode,CString strSubject,CString strFilename)
{
	switch(nErrorCode)
	{
		case ERROR_NOT_CONNECTED:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+": ָ�����ļ����������ļ�����������ָ����һ���ļ�����",
				"�ļ���������",MB_ICONSTOP|MB_OK);
			break;
		}
		case ERROR_WRITE_PROTECT:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+": ���̱�д������\r\n\r\n��ȥ��д������ʹ����һ�Ŵ��̡�",
				"�ļ���������",MB_ICONSTOP|MB_OK);
			break;
		}
		case ERROR_ACCESS_DENIED:
		{
			if(strSubject=="�޷�ִ���ļ�")
			{
				MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n������һ���Ϸ��� Win32 Ӧ�ó���",
					"�ļ���������",MB_ICONSTOP|MB_OK);
			}
			else
			{
				MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+": ���ʱ��ܾ���\r\n\r\n��ȷ�������Ƿ�δ����δ��д����\r\n�����ļ�δ��ʹ�á�",
					"�ļ���������",MB_ICONSTOP|MB_OK);
			}
			break;
		}
		case ERROR_FILE_NOT_FOUND:
		{
			if(strSubject=="�޷�ִ���ļ�")
			{
				MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n���ļ�û���ҵ���",
					"�ļ���������",MB_ICONSTOP|MB_OK);
			}
			else
			{
				MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+": ���ʱ��ܾ���\r\n\r\n��ȷ�������Ƿ�δ����δ��д����\r\n�����ļ�δ��ʹ�á�",
					"�ļ���������",MB_ICONSTOP|MB_OK);

			}
			break;
		}
		//////////////////////////////////////////
		case 0:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n����ϵͳû���㹻���ڴ����Դ��",
				"�ļ���������",MB_ICONSTOP|MB_OK);
			break;
		}
		case ERROR_PATH_NOT_FOUND:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n��·��û���ҵ���",
				"�ļ���������",MB_ICONSTOP|MB_OK);
			break;
		}
		case ERROR_BAD_FORMAT:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n������һ���Ϸ��� Win32 Ӧ�ó���",
				"�ļ���������",MB_ICONSTOP|MB_OK);
			break;
		}
		case SE_ERR_DDEBUSY:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n��̬���ݽ���(DDE)��������ɣ���Ϊ�����Ķ�̬���ݽ����������ڽ��С�",
				"�ļ���������",MB_ICONSTOP|MB_OK);
			break;
		}
		case SE_ERR_DDEFAIL:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n��̬���ݽ���(DDE)����ʧ�ܡ�",
				"�ļ���������",MB_ICONSTOP|MB_OK);
			break;
		}
		case SE_ERR_DDETIMEOUT:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n��̬���ݽ���(DDE)��������ɣ���Ϊ����ʱ��",
				"�ļ���������",MB_ICONSTOP|MB_OK);
			break;
		}
		case SE_ERR_DLLNOTFOUND:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\nָ���Ķ�̬���ӿ�(DLL)�޷��ҵ���",
				"�ļ���������",MB_ICONSTOP|MB_OK);
			break;
		}
		case SE_ERR_OOM:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\nû���㹻���ڴ���ɴ˲�����",
				"�ļ���������",MB_ICONSTOP|MB_OK);
			break;
		}
		case SE_ERR_SHARE:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n��������Υ����",
				"�ļ���������",MB_ICONSTOP|MB_OK);
			break;
		}
		//////////////////////////////////////////
		case ERROR_DISK_FULL:
		{
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+"\r\n\r\n������û���㹻�Ŀռ䡣",
				"�ļ���������",MB_ICONSTOP|MB_OK);
			break;
		}
		//////////////////////////////////////////
		default:
		{
			CString strErrorCode;
			strErrorCode.Format("%d",nErrorCode);
			MessageBox(AfxGetMainWnd()->m_hWnd,strSubject+" "+strFilename+": ��������\r\n\r\n������룺"+strErrorCode,
				"�ļ���������",MB_ICONSTOP|MB_OK);
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
	if(strHostName!="�ҵĵ���")
	{
		CString strFilename="�½��ļ�";
		int nCount=1;
		CString strExtCount;
		while(FindDirectory(strFilename)!=-1)
		{
			nCount++;
			strExtCount.Format(" (%d)",nCount);
			strFilename="�½��ļ�"+strExtCount;
		}
		Remote_NewFile(m_Tree->GetSelectedItem(),strHostName,GetCurPath(),strFilename);
		return;
	}
	///////////////////////////////////////////////////
	CString strFilename="�½��ļ�";
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
		MessageErrorBox(GetLastError(),"�޷��½��ļ�",strFilename+strExtCount);
		return;
	}
	CloseHandle(hFile);
	/////////////////////////////////////////////
	SHFILEINFO shFileInfo;
	SHGetFileInfo(strPathName,0,&shFileInfo,sizeof(SHFILEINFO),
		SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME);
	int nImageIndex=m_ListImageList.Add(shFileInfo.hIcon);
	DestroyIcon(shFileInfo.hIcon);
	//�����ļ�ͼ�������/////////////////////
	int nItemCount=m_List->GetItemCount();
	m_List->InsertItem(nItemCount,
		(LPCSTR)shFileInfo.szDisplayName,nImageIndex);
	//�����ļ�����///////////////////////////
	m_List->SetItemText(nItemCount,2,(LPCSTR)shFileInfo.szTypeName);
	//�����ļ���С///////////////////////////
	m_List->SetItemText(nItemCount,1,"0KB");
	//�����ļ��޸�ʱ��///////////////////////
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
	if(strHostName!="�ҵĵ���")
	{
		CString strFilename="�½��ļ���";
		int nCount=1;
		CString strExtCount;
		while(FindDirectory(strFilename)!=-1)
		{
			nCount++;
			strExtCount.Format(" (%d)",nCount);
			strFilename="�½��ļ���"+strExtCount;
		}
		Remote_NewFolder(m_Tree->GetSelectedItem(),strHostName,GetCurPath(),strFilename);
		return;
	}
	///////////////////////////////////////////////////
	CString strFilename="�½��ļ���";
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
		MessageErrorBox(GetLastError(),"�޷��½��ļ���",strFilename+strExtCount);
		return;
	}
	/////////////////////////////////////////////
	SHFILEINFO shFileInfo;
	SHGetFileInfo(strPathName,0,&shFileInfo,sizeof(SHFILEINFO),
		SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME);
	int nImageIndex=m_ListImageList.Add(shFileInfo.hIcon);
	DestroyIcon(shFileInfo.hIcon);
	//�����ļ�ͼ�������/////////////////////
	int nItemCount=m_List->GetItemCount();
	m_List->InsertItem(nItemCount,
		(LPCSTR)shFileInfo.szDisplayName,nImageIndex);
	//�����ļ�����///////////////////////////
	m_List->SetItemText(nItemCount,2,(LPCSTR)shFileInfo.szTypeName);
	//�����ļ��޸�ʱ��///////////////////////
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
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(!(dwFlag&TI_ROOT))//������Ǹ��ڵ�
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
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
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
		if(strHostName!="�ҵĵ���")
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
						bSearchSubdir,pStringArray); //Ŀ¼�ļ���"."��"..",����ȫ·����������
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
			pStringArray->Add(f.GetFilePath());//�����ļ�����
		}
	}
	f.Close();
}

////////////����,����,ճ������������ʼ/////////////////////////////////////////////
void CNethiefDoc::OnToolbarCut(BOOL bIsTree)
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
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
		UnsignCutDirecoryItem(bIsTree);//��Ǽ���Ŀ¼��
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
		HTREEITEM hChildItem=m_Tree->GetChildItem(m_hCutParentItem);//�õ���������
		while(hChildItem)//���������
		{
			DWORD dwFlags=m_Tree->GetItemData(hChildItem);
			if(dwFlags&TI_CUT)//�õ������Text
			{
				dwFlags&=~TI_CUT;
				m_Tree->SetItemData(hChildItem,dwFlags);
			}
			hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//�õ���һ������
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
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
	if(strDirName=="")
	{
		DWORD dwFlags=m_Tree->GetItemData(hSelectItem);
		dwFlags|=TI_CUT;
		m_Tree->SetItemData(hSelectItem,dwFlags);
		return;
	}
	HTREEITEM hChildItem=m_Tree->GetChildItem(hSelectItem);//�õ���������
	while(hChildItem)//���������
	{
		if(m_Tree->GetItemText(hChildItem)==strDirName)//�õ������Text
		{
			DWORD dwFlags=m_Tree->GetItemData(hChildItem);
			dwFlags|=TI_CUT;
			m_Tree->SetItemData(hChildItem,dwFlags);
			break;
		}
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//�õ���һ������
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
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_ROOT||dwFlag&TI_HOST)//������Ǹ��ڵ�
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
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_ROOT||dwFlag&TI_HOST)//������Ǹ��ڵ�
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
	//������:�ҵĵ���->�ҵĵ���,�ҵĵ���->Զ������,Զ������->�ҵĵ���,
	//ͬԶ������->ͬԶ������.
	//////////////////////////////////////////////////////////////////
	if(m_strFOHostName==strHostName && strHostName=="�ҵĵ���")//�ҵĵ���->�ҵĵ���(���ز���)
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
	if(m_strFOHostName=="�ҵĵ���" && strHostName!="�ҵĵ���")//�ҵĵ���->Զ������(�ϴ�)
	{
		GetOperationObject(hSelectItem,strHostName,strFrom,GetCurPath(),m_dwFOFlags);
		return;
	}
	if(m_strFOHostName!="�ҵĵ���" && strHostName=="�ҵĵ���")//Զ������->�ҵĵ���(����)
	{
		Remote_GetOperationObject(hSelectItem,m_strFOHostName,strFrom,GetCurPath(),m_dwFOFlags);
		return;
	}
	if(m_strFOHostName==strHostName && strHostName!="�ҵĵ���")//ͬԶ������->ͬԶ������(Զ�̲���)
	{
		Remote_RemotePaste(m_strFOHostName,strFrom,strTo,m_dwFOFlags);
		return;
	}
}

void CNethiefDoc::DeleteCutDirecoryItem()
{
	if(m_hCutParentItem==NULL)
		return;
	HTREEITEM hChildItem=m_Tree->GetChildItem(m_hCutParentItem);//�õ���������
	HTREEITEM hTempItem;
	while(hChildItem)//���������
	{
		DWORD dwFlags=m_Tree->GetItemData(hChildItem);
		hTempItem=hChildItem;
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//�õ���һ������
		if(dwFlags&TI_CUT)//�õ������Text
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	//����Ŀ���ļ�������û��ͬ���ļ�
	Remote_GetFileInfo(m_TM_GetOperationObject_strRemoteHostName,
		m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
		FALSE);//TRUE:����,FALSE:�ϴ�
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

////////////����,����,ճ��������������/////////////////////////////////////////////
void CNethiefDoc::OnMenuFileRun()
{
	int nSelectItem=-1;
	nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_SELECTED);
	if(nSelectItem==-1)//���û��ѡ����
		return;
	/////////////////////////////////////////////
	NM_LISTVIEW NMListView;
	NMListView.iItem=nSelectItem;
	LRESULT Result=0;
	OnListDblclk(&NMListView,&Result);//����˫��
}

void CNethiefDoc::OnToolbarRun()
{
	CString strPathName="";
	int nCount=m_List->GetSelectedCount();
	if(nCount!=0)
	{
		int nSelectItem=m_List->GetNextItem(-1,LVNI_SELECTED);
		if(m_List->GetItemText(nSelectItem,1)!="")//���ѡ�Ĳ����ļ���
		{
			CString strFilename=m_List->GetItemText(nSelectItem,0);
			strPathName=GetCurPath()+strFilename;
			///////////////////////////////////////////////////
			CString strHostName=GetCurHostName();
			if(strHostName!="�ҵĵ���" && strHostName!="")
			{
				CToolBarRunRemoteDlg dlg;
				if(dlg.DoModal()==IDOK)
				{
					int nShowCmd;
					switch(dlg.m_nComboBox)
					{
						case 0://����
						{
							nShowCmd=SW_SHOWNORMAL;
							break;
						}
						case 1://����
						{
							nShowCmd=SW_HIDE;
							break;
						}
						case 2://���
						{
							nShowCmd=SW_SHOWMAXIMIZED;
							break;
						}
						case 3://��С��
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
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);//�õ��༭��ǩ��Flag
	if(dwFlag&TI_ROOT||dwFlag&TI_HOST||dwFlag&TI_DISKDRIVE)
	{//���Ҫ�༭�ı�ǩ�����ļ��нڵ�
		MessageBeep(0xFFFFFFFF);//��һ��
		return;
	}
	m_Tree->EditLabel(hSelectItem);
}

void CNethiefDoc::OnListMenuFileRename()
{
	int nSelectItem=-1;
	nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_SELECTED);
	if(nSelectItem==-1)//���û��ѡ����
		return;
	m_List->EditLabel(nSelectItem);//�༭��ѡ��ı�ǩ
}

void CNethiefDoc::OnToolbarView()
{
	int nCount=m_List->GetSelectedCount();
	if(nCount==0)//���û��ѡ��
		return;
	int nSelectItem=-1;
	for(int i=0;i<nCount;i++)
	{
		nSelectItem=m_List->GetNextItem(nSelectItem,LVNI_SELECTED);
		if(m_List->GetItemText(nSelectItem,1)=="")//���ѡ�����ļ���
			return;
		CString strFilename=m_List->GetItemText(nSelectItem,0);
		CString strPathName=GetCurPath()+strFilename;
		////////////////////////////////////////////////////////////
		CString strHostName=GetCurHostName();
		if(strHostName!="�ҵĵ���")
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
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
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
	HTREEITEM hChildItem=m_Tree->GetChildItem(hSelectItem);//�õ���������
	while(hChildItem)//���������
	{
		if(m_Tree->GetItemText(hChildItem)==strDirName)//�õ������Text
		{
			m_Tree->DeleteItem(hChildItem);
			break;
		}
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//�õ���һ������
	}
}

void CNethiefDoc::OnTreeToolbarDelete()
{
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_HOST&&dwFlag&TI_REMOTE)//�����Զ�������ڵ�
	{
		OnToolbarDeletehost();//ɾ������
		return;
	}
	if(dwFlag&TI_ROOT||dwFlag&TI_HOST||dwFlag&TI_DISKDRIVE)//������Ǹ��ڵ�
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
	if(strHostName!="�ҵĵ���")
	{
		Remote_DeleteFile(hSelectItem,strHostName,strFrom,FALSE);
		return;
	}
	///////////////////////////////////////////////////
	SHFILEOPSTRUCT shFileOp;
	ZeroMemory(&shFileOp,sizeof(shFileOp));
	int nFlags;
	int nReturn=GetAsyncKeyState(VK_SHIFT);
	if(nReturn!=-32767)//û��Shift
		nFlags=FOF_ALLOWUNDO;
	else//����Shift
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
	if(nCount==0)//���û��ѡ����
		return;
	////////////////////////////////////////
	CString strFrom;
	SetFileOperationPath(&strFrom);
	///////////////////////////////////////////////////
	CString strHostName=GetCurHostName();
	if(strHostName!="�ҵĵ���")
	{
		Remote_DeleteFile(m_Tree->GetSelectedItem(),strHostName,strFrom,TRUE);
		return;
	}
	///////////////////////////////////////////////////
	SHFILEOPSTRUCT shFileOp;
	ZeroMemory(&shFileOp,sizeof(shFileOp));
	int nFlags;
	int nReturn=GetAsyncKeyState(VK_SHIFT);
	if(nReturn!=-32767)//û��Shift
		nFlags=FOF_ALLOWUNDO;
	else//����Shift
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
				if(m_List->GetItemText(nSelectItem,1)=="")//ɾ�������ļ���
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
				if(m_List->GetItemText(nSelectItem,1)=="")//ɾ�������ļ���
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
		DWORD dwFlag=m_Tree->GetItemData(hSelectItem);//�õ��༭��ǩ��Flag
		if(dwFlag&TI_ROOT||dwFlag&TI_HOST)
		{//���Ҫ�༭�ı�ǩ�����ļ��нڵ�
			MessageBeep(0xFFFFFFFF);//��һ��
			return;
		}

		BeginWaitCursor();
		if(GetCurDir().GetLength()==3)
		{
			CString strDiskDrive=GetCurDir();
			CString strHostName=GetCurHostName();
			if(strHostName!="�ҵĵ���")
			{
				Remote_ShowProperty_Diskdrive(hSelectItem,strHostName,strDiskDrive);
				return;
			}
			///////////////////////////////////////////////////////
			CToolBarPropertyDiskdriveDlg dlg;

			CString strVolumeNameBuffer;     //�õ�����
			DWORD dwVolumeSerialNumber;      //�õ����к�
			if(!GetVolumeInformation(strDiskDrive,
				strVolumeNameBuffer.GetBufferSetLength(100),100,&dwVolumeSerialNumber,NULL,NULL,NULL,NULL))
			{
				MessageBox(AfxGetMainWnd()->m_hWnd,"�޷���ʾ���������� "+strDiskDrive+"��",
					"��ʾ���������Գ���",MB_ICONSTOP|MB_OK);
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

			dlg.m_strTotal=ConverDWORDToCString(dwlTotalNumberOfBytes)+" �ֽ�";
			dlg.m_strUnused=ConverDWORDToCString(dwlTotalNumberOfUnusedBytes)+" �ֽ�";
			dlg.m_strUsed=ConverDWORDToCString(dwlTotalNumberOfUsedBytes)+" �ֽ�";

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

			dlg.m_strHostName="�ҵĵ���";

			int nImage,nSelectedImage;
			m_Tree->GetItemImage(hSelectItem,nImage,nSelectedImage);
			/////////////////////////////////////////////
			if(nImage==ICO_DRIVE_REMOVABLE)	//����������
			{
				dlg.m_strType="����������";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_REMOVABLE);
			}
			if(nImage==ICO_DRIVE_FIXED)		//Ӳ��������
			{
				dlg.m_strType="Ӳ��������";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_FIXED);
			}
			if(nImage==ICO_DRIVE_CDROM)		//����������
			{
				dlg.m_strType="����������";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_CDROM);
			}
			if(nImage==ICO_DRIVE_ZIP)	//���ƶ�����������
			{
				dlg.m_strType="���ƶ�����������";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_ZIP);
			}
			if(nImage==ICO_DRIVE_REMOTE)	//����������
			{
				dlg.m_strType="����������";
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
						MessageBox(AfxGetMainWnd()->m_hWnd,"�޷��޸����������� "+strDiskDrive+"��",
							"�޸����������Գ���",MB_ICONSTOP|MB_OK);
					}
				}
			}
		}
		else
		{
			CString strHostName=GetCurHostName();
			if(strHostName!="�ҵĵ���")
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
			CString strSize=ConverDWORDToCString(dwTotalSize)+" �ֽ�";
			CString strStatistic;
			strStatistic.Format("%d ���ļ���%d ���ļ���",nFileCount,nFolderCount);
			CToolBarPropertyMultiDlg dlg;
			dlg.m_strSize=strSize;
			dlg.m_strStatistic=strStatistic;
			dlg.m_strHostName="�ҵĵ���";
			dlg.DoModal();
		}
		EndWaitCursor();
		return;
	}
	///////////////////////////////////////////
	int nCount=m_List->GetSelectedCount();
	if(nCount==0)//ûѡ�ͷ���
		return;
	if(nCount==1)
	{
		int nSelectItem=m_List->GetNextItem(-1,LVNI_SELECTED);
		if(m_List->GetItemText(nSelectItem,1)!="")//ֻѡ��һ����Ŀ�����ļ�
		{
			CString strFilename=GetCurPath()+m_List->GetItemText(nSelectItem,0);
			CString strHostName=GetCurHostName();
			if(strHostName!="�ҵĵ���")
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
			dlg.m_strHostName="�ҵĵ���";

			CFileStatus status;
			CFile::GetStatus(strFilename,status);
			dlg.m_strFileSize=ConverDWORDToCString(status.m_size)+" �ֽ�";

			DWORD dwFileType;
			dwFileType=SHGetFileInfo(strFilename,NULL,NULL,NULL,SHGFI_EXETYPE);
			WORD wFileType;
			wFileType=LOWORD(dwFileType);
			CString strFileType=LOBYTE(wFileType);
			strFileType+=HIBYTE(wFileType);

			if(strFileType=="")
				dlg.m_strDataType="�ǿ�ִ���ļ�";
			if(strFileType=="NE")
				dlg.m_strDataType="Win16 NE ��ִ���ļ�";
			if(strFileType=="PE")
				dlg.m_strDataType="Win32 PE ��ִ���ļ�";
			if(strFileType=="MZ")
				dlg.m_strDataType="MS-DOS .EXE, .COM ���� .BAT �ļ�";
			////////////////////////////////////////////
			dlg.m_strCTime=status.m_ctime.Format("%Y��%m��%d�� %H:%M:%S");
			dlg.m_strMTime=status.m_mtime.Format("%Y��%m��%d�� %H:%M:%S");
			dlg.m_strATime=status.m_atime.Format("%Y��%m��%d��");
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
						MessageBox(AfxGetMainWnd()->m_hWnd,"�޷��޸ĵ��ļ����� "+strFilename+"��",
							"�޸ĵ��ļ����Գ���",MB_ICONSTOP|MB_OK);
					}
				}
			}
			DestroyIcon(shFileInfo.hIcon);
			return;
		}
	}
	/////ֻѡ��һ���ļ��� or ѡ�˶���///////////////////////////
	CString strHostName=GetCurHostName();
	if(strHostName!="�ҵĵ���")
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
		if(m_List->GetItemText(nSelectItem,1)=="")//���ѡ�����ļ���
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
	CString strSize=ConverDWORDToCString(dwTotalSize)+" �ֽ�";
	CString strStatistic;
	strStatistic.Format("%d ���ļ���%d ���ļ���",nFileCount,nFolderCount);
	CToolBarPropertyMultiDlg dlg;
	dlg.m_strSize=strSize;
	dlg.m_strStatistic=strStatistic;
	dlg.m_strHostName="�ҵĵ���";
	dlg.DoModal();

	EndWaitCursor();
}


void CNethiefDoc::FolderStatistic(CString strFindDir,int* nFileCount,
							 int* nFolderCount,DWORD* dwTotalSize)
{
	BOOL bExit;
	CFileFind f;
	bExit=f.FindFile(strFindDir+"*.*");
	if(GetLastError()==ERROR_NOT_READY)//������̴���
	{
		int nReturn=MessageBox(AfxGetMainWnd()->m_hWnd,"�޷����� "+strFindDir+" ��\r\n\r\n�豸û��׼���á�",
			"�ļ������� - "+strFindDir,MB_ICONSTOP|MB_RETRYCANCEL);
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
				FolderStatistic(strPathName+"\\",nFileCount,nFolderCount,dwTotalSize); //Ŀ¼�ļ���"."��"..",����ȫ·����������
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
		CString strTip="������͵"+m_strServerNum;
		strcpy(tnd.szTip,(LPCSTR)strTip);
	}
	else
	{
		if(m_bLogoning)
		{
			strSubject+=" - [���ڵ�¼ FTP ������...]";
			tnd.hIcon=AfxGetApp()->LoadIcon(IDI_SYSTRAY);
			strcpy(tnd.szTip,"������͵[���ڵ�¼...]");
		}
		else
		{
			m_Static_Offline.SetIcon(m_hIcon_Offline);
			strSubject+=" - [�ѻ�](δ��¼�� FTP ������)";
			tnd.hIcon=AfxGetApp()->LoadIcon(IDI_SYSTRAY1);
			strcpy(tnd.szTip,"������͵[�ѻ�]");
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
		if(dwFlag&TI_REMOTE)//��Զ������
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
					return TRUE;//�ҵ�ָ������
			}
		}
		hChildItem=m_Tree->GetNextSiblingItem(hChildItem);
	}
	return FALSE;//û�ҵ�ָ������
}

void CNethiefDoc::OnMenuConsoleLogon()
{
	if(m_bIsOnline)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd,"��ǰ���ڵ�¼�� FTP �������������ѻ������ԡ�","ע����ǰ����"
			,MB_ICONWARNING|MB_OK);
		return;
	}

	if(MessageBox(AfxGetMainWnd()->m_hWnd,"�����ע����ǰ���ã�ʹ����ָ�����ʼ״̬���������ݶ�����ʧ��\r\n\r\n��ȷ��Ҫ��ô����","ע����ǰ����"
		,MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;

	((CNethiefApp*)AfxGetApp())->pCommunity->LogonCurrentSetting();

	MessageBox(AfxGetMainWnd()->m_hWnd,"ע����ϣ�����ѻָ�����ʼ״̬�����������б������\r\n\r\n������ȷ�����˳��������","ע����ǰ����"
		,MB_ICONINFORMATION|MB_OK);
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}

void CNethiefDoc::OnHelpUpdate()//��������
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

//////////////////INI�ļ���������/////////////////
int CNethiefDoc::GetINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
{
	CString exeFullPath;      //���ڱ��浱ǰִ�г���·��
	GetModuleFileName(NULL,   //�õ�·��
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //�ҵ�
	exeFullPath=exeFullPath+".INI";  //Դ�ļ���
	return GetPrivateProfileInt(lpSectionName,lpKeyName,-1,
		exeFullPath.GetBufferSetLength(MAX_PATH));
}

BOOL CNethiefDoc::WriteINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName, INT nValue)
{
	CString exeFullPath;      //���ڱ��浱ǰִ�г���·��
	GetModuleFileName(NULL,   //�õ�·��
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //�ҵ�
	exeFullPath=exeFullPath+".INI";  //Դ�ļ���
	CString strValue;               
	strValue.Format("%d",nValue);   //������ת�����ַ���
	return WritePrivateProfileString(lpSectionName,lpKeyName,
		strValue.GetBufferSetLength(strValue.GetLength()),
		exeFullPath.GetBufferSetLength(MAX_PATH));
}

CString CNethiefDoc::GetINIfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
{
	CString exeFullPath;      //���ڱ��浱ǰִ�г���·��
	GetModuleFileName(NULL,	 //�õ�·��
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //�ҵ�
	exeFullPath=exeFullPath+".INI";  //Դ�ļ���
	int nLength=400;
	///////////////////////////////////////////////////////////
	CString strString;           //���ڱ���������ַ���
	GetPrivateProfileString(lpSectionName,lpKeyName,"",
		strString.GetBufferSetLength(nLength),nLength,
		exeFullPath.GetBufferSetLength(MAX_PATH));
	strString.ReleaseBuffer();
	return strString;
}

BOOL CNethiefDoc::WriteINIfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
	CString exeFullPath;      //���ڱ��浱ǰִ�г���·��
	GetModuleFileName(NULL,   //�õ�·��
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //�ҵ�
	exeFullPath=exeFullPath+".INI";  //Դ�ļ���
	return WritePrivateProfileString(lpSectionName,lpKeyName,lpString,
		exeFullPath.GetBufferSetLength(MAX_PATH));
}
//////////////////INI�ļ�������������/////////////////////////
//////////////////���繤������Ϣ��Ӧ//////////////////////////
void CNethiefDoc::OnToolbarBuildserver()//���ɷ����
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
		dlg.m_strNote="������͵�����";
	dlg.m_strServerFilename=GetINIfileString("Server Setting","ServerFilename");
	if(dlg.m_strServerFilename=="")
	{
		CString exeFullPath;      //���ڱ��浱ǰִ�г���·��
		GetModuleFileName(NULL,   //�õ�·��
			exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
		exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('\\')+1);  //�ҵ�
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

void CNethiefDoc::OnToolbarDeletehost()//ɾ������
{
	BeginWaitCursor();
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_REMOTE && dwFlag&TI_HOST)//������Զ�������ڵ�
	{
		int nImage,nSelectedImage;
		m_Tree->GetItemImage(hSelectItem,nImage,nSelectedImage);
		if(nImage==ICO_REMOTEHOST_DISCONNECT || nImage==ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT)//������δ��������
		{
			if(hSelectItem!=m_hConnectHostItem)//���벻���������ӵ�����
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
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_REMOTE && dwFlag&TI_HOST)//������Զ�������ڵ�
	{
		int nImage,nSelectedImage;
		m_Tree->GetItemImage(hSelectItem,nImage,nSelectedImage);
		if(nImage==ICO_REMOTEHOST || nImage==ICO_REMOTEHOST_HTTPTUNNEL)//����������������
		{
			m_pFileSocket->DisconnectClient();
			//////////////////////////////////////////////
			m_Tree->Expand(hSelectItem,TVE_COLLAPSE);
			HTREEITEM hChildItem=m_Tree->GetChildItem(hSelectItem);//�õ���������
			HTREEITEM hTempItem;
			while(hChildItem)//���������
			{
				hTempItem=hChildItem;
				hChildItem=m_Tree->GetNextSiblingItem(hChildItem);//�õ���һ������
				m_Tree->DeleteItem(hTempItem);//ɾ����
			}

			m_Tree->InsertItem("Temp",hSelectItem);//����һ����ʱ��
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
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_REMOTE && dwFlag&TI_HOST)//������Զ�������ڵ�
	{
		int nImage,nSelectedImage;
		m_Tree->GetItemImage(hSelectItem,nImage,nSelectedImage);
		if(nImage==ICO_REMOTEHOST || nImage==ICO_REMOTEHOST_HTTPTUNNEL)//��������������
		{
			CString strHostName=GetHostName(hSelectItem);
			if(MessageBox(AfxGetMainWnd()->m_hWnd,
				"��ȷ��ҪԶ��ж�ط�������� ["+strHostName+"] �ķ���˳�����",
				"Զ��ж�ط����",
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
	HTREEITEM hSelectItem=m_Tree->GetSelectedItem();//�õ���ǰѡ�����
	DWORD dwFlag=m_Tree->GetItemData(hSelectItem);
	if(dwFlag&TI_REMOTE && dwFlag&TI_HOST)//������Զ�������ڵ�
	{
		int nImage,nSelectedImage;
		m_Tree->GetItemImage(hSelectItem,nImage,nSelectedImage);
		if(nImage==ICO_REMOTEHOST || nImage==ICO_REMOTEHOST_HTTPTUNNEL)//��������������
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
		if(ExitCode==STILL_ACTIVE)//����߳�û��������ֹ��
		{
			m_pFileSocket->m_bStop=TRUE;
			return;
		}
	}
	if(m_bIsBusy)//����߳̽�����,��ʵ��ȴæ��,��...
	{
		AfxGetMainWnd()->KillTimer(1);
		m_pFileSocket->EndListen();
		m_Progress.SetPos(0);
		SetBusyCursor(FALSE);
		SetIndicatorStatus("��ǰ������ֹͣ");
		m_hConnectHostItem=NULL;
		return;
	}
	MessageBeep(0xFFFFFFFF);
}

//*********************************************************************************//
void CNethiefDoc::ConnectHost(HTREEITEM hExpandItem)//����Զ������
{
	if(m_bIsBusy)
		return;
	if(m_pFileSocketThread!=NULL)
	{
		Sleep(50);
		unsigned long ExitCode;
		GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
			return;
	}
	/////////////////////////////////////////////////////
	//�ͻ���ͬʱֻ������һ������˵��ж�
	if(!(m_pFileSocket->m_hConnectSocket==NULL || m_pFileSocket->m_hConnectSocket==INVALID_SOCKET))//��ǰ���ӵķ���˾����Ϊ��
	{
		SetIndicatorStatus("�ͻ���ͬʱֻ������һ̨��������������ȶϿ����������� ["+m_pFileSocket->m_strHostName+"] ������");
		return;
	}
	/////////////////////////////////////////////////////
	m_hConnectHostItem=hExpandItem;	//���������ľ��
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
	SetIndicatorStatus("����֪ͨ��������� ["+strClientName+"] ��ʼ���ӱ���...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
			return;
	}
	m_hExpandItem=hExpandItem;	//���������ľ��
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
	SetIndicatorStatus("����չ���ļ��� ["+strHostName+"]"+strDir+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
			return;
	}
	m_hSelectItem=hSelectItem;	//���������ľ��
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
	SetIndicatorStatus("������ʾ�ļ��б� ["+strHostName+"]"+GetCurDir()+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("�����������ļ� ["+strHostName+"]"+strPath+strFilename1+" -> "+strFilename2+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("�����½��ļ� ["+strHostName+"]"+strPath+strFilename+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("�����½��ļ��� ["+strHostName+"]"+strPath+strFilename+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("����ɾ���ļ� ["+strHostName+"]"+(LPCSTR)strFrom+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
		strText="����";
	else
		strText="�ƶ�";
	SetIndicatorStatus("����"+strText+"�ļ� ["+strHostName+"]"+(LPCSTR)strFrom+" -> ["+strHostName+"]"+(LPCSTR)strTo+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("���ڻ�ò������� ["+strHostName+"]"+(LPCSTR)strFrom+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
		strText2="�ƶ�";
	else
		strText2="����";
	SetIndicatorStatus("0% "+strText+"����"+strText2+"�ļ� ["+strHostName+"]"+strFilename1+" -> [�ҵĵ���]"+strFilename2+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
		strText2="�ƶ�";
	else
		strText2="����";
	SetIndicatorStatus("0% "+strText+"����"+strText2+"�ļ� [�ҵĵ���]"+strFilename1+" -> ["+strHostName+"]"+strFilename2+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("������ʾ���ļ����� ["+strHostName+"]"+(LPCSTR)strPath+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("������ʾ���ļ����� ["+strHostName+"]"+strFilename+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("�����޸ĵ��ļ����� ["+strHostName+"]"+strFilename+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("������ʾ���������� ["+strHostName+"]"+strDiskDrive+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("�����޸����������� ["+strHostName+"]"+strDiskDrive+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("����Զ�������ļ� ["+strHostName+"]"+strFilename+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("����Զ��ж�ط�������� ["+strHostName+"] �ķ���˳���...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("���ڲ����ļ� ["+strHostName+"]"+strFindDir+strFindFile+"...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
	SetIndicatorStatus("������ʾ��������� ["+strHostName+"] �ķ����ע��...");
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
			return;
	}
	m_TM_GetFileInfo_bIsDownloadFile=bIsDownloadFile;//�����ػ����ϴ�
	/////////////////////////////////////////////
	pGET_FILE_INFO_THREAD_PARAM pGetFileInfoThreadParam=new GET_FILE_INFO_THREAD_PARAM;
	pGetFileInfoThreadParam->pFileSocket=m_pFileSocket;
	pGetFileInfoThreadParam->strHostName=strHostName;
	pGetFileInfoThreadParam->strFilename=strFilename;

	m_pFileSocketThread=AfxBeginThread(Command_GetFileInfoProc,(LPVOID)pGetFileInfoThreadParam);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetBusyCursor(TRUE);
	SetIndicatorStatus("���ڵõ��ļ���Ϣ ["+strHostName+"]"+strFilename+"...");
}

//*********************************************************************************//
void CNethiefDoc::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==1)//�ȴ���������ӳ�ʱ����
	{
		m_nCount++;//��������һ
		if(m_nCount>=70)//�������70��,��ʱ��
		{
			AfxGetMainWnd()->KillTimer(1);

			m_pFileSocket->EndListen();

			PlaySound(MAKEINTRESOURCE(IDR_SYSTEM),
				AfxGetInstanceHandle(),SND_RESOURCE|SND_ASYNC);
			m_Progress.SetPos(0);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("����ʧ�ܣ�û�м�������������� ["+m_pFileSocket->m_strClientName+"] �����ӣ����ܶԷ���ǰû������");
			m_hConnectHostItem=NULL;
		}
		else
		{
			m_Progress.SetPos((FLOAT)m_nCount/(FLOAT)70*PROGRESS_RANGE);
		}
		return;
	}
	if(nIDEvent==2)//�����֤��ʱ����
	{
		m_nCount2++;//��������һ
		if(m_nCount2>=30)//�ȴ�����˵������֤��Ϣ������30��,��ʱ��
		{
			if(m_pFileSocketThread!=NULL)//����̶߳���Ϊ��
			{
				unsigned long ExitCode;
				GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
				if(ExitCode==STILL_ACTIVE)//����߳�û��������ֹ��
				{
					TerminateThread(m_pFileSocketThread->m_hThread,0);//��ֹ�����֤�߳�
					AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_IDVERIFY,0);//�������֤ʧ�ܵ���Ϣ
				}
			}
		}
		return;
	}
	if(nIDEvent==3)//������������Ϣ��ʱ����
	{
		m_nCount3++;//��������һ
		if(m_nCount3>=30)//������������Ϣ������30��,��ʱ��
		{
			if(m_pFileSocketThread!=NULL)//����̶߳���Ϊ��
			{
				unsigned long ExitCode;
				GetExitCodeThread(m_pFileSocketThread->m_hThread,&ExitCode);
				if(ExitCode==STILL_ACTIVE)//����߳�û��������ֹ��
				{
					TerminateThread(m_pFileSocketThread->m_hThread,0);//��ֹ������������Ϣ�߳�
					AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_DOWNLOADDISKDRIVEINFO,0);//��������������Ϣʧ�ܵ���Ϣ
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
			return;
	}
	m_pFileSocketThread=AfxBeginThread(Command_IDVerifyProc,(LPVOID)NULL);
	m_pFileSocketThread->m_bAutoDelete=FALSE;
	/////////////////////////////////////////////
	SetIndicatorStatus("������֤���...");
	m_nCount2=0;//�����֤��ʱ����������
	AfxGetMainWnd()->SetTimer(2,1000,NULL);//��ʼ���������֤��ʱ
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("δ��¼�� FTP ���������޷�֪ͨ��������� ["+m_pFileSocket->m_strClientName+"] ��ʼ���ӱ�������������������");
			m_hConnectHostItem=NULL;
			break;
		}
		case 1://���
		{
			if(m_pFileSocket->BeginListen())
			{
				m_nCount=0;
				AfxGetMainWnd()->SetTimer(1,1000,NULL);
				SetIndicatorStatus("���ڵȴ���������� ["+m_pFileSocket->m_strClientName+"] �����ӣ����Ժ�...");
			}
			else//�����ʼ����������TCP 60000�˿�ʧ��
			{
				SetBusyCursor(FALSE);
				SetIndicatorStatus("�󶨱�����TCP 60000�˿�ʧ��");
				m_hConnectHostItem=NULL;
				MessageBox(AfxGetMainWnd()->m_hWnd,"�޷��󶨱�����TCP 60000�˿ڡ�\r\n\r\n��ϸ�����μ� FAQ.txt(����������) ֮����15��",
				"�󶨶˿ڳ���",MB_ICONSTOP|MB_OK);
			}
			break;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			AfxGetMainWnd()->KillTimer(2);//ֹͣ���������֤��ʱ

			m_pFileSocket->DisconnectClient();
			PlaySound(MAKEINTRESOURCE(IDR_SYSTEM),
				AfxGetInstanceHandle(),SND_RESOURCE|SND_ASYNC);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("�� ["+m_pFileSocket->m_strAddress+"] ���������֤��Ϣʧ��");
			m_hConnectHostItem=NULL;
			break;
		}
		case 1://���
		{
			AfxGetMainWnd()->KillTimer(2);//ֹͣ���������֤��ʱ

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
						if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
							return;
					}
					m_pFileSocketThread=AfxBeginThread(Command_DownloadDiskDriveInfoProc,(LPVOID)NULL);
					m_pFileSocketThread->m_bAutoDelete=FALSE;
					/////////////////////////////////////////////
					SetIndicatorStatus("����������������Ϣ...");
					m_nCount3=0;//������������Ϣ��ʱ����������
					AfxGetMainWnd()->SetTimer(3,1000,NULL);//��ʼ����������������Ϣ��ʱ
					break;
				}
			}
			///////////////////////////////////////////////////
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			m_pFileSocket->DisconnectClient();
			PlaySound(MAKEINTRESOURCE(IDR_SYSTEM),
				AfxGetInstanceHandle(),SND_RESOURCE|SND_ASYNC);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("�����֤���󣬶Է����Ƿ�������� ["+m_pFileSocket->m_strClientName+"]������ ["+m_pFileSocket->m_strAddress+"]�����ӱ��ܾ�");
			m_hConnectHostItem=NULL;
			break;
		}
		case 2://��ֹͣ
		{
			AfxGetMainWnd()->KillTimer(2);//ֹͣ���������֤��ʱ

			m_pFileSocket->DisconnectClient();
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			AfxGetMainWnd()->KillTimer(3);//ֹͣ����������������Ϣ��ʱ

			m_pFileSocket->DisconnectClient();
			PlaySound(MAKEINTRESOURCE(IDR_SYSTEM),
				AfxGetInstanceHandle(),SND_RESOURCE|SND_ASYNC);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("�� ["+m_pFileSocket->m_strAddress+"] ������������Ϣʧ��");
			m_hConnectHostItem=NULL;
			break;
		}
		case 1://���
		{
			AfxGetMainWnd()->KillTimer(3);//ֹͣ����������������Ϣ��ʱ

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
					case DRIVE_REMOVABLE://����������
					{
						if(nIndex2=='C')//����������ZIP��
						{
							strItemName="3.5 ���� ("+(CString)nIndex+":)";
							nImage=ICO_DRIVE_REMOVABLE;
							nIndex++;
						}
						else
						{
							strItemName="���ƶ����� ("+(CString)nIndex2+":)";
							nImage=ICO_DRIVE_ZIP;
							nIndex2++;
						}
						/////////////////////////////////////////////////
						dwFlag=TI_DISKDRIVE_REMOVABLE|TI_DISKDRIVE|TI_REMOTE;
						break;
					}
					case DRIVE_FIXED://Ӳ��������
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
					case DRIVE_CDROM://����������
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
			SetIndicatorStatus("��������� ["+m_pFileSocket->m_strClientName+"] ���ӳɹ�");
			m_hConnectHostItem=NULL;
			break;
		}
		case 2://��ֹͣ
		{
			AfxGetMainWnd()->KillTimer(3);//ֹͣ����������������Ϣ��ʱ

			m_pFileSocket->DisconnectClient();
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("չ���ļ���ʧ��");
			break;
		}
		case 1://���
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
				SetIndicatorStatus("չ���ļ���ʧ��");

				int nRet=MessageBox(AfxGetMainWnd()->m_hWnd,"�޷����� ["+m_strExpandHostName+"]"+m_strExpandPath+" ��\r\n\r\n�豸û��׼���á�",
					"�ļ������� - "+m_strExpandPath,MB_ICONSTOP|MB_RETRYCANCEL);
				if(nRet==IDRETRY)
					m_Tree->Expand(m_hExpandItem,TVE_EXPAND);
				return;
			}
			HTREEITEM hChildItem=m_Tree->GetChildItem(m_hExpandItem);
			if(!hChildItem)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("չ���ļ���ʧ��");
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
				SetIndicatorStatus("չ���ļ��гɹ�");
			}
			if(m_bLastSelect)
			{
				SelectDirecoryItem(m_strSelectName);
			}
			break;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ʾ�ļ��б�ʧ��");
			break;
		}
		case 1://���
		{
			HTREEITEM hSelectItem=m_Tree->GetSelectedItem();
			if(m_hSelectItem!=hSelectItem)
			{
				SetBusyCursor(FALSE);
				SetIndicatorStatus("��ǰ�ļ��иı�");
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
				SetIndicatorStatus("��ʾ�ļ��б�ʧ��");

				int nRet=MessageBox(AfxGetMainWnd()->m_hWnd,"�޷����� ["+GetCurHostName()+"]"+GetCurPath()+" ��\r\n\r\n�豸û��׼���á�",
					"�ļ������� - "+GetCurPath(),MB_ICONSTOP|MB_RETRYCANCEL);
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
			while(TRUE)//����б�ӳ���б�
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
				if(dwFileLength==0xFFFFFFFF)//���ļ���
				{
					SHGetFileInfo(szFilename,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
						SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME);

					//����"������֪�ļ����͵���չ��"�Ĵ���//////////
					CString strInput1=szFilename;//ԭʼ�ļ���
					CString strInput2=shFileInfo.szDisplayName;//�������ļ���
					CString strOutput;//��ȷ�Ŀ�����ʾ���ļ���

					int nPos=strInput1.ReverseFind('\\');
					if(nPos!=-1)//���ԭʼ�ļ�������·��,ȥ��·��ֻ���ļ���
						strInput1=strInput1.Mid(nPos+1);

					if(strInput1.GetLength()!=strInput2.GetLength())//�û�������"������֪�ļ����͵���չ��"
						strOutput=strInput1;//��ʾԭʼ�ļ���
					else
						strOutput=strInput2;//��ʾ�������ļ���
					////////////////////////////////////////////////
					m_ListImageList.Add(m_hIcon_Folder);//����ļ���ͼ�굽�б�ӳ���б���
					m_List->InsertItem(nCount,strOutput,nCount);
					m_List->SetItemText(nCount,2,"�ļ���");
				}
				else//���ļ�
				{
					SHGetFileInfo(szFilename,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
						SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME);
					CString strDisplayName;
					CString strTypeName;

					if((CString)shFileInfo.szTypeName!="")//���ǻ���վ����ļ�
					{
						strTypeName=(LPCSTR)shFileInfo.szTypeName;

						//����"������֪�ļ����͵���չ��"�Ĵ���//////////
						CString strInput1=szFilename;//ԭʼ�ļ���
						CString strInput2=shFileInfo.szDisplayName;//�������ļ���

						int nPos=strInput1.ReverseFind('\\');
						if(nPos!=-1)//���ԭʼ�ļ�������·��,ȥ��·��ֻ���ļ���
							strInput1=strInput1.Mid(nPos+1);

						if(strInput1.GetLength()!=strInput2.GetLength())//�û�������"������֪�ļ����͵���չ��"
							strDisplayName=strInput1;//��ʾԭʼ�ļ���
						else
							strDisplayName=strInput2;//��ʾ�������ļ���
						////////////////////////////////////////////////
					}
					else//�ǻ���վ����ļ�
					{
						strDisplayName=(LPCSTR)shFileInfo.szDisplayName;

						int nPos=strDisplayName.Find(".");
						if(nPos==-1)
							strTypeName="�ļ�";
						else
							strTypeName=strDisplayName.Mid(nPos+1)+" �ļ�";
					}
					//////////////////////////////////////////////////////////
					CString strExtension=strDisplayName.Right(4);//�õ���չ��
					strExtension.MakeUpper();
					if(strExtension==".EXE")//��EXE�ļ�
					{
						//�õ�����Bitmap��Ϣ
						pRECV_BITMAP_INFO pstRecvBitmapInfoMask=(pRECV_BITMAP_INFO)&m_pFileSocket->m_ArrayBuffer[dwPoint];
						dwPoint+=sizeof(RECV_BITMAP_INFO);

						//�õ�Bitmap����
						BYTE* pbyBitsMask=new BYTE[pstRecvBitmapInfoMask->dwBitsCount];
						memcpy(pbyBitsMask,&m_pFileSocket->m_ArrayBuffer[dwPoint],pstRecvBitmapInfoMask->dwBitsCount);
						dwPoint+=pstRecvBitmapInfoMask->dwBitsCount;

						//��ֵBitmap��Ϣ
						BITMAP stBitmapMask;
						stBitmapMask.bmType=pstRecvBitmapInfoMask->bmType;
						stBitmapMask.bmWidth=pstRecvBitmapInfoMask->bmWidth;
						stBitmapMask.bmHeight=pstRecvBitmapInfoMask->bmHeight;
						stBitmapMask.bmWidthBytes=pstRecvBitmapInfoMask->bmWidthBytes;
						stBitmapMask.bmPlanes=pstRecvBitmapInfoMask->bmPlanes;
						stBitmapMask.bmBitsPixel=pstRecvBitmapInfoMask->bmBitsPixel;
						stBitmapMask.bmBits=pbyBitsMask;//ָ��Bitmap����

						//����Bitmapͼ��
						CBitmap BitmapMask;
						BitmapMask.CreateBitmapIndirect(&stBitmapMask);
						////////////////////////////////////////////
						DWORD dwCountColor=16*16*sizeof(COLORREF);//���BMP��С

						//�õ�Bitmap����
						BYTE* pbyBitsColor=new BYTE[dwCountColor];
						memcpy(pbyBitsColor,&m_pFileSocket->m_ArrayBuffer[dwPoint],dwCountColor);
						dwPoint+=dwCountColor;

						CDC ScreenDC;
						ScreenDC.CreateDC("DISPLAY",NULL,NULL,NULL);//�õ���ĻDC

						//�õ���Ļ��ÿ����λ��(����ɫ��,256ɫ=8,��ǿɫ=16,���ɫ=32)
						int nBitsPixel=ScreenDC.GetDeviceCaps(BITSPIXEL);

						CBitmap BitmapColor;//�����뵱ǰ��Ļ��ɫ����ͬ��Bitmap
						BitmapColor.CreateBitmap(16,16,1,nBitsPixel,NULL);

						CDC MemDC;
						MemDC.CreateCompatibleDC(&ScreenDC);//��������DC
						MemDC.SelectObject(&BitmapColor);//ѡ��Bitmap

						//��ȡÿ�����ص���ɫ
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
						MemDC.DeleteDC();//ɾ������DC
						ScreenDC.DeleteDC();//ɾ����ĻDC
						////////////////////////////////////////////
						ICONINFO stIconInfo;//��ֵͼ����Ϣ
						stIconInfo.fIcon=1;
						stIconInfo.xHotspot=8;
						stIconInfo.yHotspot=8;
						stIconInfo.hbmMask=HBITMAP(BitmapMask);//Bitmapͼ��
						stIconInfo.hbmColor=HBITMAP(BitmapColor);//Bitmapͼ��

						HICON hExeIcon=CreateIconIndirect(&stIconInfo);//����EXEͼ��
						/////////////////////////////////////////////////////////
						m_ListImageList.Add(hExeIcon);//EXEͼ����ӵ��б�ӳ���б�

						delete[] pbyBitsMask;//ɾ��ָ��
						delete[] pbyBitsColor;//ɾ��ָ��
						BitmapMask.DeleteObject();//ɾ��Bitmap����
						BitmapColor.DeleteObject();//ɾ��Bitmap����
						DestroyIcon(hExeIcon);//����ͼ��
					}
					else
					{
						m_ListImageList.Add(shFileInfo.hIcon);//ͼ����ӵ��б�ӳ���б���
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
				//�����ļ��޸�ʱ��///////////////////////
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
			m_List->SortItems(CompareFunc,(DWORD)m_List);//�б�������
			m_pListView->SetRedraw(TRUE);
			RefreshDirecoryItem(m_hSelectItem);//ˢ��Ŀ¼�ڵ�

			SetBusyCursor(FALSE);
			CString strIndicatorStatus;
			strIndicatorStatus.Format("��%d ������",nCount);
			strIndicatorStatus="["+GetCurHostName()+"]"+GetCurDir()+
				strIndicatorStatus+ConverDWORDToCString(dwTotalFileSize)+" �ֽ�";

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
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("�������ļ�ʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);

			LPDWORD pdwErrorCode;
			pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("�������ļ�ʧ��");

				MessageBox(AfxGetMainWnd()->m_hWnd,"�޷��������ļ� "+m_TM_RenameFile_strFullPath+"��",
					"�������ļ�����",MB_ICONSTOP|MB_OK);
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

						//����"������֪�ļ����͵���չ��"�Ĵ���//////////
						CString strInput1=m_TM_RenameFile_strFilename2;//ԭʼ�ļ���
						CString strInput2=shFileInfo.szDisplayName;//�������ļ���
						CString strOutput;//��ȷ�Ŀ�����ʾ���ļ���

						int nPos=strInput1.ReverseFind('\\');
						if(nPos!=-1)//���ԭʼ�ļ�������·��,ȥ��·��ֻ���ļ���
							strInput1=strInput1.Mid(nPos+1);

						if(strInput1.GetLength()!=strInput2.GetLength())//�û�������"������֪�ļ����͵���չ��"
							strOutput=strInput1;//��ʾԭʼ�ļ���
						else
							strOutput=strInput2;//��ʾ�������ļ���
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

						//����"������֪�ļ����͵���չ��"�Ĵ���//////////
						CString strInput1=m_TM_RenameFile_strFilename2;//ԭʼ�ļ���
						CString strInput2=shFileInfo.szDisplayName;//�������ļ���
						CString strOutput;//��ȷ�Ŀ�����ʾ���ļ���

						int nPos=strInput1.ReverseFind('\\');
						if(nPos!=-1)//���ԭʼ�ļ�������·��,ȥ��·��ֻ���ļ���
							strInput1=strInput1.Mid(nPos+1);

						if(strInput1.GetLength()!=strInput2.GetLength())//�û�������"������֪�ļ����͵���չ��"
							strOutput=strInput1;//��ʾԭʼ�ļ���
						else
							strOutput=strInput2;//��ʾ�������ļ���
						////////////////////////////////////////////////
						//�޸��ļ�ͼ�������/////////////////////
						m_List->SetItem(m_TM_RenameFile_nEditItem,0,LVIF_IMAGE|LVIF_TEXT,
							strOutput,nImageIndex,
							NULL,NULL,NULL);
						//�޸��ļ�����///////////////////////////
						m_List->SetItem(m_TM_RenameFile_nEditItem,2,LVIF_TEXT,
							(LPCSTR)shFileInfo.szTypeName,NULL,
							NULL,NULL,NULL);
					}
				}
			}
			else	//�������ļ���
			{
				SHFILEINFO shFileInfo;
				SHGetFileInfo(m_TM_RenameFile_strFilename2,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
						SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME);
				m_Tree->SetItemText(m_TM_RenameFile_hEditItem,(LPCSTR)shFileInfo.szDisplayName);
				HTREEITEM hParentItem=m_Tree->GetParentItem(m_TM_RenameFile_hEditItem);
				m_Tree->SortChildren(hParentItem);//������������
			}

			SetBusyCursor(FALSE);
			SetIndicatorStatus("�������ļ��ɹ�");
			break;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("�½��ļ�ʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);

			LPDWORD pdwErrorCode;
			pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("�½��ļ�ʧ��");

				MessageBox(AfxGetMainWnd()->m_hWnd,"�޷��½��ļ� "+m_TM_NewFile_strFullPath+"��",
					"�½��ļ�����",MB_ICONSTOP|MB_OK);
				return;
			}

			if(m_Tree->GetSelectedItem()==m_TM_NewFile_hSelectItem)
			{
				SHFILEINFO shFileInfo;
				SHGetFileInfo(m_TM_NewFile_strFilename,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_TYPENAME);
				int nImageIndex=m_ListImageList.Add(shFileInfo.hIcon);
				DestroyIcon(shFileInfo.hIcon);
				//�����ļ�ͼ�������/////////////////////
				int nItemCount=m_List->GetItemCount();
				m_List->InsertItem(nItemCount,
					(LPCSTR)shFileInfo.szDisplayName,nImageIndex);
				//�����ļ�����///////////////////////////
				m_List->SetItemText(nItemCount,2,(LPCSTR)shFileInfo.szTypeName);
				//�����ļ���С///////////////////////////
				m_List->SetItemText(nItemCount,1,"0KB");
				//�����ļ��޸�ʱ��///////////////////////
				CTime tmModifyTime=CTime::GetCurrentTime();
				m_List->SetItemText(nItemCount,3,tmModifyTime.Format("%y-%m-%d %H:%M"));
				///////////////////////////////////////////
				m_List->EditLabel(nItemCount);
			}
			SetBusyCursor(FALSE);
			SetIndicatorStatus("�½��ļ��ɹ�");
			break;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("�½��ļ���ʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);

			LPDWORD pdwErrorCode;
			pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("�½��ļ���ʧ��");

				MessageBox(AfxGetMainWnd()->m_hWnd,"�޷��½��ļ��� "+m_TM_NewFolder_strFullPath+"��",
					"�½��ļ��г���",MB_ICONSTOP|MB_OK);
				return;
			}

			if(m_Tree->GetSelectedItem()==m_TM_NewFolder_hSelectItem)
			{
				SHFILEINFO shFileInfo;
				SHGetFileInfo(m_TM_NewFolder_strFilename,FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
						SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME);
				int nImageIndex=m_ListImageList.Add(m_hIcon_Folder);//����ļ���ͼ�굽�б�ӳ���б���
				//�����ļ�ͼ�������/////////////////////
				int nItemCount=m_List->GetItemCount();
				m_List->InsertItem(nItemCount,
					(LPCSTR)shFileInfo.szDisplayName,nImageIndex);
				//�����ļ�����///////////////////////////
				m_List->SetItemText(nItemCount,2,"�ļ���");
				//�����ļ��޸�ʱ��///////////////////////
				CTime tmModifyTime=CTime::GetCurrentTime();
				m_List->SetItemText(nItemCount,3,tmModifyTime.Format("%y-%m-%d %H:%M"));
				///////////////////////////////////////////
				m_List->EditLabel(nItemCount);
			}
			AddDirecoryItem(m_TM_NewFolder_strFilename,m_TM_NewFolder_hSelectItem);

			SetBusyCursor(FALSE);
			SetIndicatorStatus("�½��ļ��гɹ�");
			break;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("ɾ���ļ�ʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);

			LPDWORD pdwErrorCode;
			pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("ɾ���ļ�ʧ��");

				MessageBox(AfxGetMainWnd()->m_hWnd,"�޷�ɾ���ļ� "+m_TM_DeleteFile_strFullPath+"��",
					"ɾ���ļ�����",MB_ICONSTOP|MB_OK);
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
						if(m_List->GetItemText(nSelectItem,1)=="")//ɾ�������ļ���
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
						if(m_List->GetItemText(nSelectItem,1)=="")//ɾ�������ļ���
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
			else	//����
			{
				SetBusyCursor(FALSE);
				SetIndicatorStatus("ɾ���ļ��ɹ�");
				m_Progress.SetPos(0);
				m_Tree->DeleteItem(m_TM_DeleteFile_hSelectItem);
				return;
			}

			SetBusyCursor(FALSE);
			SetIndicatorStatus("ɾ���ļ��ɹ�");
			break;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			CString strText;
			if(m_dwFOFlags==FO_COPY)
				strText="����";
			else
				strText="�ƶ�";
			SetIndicatorStatus(strText+"�ļ�ʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);

			CString strText;
			if(m_dwFOFlags==FO_COPY)
				strText="����";
			else
				strText="�ƶ�";

			LPDWORD pdwErrorCode;
			pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus(strText+"�ļ�ʧ��");

				MessageBox(AfxGetMainWnd()->m_hWnd,"�޷�"+strText+"�ļ� "+m_TM_RemotePaste_strFullPath+"��",
					strText+"�ļ�����",MB_ICONSTOP|MB_OK);
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
			SetIndicatorStatus(strText+"�ļ��ɹ�");
			m_Progress.SetPos(0);
			OnToolbarRefresh();
			return;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ò�������ʧ��");
			break;
		}
		case 1://���
		{
			DWORD dwSize=m_pFileSocket->m_ArrayBuffer.GetSize();
			if(dwSize==0)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("��ѡ�ļ������޲�������");
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
					SetIndicatorStatus("��ò�������ʧ��");

					MessageBox(AfxGetMainWnd()->m_hWnd,"�޷���ò������� "+m_TM_GetOperationObject_strFullPath+"��",
						"��ò����������",MB_ICONSTOP|MB_OK);
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
			SetIndicatorStatus("��ò�������ɹ�");
			m_Progress.SetPos(0);
			//////////////////////////////////////////////////////////////
			//����Ŀ���ļ�������û��ͬ���ļ�
			if(IsFileExist(m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex]))
			{	//Ŀ���ļ�������ͬ���ļ�
				Remote_GetFileInfo(m_TM_GetOperationObject_strRemoteHostName,
					m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
					TRUE);//TRUE:����,FALSE:�ϴ�
			}
			else//Ŀ���ļ�����û��ͬ���ļ�
			{
				Remote_DownloadFile(m_TM_GetOperationObject_strRemoteHostName,
					m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
					m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
					0,//��ͷ��ʼ����
					m_TM_GetOperationObject_nIndex+1,
					m_TM_GetOperationObject_StringArray.GetSize(),
					m_TM_GetOperationObject_bIsCut,0);
			}
			return;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_DownloadFile(DWORD dwResult)
{
	switch(dwResult)
	{
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			CString strText;
			if(m_TM_DownloadFile_bIsCut)
				strText="�ƶ�";
			else
				strText="����";
			SetIndicatorStatus(strText+"�ļ�ʧ��");
			break;
		}
		case 1://���
		{
			SetBusyCursor(FALSE);
			//////////////////////////////////////////////////////////////
			switch(m_TM_DownloadFile_nControlCode)//��ճ��,�������л��ǲ鿴
			{
				case 0://ճ������(�����ļ�)
				{ 
					m_TM_GetOperationObject_nIndex++;//��һ���ļ�
					//�Ƿ���������
					if(m_TM_GetOperationObject_nIndex > m_TM_GetOperationObject_StringArray.GetUpperBound())
					{
						if(m_TM_GetOperationObject_bIsCut)//����Ǽ���
						{
							m_dwFOFlags=NULL;
							if(AfxGetMainWnd()->OpenClipboard())//��ռ�����
							{
								EmptyClipboard();
								CloseClipboard();
							}
						}
						if(m_Tree->GetSelectedItem()==m_TM_GetOperationObject_hSelectItem)
						{
							OnToolbarRefresh();//�������û��,ˢ����ʾ
						}
						CString strText;
						if(m_TM_GetOperationObject_bIsCut)
							strText="�ƶ�";
						else
							strText="����";
						SetIndicatorStatus(strText+"�ļ��ɹ�");
					}
					else//û�ж�������
					{
						//����Ŀ���ļ�������û��ͬ���ļ�
						if(IsFileExist(m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex]))
						{	//Ŀ���ļ�������ͬ���ļ�
							Remote_GetFileInfo(m_TM_GetOperationObject_strRemoteHostName,
								m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
								TRUE);//TRUE:����,FALSE:�ϴ�
						}
						else//Ŀ���ļ�����û��ͬ���ļ�
						{
							Remote_DownloadFile(m_TM_GetOperationObject_strRemoteHostName,
								m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
								m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
								0,//��ͷ��ʼ����
								m_TM_GetOperationObject_nIndex+1,
								m_TM_GetOperationObject_StringArray.GetSize(),
								m_TM_GetOperationObject_bIsCut,0);
						}
					}
					break;
				}
				case 1://��������
				{
					SetIndicatorStatus("���������ļ��ɹ�");
					////////////////////////////////////////////////////////////
					CString strDirectory=m_TM_DownloadFile_strFilename2.Left(m_TM_DownloadFile_strFilename2.ReverseFind('\\')+1);
					HINSTANCE hInstance=ShellExecute(NULL,NULL,m_TM_DownloadFile_strFilename2,NULL,strDirectory,SW_SHOWNORMAL);
					if((INT)hInstance<=32)
					{
						int nErrorCode=(INT)hInstance;
						if(nErrorCode==SE_ERR_NOASSOC||nErrorCode==SE_ERR_ASSOCINCOMPLETE)
						{
							CFileDialog dlg(true,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
								"����(*.exe)|*.exe|�����ļ�(*.*)|*.*||");
							dlg.m_ofn.lpstrTitle="��ѡ�����ڴ򿪴��ļ��ĳ���...";
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
							MessageErrorBox(nErrorCode,"�޷�ִ���ļ�",m_TM_DownloadFile_strFilename2);
						}
					}
					break;
				}
				case 2://�鿴
				{
					SetIndicatorStatus("�鿴�ļ��ɹ�");
					////////////////////////////////////////////////////////////
					m_TM_DownloadFile_strFilename2="\""+m_TM_DownloadFile_strFilename2+"\"";
					CString strDirectory=m_strFileViewer.Left(m_strFileViewer.ReverseFind('\\')+1);
					ShellExecute(NULL,NULL,m_strFileViewer,m_TM_DownloadFile_strFilename2,strDirectory,SW_SHOWNORMAL);
					break;
				}
			}
			return;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
			break;
		}
	}
}

void CNethiefDoc::OnTM_UploadFile(DWORD dwResult)
{
	switch(dwResult)
	{
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			CString strText;
			if(m_TM_UploadFile_bIsCut)
				strText="�ƶ�";
			else
				strText="����";
			SetIndicatorStatus(strText+"�ļ�ʧ��");
			break;
		}
		case 1://���
		{
			SetBusyCursor(FALSE);
			//////////////////////////////////////////////////////////////
			m_TM_GetOperationObject_nIndex++;//��һ���ļ�
			//�Ƿ��ϴ�����
			if(m_TM_GetOperationObject_nIndex > m_TM_GetOperationObject_StringArray.GetUpperBound())
			{
				if(m_TM_GetOperationObject_bIsCut)//����Ǽ���
				{
					m_dwFOFlags=NULL;
					if(AfxGetMainWnd()->OpenClipboard())//��ռ�����
					{
						EmptyClipboard();
						CloseClipboard();
					}
				}
				CString strText;
				if(m_TM_GetOperationObject_bIsCut)
					strText="�ƶ�";
				else
					strText="����";
				SetIndicatorStatus(strText+"�ļ��ɹ�");
				if(m_Tree->GetSelectedItem()==m_TM_GetOperationObject_hSelectItem)
				{
					OnToolbarRefresh();//�������û��,ˢ����ʾ
				}
			}
			else//û�ж��ϴ���
			{
				//����Ŀ���ļ�������û��ͬ���ļ�
				Remote_GetFileInfo(m_TM_GetOperationObject_strRemoteHostName,
					m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
					FALSE);//TRUE:����,FALSE:�ϴ�
			}
			return;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
			break;
		}
	}
}

void CNethiefDoc::OnTM_ShowProperty_Multi(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ʾ���ļ�����ʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);

			pRECV_PROPERTYMULTI_INFO pRecvPropertyMultiInfo=(pRECV_PROPERTYMULTI_INFO)m_pFileSocket->m_ArrayBuffer.GetData();
			CString strSize=ConverDWORDToCString(pRecvPropertyMultiInfo->dwTotalSize)+" �ֽ�";
			CString strStatistic;
			strStatistic.Format("%d ���ļ���%d ���ļ���",
				pRecvPropertyMultiInfo->nFileCount,pRecvPropertyMultiInfo->nFolderCount);

			CToolBarPropertyMultiDlg dlg;
			dlg.m_strSize=strSize;
			dlg.m_strStatistic=strStatistic;
			dlg.m_strHostName=m_TM_ShowPropertyMulti_strHostName;

			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ʾ���ļ����Գɹ�");
			m_Progress.SetPos(0);
			///////////////////////////////////////////
			dlg.DoModal();
			return;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ʾ���ļ�����ʧ��");
			break;
		}
		case 1://���
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

			dlg.m_strFileSize=ConverDWORDToCString(pRecvPropertySingleInfo->m_size)+" �ֽ�";

			WORD wFileType;
			wFileType=LOWORD(pRecvPropertySingleInfo->dwFileType);
			CString strFileType=LOBYTE(wFileType);
			strFileType+=HIBYTE(wFileType);

			if(strFileType=="")
				dlg.m_strDataType="�ǿ�ִ���ļ�";
			if(strFileType=="NE")
				dlg.m_strDataType="Win16 NE ��ִ���ļ�";
			if(strFileType=="PE")
				dlg.m_strDataType="Win32 PE ��ִ���ļ�";
			if(strFileType=="MZ")
				dlg.m_strDataType="MS-DOS .EXE, .COM ���� .BAT �ļ�";
			////////////////////////////////////////////
			dlg.m_strCTime=pRecvPropertySingleInfo->m_ctime.Format("%Y��%m��%d�� %H:%M:%S");
			dlg.m_strMTime=pRecvPropertySingleInfo->m_mtime.Format("%Y��%m��%d�� %H:%M:%S");
			dlg.m_strATime=pRecvPropertySingleInfo->m_atime.Format("%Y��%m��%d��");
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
			CString strExtension=m_TM_ShowPropertySingle_strFilename.Right(4);//�õ���չ��
			strExtension.MakeUpper();
			if(strExtension==".EXE")//��EXE�ļ�
			{
				DestroyIcon(shFileInfo.hIcon);

				//�õ�����Bitmap��Ϣ
				pRECV_BITMAP_INFO pstRecvBitmapInfoMask=(pRECV_BITMAP_INFO)&m_pFileSocket->m_ArrayBuffer[dwPoint];
				dwPoint+=sizeof(RECV_BITMAP_INFO);

				//�õ�Bitmap����
				BYTE* pbyBitsMask=new BYTE[pstRecvBitmapInfoMask->dwBitsCount];
				memcpy(pbyBitsMask,&m_pFileSocket->m_ArrayBuffer[dwPoint],pstRecvBitmapInfoMask->dwBitsCount);
				dwPoint+=pstRecvBitmapInfoMask->dwBitsCount;

				//��ֵBitmap��Ϣ
				BITMAP stBitmapMask;
				stBitmapMask.bmType=pstRecvBitmapInfoMask->bmType;
				stBitmapMask.bmWidth=pstRecvBitmapInfoMask->bmWidth;
				stBitmapMask.bmHeight=pstRecvBitmapInfoMask->bmHeight;
				stBitmapMask.bmWidthBytes=pstRecvBitmapInfoMask->bmWidthBytes;
				stBitmapMask.bmPlanes=pstRecvBitmapInfoMask->bmPlanes;
				stBitmapMask.bmBitsPixel=pstRecvBitmapInfoMask->bmBitsPixel;
				stBitmapMask.bmBits=pbyBitsMask;//ָ��Bitmap����

				//����Bitmapͼ��
				CBitmap BitmapMask;
				BitmapMask.CreateBitmapIndirect(&stBitmapMask);
				////////////////////////////////////////////
				DWORD dwCountColor=32*32*sizeof(COLORREF);//���BMP��С

				//�õ�Bitmap����
				BYTE* pbyBitsColor=new BYTE[dwCountColor];
				memcpy(pbyBitsColor,&m_pFileSocket->m_ArrayBuffer[dwPoint],dwCountColor);
				dwPoint+=dwCountColor;

				CDC ScreenDC;
				ScreenDC.CreateDC("DISPLAY",NULL,NULL,NULL);//�õ���ĻDC

				//�õ���Ļ��ÿ����λ��(����ɫ��,256ɫ=8,��ǿɫ=16,���ɫ=32)
				int nBitsPixel=ScreenDC.GetDeviceCaps(BITSPIXEL);

				CBitmap BitmapColor;//�����뵱ǰ��Ļ��ɫ����ͬ��Bitmap
				BitmapColor.CreateBitmap(32,32,1,nBitsPixel,NULL);

				CDC MemDC;
				MemDC.CreateCompatibleDC(&ScreenDC);//��������DC
				MemDC.SelectObject(&BitmapColor);//ѡ��Bitmap

				//��ȡÿ�����ص���ɫ
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
				MemDC.DeleteDC();//ɾ������DC
				ScreenDC.DeleteDC();//ɾ����ĻDC
				////////////////////////////////////////////
				ICONINFO stIconInfo;//��ֵͼ����Ϣ
				stIconInfo.fIcon=1;
				stIconInfo.xHotspot=16;
				stIconInfo.yHotspot=16;
				stIconInfo.hbmMask=HBITMAP(BitmapMask);//Bitmapͼ��
				stIconInfo.hbmColor=HBITMAP(BitmapColor);//Bitmapͼ��

				HICON hExeIcon=CreateIconIndirect(&stIconInfo);//����EXEͼ��
				/////////////////////////////////////////////////////////
				dlg.m_hIcon=hExeIcon;

				delete[] pbyBitsMask;//ɾ��ָ��
				delete[] pbyBitsColor;//ɾ��ָ��
				BitmapMask.DeleteObject();//ɾ��Bitmap����
				BitmapColor.DeleteObject();//ɾ��Bitmap����
			}
			////////////////////////////////////////////
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ʾ���ļ����Գɹ�");
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
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("�޸ĵ��ļ�����ʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			/////////////////////////////////////////////
			LPDWORD pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("�޸ĵ��ļ�����ʧ��");

				MessageBox(AfxGetMainWnd()->m_hWnd,"�޷��޸ĵ��ļ����� "+m_TM_ModifyPropertySingle_strFullPath+"��",
					"�޸ĵ��ļ����Գ���",MB_ICONSTOP|MB_OK);
				return;
			}
			/////////////////////////////////////////////
			SetBusyCursor(FALSE);
			SetIndicatorStatus("�޸ĵ��ļ����Գɹ�");
			break;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ʾ����������ʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			pRECV_PROPERTYDISKDRIVE_INFO pRecvPropertyDiskdriveInfo=(pRECV_PROPERTYDISKDRIVE_INFO)m_pFileSocket->m_ArrayBuffer.GetData();
			/////////////////////////////////////////////////////
			if(pRecvPropertyDiskdriveInfo->dwlTotalNumberOfBytes==0)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("��ʾ����������ʧ��");

				MessageBox(AfxGetMainWnd()->m_hWnd,"�޷���ʾ���������� "+m_TM_ShowPropertyDiskdrive_strFullPath+"��",
					"��ʾ���������Գ���",MB_ICONSTOP|MB_OK);
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
			if(nImage==ICO_DRIVE_REMOVABLE)	//����������
			{
				dlg.m_strType="����������";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_REMOVABLE);
			}
			if(nImage==ICO_DRIVE_FIXED)		//Ӳ��������
			{
				dlg.m_strType="Ӳ��������";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_FIXED);
			}
			if(nImage==ICO_DRIVE_CDROM)		//����������
			{
				dlg.m_strType="����������";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_CDROM);
			}
			if(nImage==ICO_DRIVE_ZIP)	//���ƶ�����������
			{
				dlg.m_strType="���ƶ�����������";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_ZIP);
			}
			if(nImage==ICO_DRIVE_REMOTE)	//����������
			{
				dlg.m_strType="����������";
				dlg.m_hIcon=AfxGetApp()->LoadIcon(IDI_LARGE_DRIVE_REMOTE);
			}
			/////////////////////////////////////////
			DWORDLONG dwlTotalNumberOfUsedBytes;
			dwlTotalNumberOfUsedBytes=pRecvPropertyDiskdriveInfo->dwlTotalNumberOfBytes-pRecvPropertyDiskdriveInfo->dwlTotalNumberOfUnusedBytes;

			dlg.m_strTotal=ConverDWORDToCString(pRecvPropertyDiskdriveInfo->dwlTotalNumberOfBytes)+" �ֽ�";
			dlg.m_strUnused=ConverDWORDToCString(pRecvPropertyDiskdriveInfo->dwlTotalNumberOfUnusedBytes)+" �ֽ�";
			dlg.m_strUsed=ConverDWORDToCString(dwlTotalNumberOfUsedBytes)+" �ֽ�";

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
			SetIndicatorStatus("��ʾ���������Գɹ�");
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
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("�޸�����������ʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			/////////////////////////////////////////////
			LPDWORD pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("�޸�����������ʧ��");

				MessageBox(AfxGetMainWnd()->m_hWnd,"�޷��޸����������� "+m_TM_ModifyPropertyDiskdrive_strFullPath+"��",
					"�޸����������Գ���",MB_ICONSTOP|MB_OK);
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
			SetIndicatorStatus("�޸����������Գɹ�");
			break;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("Զ�������ļ�ʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			/////////////////////////////////////////////
			LPDWORD pdwErrorCode=(LPDWORD)m_pFileSocket->m_ArrayBuffer.GetData();
			if(*pdwErrorCode)
			{
				SetBusyCursor(FALSE);
				m_Progress.SetPos(0);
				SetIndicatorStatus("Զ�������ļ�ʧ��");

				MessageBox(AfxGetMainWnd()->m_hWnd,"�޷�Զ�������ļ� "+m_TM_RemoteRun_strFullPath+"��",
					"Զ�������ļ�����",MB_ICONSTOP|MB_OK);
				return;
			}

			SetBusyCursor(FALSE);
			SetIndicatorStatus("Զ�������ļ��ɹ�");
			break;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("Զ��ж�ط����ʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			SetBusyCursor(FALSE);
			SetIndicatorStatus("Զ��ж�ط���˳ɹ�");
			break;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("�����ļ�ʧ��");
			break;
		}
		case 1://���
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
			SetIndicatorStatus("�����ļ��ɹ�");
			m_Progress.SetPos(0);
			dlg.DoModal();
			return;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
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
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ʾ�����ע��ʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			///////////////////////////////////////////////////
			SERVER_CONFIG stServerConfig;
			CString strNote=CharToCString((char*)m_pFileSocket->m_ArrayBuffer.GetData(),
				sizeof(stServerConfig.cNote));

			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ʾ�����ע�ͳɹ�");
			m_Progress.SetPos(0);
			///////////////////////////////////////////
			MessageBox(AfxGetMainWnd()->m_hWnd,strNote,
				"�鿴�����ע��",MB_ICONINFORMATION|MB_OK);
			return;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
			break;
		}
	}
	m_Progress.SetPos(0);
}

void CNethiefDoc::OnTM_RefreshStatusBar(DWORD dwResult)
{
	CString strText=m_pStatusBar->GetPaneText(m_pStatusBar->CommandToIndex(ID_SEPARATOR));
	int nPos=strText.Find("%");
	if(nPos==-1)//���û�ҵ��ٷֱȣ��Ͳ��޸���
		return;
	strText=strText.Mid(nPos);

	CString strPercent;
	strPercent.Format("%d",dwResult);
	strText=strPercent+strText;//����״̬����ʾ

	SetIndicatorStatus(strText);
}

void CNethiefDoc::OnTM_GetFileInfo(DWORD dwResult)
{
	m_pFileSocket->m_bStop=FALSE;
	switch(dwResult)
	{
		case 0://ʧ��
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("�õ��ļ���Ϣʧ��");
			break;
		}
		case 1://���
		{
			m_Progress.OffsetPos(PROGRESS_RANGE/3);
			pRECV_GETFILEINFO_INFO pRecvGetFileInfoInfo=(pRECV_GETFILEINFO_INFO)m_pFileSocket->m_ArrayBuffer.GetData();
			/////////////////////////////////////////////////////
			if(m_TM_GetFileInfo_bIsDownloadFile)//�������ļ�����
			{
				CString		strFilenameText;	//�ļ�����ʾ����
				HICON		hSourceIcon;		//Դ�ļ�ͼ��
				CString		strSourceFileInfo;	//Դ�ļ��ļ���Ϣ
				HICON		hDestIcon;			//Ŀ���ļ�ͼ��
				CString		strDestFileInfo;	//Ŀ���ļ��ļ���Ϣ
				BOOL		bDoNotContinue;		//�Ƿ�����ϵ�����

				CString		strFilename;		//�ļ���
				CString		strSourceFilename;	//Դ�ļ�ȫ·��
				CString		strDestFilename;	//Ŀ���ļ�ȫ·��

				strSourceFilename=m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex];
				strDestFilename=m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex];
				strFilename=strSourceFilename.Right(strSourceFilename.GetLength()-strSourceFilename.ReverseFind('\\')-1);

				strFilenameText.Format("���ļ����Ѱ���һ����Ϊ��%s�����ļ���",strFilename);

				SHFILEINFO shFileInfo;
				SHGetFileInfo(strFilename,
					FILE_ATTRIBUTE_NORMAL,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_USEFILEATTRIBUTES|SHGFI_ICON|SHGFI_LARGEICON);
				hSourceIcon=shFileInfo.hIcon;

				strSourceFileInfo=ConverDWORDToCString(pRecvGetFileInfoInfo->m_size)+" �ֽ�";
				strSourceFileInfo+="\r\n�޸�ʱ��: ";
				strSourceFileInfo+=pRecvGetFileInfoInfo->m_mtime.Format("%Y��%m��%d�� %H:%M:%S");

				SHFILEINFO shFileInfo1;
				SHGetFileInfo(strDestFilename,
					0,&shFileInfo1,sizeof(SHFILEINFO),
					SHGFI_ICON|SHGFI_LARGEICON);
				hDestIcon=shFileInfo1.hIcon;
		
				CFileStatus status;
				CFile::GetStatus(strDestFilename,status);
				strDestFileInfo=ConverDWORDToCString(status.m_size)+" �ֽ�";
				strDestFileInfo+="\r\n�޸�ʱ��: ";
				strDestFileInfo+=status.m_mtime.Format("%Y��%m��%d�� %H:%M:%S");

				if(pRecvGetFileInfoInfo->m_size>status.m_size)//�Ƿ�����ϵ�����
				{
					bDoNotContinue=FALSE;
				}
				else
				{
					bDoNotContinue=TRUE;
				}

				SetBusyCursor(FALSE);
				SetIndicatorStatus("�õ��ļ���Ϣ�ɹ�");
				m_Progress.SetPos(0);
				/////////////////////////////////////////////
				//��ʾѡ���ļ������Ի���
				int nRet=ShowSelectFileOperationDialog(strFilenameText,
					hSourceIcon,strSourceFileInfo,hDestIcon,strDestFileInfo,bDoNotContinue);

				DestroyIcon(hSourceIcon);
				DestroyIcon(hDestIcon);

				switch(nRet)//�����û���ͬ��ѡ��,���в�ͬ�Ĵ���
				{
					case -1://����
					{
						Remote_DownloadFile(m_TM_GetOperationObject_strRemoteHostName,
							m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							0,//��ͷ��ʼ����
							m_TM_GetOperationObject_nIndex+1,
							m_TM_GetOperationObject_StringArray.GetSize(),
							m_TM_GetOperationObject_bIsCut,0);
						break;
					}
					case -2://�ϵ�����
					{
						Remote_DownloadFile(m_TM_GetOperationObject_strRemoteHostName,
							m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							status.m_size,//�ϵ�����
							m_TM_GetOperationObject_nIndex+1,
							m_TM_GetOperationObject_StringArray.GetSize(),
							m_TM_GetOperationObject_bIsCut,0);
						break;
					}
					case IDCANCEL://ȡ��
					{
						m_TM_DownloadFile_nControlCode=0;//�������ļ�,�����ǲ鿴�򱾵�����
						OnTM_DownloadFile(1);//��OnTM_DownloadFile���ʱ�Ĵ�����ͬ
						break;
					}
				}
			}
			else//���ϴ��ļ�
			{
				if(!pRecvGetFileInfoInfo->bIsFileExist)//�ļ�������
				{
					SetBusyCursor(FALSE);
					SetIndicatorStatus("�õ��ļ���Ϣ�ɹ�");
					m_Progress.SetPos(0);

					Remote_UploadFile(m_TM_GetOperationObject_strRemoteHostName,
						m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
						m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
						0,//��ͷ��ʼ�ϴ�
						m_TM_GetOperationObject_nIndex+1,
						m_TM_GetOperationObject_StringArray.GetSize(),
						m_TM_GetOperationObject_bIsCut);
					return;
				}

				CString		strFilenameText;	//�ļ�����ʾ����
				HICON		hSourceIcon;		//Դ�ļ�ͼ��
				CString		strSourceFileInfo;	//Դ�ļ��ļ���Ϣ
				HICON		hDestIcon;			//Ŀ���ļ�ͼ��
				CString		strDestFileInfo;	//Ŀ���ļ��ļ���Ϣ
				BOOL		bDoNotContinue;		//�Ƿ�����ϵ�����

				CString		strFilename;		//�ļ���
				CString		strSourceFilename;	//Դ�ļ�ȫ·��
				CString		strDestFilename;	//Ŀ���ļ�ȫ·��

				strSourceFilename=m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex];
				strDestFilename=m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex];
				strFilename=strSourceFilename.Right(strSourceFilename.GetLength()-strSourceFilename.ReverseFind('\\')-1);

				strFilenameText.Format("���ļ����Ѱ���һ����Ϊ��%s�����ļ���",strFilename);

				SHFILEINFO shFileInfo;
				SHGetFileInfo(strSourceFilename,
					0,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_ICON|SHGFI_LARGEICON);
				hSourceIcon=shFileInfo.hIcon;
		
				CFileStatus status;
				CFile::GetStatus(strSourceFilename,status);
				strSourceFileInfo=ConverDWORDToCString(status.m_size)+" �ֽ�";
				strSourceFileInfo+="\r\n�޸�ʱ��: ";
				strSourceFileInfo+=status.m_mtime.Format("%Y��%m��%d�� %H:%M:%S");

				SHFILEINFO shFileInfo1;
				SHGetFileInfo(strFilename,
					FILE_ATTRIBUTE_NORMAL,&shFileInfo1,sizeof(SHFILEINFO),
					SHGFI_USEFILEATTRIBUTES|SHGFI_ICON|SHGFI_LARGEICON);
				hDestIcon=shFileInfo1.hIcon;

				strDestFileInfo=ConverDWORDToCString(pRecvGetFileInfoInfo->m_size)+" �ֽ�";
				strDestFileInfo+="\r\n�޸�ʱ��: ";
				strDestFileInfo+=pRecvGetFileInfoInfo->m_mtime.Format("%Y��%m��%d�� %H:%M:%S");

				if(status.m_size>pRecvGetFileInfoInfo->m_size)//�Ƿ�����ϵ�����
				{
					bDoNotContinue=FALSE;
				}
				else
				{
					bDoNotContinue=TRUE;
				}

				SetBusyCursor(FALSE);
				SetIndicatorStatus("�õ��ļ���Ϣ�ɹ�");
				m_Progress.SetPos(0);
				/////////////////////////////////////////////
				//��ʾѡ���ļ������Ի���
				int nRet=ShowSelectFileOperationDialog(strFilenameText,
					hSourceIcon,strSourceFileInfo,hDestIcon,strDestFileInfo,bDoNotContinue);

				DestroyIcon(hSourceIcon);
				DestroyIcon(hDestIcon);

				switch(nRet)//�����û���ͬ��ѡ��,���в�ͬ�Ĵ���
				{
					case -1://����
					{
						Remote_UploadFile(m_TM_GetOperationObject_strRemoteHostName,
							m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							0,//��ͷ��ʼ�ϴ�
							m_TM_GetOperationObject_nIndex+1,
							m_TM_GetOperationObject_StringArray.GetSize(),
							m_TM_GetOperationObject_bIsCut);
						break;
					}
					case -2://�ϵ�����
					{
						Remote_UploadFile(m_TM_GetOperationObject_strRemoteHostName,
							m_TM_GetOperationObject_strPath+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							m_TM_GetOperationObject_strPath2+m_TM_GetOperationObject_StringArray[m_TM_GetOperationObject_nIndex],
							pRecvGetFileInfoInfo->m_size,//�ϵ�����
							m_TM_GetOperationObject_nIndex+1,
							m_TM_GetOperationObject_StringArray.GetSize(),
							m_TM_GetOperationObject_bIsCut);
						break;
					}
					case IDCANCEL://ȡ��
					{
						OnTM_UploadFile(1);//��OnTM_UploadFile���ʱ�Ĵ�����ͬ
						break;
					}
				}
			}
			return;
		}
		case 2://��ֹͣ
		{
			SetBusyCursor(FALSE);
			SetIndicatorStatus("��ǰ������ֹͣ");
			break;
		}
	}
	m_Progress.SetPos(0);
}

//private///////////////////////////////////////////////////////////////
//��ʾѡ���ļ������Ի���
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

//����ָ���ļ��Ƿ����
BOOL CNethiefDoc::IsFileExist(CString strFilename)
{
	CFileFind f;
	return f.FindFile(strFilename);
}

////////////////////////////////////////////////////////////////////////
