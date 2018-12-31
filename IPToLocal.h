//Download by http://www.NewXing.com
// IPToLocal.h: interface for the CIPToLocal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPTOLOCAL_H__74324FAA_6B2C_40E2_AECE_8CD9A971AEA3__INCLUDED_)
#define AFX_IPTOLOCAL_H__74324FAA_6B2C_40E2_AECE_8CD9A971AEA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
/////////////////////////////////////////
#pragma pack(1)

typedef struct
{
	char cStartIP[17];
	char cEndIP[17];
	char cLocal[126];
}
IP_ADDRESS_SEGMENT_OF_FILE,*pIP_ADDRESS_SEGMENT_OF_FILE;

typedef struct
{
	DWORD dwStartIP;
	DWORD dwEndIP;
	CString strLocal;
}
IP_ADDRESS_SEGMENT_OF_LIST,*pIP_ADDRESS_SEGMENT_OF_LIST;

#pragma pack()
/////////////////////////////////////////

class CIPToLocal  
{
public:
	CIPToLocal();
	virtual ~CIPToLocal();

	void LoadIPAddress(CString strFilename);
	CString GetIPAddressLocal(CString strIPAddress);

private:
	CList<IP_ADDRESS_SEGMENT_OF_LIST,IP_ADDRESS_SEGMENT_OF_LIST&> m_IPAddressList;
};

#endif // !defined(AFX_IPTOLOCAL_H__74324FAA_6B2C_40E2_AECE_8CD9A971AEA3__INCLUDED_)
