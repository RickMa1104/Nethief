//Download by http://www.NewXing.com
// TipOfTheDayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "TipOfTheDayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTipOfTheDayDlg dialog


CTipOfTheDayDlg::CTipOfTheDayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTipOfTheDayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTipOfTheDayDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTipOfTheDayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTipOfTheDayDlg)
	DDX_Control(pDX, IDC_STATIC_LIGHT, m_Static_Light);
	DDX_Control(pDX, IDC_STATIC_TIPS, m_Static_Tips);
	DDX_Control(pDX, IDC_STATIC_DOYOUKNOW, m_Static_Doyouknow);
	DDX_Control(pDX, IDC_STATIC_BACKGROUND, m_Static_Background);
	DDX_Control(pDX, IDC_CHECK1, m_Check);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTipOfTheDayDlg, CDialog)
	//{{AFX_MSG_MAP(CTipOfTheDayDlg)
	ON_BN_CLICKED(IDC_BUTTON_README, OnButtonReadme)
	ON_BN_CLICKED(IDC_BUTTON_HOMEPAGE, OnButtonHomepage)
	ON_BN_CLICKED(IDC_BUTTON_EMAIL, OnButtonEmail)
	ON_BN_CLICKED(IDC_BUTTON_TIPS, OnButtonTips)
	ON_BN_CLICKED(ID_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTipOfTheDayDlg message handlers
BOOL CTipOfTheDayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int bStartup=GetINIfileInt("Tip of the day","Startup");
	m_Check.SetCheck(bStartup);

	ShowTips();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTipOfTheDayDlg::OnButtonReadme() 
{
	// TODO: Add your control notification handler code here
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('\\')+1);  //找到
	exeFullPath.ReleaseBuffer();
	CString strLogFileName=exeFullPath+"Readme.txt";  //源文件名

	ShellExecute(NULL,NULL,strLogFileName,NULL,exeFullPath,SW_SHOWMAXIMIZED);
}

void CTipOfTheDayDlg::OnButtonHomepage() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,NULL,"http://www.greenstuffsoft.net/"
		,NULL,NULL,SW_SHOWNORMAL);
}

void CTipOfTheDayDlg::OnButtonEmail() 
{
	// TODO: Add your control notification handler code here
	CNethiefApp* app=(CNethiefApp*)AfxGetApp();
	ShellExecute(NULL,NULL,
		"mailto:webmaster@greenstuffsoft.net?subject=关于 "+app->m_strVersion
		,NULL,NULL,SW_SHOWNORMAL);
}

void CTipOfTheDayDlg::OnButtonTips() 
{
	// TODO: Add your control notification handler code here
	ShowTips();
}

void CTipOfTheDayDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	int bStartup=m_Check.GetCheck();
	WriteINIfileInt("Tip of the day","Startup",bStartup);

	CDialog::OnOK();
}

void CTipOfTheDayDlg::ShowTips()
{
	int nCount=GetINIfileInt("Tip of the day","Count");
	if(nCount==-1)
		MessageBox("读取配置文件错误，请重新安装本软件。");
	CString strCount;
	strCount.Format("Tip%d",nCount);
	CString strTip;
	strTip=GetINIfileString("Tip of the day",strCount);
	if(strTip=="")
	{
		nCount=0;
		strCount.Format("Tip%d",nCount);
		strTip=GetINIfileString("Tip of the day",strCount);
	}
	nCount++;
	WriteINIfileInt("Tip of the day","Count",nCount);

	m_Static_Tips.SetWindowText(strTip);
}

//////////////////INI文件操作函数/////////////////
int CTipOfTheDayDlg::GetINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,   //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //找到
	exeFullPath=exeFullPath+".INI";  //源文件名
	return GetPrivateProfileInt(lpSectionName,lpKeyName,-1,
		exeFullPath.GetBufferSetLength(MAX_PATH));
}

BOOL CTipOfTheDayDlg::WriteINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName, INT nValue)
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

CString CTipOfTheDayDlg::GetINIfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
{
	CString exeFullPath;      //用于保存当前执行程序路径
	GetModuleFileName(NULL,	 //得到路径
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //找到
	exeFullPath=exeFullPath+".INI";  //源文件名
	int nLength=400;
	///////////////////////////////////////////////////////////
	CString strString;           //用于保存读出的字符串
	GetPrivateProfileString(lpSectionName,lpKeyName,"",
		strString.GetBufferSetLength(nLength),nLength,
		exeFullPath.GetBufferSetLength(MAX_PATH));
	strString.ReleaseBuffer();
	return strString;
}
//////////////////////////////////////////////////////////////
