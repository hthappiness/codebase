
#include "workNode.h"

namespace dataFlow{

void CWorkNode::raiseException(long lErrCode, std::string strErrMsg)
{
    throw CDFException(lErrCode, strErrMsg);
}

void CWorkNode::setExceptionCallback(EXCETION_CB_PFN pfnExceptionCb)
{
    m_pfnExceptionCb = pfnExceptionCb;
}

}
