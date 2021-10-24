#include <iostream>
#include "restclient-cpp/restclient.h"
#include <unistd.h>
#include <stdlib.h>
//#include "pugixml.hpp"
#include <string.h>
#include <vector>
#include <string>
#include <array>
#include <assert.h>
#include "CommonConfig.h"
//using namespace pugi;

void GetDetailWeather(const std::string strContext)
{
    /*<li class="sky skyid lv1">
<h1>6日（明天）</h1>
<big class="png40 d00"></big>
<big class="png40 n00"></big>
<p title="晴" class="wea">晴</p>
<p class="tem">
<span>11</span>/<i>-2℃</i>
</p>
<p class="win">
<em>
<span title="北风" class="N"></span>
<span title="西南风" class="SW"></span>
</em>
<i>4-5级转<3级</i>
</p>
<div class="slid"></div>*/
    {
        auto h1Start = strContext.find("<h1>");
        auto h1End = strContext.find("</h1>");
        std::string strDate = strContext.substr(h1Start+4,h1End-h1Start-4);
        std::cout<<"Date : "<<strDate<<std::endl;
    }
    {
        auto weatherStart = strContext.find("class=\"wea\">");
        auto weatherEnd = strContext.find("</p>",weatherStart);
        std::string strDate = strContext.substr(weatherStart+12,weatherEnd-weatherStart-12);
        std::cout<<"Weather : "<<strDate<<std::endl;
    }
    {
        auto weatherStart = strContext.find("<span>");
        auto weatherEnd = strContext.find("</span>",weatherStart);
        std::string strDate = strContext.substr(weatherStart+6,weatherEnd-weatherStart-6);
        std::cout<<"HighTemp : "<<strDate<<std::endl;
    }

    {
        auto weatherStart = strContext.find("<i>");
        auto weatherEnd = strContext.find("</i>",weatherStart);
        std::string strDate = strContext.substr(weatherStart+3,weatherEnd-weatherStart-3);
        std::cout<<"LowTemp : "<<strDate<<std::endl;
    }

    std::size_t WindDirectIndex = 0;
    {
        auto windStart = strContext.find("<span title=\"");
        auto windEnd = strContext.find("\"",windStart+13);
        std::string strDate = strContext.substr(windStart+13,windEnd-windStart-13);
        std::cout<<"WindLevel : "<<strDate<<std::endl;
        WindDirectIndex = windEnd;
    }
    {
        auto windStart = strContext.find("<i>",WindDirectIndex);
        auto windEnd = strContext.find("</i>",windStart+3);
        std::string strDate = strContext.substr(windStart+3,windEnd-windStart-3);
        std::cout<<"WindDirect : "<<strDate<<std::endl;
    }
}

