#include <iostream>

class CParam
{
public:
    void setMagic(int iMagic){
       m_iMagic = iMagic;
    }
    CParam() = default;
    //CParam(CParam&) = default;
    CParam& operator=(const CParam&) = default;

    void printParam(){
       std::cout<<m_iMagic<<std::endl;
    }
private:
    int m_iMagic;
};

void refTestParam(CParam& tmpParam)
{
    CParam stParam;
    stParam = tmpParam;
    stParam.printParam();
}

int main()
{
    CParam refParam;
    refParam.setMagic(1234);

    refTestParam(refParam);
}
