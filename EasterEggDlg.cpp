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
��    ���� �߲�(greenstuff)\r\n\
��ʵ������ ����\r\n\
��    �� ��\r\n\
�������£� 81��2��\r\n\
��    �ڣ� ����\r\n\
Ѫ    �ͣ� O\r\n\
��    �ã� ����򣬿�����\r\n\
��    �룺 ������ı����磡\r\n\
\r\n\
���˼�飺\r\n\
������һ�� Windows �µ� C++ ����Ա��\
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
