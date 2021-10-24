#include "NetProtoPrint.h"
std::string UnsignedCharToHex(const unsigned char ch)
{
    const char buf[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    char result[3]={0};
    result[0]=buf[ch/16];
    result[1]=buf[ch%16];
    return std::string(result);
}

std::string MacAddrToString(const MacAddrHeader& header)
{
    std::string strResult;
    {
        std::string strSrc;
        for(int i = 0 ; i < 5 ; i++)
        {
            strSrc += UnsignedCharToHex(header.mSrcMacAddr[i]);
            strSrc += "-";
        }
        strSrc += UnsignedCharToHex(header.mSrcMacAddr[5]);

        strResult = "SourceMac: "+strSrc;
    }

    {
        std::string strDst;
        for(int i = 0 ; i < 5 ; i++)
        {
            strDst += UnsignedCharToHex(header.mDestMacAddr[i]);
            strDst += "-";
        }
        strDst += UnsignedCharToHex(header.mDestMacAddr[5]);

        strResult = strResult +" DestMac: "+strDst;
    }

	//
    {
		if (header.m_type == 0x0800)
		{
			strResult += " Type: IPV4 Frame";
		}
		else if(header.m_type == 0x0806)
		{
			strResult += " Type: ARP Frame";
		}
		else if (header.m_type == 0x8035)
		{
			strResult += " Type: RARP Frame";
		}
		else if (header.m_type == 0x86DD)
		{
			strResult += " Type: IPV6 Frame";
		}
		else if (header.m_type == 0x8006)
		{
			strResult += " Type: Nestar Frame";
		}
		else
		{
			std::string strType = " Type: " + std::to_string(header.m_type);
			strResult += strType;
		}
    }
    return strResult;
}

std::string V4AddrToString(const unsigned int v4Addr)
{
    unsigned int nCopy = v4Addr;
    unsigned char * pData=reinterpret_cast<unsigned char *>(&nCopy);
    char buff[32]={0};
    sprintf(buff,"%d.%d.%d.%d",pData[0],pData[1],pData[2],pData[3]);
    return std::string(buff);
}

std::string IpV4AddrToString(const IpV4AddrHeader& ip)
{
    std::string strResult;
    //Version and HeaderLength
    int nHeaderLength = 0;
    {
        strResult = strResult + " Version: ";
        strResult = strResult + std::to_string((ip.m_VersionHeaderLength&0xF0)>>4);

        strResult = strResult + " HeaderLen: ";
        strResult = strResult + std::to_string(4*(ip.m_VersionHeaderLength&0x0F));
    }

    //TOS
    {
        strResult = strResult + " TOS: ";
        strResult = strResult + std::to_string(ip.m_Tos);
    }

    //IP Length
    {
        strResult = strResult + " IpDataLength: ";
        strResult = strResult + std::to_string(ip.m_totalLength);
    }

    //Identification
    {
        strResult = strResult + " IpIdentify: ";
        strResult = strResult + std::to_string(ip.m_identification);
    }

    //flags
    {
        strResult = strResult + " Flags: ";
        strResult = strResult + std::to_string(ip.m_flagsFragmentOffSet&0xE000);


    }

    //TTL and Protocol
    {
        strResult = strResult + " TTL: ";
        strResult = strResult + std::to_string(ip.m_ttl);

        strResult = strResult + " Protocol: ";
        strResult = strResult + std::to_string(ip.m_proto); 
    }

    //Check Sum
    {
        strResult = strResult + " CheckSum: ";
        strResult = strResult + std::to_string(ip.m_headerCheckSum);
    }

    //IP source address
    {
        strResult = strResult + " SrcAddr: "+V4AddrToString(ip.m_srcIpAddr);
    }

    //Ip destination address
    {
        strResult = strResult + " DstAddr: "+V4AddrToString(ip.m_dstIpAddr);
    }

    return strResult;
}