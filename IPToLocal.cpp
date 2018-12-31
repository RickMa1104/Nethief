//Download by http://www.NewXing.com
// IPToLocal.cpp: implementation of the CIPToLocal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Nethief.h"
#include "IPToLocal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIPToLocal::CIPToLocal()
{

}

CIPToLocal::~CIPToLocal()
{

}

void CIPToLocal::LoadIPAddress(CString strFilename)
{
	CFile File(strFilename,
		CFile::modeRead|CFile::shareDenyNone);
	File.Seek(194,CFile::begin);

	IP_ADDRESS_SEGMENT_OF_FILE stIPAddressSegmentOfFile;
	IP_ADDRESS_SEGMENT_OF_LIST stIPAddressSegmentOfList;
	while(TRUE)
	{
		UINT nRet=File.Read(&stIPAddressSegmentOfFile,sizeof(stIPAddressSegmentOfFile));
		if(nRet!=sizeof(stIPAddressSegmentOfFile))
			break;
		//////////////////////////////////////////////////////////////
		stIPAddressSegmentOfFile.cStartIP[15]='\0';
		CString strStartIP=stIPAddressSegmentOfFile.cStartIP;

		stIPAddressSegmentOfList.dwStartIP=atoi(strStartIP.Mid(0,3))*256*256*256+atoi(strStartIP.Mid(4,3))*256*256+atoi(strStartIP.Mid(8,3))*256+atoi(strStartIP.Mid(12,3));
		//////////////////////////////////////////
		stIPAddressSegmentOfFile.cEndIP[15]='\0';
		CString strEndIP=stIPAddressSegmentOfFile.cEndIP;

		stIPAddressSegmentOfList.dwEndIP=atoi(strEndIP.Mid(0,3))*256*256*256+atoi(strEndIP.Mid(4,3))*256*256+atoi(strEndIP.Mid(8,3))*256+atoi(strEndIP.Mid(12,3));
		//////////////////////////////////////////
		char cLocal[sizeof(stIPAddressSegmentOfFile.cLocal)];
		ZeroMemory(cLocal,sizeof(cLocal));

		int nCount=0;
		for(int i=0;i<sizeof(stIPAddressSegmentOfFile.cLocal);i++)
		{
			if(stIPAddressSegmentOfFile.cLocal[i]!=' ')
			{
				cLocal[nCount]=stIPAddressSegmentOfFile.cLocal[i];
				nCount++;
			}
		}
		stIPAddressSegmentOfList.strLocal=cLocal;
		//////////////////////////////////////////////////////////////
		m_IPAddressList.AddTail(stIPAddressSegmentOfList);
	}
	File.Close();
}

CString CIPToLocal::GetIPAddressLocal(CString strIPAddress)
{
	int nPos1=strIPAddress.Find(".");
	strIPAddress.SetAt(nPos1,'|');
	int nPos2=strIPAddress.Find(".");
	strIPAddress.SetAt(nPos2,'|');
	int nPos3=strIPAddress.Find(".");
	strIPAddress.SetAt(nPos3,'|');

	DWORD dwIPAddress=atoi(strIPAddress.Mid(0,nPos1))*256*256*256+atoi(strIPAddress.Mid(nPos1+1,nPos2-nPos1-1))*256*256+atoi(strIPAddress.Mid(nPos2+1,nPos3-nPos2-1))*256+atoi(strIPAddress.Mid(nPos3+1,strIPAddress.GetLength()-nPos3-1));
	//////////////////////////////////////////
	CString strLocal="Î´ÖªIPµØÖ·";
	pIP_ADDRESS_SEGMENT_OF_LIST pstIPAddressSegmentOfList;
	DWORD dwMixIPAddressRange=0xFFFFFFFF;
	POSITION Pos=m_IPAddressList.GetHeadPosition();
	while(Pos!=NULL)
	{
		pstIPAddressSegmentOfList=&m_IPAddressList.GetNext(Pos);

		if(pstIPAddressSegmentOfList->dwStartIP<=dwIPAddress && pstIPAddressSegmentOfList->dwEndIP>=dwIPAddress)
		{
			DWORD dwIPAddressRange=pstIPAddressSegmentOfList->dwEndIP-pstIPAddressSegmentOfList->dwStartIP;
			if(dwIPAddressRange<=dwMixIPAddressRange)
			{
				dwMixIPAddressRange=dwIPAddressRange;
				strLocal=pstIPAddressSegmentOfList->strLocal;
			}
		}
	}
	return strLocal;
}