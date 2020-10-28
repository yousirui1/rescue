|// UDPTrackerClient.h: interface for the CUDPTrackerClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDPTRACKERCLIENT_H__69B6ACC8_8193_4680_81D8_925B1550E92C__INCLUDED_)
#define AFX_UDPTRACKERCLIENT_H__69B6ACC8_8193_4680_81D8_925B1550E92C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#ifndef _DISABLEWARNING4786_4355
#define _DISABLEWARNING4786_4355
#pragma warning( disable : 4786 )
#pragma warning( disable : 4355 )
#endif
#ifndef _ENABLEUSESTL
#define _ENABLEUSESTL
#include <list>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <string>
using namespace std;
#endif

class CPeerHostInfo
{
public:								
	DWORD		IP;//节点IP
	WORD		Port;//节点端口
};	
class CUDPTrackerClient  
{
public:
	CUDPTrackerClient();
	virtual ~CUDPTrackerClient();
	void CancelSocketOperate();
	BOOL Connect(const char * szServer,WORD wPort = 0);	
	DWORD Announcing(BYTE* pInfoHash,BYTE * pPeerID,
		__int64 idownloaded,__int64 ileft,__int64 iuploaded,
		int		ievent,
		DWORD dwIP,WORD  wPort);
	
	BOOL Disconnect();
public:	
	SOCKET			m_socket;
	DWORD			m_dwIP;
	WORD			m_wPort;	
	__int64			m_iConnection_id;
	DWORD			m_dwConnectTick;
	string			m_strError;		//如果请求失败,此变量保存错误信息
	DWORD m_dwDonePeers;	//种子数
	DWORD m_dwNumPeers;	//当前下载者个数
	DWORD m_dwInterval;	//查询间隔时间
	list<CPeerHostInfo> m_listPeers;
};

#endif // !defined(AFX_UDPTRACKERCLIENT_H__69B6ACC8_8193_4680_81D8_925B1550E92C__INCLUDED_)
// UDPTrackerClient.cpp: implementation of the CUDPTrackerClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UDPTrackerClient.h"

#include "DataStream.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define RECVBUFSIZE 2048

CUDPTrackerClient::CUDPTrackerClient()
{
	m_socket = INVALID_SOCKET;	
	m_iConnection_id = 0;
	m_dwConnectTick = 0;
	m_dwIP = 0;
	m_wPort = 0;
	m_dwDonePeers = 0;	//种子数
	m_dwNumPeers = 0;	//当前下载者个数
	m_dwInterval = 0;	//查询间隔时间
}

CUDPTrackerClient::~CUDPTrackerClient()
{
	Disconnect();

}
void CUDPTrackerClient::CancelSocketOperate()
{
	if(m_socket != INVALID_SOCKET)
	{
		LINGER lingerStruct;    
		// If we're supposed to abort the connection, set the linger value
		// on the socket to 0.    
		lingerStruct.l_onoff = 1;
		lingerStruct.l_linger = 0;
		setsockopt(m_socket, SOL_SOCKET, SO_LINGER,
			(char *)&lingerStruct, sizeof(lingerStruct) );
	}
}
BOOL CUDPTrackerClient::Disconnect()
{
	m_iConnection_id = 0;
	m_dwDonePeers = 0;	//种子数
	m_dwNumPeers = 0;	//当前下载者个数
	m_dwInterval = 0;	//查询间隔时间
	if ( m_socket != INVALID_SOCKET )
	{
		m_dwIP = 0;
		m_wPort = 0;
		// Now close the socket handle.  This will do an abortive or 
		// graceful close, as requested.      
		shutdown(m_socket,SD_BOTH);
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return TRUE;
	}
	return FALSE;
}

