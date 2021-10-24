#include "MacAddrAnalysis.h"
#include "NetProtoPrint.h"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <Winsock2.h>
void AnalysisMacAddr(const struct pcap_pkthdr *header, const unsigned char *pkt_data)
{
    struct tm *ltime;
	char timestr[16];
	time_t local_tv_sec;

	/*
	 * unused parameters
	 */
	(VOID)(pkt_data);

	/* convert the timestamp to readable format */
	local_tv_sec = header->ts.tv_sec;
	ltime=localtime(&local_tv_sec);
	strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);
    if(header->len > sizeof(MacAddrHeader))
    {
	    printf("%s,%.6d len:%d\t\t", timestr, header->ts.tv_usec, header->len);
        MacAddrHeader Header;
        memcpy(&Header,pkt_data,sizeof(MacAddrHeader));
		Header.m_type = ntohs(Header.m_type);
        std::cout<<MacAddrToString(Header)<<std::endl;
		if( (0x0800 == Header.m_type) && 
		    (header->len > sizeof(MacAddrHeader)+sizeof(IpV4AddrHeader)) )
		{
			IpV4AddrHeader v4Header;
			memcpy(&v4Header,pkt_data+sizeof(MacAddrHeader),sizeof(IpV4AddrHeader));
			{
				v4Header.m_totalLength = ntohs(v4Header.m_totalLength);
			}
			std::cout<<IpV4AddrToString(v4Header)<<std::endl;
		}
    }
}

void AnalysisIpAddr(const struct pcap_pkthdr *header, const unsigned short type, const unsigned char *pkt_data)
{

}