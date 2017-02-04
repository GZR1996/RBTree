#pragma once

enum RBTColor { RED, BLACK };

template <class T>
class RBTNode {
public:
	RBTColor color;    // 颜色
	T key;            // 关键字(键值)
	RBTNode *left;    // 左孩子
	RBTNode *right;    // 右孩子
	RBTNode *parent; // 父结点

	RBTNode(T value, RBTColor c, RBTNode *p, RBTNode *l, RBTNode *r) :
		key(value), color(c), parent(), left(l), right(r) {}
};

