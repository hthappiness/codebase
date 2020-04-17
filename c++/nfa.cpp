
#include <string>

class CNFA
{

public : 
    explicit CNfa(int i){

    }

    bool isMatchPattern(std::string& dstStr);

private:
    int processNextChar();

private:
    /* data structure which describe the NFA */
    std::string m_patternStr;

    /* state machine graph,check the transfrom is valid. */
    CDFAGraph m_grammerGraph;
};

//describe the abstract grammer tree, abstract way
//如何描述抽象的语法树等形式，描述模式;文法，左递归如何消除，
// syntax pattern  <---match?--->   inputString 