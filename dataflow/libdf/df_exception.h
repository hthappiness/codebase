
#include <stdexcept>
#include <string>

namespace dataFlow{

class CDFException : public std::runtime_error
{
public:
    CDFException(long lErrCode, std::string strErrMsg);

private:
    long m_lErrCode;
};

}