//Download by http://www.NewXing.com
// EasterEggDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Nethief.h"
#include "EasterEggDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEasterEggDlg dialog


CEasterEggDlg::CEasterEggDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEasterEggDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEasterEggDlg)
	m_strStatic = _T("");
	//}}AFX_DATA_INIT
	m_strStatic="\
网    名： 蔬菜(greenstuff)\r\n\
真实姓名： 张磊\r\n\
性    别： 男\r\n\
出生年月： 81年2月\r\n\
户    口： 北京\r\n\
血    型： O\r\n\
爱    好： 编程序，看电视\r\n\
理    想： 用软件改变世界！\r\n\
\r\n\
个人简介：\r\n\
本人是一名 Windows 下的 C++ 程序员。\
";
}


void CEasterEggDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEasterEggDlg)
	DDX_Control(pDX, IDC_STATIC1, m_Static);
	DDX_Text(pDX, IDC_STATIC1, m_strStatic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEasterEggDlg, CDialog)
	//{{AFX_MSG_MAP(CEasterEggDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasterEggDlg message handlers
