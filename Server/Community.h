//Download by http://www.NewXing.com
// Community.h: interface for the CCommunity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMUNITY_H__06D61C8F_E2AA_11D4_89D9_505604C10000__INCLUDED_)
#define AFX_COMMUNITY_H__06D61C8F_E2AA_11D4_89D9_505604C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//*************************************
#pragma pack(1) //�ֽڶ���

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

	//********************************************
	CString		m_strHostName;			//������������
	CTime		m_tmOnlineTime;			//����������ʱ��
	BOOL		m_bIsProxy;				//�������ô�������
	BYTE		m_byCheckSum_NotifyPack[16];//���������֪ͨ����MD5У���

	CString		m_strConnectInfoURL;
	CString		m_strNotifyInfoURL;
	CString		m_strDownloadInfoURL;
	CString		m_strKeyName;
	CString		m_strFilename;
	CString		m_strNote;
	BYTE		m_byCheckSum[16];
	CString		m_strDataPassword;		//���ݼ�����Կ

	SOCKET		m_hNotifySocket;		//UDP����֪ͨ��Socket
	//********************************************
	BOOL		SendNotify(BOOL bFirst);
	BOOL		IsOnline();
	CString		IsConnectServer();
	void		DownloadFile();
	//********************************************
private:
	char		m_cHostname[128];
	//********************************************
	void		CStringToChar(char* cText,CString strText);
	CString		CharToCString(char* cText,UINT nSize);
	CString		RegReadValue(HKEY hKey,CString sKey,CString sChildKey);
	BOOL		RegReadValue(/*IN */HKEY hKey,/*IN */CString strKey,/*IN */CString strChildKey,/*IN */DWORD dwType,/*OUT*/LPBYTE lpData,/*IN/OUT*/LPDWORD lpcbData);
	void		EncryptData(LPVOID pVoid,DWORD dwBufLen,CString strKey);
	void		prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key);
	void		rc4(unsigned char *buffer_ptr, DWORD buffer_len, rc4_key *key);
	//********************************************
};

#endif // !defined(AFX_COMMUNITY_H__06D61C8F_E2AA_11D4_89D9_505604C10000__INCLUDED_)
