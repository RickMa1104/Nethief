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
#include "IPToLocal.h"      //�����ҶԷ���ַ����
#include <afxmt.h>
//*************************************
#pragma pack(1) //�ֽڶ���

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

#pragma pack()	//ʹ��ȱʡ����

#define swap_byte(x,y) t = *(x); *(x) = *(y); *(y) = t
//*************************************
class CCommunity  
{
public:
	CCommunity();
	virtual ~CCommunity();

public:
	//*****************************************************************
	////////////////////////��Ա����/////////////////////////////
	BOOL				m_bIsRegistered;	//�ͻ����Ƿ��Ѿ�ע��

	CString				m_strFtpServerAddress;	//FTP��������������IP��ַ
	CString				m_strFtpUsername;		//��ҳ�ռ���û���
	CString				m_strFtpPassword;		//��ҳ�ռ������
	CString				m_strHomepage;			//��ҳ�ռ����ַ
	CString				m_strFileSavePath;		//FTP�������ϴ�������ļ���Ŀ¼
	CString				m_strNotifyFilename;	//����ʵ�ַ��������֪ͨ����ҳ�ռ���ʱ�ļ�
	CString				m_strConnectFilename;	//���������˽������ӵ���ҳ�ռ���ʱ�ļ�
	CString				m_strClientFilename;	//���ڱ���FTP��¼״̬����ҳ�ռ���ʱ�ļ�
	CString				m_strDownloadFilename;	//���ڷ�����Զ����ز����г���
	CString				m_strDataPassword;		//���ݼ�����Կ

	BYTE				m_byCheckSum_NotifyPack[16];//���������֪ͨ����MD5У���

	int					m_nSubItem;			//�б����з�ʽ
	BOOL				m_bAscending;		//����˳��

	CString				m_strHostName;	//�ͻ��˵��û���
	CString				m_strIPAddr;	//�ͻ��˵�IP��ַ
	CString				m_strPosition;	//�ͻ��˵ĵ���λ��
	int					m_nOnlineNum;	//������

	CInternetSession	m_Session;		//Internet�Ự
	CFtpConnection*		m_pFtpConn;		//FTP����

	SOCKET				m_hNotifySocket;	//UDP����֪ͨ,Listening UDP 60000
	SOCKET				m_hListenSocket;	//TCP����֪ͨ,Listening TCP 60001

	CCriticalSection	m_CriticalSection;//�ٽ��
	CCriticalSection	m_CriticalSection_List;//�ٽ��
	////////////////////////��Ա����/////////////////////////////
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
	////////////////////////��Ա����/////////////////////////////
	CIPToLocal	m_IPToLocal;  //���û���ַ����
	char		m_cHostname[128];
	////////////////////////��Ա����/////////////////////////////
	void		CStringToChar(char* cText,CString strText);
	CString		CharToCString(char* cText,UINT nSize);
	CString		RegReadValue(HKEY hKey,CString sKey,CString sChildKey);
	void		prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key);
	void		rc4(unsigned char *buffer_ptr, DWORD buffer_len, rc4_key *key);
	void		EncryptData(LPVOID pVoid,DWORD dwBufLen,CString strKey);
	CString		GetCurDir();
	int			GetINIfileInt(LPCTSTR lpSectionName,  //��INI�ж�ȡ����ֵ
					LPCTSTR lpKeyName);
	BOOL		WriteINIfileInt(LPCTSTR lpSectionName, //��INI��д������ֵ
					LPCTSTR lpKeyName,INT nValue);
	CString		GetINIfileString(LPCTSTR lpSectionName, //��INI�ж�ȡ�ַ�ֵ
					LPCTSTR lpKeyName);
	BOOL		WriteINIfileString(LPCTSTR lpSectionName,  //��INI��д���ַ�ֵ
					LPCTSTR lpKeyName,LPCTSTR lpString);
	static int	CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort);
	void		ShowSelectIPDialog();
	void		ShowEditText(CEdit* pEdit_Test,CString strText);
	//*****************************************************************
};

#endif // !defined(AFX_COMMUNITY_H__C13CA113_E018_11D4_89D9_F84105C10000__INCLUDED_)
