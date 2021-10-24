#ifndef _C_CITY_CODE_GENERATE_H_
#define _C_CITY_CODE_GENERATE_H_
#include <vector>
#include "CGearWhell.h"
#include "CommonEnum.h"
#if 0
#define DEBUG_LINE std::cout<<__LINE__<<std::endl;
#else 
#define DEBUG_LINE
#endif

class CGetCityCodeProvince final
{
public:
    explicit CGetCityCodeProvince()
    {
        //101-10-07-07-007
        m_whellVec.push_back(CGearWhell(1203,1900,1));
        m_whellVec.push_back(CGearWhell(101,199,1));
        m_whellVec.push_back(CGearWhell(101,199,1));
        m_whellVec.push_back(CGearWhell(101,199,1));
        m_whellVec.push_back(CGearWhell(1001,1999,1));
    }

    void SetResult(const DIFF_LEVEL& difLev )
    {
        int indexLine = 0;
        if(difLev == COUNTRY_DIFF)
        {
            DEBUG_LINE
            indexLine = 0;
        }
        if(difLev == PROVINCE_DIFF)
        {
            DEBUG_LINE
            indexLine = 1;
        }
        if(difLev == CITY_DIFF)
        {
            DEBUG_LINE
            indexLine = 2;
        }
        if(difLev == AREA_DIFF)
        {
            DEBUG_LINE
            indexLine = 3;
        }
        m_whellVec[indexLine].Walk();
        for(int i = indexLine+1 ; i < m_whellVec.size() ;i++)
        {
            DEBUG_LINE
            m_whellVec[i].Reset();
        }
    }

    std::string GetCityCode()
    {
        std::string strResult = "";
        int nCount = m_whellVec.size();
        int i = 0;
        for(i = 0 ; i < nCount -1; i++)
        {
            strResult=strResult+m_whellVec[i].ToString()+"-";
        }
        strResult=strResult+m_whellVec[i].ToString();
        return strResult;
    }
private:
    std::vector<CGearWhell> m_whellVec;
};
#endif