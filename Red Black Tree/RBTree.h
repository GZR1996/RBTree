#pragma once

#include <TreeNode.h>

template <class T>
class RBTree {

#define rb_parent(r)		((r)->parent)
#define rb_color(r)			((r)->color)
#define rb_is_red(r)		((r)->color==RED)
#define rb_is_black(r)		((r)->color==BLACK)
#define rb_set_black(r)		do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)		do { (r)->color = RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)	do { (r)->color = (c); } while (0)

private:
	RBTNode<T> *tree_root;

public:
	RBTree();
	~RBTree();

	void preOrder();
	void inOrder();
	void postOrder();

	RBTNode<T>* search(T key);
	RBTNode<T>* iterativeSearch(T key);

	T minimum();
	T maximum();

	// 后继结点
	RBTNode<T>* successor(RBTNode<T> *x);
	// 前驱结点
	RBTNode<T>* predecessor(RBTNode<T> *x);

	void insert(T key);
	void remove(T key);
	void destroy();
	void print();

private:
	void preOrder(RBTNode<T> *tree)const;
	void inOrder(RBTNode<T> *tree)const;
	void postOrder(RBTNode<T> *tree)const;

	RBTNode<T>* search(RBTNode<T> *x, T key)const;
	RBTNode<T>* iterativeSearch(RBTNode<T>* x, T key) const;

	RBTNode<T>* minimum(RBTNode<T>* tree);
	RBTNode<T>* maximum(RBTNode<T>* tree);

	void leftRotate(RBTNode<T>* &root, RBTNode<T>* x);
	void rightRotate(RBTNode<T>* &root, RBTNode<T>* y);

	void insert(RBTNode<T>* &root, RBTNode<T>* node);
	void insertFixUp(RBTNode<T>* &root, RBTNode<T>* node);
	void remove(RBTNode<T>* &root, RBTNode<T> *node);
	void removeFixUp(RBTNode<T>* &root, RBTNode<T> *node, RBTNode<T> *parent);
	void destroy(RBTNode<T>* &tree);
	void print(RBTNode<T>* tree, T key, int direction);
};

template<class T>
inline void RBTree<T>::leftRotate(RBTNode<T>*& root, RBTNode<T>* x) {
	RBTNode<T> *y = x->right;

	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;

	y->parent = x->parent;

	if (x->parent == NULL)
		root = y;
	else {
		if (x->parent->left == x)
			x->parent->left = y;
		else
			x->parent->right = y;
	}

	y->left = x;
	x->parent = y;
}

template<class T>
inline void RBTree<T>::rightRotate(RBTNode<T>*& root, RBTNode<T>* y) {
	RBTNode<T> *x = y->left;

	y->left = x->right;
	if (x->right != NULL)
		x->right->parent = y;

	x->parent = y->parent;

	if (y->parent == NULL) {
		root = x;           
	}
	else {
		if (y == y->parent->right)
			y->parent->right = x;   
		else
			y->parent->left = x;   
	}

	x->right = y;
	y->parent = x;
}

template<class T>
inline void RBTree<T>::insert(RBTNode<T>*& root, RBTNode<T>* node) {
	RBTNode<T> *y = NULL;
	RBTNode<T> *x = root;

	while (x != NULL) {
		y = x;
		if (node->key < x->key)
			x = x->left;
		else
			x = x->right;
	}

	node->parent = y;
	if (y != NULL) {
		if (node->key < y->key)
			y->left = node;
		else
			y->right = node;
	}
	else
		root = node;

	node->color = RED;
	insertFixUp(root, node);
}

