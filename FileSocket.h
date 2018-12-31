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
	BOOL			m_bStop;				//ֹͣ��ǰ����

	CString			m_strAddress;			//�Է���IP��ַ���˿�
	CString			m_strClientName;		//���ڽ������ӵĿͻ���������
	BOOL			m_bIsHttpTunnel;		//���ڽ������ӵĿͻ����Ƿ���HTTP�������

	CByteArray		m_ArrayBuffer;
	CProgressCtrl*	m_pProgress;			//״̬��������ָ��

	SOCKET			m_hListenSocket;		//����Socket,TCP 60000
	
	//��ǰ���ӵķ������Ϣ///////////
	SOCKET			m_hConnectSocket;	//��ǰ����Socket
	CString			m_strHostName;		//��ǰ���ӷ���˵�������
	int				m_nHostAttribute;	//��ǰ���ӷ���˵���������
	/////////////////////////////////

	////////////////////////��Ա����/////////////////////////////
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
	////////////////////////��Ա����/////////////////////////////

	////////////////////////��Ա����/////////////////////////////
	void	CStringToChar(char* cText,CString strText);
	CString	CharToCString(char* cText,UINT nSize);
	void	CreateMultilevelDir(CString strDirName);

	int		SendCommand(SOCKET hSocket,int nControlCode,DWORD dwCommand,BYTE* byBuf=NULL,DWORD dwBufSize=NULL,BOOL bProgress=TRUE);
	DWORD	RecvCommandDataSize(SOCKET hSocket);
	int		RecvCommandData(SOCKET hSocket,BYTE* byBuf,DWORD dwBufSize,BOOL bProgress=TRUE);

	BOOL	HttpTunnelSendHeader(DWORD dwDataSize);//����HTTPͷ
	BOOL	HttpTunnelRecvHeader();//����HTTPͷ
	//*****************************************************************
};

#endif // !defined(AFX_FILESOCKET_H__45505701_10D1_11D5_89D9_409906C10000__INCLUDED_)
