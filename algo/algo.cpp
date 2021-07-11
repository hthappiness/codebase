// algo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <climits>

#include<stdlib.h>

template <typename t>
class TreeVarTypeTraits
{
	static const std::string empty = "Not define";
};

template < >
class TreeVarTypeTraits<int>
{
	static const int empty = INT_MIN;
};

template < >
class TreeVarTypeTraits<std::string>
{
	static const std::string empty ;
};
const std::string TreeVarTypeTraits<std::string>::empty = " ";

//包括树的输入，遍历
template<typename T>
class CTree : public std::enable_shared_from_this<CTree<T>>
{
public:
	//template<typename T>
	struct CTreeNode
	{
		CTreeNode(T t)
		{
			var = t;
		}
		T var;
		CTreeNode* pstParent = nullptr;
		CTreeNode* pstLeft = nullptr;
		CTreeNode* pstRight = nullptr;
	};

	//template<typename T>
	CTreeNode* makeSubTree(const std::vector<T>& list, CTreeNode* parent, int index)
	{
		if (index >= list.size())
		{
			return nullptr;
		}
		CTreeNode* node = new CTreeNode(list[index]);
		node->pstParent = parent;
		node->pstLeft = makeSubTree(list, node, 2 * index + 1);
		node->pstRight = makeSubTree(list, node, 2 * index + 2);
		return node;
	}

	//template<typename T>
	void makeTree(std::vector<T>& list)
	{
		if (list.empty())
		{
			return ;
		}
		int index = 0;
		CTreeNode* root = new CTreeNode(list[0]);
		root->pstParent = nullptr;
		root->pstLeft = makeSubTree(list, root, 2 * index + 1);
		root->pstRight = makeSubTree(list, root, 2 * index + 2);

		m_root = root;

		//return this->shared_from_this();
	}

	/* 先后关系+树的递归结构 */
	/* 先序遍历:先访问根节点，再访问左子树，再访问右子树 */
	void preOrder()
	{
		CTreeNode* node = m_root;
		std::stack<CTreeNode*> queue;
		std::vector<T> result;
		while ( node )
		{
			while (node)
			{
				result.push_back(node->var);

				if (node->pstRight)
				{
					queue.push(node->pstRight);
				}

				node = node->pstLeft;
			}

			if (queue.empty())
			{
				break;
			}
			node = queue.top();
			if (node)
			{
				queue.pop();
			}
		}

		for (auto& item : result)
		{
			static int i = 0;
			std::cout << i++ <<" : " << item << std::endl;
		}
	}

	/* 中序遍历:先访问左子树，再访问根节点，再访问右子树 */
	void inOrder()
	{
		CTreeNode* node = m_root;
		std::stack<CTreeNode*> nodeStack;
		std::vector<T> result;

		while ( node || !nodeStack.empty())
		{
			while (node != nullptr)
			{
				nodeStack.push(node);
				node = node->pstLeft;
			}

			CTreeNode* tmpNode = nodeStack.top();
			nodeStack.pop();
			result.push_back(tmpNode->var);
			node = tmpNode->pstRight;
		}

		for (auto& item : result)
		{
			static int i = 0;
			std::cout << i++ << " : " << item << std::endl;
		}
	}

	/* 后序遍历:先访问左子树，再访问右子树，再访问根节点 */
	void postOrder()
	{
		CTreeNode* node = m_root;
		std::stack<CTreeNode*> nodeStack;
		std::vector<T> result;

		nodeStack.push(node);

		while ( !nodeStack.empty())
		{
			node = nodeStack.top();
			nodeStack.pop();

			result.push_back(node->var);

			if (node->pstLeft)
			{
				nodeStack.push(node->pstLeft);
			}

			if (node->pstRight)
			{
				nodeStack.push(node->pstRight);
			}
		}

		for (auto iter = result.crbegin(); iter != result.crend(); iter++)
		{
			static int i = 0;
			std::cout << i++ << " : " << *iter << std::endl;
		}
	}

	void postOrderV2()
	{
		std::stack<CTreeNode*> nodeStack;
		std::vector<T> result;

		nodeStack.push(m_root);

		CTreeNode* prev = nullptr;
		while (!nodeStack.empty())
		{
			CTreeNode* cur = nodeStack.top();
			if ( !prev || prev->pstLeft == cur || prev->pstRight == cur)
			{
				if ( cur->pstLeft )
				{
					nodeStack.push(cur->pstLeft);
				}
				else if ( cur->pstRight )
				{
					nodeStack.push(cur->pstRight);
				}
			}
			else if (cur->pstLeft == prev) /* 当前遍历的节点是上一个节点的父节点，也就是树的root，此时需要遍历右子树 */
			{
				if (cur->pstRight)
				{
					nodeStack.push(cur->pstRight);
				}
			}
			else  /* 当前遍历的节点是pstRight是prev */
			{
				result.push_back(cur->var);
				nodeStack.pop();
			}
			prev = cur;
		}
	}

private:
	CTreeNode* m_root;
};

int main()
{
	std::vector<int> vecInt = { 1, 2 ,4, 2, 4, 12, 26, 38, 12, 111, 444, 87, 90};

	CTree<int> tree;
	tree.makeTree(vecInt);
	
	std::cout << "----- pre order  -----" << std::endl;
	tree.preOrder();

	std::cout << "----- in order  -----" << std::endl;
	tree.inOrder();

	std::cout << "----- post order  -----" << std::endl;
	tree.postOrder();
	
	system("pause");
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
