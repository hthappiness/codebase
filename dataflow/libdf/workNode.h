
#pragma once
#include "df_exception.h"
#include <functional>

namespace dataFlow
{
	class CWorkNode
	{
		using EXCETION_CB_PFN = std::function<void(long, std::string&)>;
	public:
		CWorkNode()  = default;
		~CWorkNode() = default;

		//void raiseException(long lErrCode, std::string strErrMsg);
		//void setExceptionCallback(EXCETION_CB_PFN);
	private:
		EXCETION_CB_PFN m_pfnExceptionCb;
	};

}