//szServer连接的主机,可以是下列形式的字符串:
//easeso.com:1000
//easeso.com
//如果wPort不为0,则szServer不应该包含端口信息
BOOL CUDPTrackerClient::Connect(const char * szServer,WORD wPort)
{
	m_strError = "";
	BOOL bRes = FALSE;
	if ( m_socket == INVALID_SOCKET )
	{
		//用UDP初始化套接字
		BOOL optval = TRUE;			
		m_socket =socket(AF_INET,SOCK_DGRAM,0);
		if(m_socket == INVALID_SOCKET)
			return FALSE;
		//设置超时时间
		int TimeOut=10000;
		int err = setsockopt (m_socket, SOL_SOCKET,SO_RCVTIMEO,(CHAR *) &TimeOut,sizeof (TimeOut));
	}
	if(m_dwIP == 0)
	{
		CString strServer = szServer;
		CString strHost;
		if(wPort == 0)
		{
			int iNext = strServer.Find(':');
			if(iNext>0)
			{			
				strHost = strServer.Mid(0,iNext);			
				CString strPort = strServer.Mid(iNext+1);			
				m_wPort = (WORD)atoi(strPort);
			}
			else
				strHost = strServer;
		}
		else
		{
			strHost = strServer;
			m_wPort = wPort;
		}
		if(m_wPort == 0)
			m_wPort = 80;		

		//Check if address is an IP or a Domain Name
		int a = strHost[0];
		if (a > 47 && a < 58) 
			m_dwIP = inet_addr(strHost);
		else
		{
			struct hostent *pHost;
			pHost = gethostbyname(strHost);
			if(pHost != NULL)
				m_dwIP = *((ULONG*)pHost->h_addr);
			else
				m_dwIP = 0;
		}
	}
	if((GetTickCount()-m_dwConnectTick)>30000)
	{
		m_dwConnectTick = 0;
		m_iConnection_id = 0;
	}
	if(m_socket != INVALID_SOCKET && m_dwIP && m_wPort && m_iConnection_id ==0)
	{
		DWORD dwTransaction_id = GetTickCount();
		SOCKADDR_IN		from;
		int fromlength=sizeof(SOCKADDR);

		char buf[RECVBUFSIZE];
		from.sin_family=AF_INET;
		from.sin_addr.s_addr=m_dwIP;
		from.sin_port=htons(m_wPort);
		CDataStream sendstream(buf,2047);
		sendstream.clear();
		__int64 iCID = 0x41727101980;
		sendstream.writeint64(CNetworkByteOrder::convert(iCID));
		sendstream.writedword(CNetworkByteOrder::convert((int)0));
		sendstream.writedword(dwTransaction_id);
		
		int iRes = 0;
		int iTimes = 6;
		while(iTimes>0&&m_dwIP)
		{
			sendto(m_socket,sendstream.getbuffer(),sendstream.size(),0,(struct sockaddr FAR *)&from,sizeof(from));
			iRes = recvfrom(m_socket,buf,RECVBUFSIZE-1,0,(struct sockaddr FAR *)&from,(int FAR *)&fromlength);
			if(iRes >=0)
				break;
			iTimes--;
		}
		if(iRes>=16)
		{
			CDataStream recvstream(buf,RECVBUFSIZE-1);
			DWORD dwAction = (DWORD)CNetworkByteOrder::convert((int)recvstream.readdword());
			DWORD dwTIDResp= recvstream.readdword();
			if(dwTIDResp == dwTransaction_id)
			{
				if(dwAction == 0)
				{
					m_iConnection_id = recvstream.readint64();			
					//BitComet将回复0x16字节数据,最后6字节是服务器查看到的本地IP和UDP端口
				}
				else if(dwAction == 3)//得到一个错误信息包
				{
					buf[iRes]=0;
					m_strError = recvstream.readstring();
				}
			}
		}
	}
	if(m_iConnection_id)
		bRes = TRUE;
	return bRes;
}
//提交请求
//pInfoHash 20字节的数据缓冲区指针
//pPeerID	20字节的数据缓冲区指针
//ievent参数值:
//none = 0 
//completed = 1 
//started = 2 
//stopped = 3 