template<class T>
inline void RBTree<T>::insertFixUp(RBTNode<T>*& root, RBTNode<T>* node) {
	RBTNode<T> *parent, *grand_parent;

	while ((parent = rb_parent(node)) && rb_is_red(parent)) {
		grand_parent = rb_parent(parent);

		if (parent == grand_parent->left) {
			RBTNode<T> *uncle = grand_parent->right;
			if (uncle&&rb_is_red(uncle)) {
				rb_set_black(uncle);
				rb_set_black(parent);
				rb_set_red(grand_parent);
				node = grand_parent;
				continue;
			}

			if (parent->right == node) {
				RBTNode<T> *tmp;
				leftRotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			rb_set_black(parent);
			rb_set_red(grand_parent);
			rightRotate(root, grand_parent);
		}
		else {
			RBTNode<T> *uncle = grand_parent->left;
			if (uncle && rb_is_red(uncle)) {
				rb_set_black(uncle);
				rb_set_black(parent);
				rb_set_red(grand_parent);
				node = grand_parent;
				continue;
			}

			if (parent->left == node) {
				RBTNode<T> *tmp;
				rightRotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			rb_set_black(parent);
			rb_set_red(grand_parent);
			leftRotate(root, grand_parent);
		}
	}

	rb_set_black(root);
}

template<class T>
inline void RBTree<T>::remove(RBTNode<T>*& root, RBTNode<T>* node) {
	RBTNode<T> *child, *parent;
	RBTColor color;

	// 删除节点左右子树同时不为空
	if ((node->left != NULL) && (node->right != NULL)) {
		RBTNode<T> *replace = node;

		replace = replace->right;
		while (replace->left != NULL)
			replace = replace->left;

		if (rb_parent(node)) {
			if (rb_parent(node)->left == node)
				rb_parent(node)->left = replace;
			else
				rb_parent(node)->right = replace;
		}
		else
			root = replace;

		child = replace->right;
		parent = rb_parent(replace);
		color = rb_color(replace);

		if (parent == node)
			parent = replace;
		else {
			if (child)
				rb_set_parent(child, parent);
			parent->left = child;

			replace->right = node->right;
			rb_set_parent(node->right, replace);
		}

		replace->parent = node->parent;
		replace->color = node->color;
		replace->left = node->left;
		node->left->parent = replace;

		if (color == BLACK)
			removeFixUp(root, child, parent);

		delete node;
		return;
	}

	// 删除节点左或右只有一个为空
	if (node->left != NULL)
		child = node->left;
	else
		child = node->right;

	// 这里包括了左右同时为空的情况，因为为空时会 = NULL
	parent = node->parent;
	color = node->color;

	if (child)
		child->parent = parent;

	if (parent) {
		if (parent->left == node)
			parent->left = child;
		else
			parent->right = child;
	}
	else
		root = child;

	if (color == BLACK)
		removeFixUp(root, child, parent);

	delete node;
}

// 平衡原理
// http://www.cnblogs.com/skywang12345/p/3245399.html
template<class T>
inline void RBTree<T>::removeFixUp(RBTNode<T>*& root, RBTNode<T>* node, RBTNode<T>* parent) {
	RBTNode<T> *other;

	while ((!node || rb_is_black(node)) && node != root)
	{
		// 先区分待平衡节点是左节点还是右节点
		if (parent->left == node)
		{
			other = parent->right;
			if (rb_is_red(other))
			{
				// Case 1: x的兄弟w是红色的  
				rb_set_black(other);
				rb_set_red(parent);
				leftRotate(root, parent);
				other = parent->right;
			}
			if ((!other->left || rb_is_black(other->left)) &&
				(!other->right || rb_is_black(other->right)))
			{
				// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}
			else
			{
				if (!other->right || rb_is_black(other->right))
				{
					// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
					rb_set_black(other->left);
					rb_set_red(other);
					rightRotate(root, other);
					other = parent->right;
				}
				// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->right);
				leftRotate(root, parent);
				node = root;
				break;
			}
		}
		else
		{
			other = parent->left;
			if (rb_is_red(other))
			{
				// Case 1: x的兄弟w是红色的  
				rb_set_black(other);
				rb_set_red(parent);
				rightRotate(root, parent);
				other = parent->left;
			}
			if ((!other->left || rb_is_black(other->left)) &&
				(!other->right || rb_is_black(other->right)))
			{
				// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}
			else
			{
				if (!other->left || rb_is_black(other->left))
				{
					// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
					rb_set_black(other->right);
					rb_set_red(other);
					leftRotate(root, other);
					other = parent->left;
				}
				// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->left);
				rightRotate(root, parent);
				node = root;
				break;
			}
		}
	}
	if (node)
		rb_set_black(node);
}

template<class T>
inline void RBTree<T>::destroy(RBTNode<T>*& tree) {
	if (tree == NULL)
		return;

	if (tree->left != NULL)
		return destroy(tree->left);
	if (tree->right != NULL)
		return destroy(tree->right);

	delete tree;
	tree = NULL;
}

/*
 * 打印"二叉查找树"
 *
 * key        -- 节点的键值
 * direction  --  0，表示该节点是根节点;
 *               -1，表示该节点是它的父结点的左孩子;
 *                1，表示该节点是它的父结点的右孩子。
 */
template<class T>
inline void RBTree<T>::print(RBTNode<T>* tree, T key, int direction) {
	if (tree != NULL) {
		if (direction == 0)
			// setw(int n)用来控制输出间隔
			std::cout << "  " << tree->key << "(B) is root" << std::endl;
		else
			std::cout << "  " << tree->key << (rb_is_red(tree) ? "(R)" : "(B)")
			<< " is " << "  " << key << "'s " << "            "
			<< (direction == 1 ? "right child" : "left child") << std::endl;
		
		print(tree->left, tree->key, -1);
		print(tree->right, tree->key, 1);
	}
}

template<class T>
inline T RBTree<T>::minimum() {
	RBTNode<T> *node = minimum(tree_root);
	if (node != NULL)
		return node->key;
	else
		return (T)NULL;
}

template<class T>
inline T RBTree<T>::maximum() {
	RBTNode<T> *node = maximum(tree_root);
	if (node != NULL)
		return node->key;
	else
		return (T)NULL;
}

template<class T>
inline RBTNode<T>* RBTree<T>::successor(RBTNode<T>* x) {
	if (x->right != NULL)
		return minimum(x->right);

	RBTNode<T> *y = x->parent;
	while ((y != NULL) && (x == y->right)) {
		x = y;
		y = y->parent;
	}
	return y;
}

template<class T>
inline RBTNode<T>* RBTree<T>::predecessor(RBTNode<T>* x) {
	if (x->left != NULL)
		return maximum(x->left);

	RBTNode<T> *y = x->parent;
	while (y != NULL&&x == y->left) {
		x = y;
		y = y->parent;
	}
	return y;
}

template<class T>
inline void RBTree<T>::insert(T key) {
	RBTNode<T> *z = NULL;
	if ((z = new RBTNode<T>(key, BLACK, NULL, NULL, NULL)) == NULL)
		return;
	insert(tree_root, z);
}

template<class T>
inline void RBTree<T>::remove(T key) {
	RBTNode<T> *node;
	if ((node = search(tree_root, key)) != NULL)
		remove(tree_root, node);
}

template<class T>
inline void RBTree<T>::destroy() {
	destroy(tree_root);
}

template<class T>
inline void RBTree<T>::print() {
	if (tree_root != NULL)
		print(tree_root, tree_root->key, 0);
}

template<class T>
inline void RBTree<T>::preOrder(RBTNode<T>* tree) const {
	if (tree != NULL) {
		std::cout << tree->key << " ";
		preOrder(tree->left);
		preOrder(tree->right);
	}
}

template<class T>
inline void RBTree<T>::inOrder(RBTNode<T>* tree) const {
	if (tree != NULL) {
		preOrder(tree->left);
		std::cout << tree->key << " ";
		preOrder(tree->right);
	}
}

template<class T>
inline void RBTree<T>::postOrder(RBTNode<T>* tree) const {
	if (tree != NULL) {
		preOrder(tree->left);
		preOrder(tree->right);
		std::cout << tree->key << " ";
	}
}

template<class T>
inline RBTNode<T>* RBTree<T>::search(RBTNode<T>* x, T key) const {
	if (x == NULL || x->key == key)
		return x;

	if (key < x->key)
		return search(x->left, key);
	else
		return search(x->right, key);
}

template<class T>
inline RBTNode<T>* RBTree<T>::iterativeSearch(RBTNode<T>* x, T key) const {
	while (x != NULL &&x->key != key) {
		if (key < x->key)
			x = x->left;
		else
			x = x->right;
	}

	return x;
}

template<class T>
inline RBTNode<T>* RBTree<T>::minimum(RBTNode<T>* tree) {
	if (tree == NULL)
		return NULL;

	while (tree->left != NULL)
		tree = tree->left;
	return tree;
}

template<class T>
inline RBTNode<T>* RBTree<T>::maximum(RBTNode<T>* tree) {
	if (tree == NULL)
		return NULL;

	while (tree->right != NULL)
		tree = tree->right;
	return tree;
}

template<class T>
inline RBTree<T>::RBTree():tree_root(NULL) {
	tree_root = NULL;
}

template<class T>
inline RBTree<T>::~RBTree() {
	destroy();
}

template<class T>
inline void RBTree<T>::preOrder() {
	preOrder(tree_root);
}

template<class T>
inline void RBTree<T>::inOrder() {
	inOrder(tree_root);
}

template<class T>
inline void RBTree<T>::postOrder() {
	postOrder(tree_root);
}

// 保留疑问
template<class T>
inline RBTNode<T>* RBTree<T>::search(T key) {
	return search(tree_root, key);
}

template<class T>
inline RBTNode<T>* RBTree<T>::iterativeSearch(T key) {
	return iterativeSearch(tree_root, key);
}
