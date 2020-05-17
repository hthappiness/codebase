
#include <iostream>
#include "df_exception.h"

namespace dataFlow{

CDFException::CDFException(long lErrCode, std::string what)
{
    //out put the exception log information.
    std::cout<<what;
    m_lErrCode = lErrCode;
}

}