DWORD CUDPTrackerClient::Announcing(BYTE* pInfoHash,BYTE * pPeerID,
									__int64 idownloaded,__int64 ileft,__int64 iuploaded,
									int		ievent,
									DWORD dwIP,WORD  wPort)
{	
	m_listPeers.clear();
	m_dwNumPeers = 0;
	m_dwDonePeers = 0;
	m_strError = "";
	DWORD dwReturnCode = 0;
	if(m_iConnection_id && m_socket != INVALID_SOCKET && m_dwIP & m_wPort)
	{
		DWORD dwTransaction_id = GetTickCount();
		//srand(dwTransaction_id);
		//DWORD dwKey = rand();
		DWORD dwKey = 0x3753;
		SOCKADDR_IN		from;
		int fromlength=sizeof(SOCKADDR);
		char buf[RECVBUFSIZE];
		from.sin_family=AF_INET;
		from.sin_addr.s_addr=m_dwIP;
		from.sin_port=htons(m_wPort);
		CDataStream sendstream(buf,RECVBUFSIZE-1);
		sendstream.clear();		
		sendstream.writeint64(m_iConnection_id);
		sendstream.writedword(CNetworkByteOrder::convert((int)1));
		sendstream.writedword(dwTransaction_id);
		sendstream.writedata(pInfoHash,20);
		sendstream.writedata(pPeerID,20);
		sendstream.writeint64(CNetworkByteOrder::convert(idownloaded));
		sendstream.writeint64(CNetworkByteOrder::convert(ileft));
		sendstream.writeint64(CNetworkByteOrder::convert(iuploaded));
		sendstream.writedword(CNetworkByteOrder::convert(ievent));
		sendstream.writedword(dwIP);
		sendstream.writedword(CNetworkByteOrder::convert((int)dwKey));
		sendstream.writedword(CNetworkByteOrder::convert((int)200));
		sendstream.writedword(CNetworkByteOrder::convert(wPort));
		
		int iRes = 0;
		int iTimes = 2;
		while(iTimes>0&&m_dwIP)
		{
			sendto(m_socket,sendstream.getbuffer(),sendstream.size(),0,(struct sockaddr FAR *)&from,sizeof(from));
			iRes = recvfrom(m_socket,buf,RECVBUFSIZE-1,0,(struct sockaddr FAR *)&from,(int FAR *)&fromlength);
			if(iRes >=0)
				break;
			iTimes--;
		}
		if(iRes>=20)
		{			
			CDataStream recvstream(buf,RECVBUFSIZE-1);
			DWORD dwAction = (DWORD)CNetworkByteOrder::convert((int)recvstream.readdword());
			DWORD dwTIDResp= recvstream.readdword();
			if(dwTIDResp == dwTransaction_id)
			{
				if(dwAction == 1)
				{
					m_dwInterval = (DWORD)CNetworkByteOrder::convert((int)recvstream.readdword());
					m_dwNumPeers = (DWORD)CNetworkByteOrder::convert((int)recvstream.readdword());
					m_dwDonePeers = (DWORD)CNetworkByteOrder::convert((int)recvstream.readdword());					
					CPeerHostInfo hi;					
					for(int iCurPos = 20;iCurPos+6<=iRes;iCurPos+=6)
					{
						hi.IP= recvstream.readdword();
						hi.Port = (WORD)CNetworkByteOrder::convert((unsigned short)recvstream.readword());
						m_listPeers.push_back(hi);
					}
					if(m_dwNumPeers>m_listPeers.size())
					{
						iRes = 0;
						iTimes = 6;
						while(iTimes>0&&m_dwIP)
						{
							iRes = recvfrom(m_socket,buf,RECVBUFSIZE-1,0,(struct sockaddr FAR *)&from,(int FAR *)&fromlength);
							if(iRes >=0)
								break;
							iTimes--;
						}
						if(iRes>=6)
						{
							for(iCurPos = 0;iCurPos+6<=iRes;iCurPos+=6)
							{
								hi.IP= recvstream.readdword();
								hi.Port = (DWORD)CNetworkByteOrder::convert((int)recvstream.readword());
								m_listPeers.push_back(hi);
							}
						}						
					}
					m_dwNumPeers = m_listPeers.size();
					dwReturnCode = 200;
				}
				else if(dwAction == 3)//得到一个错误信息包
				{
					buf[iRes]=0;
					m_strError = recvstream.readstring();
					dwReturnCode = 400;
				}
			}
		}
	}
	//每次都要求重新连接
	m_iConnection_id = 0;
	return dwReturnCode;
}

