// LinkStatic.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "Nethief.h"
#include "LinkStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinkStatic

CLinkStatic::CLinkStatic()
{
}

CLinkStatic::~CLinkStatic()
{
}


BEGIN_MESSAGE_MAP(CLinkStatic, CStatic)
	//{{AFX_MSG_MAP(CLinkStatic)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinkStatic message handlers

void CLinkStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nCtrlID=GetDlgCtrlID(); // 产生消息的ID
	switch(nCtrlID)
	{
		case IDC_STATIC_EMAIL:  //发送E-mail
		{
			CNethiefApp* app=(CNethiefApp*)AfxGetApp();
			ShellExecute(NULL,NULL,
				"mailto:webmaster@greenstuffsoft.net?subject=关于 "+app->m_strVersion
				,NULL,NULL,SW_SHOWNORMAL);
			break;
		}
		case IDC_STATIC_HOMEPAGE:  //去我的主页
		{
			ShellExecute(NULL,NULL,"http://www.greenstuffsoft.net/"
				,NULL,NULL,SW_SHOWNORMAL);	
			break;
		}
		case IDC_STATIC_CHATROOM:  //去聊天室
		{
			ShellExecute(NULL,NULL,"http://chat.netsh.com/fcgi-bin/listmy.fcgi?port=9426"
				,NULL,NULL,SW_SHOWNORMAL);
			break;
		}
	}
	
	CStatic::OnLButtonDown(nFlags, point);
}