bool ParsePosition(const std::string strContext)
{
    /*<div class="crumbs fl">
	  <a href="http://shanxi.weather.com.cn" target="_blank">山西</a>
	  <span>&gt;</span>
	  <a href="http://www.weather.com.cn/weather/101100701.shtml" target="_blank">临汾</a><span>&gt;</span>  <span>襄汾</span>
	  </div>*/
    std::string strFirstTag=R"(<div class="crumbs fl">)";
    auto firstIndex = strContext.find(strFirstTag)+strFirstTag.length();
    if(firstIndex == std::string::npos)
    {
        return false;
    }
    auto lastIndex = strContext.find("</div>",firstIndex);
    if(lastIndex == std::string::npos)
    {
        return false;
    }
    std::size_t endIndex = 0;
    std::size_t startIndex = firstIndex;
    do
    {
        std::string strKeyTag="target=\"_blank\">";
        startIndex = strContext.find(strKeyTag,startIndex)+strKeyTag.length();
        if(startIndex == std::string::npos)
        {
            break;
        }
        endIndex = strContext.find("<",startIndex);
        if(endIndex < lastIndex)
        {
            std::cout<<strContext.substr(startIndex,endIndex-startIndex)<<",";
            startIndex = endIndex;
        }
        else
        {
            break;
        }
        if(endIndex == std::string::npos)
        {
            break;
        }
    } while (endIndex < lastIndex);
    std::string strKeyTag = "<span>";
    std::size_t lastStartIndex = firstIndex;
    startIndex = firstIndex;
    
    while(startIndex != std::string::npos && startIndex < lastIndex)
    {
        lastStartIndex = startIndex;
        startIndex = strContext.find(strKeyTag,startIndex)+strKeyTag.length();
        if(startIndex == std::string::npos)
        {
            break;
        }
    }

    endIndex = strContext.find("</span>",lastStartIndex);
    if(endIndex < lastIndex)
    {
        std::cout<<strContext.substr(lastStartIndex,endIndex-lastStartIndex)<<",";
        startIndex = endIndex;
    }
    else
    {
        
    }
    return true;
}
bool ParseHtmlGetWeather2(const std::string strContext)
{
    //std::cout<<__LINE__<<std::endl;
    return  ParsePosition(strContext);
    //return ;
    //xml_document doc;
    std::string strStartTag="<ul class=\"t clearfix\">";
    std::string strEndTag = "</ul>";
    std::size_t startIndex = strContext.find(strStartTag);
    std::size_t endIndex = strContext.find(strEndTag,startIndex);
    std::string subStr = strContext.substr(startIndex,endIndex-startIndex+strEndTag.length());
    std::cout<<"SUB STR"<<std::endl;
    //std::cout<<subStr<<std::endl;
    {
        std::size_t liStart = 0;
        std::size_t liEnd = 0;
       
        while(liEnd != std::string::npos)
        {
            liStart = subStr.find("<li",liEnd);
            liEnd = subStr.find("</li>",liStart);
            if(liEnd != std::string::npos)
            {
                std::string strLi = subStr.substr(liStart+3,liEnd-liStart-7);
                std::cout<<"------------------------------------"<<std::endl;
               // std::cout<<strLi<<std::endl;
                std::cout<<"------------------------------------"<<std::endl;
                GetDetailWeather(strLi);
                std::cout<<"------------------------------------"<<std::endl;
            }
           
            liStart = liEnd;
        }
    }
}



void ParseHtmlGetWeather(const std::string strContext)
{
    std::cout<<__LINE__<<std::endl;
    ParsePosition(strContext);
    //xml_document doc;
    std::string strStartTag="<ul class=\"t clearfix\">";
    std::string strEndTag = "</ul>";
    std::size_t startIndex = strContext.find(strStartTag);
    std::size_t endIndex = strContext.find(strEndTag,startIndex);
    std::string subStr = strContext.substr(startIndex,endIndex-startIndex+strEndTag.length());
    std::cout<<"SUB STR"<<std::endl;
    std::cout<<subStr<<std::endl;
    std::cout<<"-------------------------------";
	//load_document_copy(doc,subStr.c_str(),subStr.length());
    //doc.print(std::cout);
    std::cout<<"-------------------------------";
    std::vector<std::vector<std::string>> xPathArray=
    {
        {
            "ul/li[position()=1]/h1",
            "ul/li[position()=1]/p[position()=1]",
            "ul/li[position()=1]/p[position()=2]/span",
            "ul/li[position()=1]/p[position()=2]/i",
            "ul/li[position()=1]/p[position()=3]/i",
            "ul/li[position()=1]/p[position()=3]/em/span[position()=1]/@title"
        },
        {
            "ul/li[position()=2]/h1",
            "ul/li[position()=2]/p[position()=1]",
            "ul/li[position()=2]/p[position()=2]/span",
            "ul/li[position()=2]/p[position()=2]/i",
            "ul/li[position()=2]/p[position()=3]/i",
            "ul/li[position()=2]/p[position()=3]/em/span[position()=1]/@title"
        },
        {
            "ul/li[position()=3]/h1",
            "ul/li[position()=3]/p[position()=1]",
            "ul/li[position()=3]/p[position()=2]/span",
            "ul/li[position()=3]/p[position()=2]/i",
            "ul/li[position()=3]/p[position()=3]/i",
            "ul/li[position()=3]/p[position()=3]/em/span[position()=1]/@title"
        },
        {
            "ul/li[position()=4]/h1",
            "ul/li[position()=4]/p[position()=1]",
            "ul/li[position()=4]/p[position()=2]/span",
            "ul/li[position()=4]/p[position()=2]/i",
            "ul/li[position()=4]/p[position()=3]/i",
            "ul/li[position()=4]/p[position()=3]/em/span[position()=1]/@title"
        },
        {
            "ul/li[position()=5]/h1",
            "ul/li[position()=5]/p[position()=1]",
            "ul/li[position()=5]/p[position()=2]/span",
            "ul/li[position()=5]/p[position()=2]/i",
            "ul/li[position()=5]/p[position()=3]/i",
            "ul/li[position()=5]/p[position()=3]/em/span[position()=1]/@title"
        },
        {
            "ul/li[position()=6]/h1",
            "ul/li[position()=6]/p[position()=1]",
            "ul/li[position()=6]/p[position()=2]/span",
            "ul/li[position()=6]/p[position()=2]/i",
            "ul/li[position()=6]/p[position()=3]/i",
            "ul/li[position()=6]/p[position()=3]/em/span[position()=1]/@title"
        },
        {
            "ul/li[position()=7]/h1",
            "ul/li[position()=7]/p[position()=1]",
            "ul/li[position()=7]/p[position()=2]/span",
            "ul/li[position()=7]/p[position()=2]/i",
            "ul/li[position()=7]/p[position()=3]/i",
            "ul/li[position()=7]/p[position()=3]/em/span[position()=1]/@title"
        },
    };
    /*for(const auto& arrayItem:xPathArray)
    {
        for(const auto& pathItem:arrayItem)
        {
            xpath_query q(pathItem.c_str());
	        xpath_node n2 = doc.select_node(q);
            std::cout<<"  "<<n2.node().text().as_string();
        }
        std::cout<<__LINE__<<std::endl;
    }*/
   
	//xpath_node_set_tester(sorted, "sorted order failed") % 4 % 7;
	//xpath_node_set_tester(reverse_sorted, "reverse sorted order failed") % 7 % 4;
}

