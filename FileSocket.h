//Download by http://www.NewXing.com
// FileSocket.h: interface for the CFileSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESOCKET_H__45505701_10D1_11D5_89D9_409906C10000__INCLUDED_)
#define AFX_FILESOCKET_H__45505701_10D1_11D5_89D9_409906C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

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
	BOOL			m_bStop;				//停止当前操作

	CString			m_strAddress;			//对方的IP地址及端口
	CString			m_strClientName;		//正在进行连接的客户端主机名
	BOOL			m_bIsHttpTunnel;		//正在进行连接的客户端是否是HTTP隧道主机

	CByteArray		m_ArrayBuffer;
	CProgressCtrl*	m_pProgress;			//状态栏进度条指针

	SOCKET			m_hListenSocket;		//监听Socket,TCP 60000
	
	//当前连接的服务端信息///////////
	SOCKET			m_hConnectSocket;	//当前连接Socket
	CString			m_strHostName;		//当前连接服务端的主机名
	int				m_nHostAttribute;	//当前连接服务端的主机属性
	/////////////////////////////////

	////////////////////////成员函数/////////////////////////////
	void	Initial(CProgressCtrl* pProgress);
	void	Close();
	//////////////////////////////////////
	void	NotifyClientConnect(CString strClientName,BOOL bIsHttpTunnel);
	BOOL	BeginListen();
	void	EndListen();
	void	DisconnectClient();

	void	Command_IDVerify();
	void	Command_DownloadDiskDriveInfo();
	void	Command_ExpandDirectory(CString strHostName,CString strPath);
	void	Command_ShowFileList(CString strHostName,CString strPath);
	void	Command_RenameFile(CString strHostName,CString strPath,CString strFilename1,CString strFilename2);
	void	Command_NewFile(CString strHostName,CString strFullFilename);
	void	Command_NewFolder(CString strHostName,CString strFullFilename);
	void	Command_DeleteFile(CString strHostName,CString strFrom);
	void	Command_RemotePaste(CString strHostName,CString strFrom,CString strTo,DWORD dwFOFlags);
	void	Command_GetOperationObject(CString strHostName,CString strFrom);
	void	Command_DownloadFile(CString strHostName,CString strFilename1,CString strFilename2,DWORD dwStartPosition,BOOL bIsCut);
	void	Command_UploadFile(CString strHostName,CString strFilename1,CString strFilename2,DWORD dwStartPosition,BOOL bIsCut);
	void	Command_ShowProperty_Multi(CString strHostName,CString strPath);
	void	Command_ShowProperty_Single(CString strHostName,CString strFilename);
	void	Command_ModifyProperty_Single(CString strHostName,CString strFilename,DWORD dwFileAttributes);
	void	Command_ShowProperty_Diskdrive(CString strHostName,CString strDiskDrive);
	void	Command_ModifyProperty_Diskdrive(CString strHostName,CString strDiskDrive,CString strVolumeName);
	void	Command_RemoteRun(CString strHostName,CString strFilename,CString strParam,int nShowCmd);
	void	Command_RemoteUninstall(CString strHostName);
	void	Command_FindFile(CString strHostName,CString strFindDir,CString strFindFile,BOOL bSearchSubdir);
	void	Command_ShowNote(CString strHostName);
	void	Command_GetFileInfo(CString strHostName,CString strFilename);
	///////////////////////////////////////
	void	OnSocketAccept();
	BOOL	IsClientConnected(CString strHostName);
	//*****************************************************************
private:
	//*****************************************************************
	////////////////////////成员变量/////////////////////////////

	////////////////////////成员函数/////////////////////////////
	void	CStringToChar(char* cText,CString strText);
	CString	CharToCString(char* cText,UINT nSize);
	void	CreateMultilevelDir(CString strDirName);

	int		SendCommand(SOCKET hSocket,int nControlCode,DWORD dwCommand,BYTE* byBuf=NULL,DWORD dwBufSize=NULL,BOOL bProgress=TRUE);
	DWORD	RecvCommandDataSize(SOCKET hSocket);
	int		RecvCommandData(SOCKET hSocket,BYTE* byBuf,DWORD dwBufSize,BOOL bProgress=TRUE);

	BOOL	HttpTunnelSendHeader(DWORD dwDataSize);//发送HTTP头
	BOOL	HttpTunnelRecvHeader();//接收HTTP头
	//*****************************************************************
};

#endif // !defined(AFX_FILESOCKET_H__45505701_10D1_11D5_89D9_409906C10000__INCLUDED_)
