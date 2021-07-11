
#include <iostream>

#include <stack>

//红黑树的定义或者说是性质
//1、每个结点是红色或者黑色的
//2、根节点是黑色的
//3、每个叶节点（NIL）是黑色的
//4、如果一个节点是红色的，则它的两个子节点都是黑色的
//5、对每个节点，从该节点到其所有后代叶节点的简单路径上，均包含相同数目的黑色结点

//迭代流程
//1、初始化
//2、保持
//3、结束条件

template<typename Key>
class CRbTree
{
public:

	enum class color
	{
		COLOR_RED,
		COLOR_BLACK,
		COLOR_BUTT
	};

	class CTreeNode
	{
	public:
		CTreeNode(Key key)
		{
			m_color = COLOR_BLACK;
			m_left  = &sentinel;
			m_right = &sentinel;
		}
		CTreeNode* m_parent{nullptr};
		CTreeNode* m_left{nullptr};
		CTreeNode* m_right{nullptr};
		color m_color;
		Key m_key;
	};

	static constexpr auto sentinel = CTreeNode(nullptr);

	//************************************
	// Method:    insert
	// FullName:  CRbTree<Key>::insert
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: Key key
	//************************************
	CTreeNode* insert(Key key)
	{
		CTreeNode* z = new CTreeNode(key);
		CTreeNode* y = nullptr;
		CTreeNode* x = m_root;

		while (x != nullptr) {
			y = x;
			if (z->m_key < x->m_key) {
				x = x->m_left;
			}
			else {
				x = x->m_right;
			}
		}
		z->m_parent = y;

		/* 如果是空树 */
		if (y == nullptr) {
			m_root = z;
		}
		/* 左孩子 */
		else if (z->m_key < y->m_key) {
			y->m_left = z;
		}
		/* 右孩子 */
		else {
			y->m_right = z;
		}

		/* 新插入的节点是红色 */
		z->m_left  = nullptr;
		z->m_right = nullptr;
		z->m_color = COLOR_RED;

		return rbtreeInsertFixup(z);
	}

	//************************************
	// Method:    deleteNode
	// FullName:  CRbTree<Key>::deleteNode
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: Key key
	//************************************
	int deleteNode(Key key)
	{
		Node* zc, * zp, * zl; // z's child, parent, left.
		Node* old;
		Color color;

		old = z;

		if (z->left && z->right) {
			z = z->right;
			while ((zl = z->left) != NULL) {
				z = zl;
			}
			zc = z->right;
			zp = z->parent;
			color = z->color;
			if (zc) {
				zc->parent = zp;
			}
			if (zp) {
				if (zp->left == z) {
					zp->left = zc;
				}
				else {
					zp->right = zc;
				}
			}
			else {
				root = zc;
			}
			if (z->parent == old) {
				zp = z;
			}
			z->parent = old->parent;
			z->color = old->color;
			z->right = old->right;
			z->left = old->left;
			if (old->parent) {
				if (old->parent->left == old) {
					old->parent->left = z;
				}
				else {
					old->parent->right = z;
				}
			}
			else {
				root = z;
			}
			old->left->parent = z;
			if (old->right) {
				old->right->parent = z;
			}
		}
		else {
			if (!z->left) {
				zc = z->right;
			}
			else if (!z->right) {
				zc = z->left;
			}
			zp = z->parent;
			color = z->color;
			if (zc) {
				zc->parent = zp;
			}
			if (zp) {
				if (zp->left == z) {
					zp->left = zc;
				}
				else {
					zp->right = zc;
				}
			}
			else {
				root = zc;
			}
		}
		free(old);

		if (color == BLACK) {
			root = rbtreeDeleteFixup(root, zc, zp);
		}
	}

	//************************************
	// Method:    search
	// FullName:  CRbTree<Key>::search
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: Key key
	//************************************
	CTreeNode* search(Key key) const
	{
		CTreeNode* tmp = m_root;
		while (tmp != nullptr && key != tmp->key) {
			if (value < tmp->key) {
				tmp = tmp->m_left;
			}
			else {
				tmp = tmp->m_right;
			}
		}

		return tmp;
	}