// DataStream.h: interface for the CDataStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASTREAM_H__D90A2534_EA73_4BEA_8B7E_87E59A3D1D26__INCLUDED_)
#define AFX_DATASTREAM_H__D90A2534_EA73_4BEA_8B7E_87E59A3D1D26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdio.h> 
//数据流操作函数
class CDataStream  
{
public :
	CDataStream(char * szBuf,int isize)
	{
		m_isize = isize;
		buffer = szBuf;
		current = buffer;
	}
	~CDataStream()
	{
	}
    void clear()
	{
		current = buffer;
		current[0]=0;
	}
	//此函数不动态增加内存,一次打印的数据长度不应该超过缓冲区的三分之一,否则可能导致失败
	bool printf(const char * format,...)
	{
		if(current)
		{
			if(current - buffer > (m_isize*2)/3)
				return false;
			va_list argPtr ;
			va_start( argPtr, format ) ;
			int count = vsprintf( current, format, argPtr ) ;
			va_end( argPtr );
			current += count ;
			return true;
		}
		return false;
	}
	//此函数拷贝字符串
	bool strcpy(const char * szStr)
	{
		if(current&&szStr)
		{
			int ilen = lstrlen(szStr);
			if((m_isize-(current - buffer)) < (ilen +2))
				return false;
			memcpy(current,szStr,ilen+1);
			current += ilen;
			return true;
		}
		return false;
	}
	char * getcurrentpos()
	{
		return current;
	}
	void move(int ilen)//当前指针向后移动ilen
	{
		current += ilen;
	}
	void reset()
	{
		current = buffer;
	}
	BYTE readbyte()
	{
		current ++;
		return *(current-1);
	}
	void writebyte(BYTE btValue)
	{
		*current = btValue;
		current ++;		
	}
	WORD readword()
	{
		current +=2;
		return *((WORD*)(current-2));
	}
	void writeword(WORD wValue)
	{		
		*((WORD*)current) = wValue;
		current +=2;
	}
	DWORD readdword()
	{
		current +=4;
		return *((DWORD*)(current-4));
	}
	void writedword(DWORD dwValue)
	{
		*((DWORD*)current) = dwValue;
		current +=4;
	}
	__int64 readint64()
	{
		current +=8;
		return *((__int64*)(current-8));
	}
	void writeint64(__int64 iValue)
	{
		*((__int64*)current) = iValue;
		current +=8;
	}
	BYTE * readdata(DWORD dwLen)
	{
		current +=dwLen;
		return (BYTE*)(current-dwLen);
	}
	void writedata(BYTE * pData,DWORD dwLen)
	{
		memcpy(current,pData,dwLen);		
		current +=dwLen;
	}
	char * readstring()
	{
		char * szRes = current;
		int ilen = lstrlen(current);
		current +=(ilen+1);
		return szRes;
	}
	int size()
	{
		return (int)(current-buffer);
	}
	const char * getbuffer(){return buffer;}
private :
	char* buffer;
    char* current;
	int m_isize;
};
class CNetworkByteOrder
{
public:
	static unsigned short int convert(unsigned short int iValue)
	{
		unsigned short int iData;
		((BYTE*)&iData)[0] = ((BYTE*)&iValue)[1];
		((BYTE*)&iData)[1] = ((BYTE*)&iValue)[0];
		return iData;
	}
	static int convert(int iValue)
	{
		int iData;
		((BYTE*)&iData)[0] = ((BYTE*)&iValue)[3];
		((BYTE*)&iData)[1] = ((BYTE*)&iValue)[2];
		((BYTE*)&iData)[2] = ((BYTE*)&iValue)[1];
		((BYTE*)&iData)[3] = ((BYTE*)&iValue)[0];
		return iData;
	}
	static __int64 convert(__int64 iValue)
	{
		__int64 iData;
		((BYTE*)&iData)[0] = ((BYTE*)&iValue)[7];
		((BYTE*)&iData)[1] = ((BYTE*)&iValue)[6];
		((BYTE*)&iData)[2] = ((BYTE*)&iValue)[5];
		((BYTE*)&iData)[3] = ((BYTE*)&iValue)[4];
		((BYTE*)&iData)[4] = ((BYTE*)&iValue)[3];
		((BYTE*)&iData)[5] = ((BYTE*)&iValue)[2];
		((BYTE*)&iData)[6] = ((BYTE*)&iValue)[1];
		((BYTE*)&iData)[7] = ((BYTE*)&iValue)[0];
		return iData;
	}

};
#endif // !defined(AFX_DATASTREAM_H__D90A2534_EA73_4BEA_8B7E_87E59A3D1D26__INCLUDED_)