#if 0
#define DEBUG_LINE std::cout<<__LINE__<<std::endl;
#else
#define DEBUG_LINE
#endif

class CGearWhell final
{
public:
    explicit CGearWhell(const int minIndex,const int maxIndex,const int step):
    m_START_INDEX(minIndex),m_END_INDEX(maxIndex),m_curIndex(minIndex),m_STEP(step)
    {
        assert(minIndex < maxIndex);
        assert(maxIndex - minIndex > step);
    }

    void Reset()
    {
        m_curIndex=m_START_INDEX;
    }

    bool IsInitState() const 
    {
        return m_curIndex==m_START_INDEX;
    }
    bool Walk()
    {
       if(m_curIndex+m_STEP <= m_END_INDEX)
       {
           DEBUG_LINE
           m_curIndex+=m_STEP;
           return true;
       }
       else
       {
           DEBUG_LINE
           return false;
       }
       
    }

    int GetValue()const 
    {
        return m_curIndex;
    }

    std::string ToString() const 
    {
        std::string strCur = std::to_string(m_curIndex);
        //std::cout<<strCur<<std::endl;
        return strCur.substr(1,strCur.length()-1);
    }
private:
    int m_START_INDEX;
    int m_END_INDEX;
    int m_curIndex;
    int m_STEP;
};

class CGetCityCode final
{
public:
    explicit CGetCityCode()
    {
        m_whellVec.push_back(CGearWhell(1101,1900,1));
        m_whellVec.push_back(CGearWhell(101,199,1));
        m_whellVec.push_back(CGearWhell(101,199,1));
        m_whellVec.push_back(CGearWhell(100,199,0));
        m_index=3;
    }

    void SetResult(bool good)
    {
        if(good)
        {
            DEBUG_LINE
            m_whellVec[m_whellVec.size()-2].Walk();
        }
        else
        {
            std::size_t index = 0;
            for(std::size_t i = 0 ; i < m_whellVec.size()-1 ; i++)
            {
                if(m_whellVec[i].IsInitState())
                {

                }
                else
                {
                    index = i;
                    break;
                }
            }

            if(m_whellVec[index].Walk())
            {
                DEBUG_LINE
            }
            else
            {
                if(index > 0)
                {
                    DEBUG_LINE
                    index --;
                    m_whellVec[index].Walk();
                }
                /* code */
            }
            for(std::size_t i = index+1; i < m_whellVec.size()-1 ; i++)
            {
                DEBUG_LINE
                m_whellVec[i].Reset();
            }
        }
        
        
    }
    std::string GetCityCode()
    {
        std::string strResult = "";
        for(const auto item:m_whellVec)
        {
            strResult+=item.ToString();
        }
        return strResult;
    }
private:
    std::vector<CGearWhell> m_whellVec;
    int m_index;
    bool m_lastResult;
};