	//************************************
	// Method:    preOrder
	// FullName:  CRbTree<Key>::preOrder
	// Access:    public 
	// Returns:   int
	// Qualifier:
	//************************************
	int preOrder() 
	{
	}
	//************************************
	// Method:    inOrder
	// FullName:  CRbTree<Key>::inOrder
	// Access:    public 
	// Returns:   int
	// Qualifier:
	//************************************
	void inOrder(CTreeNode* node) 
	{
		if (node != nullptr) {
			inOrder(node->m_left);
			std::cout << "key:" << node->m_key <<" color:"<< node->m_color<<std::endl;
			inOrder(node->m_right);
		}
	}

	void inOrderTraversal()
	{
		if (m_root == nullptr)
			return;
		std::stack<CTreeNode*> nodeStack;
		nodeStack.push(m_root);
		while (!nodeStack.empty()) {

			//这个迭代就是处理一棵树上的中序遍历过程，所以，当右子树不为空的时候，那么就break，重新以右子树为遍历单位，重复这个过程

			//该过程一直找到没有左节点的节点才停止
			while (nodeStack.top()->m_left != nullptr) {
				nodeStack.push(nodeStack.top()->m_left);
			}
			//此时的S.top()是一个没有left的节点，按照中序遍历的特性，可以将其直接输出。
			//while循环会一直将栈顶输出，直到遇到有右节点的节点，这样能保证栈中元素不会重复寻找左孩子
			while (!nodeStack.empty()) {
				CTreeNode* cur = nodeStack.top();
				cout << cur->m_key << " ";
				nodeStack.pop();

				if (cur > m_right != nullptr) {
					nodeStack.push(cur->m_right);
					/* 跳出，重新处理m_right子树 */
					break;
				}
			}
		}
		cout << endl;
	}

	//************************************
	// Method:    postOrder
	// FullName:  CRbTree<Key>::postOrder
	// Access:    public 
	// Returns:   int
	// Qualifier:
	//************************************
	int postOrder() 
	{
	}

	CTreeNode* maximum()
	{
		CTreeNode* tmp = m_root;
		while (tmp->m_right != nullptr) {
			tmp = tmp->m_right;
		}

		return tmp;
	}

	CTreeNode* minimum()
	{
		CTreeNode* tmp = m_root;
		while (tmp->m_left != nullptr) 
		{
			tmp = tmp->m_left;
		}

		return tmp;
	}

	/* 后继 */
	CTreeNode* successor(CTreeNode* x) 
	{
		if (x->m_right != nullptr) 
		{
			return minimum(x->m_right);
		}
		/* 当右子树为空，则向父节点找 */
		else 
		{
			CTreeNode* y = x->parent;
			/*         
			 *这个迭代看起来有点难理解；开始+保持+终止
			 *当父节点不为空而且为右节点的时候，则一直迭代；直观来看，就是当为左子树的时候，就是后继，参考中序遍历的顺序
			 */
			while (y != nullptr && x == y->m_right) {
				x = y;
				y = y->parent;
			}

			return y;
		}
	}

	/* 前驱 */
	CTreeNode* predecessor(CTreeNode* x) 
	{
		if (x->m_left != nullptr)
		{
			return maximum(x->m_left);
		}
		else 
		{
			CTreeNode* y = x->m_parent;
			while (y != nullptr && x != y->m_right)
			{
				x = y;
				y = y->m_parent;
			}

			return y;
		}
	}
private:

	/*
	 * -----------------------------------------------------------
	 * |   x                 y
	 * |  / \     ==>       / \
	 * | a   y             x   c
	 * |    / \           / \
	 * |   b   c         a   b
	 * -----------------------------------------------------------
	*/
	int leftRotate(CTreeNode* x) 
	{
		// Set y.
		CTreeNode* y = x->m_right;

		// Turn y's left subtree into x's right subtree.
		x->m_right = y->m_left;
		if (y->m_left != nullptr) {
			y->m_left->m_parent = x;
		}

		y->m_parent = x->m_parent;

		// Link x's parent to y.
		if (x->m_parent == nullptr) { // When x is root.
			m_root = y;
		}
		else if (x == x->m_parent->m_left) { // When x is its parent's left child.
			x->m_parent->m_left = y;
		}
		else { // When x is its parent's right child.
			x->m_parent->m_right = y;
		}

		// Put x on y's left.
		y->m_left = x;
		x->m_parent = y;

		return 0;
	}

