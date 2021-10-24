#ifndef _C_FILE_H_
#define _C_FILE_H_
#include <string>
#include <iostream>
#include <fstream>
class CReadFile
{
public:
    explicit CReadFile(const std::string& fileName){
        m_fin.open(fileName);
    }
    virtual ~CReadFile(){
        m_fin.close();
    }
    bool ReadLine(std::string& strLine){
        if(!m_fin.eof())
        {
            getline(m_fin, strLine);
            return true;
        }
        else
        {
            return false;
        }
        
    }
private:
    std::ifstream m_fin;
};
#endif