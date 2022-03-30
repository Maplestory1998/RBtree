#pragma once
#ifndef RBTREE_H
#define RBTREE_H
#include<vector>

using namespace std;
using key_type = int; //default 

/* RED: 0*/
using color_type = bool;

const color_type RED = false;
const color_type BLACK = true;

struct RBNode
{
	color_type color;
	key_type key;
	RBNode* p;
	RBNode* left, * right;

	RBNode(color_type c, key_type k, RBNode* parent = nullptr, RBNode* l = nullptr, RBNode* r = nullptr) :
		color(c), key(k), p(parent), left(l), right(r) {}
};

class RBTree
{
private:
	RBNode* root = nullptr;
	RBNode* medianNode = nullptr;
	int offset = 0;

	void leftRotate(RBNode* x);
	void rightRotate(RBNode* y);

	void insertFixup(RBNode* node);
	void insertNode(RBNode* node);

	void transplant(RBNode* n1, RBNode* n2);
	void deleteFixup(RBNode* node, RBNode* parent);
	void deleteNode(RBNode* node);

	//minNode of subtree
	RBNode* minNodeSubtree(RBNode* node);

	//maxNode of subtree
	RBNode* maxNodeSubtree(RBNode* node);

	//successor
	RBNode* successor(RBNode* node);

	//predeessor
	RBNode* predecessor(RBNode* node);

	//inorder traverse implemented by stack
	vector<RBNode*> inOrder();

	void printTree(vector<RBNode*>);
public:
	//inorder traverse
	void traverse();

	//only for intersection.
	vector<key_type> inOrdertraverse();

	//search
	RBNode* search(key_type val);

	//insert
	void insert(key_type val);

	//delete
	void deleteKey(key_type val);

	//min Node of tree
	inline RBNode* min()
	{
		return minNodeSubtree(root);
	}

	//max Node of tree;
	inline RBNode* max()
	{
		return maxNodeSubtree(root);
	}

	//median
	key_type median();

	int retoffset()
	{
		return offset;
	}
};

vector<key_type> intersection(RBTree t1, RBTree t2);



#endif // !RBTREE_H

