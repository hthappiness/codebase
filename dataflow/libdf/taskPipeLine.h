/*
 *task pipeline,
 */

#pragma once
#include <vector>
#include <memory>
namespace dataFlow {

	class CTaskPipeLine
	{
	public:
		CTaskPipeLine() = default;
		~CTaskPipeLine() = default;

		int AppendNode(int iChanId, CWorkNode* pstWorkNode, UnifiedMap initParam);
		int DelNode();

		int start();
		int stop();

	private:
		struct CNodeInfo {
			int iNodeType;
			int iChannelNum;

			CWorkNode* pstWorkNode;
			UnifiedMap initParam;
		};

		std::vector<std::shared_ptr<NodeInfo>> m_NodeVec;
	};

};
