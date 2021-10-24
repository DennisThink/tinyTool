#ifndef _DENNIS_THINK_TCP_IP_LIB_MAC_ADDR_ANALYSIS_H_
#define _DENNIS_THINK_TCP_IP_LIB_MAC_ADDR_ANALYSIS_H_
#include <pcap.h>
void AnalysisMacAddr(const struct pcap_pkthdr *header, const unsigned char *pkt_data);
void AnalysisIpAddr(const struct pcap_pkthdr *header,const unsigned short type, const unsigned char *pkt_data);
#endif