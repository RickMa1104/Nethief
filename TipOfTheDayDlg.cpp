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
	CString exeFullPath;      //���ڱ��浱ǰִ�г���·��
	GetModuleFileName(NULL,   //�õ�·��
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('\\')+1);  //�ҵ�
	exeFullPath.ReleaseBuffer();
	CString strLogFileName=exeFullPath+"Readme.txt";  //Դ�ļ���

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
		"mailto:webmaster@greenstuffsoft.net?subject=���� "+app->m_strVersion
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
		MessageBox("��ȡ�����ļ����������°�װ�������");
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

//////////////////INI�ļ���������/////////////////
int CTipOfTheDayDlg::GetINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
{
	CString exeFullPath;      //���ڱ��浱ǰִ�г���·��
	GetModuleFileName(NULL,   //�õ�·��
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //�ҵ�
	exeFullPath=exeFullPath+".INI";  //Դ�ļ���
	return GetPrivateProfileInt(lpSectionName,lpKeyName,-1,
		exeFullPath.GetBufferSetLength(MAX_PATH));
}

BOOL CTipOfTheDayDlg::WriteINIfileInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName, INT nValue)
{
	CString exeFullPath;      //���ڱ��浱ǰִ�г���·��
	GetModuleFileName(NULL,   //�õ�·��
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //�ҵ�
	exeFullPath=exeFullPath+".INI";  //Դ�ļ���
	CString strValue;               
	strValue.Format("%d",nValue);   //������ת�����ַ���
	return WritePrivateProfileString(lpSectionName,lpKeyName,
		strValue.GetBufferSetLength(strValue.GetLength()),
		exeFullPath.GetBufferSetLength(MAX_PATH));
}

CString CTipOfTheDayDlg::GetINIfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName)
{
	CString exeFullPath;      //���ڱ��浱ǰִ�г���·��
	GetModuleFileName(NULL,	 //�õ�·��
		exeFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
	exeFullPath.GetBufferSetLength(exeFullPath.ReverseFind('.'));  //�ҵ�
	exeFullPath=exeFullPath+".INI";  //Դ�ļ���
	int nLength=400;
	///////////////////////////////////////////////////////////
	CString strString;           //���ڱ���������ַ���
	GetPrivateProfileString(lpSectionName,lpKeyName,"",
		strString.GetBufferSetLength(nLength),nLength,
		exeFullPath.GetBufferSetLength(MAX_PATH));
	strString.ReleaseBuffer();
	return strString;
}
//////////////////////////////////////////////////////////////