class CGetCityCodeProvince final
{
public:
    explicit CGetCityCodeProvince()
    {
        //101  10 07 07 007
        m_whellVec.push_back(CGearWhell(1101,1900,1));
        m_whellVec.push_back(CGearWhell(105,199,1));
        m_whellVec.push_back(CGearWhell(101,199,1));
        m_whellVec.push_back(CGearWhell(101,199,1));
        m_whellVec.push_back(CGearWhell(1001,1999,1));
    }

    void SetResult(bool good)
    {
        if(!m_lastResult && !good)
        {
            m_index = 4;
            int index = m_whellVec.size()-1;
            for(std::size_t i = 0 ; i < m_whellVec.size(); i++)
            {
                if(m_whellVec[i].IsInitState())
                {

                }
                else
                {
                    index = i;
                    break;
                }
            }
            for(std::size_t i = index+1; i < m_whellVec.size(); i++)
            {
                DEBUG_LINE
                m_whellVec[i].Reset();
            }
            m_whellVec[2].Walk();
            return ;
        }
        m_index=5;
        if(good)
        {
            DEBUG_LINE
            m_whellVec[m_whellVec.size()-1].Walk();
            m_lastResult = good;
        }
        else
        {
            int index = m_whellVec.size()-1;
            for(std::size_t i = 0 ; i < m_whellVec.size(); i++)
            {
                if(m_whellVec[i].IsInitState())
                {

                }
                else
                {
                    index = i;
                    break;
                }
            }

            if(m_whellVec[index].Walk())
            {
                DEBUG_LINE
            }
            else
            {
                if(index > 0)
                {
                    DEBUG_LINE
                    index --;
                    m_whellVec[index].Walk();
                }
                /* code */
            }
            for(std::size_t i = index+1; i < m_whellVec.size(); i++)
            {
                DEBUG_LINE
                m_whellVec[i].Reset();
            }
            m_lastResult = good;
        }
        
        
    }
    std::string GetCityCode()
    {
        std::string strResult = "";
        for(int i = 0 ; i < m_index ; i++)
        {
            strResult+=m_whellVec[i].ToString();
        }
        return strResult;
    }
private:
    std::vector<CGearWhell> m_whellVec;
    int m_index=4;
    bool m_lastResult=true;
};


int main(int argc,char * argv[])
{
    std::cout<<g_SERVER_VERSION<<std::endl;
    std::cout<<argc<<argv[0]<<std::endl;
    long long startIndex = 101060101;
    long long LAST_INDEX = 101900707007;
    srand(time(nullptr));
    CGetCityCodeProvince cityCodeUtil;
    int falseCount = 0;
    std::vector<std::string> goodVec={
        "001",
        "002",
        "003",
        "004",
        "005",
        "006",
        "007",
        "008",
        "009",
        "001"
        "012",
        "013",
        "014",
        "015",
        "016",
        "017",
        "018",
        "019",
        "020",
        "021",
    };
    while(startIndex < LAST_INDEX)
    {
        std::string strCode = std::to_string(startIndex);
        //std::cout<<strCode<<std::endl;
        std::string strWeather="http://www.weather.com.cn/weather/"+strCode+".shtml";
        RestClient::Response res = RestClient::get(strWeather);
        //std::cout<<res.code<<std::endl;
        //std::cout<<res.body<<std::endl;
        bool bResult = ParseHtmlGetWeather2(res.body);
        if(bResult)
        {
            falseCount = 0;
            startIndex++;
            //goodVec.push_back(strCode);
        }
        else
        {
            falseCount++;
        }
        //cityCodeUtil.SetResult(bResult);
        
        if(bResult)
        {
            std::cout<<strCode<<std::endl;   
        }
        else
        {
            //std::cout<<"ERROR "<<strCode<<std::endl;
        }
        if(falseCount > 2)
        {
            startIndex = (startIndex/10+1)*10+1;
        }
        
        //startIndex++;
        //sleep(rand()%10);    
    }
  
    return 0;
}