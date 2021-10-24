#ifndef _C_GEAR_WHELL_
#define _C_GEAR_WHELL_
#include <string>
#include <assert.h>
#define DEBUG_LINE 
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
#endif