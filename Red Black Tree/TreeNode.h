#pragma once

enum RBTColor { RED, BLACK };

template <class T>
class RBTNode {
public:
	RBTColor color;    // ��ɫ
	T key;            // �ؼ���(��ֵ)
	RBTNode *left;    // ����
	RBTNode *right;    // �Һ���
	RBTNode *parent; // �����

	RBTNode(T value, RBTColor c, RBTNode *p, RBTNode *l, RBTNode *r) :
		key(value), color(c), parent(), left(l), right(r) {}
};