	/*
	 * -----------------------------------------------------------
	 * |      x              y
	 * |     / \    ==>     / \
	 * |    y   c          a   x
	 * |   / \                / \
	 * |  a   b              b   c
	 * -----------------------------------------------------------
	*/
	int rightRotate(CTreeNode* x) 
	{
		// Set y.
		CTreeNode* y = x->m_left;

		// Turn y's right subtree into x's left subtree.
		x->m_left = y->m_right;
		if (y->m_right != nullptr) {
			y->m_right->parent = x;
		}

		y->m_parent = x->m_parent;
		
		// Link x's parent to y.
		if (x->m_parent == nullptr) { // When x is root.
			m_root = y;
		}
		else if (x == x->m_parent->m_left) { // When x is its parent's left child.
			x->m_parent->m_left = y;
		}
		else { // When x is its parent's right child.
			x->m_parent->m_right = y;
		}

		// Put x on y's right.
		y->m_right = x;
		x->m_parent = y;

		return 0;
	}

	int getBlackHeight() 
	{
	
	}

	CTreeNode* rbtreeInsertFixup(CTreeNode* pNode)
	{
		CTreeNode* z = pNode;
		CTreeNode *zp, *zgp, *zu, *tmp; // z's parent, grandparent, uncle.
		while ((zp = z->parent) && zp->color == RED) {
			zgp = zp->parent;
			if (zp == zgp->left) {
				zu = zgp->right;
				if (zu && zu->color == RED) {
					zu->color = BLACK;
					zp->color = BLACK;
					zgp->color = RED;
					z = zgp;
				}
				else {
					if (zp->right == z) {
						root = leftRotate(root, zp);
						tmp = zp;
						zp = z;
						z = tmp;
					}
					zp->color = BLACK;
					zgp->color = RED;
					root = rightRotate(root, zgp);
				}
			}
			else
			{
				zu = zgp->left;
				if (zu && zu->color == RED) {
					zu->color = BLACK;
					zp->color = BLACK;
					zgp->color = RED;
					z = zgp;
				}
				else {
					if (zp->left == z) {
						root = rightRotate(root, zp);
						tmp = zp;
						zp = z;
						z = tmp;
					}
					zp->color = BLACK;
					zgp->color = RED;
					root = leftRotate(root, zgp);
				}
			}
		}
		root->color = BLACK;

		return root;
	}

	CTreeNode* rbtreeDeleteFixup()
	{
		Node* other, * oleft, * oright;
		while ((!node || node->color == BLACK) && node != root) {
			if (p->left == node) {
				other = p->right;
				if (other->color == RED) {
					other->color = BLACK;
					p->color = RED;
					root = leftRotate(root, p);
					other = p->right;
				}
				if ((!other->left || other->left->color == BLACK) && (!other->right || other->right->color == BLACK)) {
					other->color = RED;
					node = p;
					p = node->parent;
				}
				else {
					if (!other->right || other->right->color == BLACK) {
						if ((oleft = other->left)) {
							oleft->color = BLACK;
						}
						other->color = RED;
						root = rightRotate(root, other);
						other = p->right;
					}
					other->color = p->color;
					p->color = BLACK;
					if (other->right) {
						other->right->color = BLACK;
					}
					root = leftRotate(root, p);
					node = root;
					break;
				}
			}
			else
			{
				other = p->left;
				if (other->color == RED) {
					other->color = BLACK;
					p->color = RED;
					root = rightRotate(root, p);
					other = p->left;
				}
				if ((!other->left || other->left->color == BLACK) && (!other->right || other->right->color == BLACK)) {
					other->color = RED;
					node = p;
					p = node->parent;
				}
				else {
					if (!other->left || other->left->color == BLACK) {
						if ((oright = other->right)) {
							oright->color = BLACK;
						}
						other->color = RED;
						root = leftRotate(root, other);
						other = p->left;
					}

					other->color = p->color;
					p->color = BLACK;
					if (other->left) {
						other->left->color = BLACK;
					}
					root = rightRotate(root, p);
					node = root;
					break;
				}
			}
		}

		if (node) {
			node->color = BLACK;
		}

		return root;
	}

private:
	CTreeNode* m_root{nullptr};
};