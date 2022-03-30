#include "rbtree.h"
#include<iostream>
#include<stack>
#include<random>



void RBTree::leftRotate(RBNode* x)
{
	RBNode* y = x->right;
	x->right = y->left;
	if (y->left != nullptr)
	{
		y->left->p = x;
	}
	y->p = x->p;
	if (x->p == nullptr)
		root = y;
	else if (x == x->p->left)
		x->p->left = y;
	else x->p->right = y;
	y->left = x;
	x->p = y;
	return;
}

void RBTree::rightRotate(RBNode* y)
{
	RBNode* x = y->left;
	y->left = x->right;
	if (x->right != nullptr)
	{
		x->right->p = y;
	}
	x->p = y->p;
	if (y->p == nullptr)
	{
		root = x;
	}
	else if (y == x->p->left)
		y->p->left = x;
	else y->p->right = x;
	x->right = y;
	y->p = x;
	return;
}


void RBTree::insertFixup(RBNode* node)
{
	while (node->p && node->p->color == RED)
	{
		RBNode* pp = node->p->p;
		RBNode* y = nullptr;
		if (node->p == pp->left) //left
		{
			y = pp->right;
			if (y && y->color == RED)
			{
				node->p->color = BLACK;
				y->color = BLACK;
				pp->color = RED;
				node = pp;
				continue;
			}
			else if (node == node->p->right)
			{
				node = node->p;
				leftRotate(node);
			}
			node->p->color = BLACK;
			pp->color = RED;
			rightRotate(pp);
		}
		else if (node->p == pp->right)
		{
			y = pp->left;
			if (y && y->color == RED)
			{
				node->p->color = BLACK;
				y->color = BLACK;
				pp->color = RED;
				node = pp;
				continue;
			}
			else if (node == node->p->left)
			{
				node = node->p;
				rightRotate(node);
			}
			node->p->color = BLACK;
			pp->color = RED;
			leftRotate(pp);
		}
	}
	root->color = BLACK;
}

void RBTree::insertNode(RBNode* node)
{
	RBNode* x = root;
	RBNode* y = nullptr;   //x's parent
	while (x != nullptr)
	{
		y = x;
		if (node->key < x->key) x = x->left;
		else if (node->key > x->key) x = x->right;
		else
		{
			delete(node);
			return;
		}
	}
	node->p = y;

	if (y == nullptr)
	{
		root = node;
		medianNode = root;
	}
	
	else if (node->key < y->key)
		y->left = node;
	else y->right = node;

	/* set in insert()
	node->left = nullptr;
	node->right = nullptr;
	node->color = RED;
	*/
	insertFixup(node);
	if (medianNode == node) return;

	offset +=( medianNode->key < node->key )? 1 : -1;
	if (offset == 2)
	{
		medianNode = successor(medianNode);
		offset = 0;
	}
	else if (offset == -1 )
	{
		medianNode = predecessor(medianNode);
		offset = 1;
	}	
}

void RBTree::transplant(RBNode* n1, RBNode* n2)
{
	if (nullptr == n1->p)
		root = n2;
	else if (n1->p->left == n1)
		n1->p->left = n2;
	else n1->p->right = n2;

	if (n2) n2->p = n1->p;

}

void RBTree::deleteFixup(RBNode* node, RBNode* parent)
{
	RBNode* ptr = nullptr;
	while (node != root && (!node || node->color == BLACK))
	{
		if (node == parent->left)
		{
			ptr = parent->right;
			if (ptr && ptr->color == RED)
			{
				ptr->color = BLACK;
				parent->color = RED;
				leftRotate(parent);
				ptr = parent->right;
			}
			if (ptr && (!ptr->left || ptr->left->color == BLACK) && (!ptr->right || ptr->right->color == BLACK))
			{
				ptr->color = RED;
				node = parent;
				parent = node->p;
			}
			else
			{
				if (ptr && (!ptr->right || ptr->right->color == BLACK) )
				{
					ptr->left->color = BLACK;
					ptr->color = RED;
					rightRotate(ptr);
					ptr = parent->right;
				}
				if(ptr) ptr->color = parent->color;
				parent->color = BLACK;
				if (ptr && ptr->right) ptr->right->color = BLACK;
				leftRotate(parent);
				node = root;
			}
		}
		else if (node == parent->right)
		{
			ptr = parent->left;
			if (ptr && ptr->color == RED)
			{
				ptr->color = BLACK;
				parent->color = RED;
				rightRotate(parent);
				ptr = parent->left;
			}
			if (ptr &&(!ptr->left || ptr->left->color == BLACK) && (!ptr->right || ptr->right->color == BLACK))
			{
				ptr->color = RED;
				node = parent;
				parent = node->p;
			}
			else
			{
				if (ptr && (!ptr->left || ptr->left->color == BLACK))
				{
					ptr->right->color = BLACK;
					ptr->color = RED;
					leftRotate(ptr);
					ptr = parent->left;
				}
				if(ptr) ptr->color = parent->color;
				parent->color = BLACK;
				if (ptr && ptr->left) ptr->left->color = BLACK;
				rightRotate(parent);
				node = root;
			}
		}
	}
	if (node) node->color = BLACK;

}

void RBTree::deleteNode(RBNode* node)
{
	if (!node) return;

	/* update median */
	if (node == medianNode)
	{
		if(offset > 0)
		{
			medianNode = successor(medianNode);
			offset = 0;
		}
		else if (offset == 0)
		{
			medianNode = predecessor(medianNode);
			offset = 1;

			if (medianNode == nullptr) offset = 0;
		}
	}
	else if (medianNode->key < node->key)
	{
		if (offset > 0) offset = 0;
		else if (offset == 0)
		{
			offset = 1;
			medianNode = predecessor(medianNode);
		}
	}
	else if (medianNode->key > node->key)
	{
		if (offset > 0)
		{
			medianNode = successor(medianNode);
			offset = 0;
		}
		else if (offset == 0) offset = 1;

	}


	RBNode* child = nullptr;  //record the child node that replaces the deleted node
	RBNode* y = node;
	RBNode* parent = node->p;   //record x's parent, in case that x is NULL
	color_type yOldColor = y->color;
	if (node->left == nullptr) //left null, don't change color
	{
		child = node->right;
		transplant(node, child);
	}
	else if (node->right == nullptr)
	{
		child = node->left;
		transplant(node, child);
	}
	else
	{
		RBNode *y = minNodeSubtree(node->right);
		if (node->p)
		{
			if (node->p && node->p->left == node)
				node->p->left = y;
			else node->p->right = y;
		}
		else root = y;

		child = y->right;
		parent = y->p;
		yOldColor = y->color;
		if (parent == node) parent = y;
		else
		{
			if (child)
				child->p = parent;
			parent->left = child;

			y->right = node->right;
			node->right->p = y;
		}

		y->p = node->p;
		y->left = node->left;
		y->left->p = y;
		y->color = node->color;
	}

	delete(node);  //free memory
	if (yOldColor == BLACK)
	{
		deleteFixup(child, parent);
	}


}

void RBTree::insert(key_type key)
{
	RBNode* node = new RBNode(RED, key); //RED
	if (node == nullptr)
	{
		cout << "allocate fail in" << __FUNCTION__ << endl;
		return;
	}

	insertNode(node);

}

void RBTree::deleteKey(key_type val)
{
	RBNode* node = search(val);
	if (node)
	{
		deleteNode(node);
	}

}

key_type RBTree::median()
{
	return medianNode->key;
}



vector<RBNode*> RBTree::inOrder()
{
	vector<RBNode*> ret;
	if (root == nullptr) return ret;
	RBNode* node = root;
	stack<RBNode*> stk;
	while (!stk.empty() || node)
	{
		while (node)
		{
			stk.push(node);
			node = node->left;
		}
		RBNode* ptr = stk.top();
		stk.pop();
		ret.push_back(ptr);
		node = ptr->right;
	}
	return ret;
}

void RBTree::printTree(vector<RBNode*> vec)
{
	for (auto c : vec)
	{
		cout << "key:" << c->key << "  color:" << ((c->color == RED) ? "RED" : "BLACK") << "  parent:";
		if (c->p) cout << c->p->key << "  left:";
		else cout << "NIL" << "  left:";
		if (c->left) cout << c->left->key << "  right:";
		else cout << "NIL" << "  right:";
		if (c->right) cout << c->right->key << endl;
		else cout << "NIL" << endl;
	}
}

void RBTree::traverse()
{
	vector<RBNode*> res = inOrder();
	printTree(res);
}

vector<key_type> RBTree::inOrdertraverse()
{
	vector<key_type> ret;
	if (root == nullptr) return ret;
	RBNode* node = root;
	stack<RBNode*> stk;
	while (!stk.empty() || node)
	{
		while (node)
		{
			stk.push(node);
			node = node->left;
		}
		RBNode* ptr = stk.top();
		stk.pop();
		ret.push_back(ptr->key);
		node = ptr->right;
	}
	return ret;
}

RBNode* RBTree::search(key_type val)
{
	RBNode* node = root;
	while (node && node->key != val)
	{
		if (node->key > val) node = node->left;
		else node = node->right;
	}
	return node;
}



RBNode* RBTree::minNodeSubtree(RBNode* node)
{
	if (!node) return nullptr;
	while (node->left)
	{
		node = node->left;
	}
	return node;
}

RBNode* RBTree::maxNodeSubtree(RBNode* node)
{
	if (!node) return nullptr;
	while (node->right)
	{
		node = node->right;
	}
	return node;
}

RBNode* RBTree::successor(RBNode* node)
{
	if (node->right)
		return minNodeSubtree(node->right);
	RBNode* y = node->p;
	while (y && node == y->right)
	{
		node = y;
		y = y->p;
	}
	return y;
}

RBNode* RBTree::predecessor(RBNode* node)
{
	if (node->left)
		return maxNodeSubtree(node->left);
	RBNode* y = node->p;
	while (y && node == y->left)
	{
		node = y;
		y = y->p;
	}
	return y;
}



vector<key_type> intersection(RBTree t1, RBTree t2)
{
	vector<key_type> vec1 = t1.inOrdertraverse();
	vector<key_type> vec2 = t2.inOrdertraverse();
	vector<key_type> ret;

	int ptr1 = 0, ptr2 = 0;
	int m = vec1.size();
	int n = vec2.size();
	while (ptr1 < m && ptr2 < n)
	{
		if (vec1[ptr1] == vec2[ptr2])
		{
			ret.push_back(vec1[ptr1]);
			ptr1++;
			ptr2++;
		}
		else if (vec1[ptr1] < vec2[ptr2])  ptr1++;
		else ptr2++;
	}

	return ret;
}





int main()
{
	RBTree ptr;

	/* insert and traverse */
	static default_random_engine e;
	static uniform_int_distribution<unsigned> u(0, 400);
	vector<int> vec(10, 0);
	for (int i = 0; i < 10; i++)
	{
		vec[i] = u(e);
		ptr.insert(vec[i]);
	}
	cout << "After insert:" << endl;
	ptr.traverse();
	cout << "median:   "<<ptr.median() << endl;
	
	/* delete and traverse*/
	for (int i = 0; i < 10; i += 2)
	{
		ptr.deleteKey(vec[i]);
	}
	cout << endl << "After delete:" << endl;
	ptr.traverse();
	cout << "median:   " << ptr.median() << endl;

	/* min, max*/
	cout <<endl<< "min: "<<ptr.min()->key << " max: " << ptr.max()->key << endl;
	
	/* search vec[1]*/
	cout << endl << "search: " << ptr.search(vec[1])->key << endl;

	/* median */
	RBTree ptr2;
	ptr2.insert(11);
	ptr2.insert(27);
	ptr2.insert(33);
	cout << endl;
	ptr2.traverse();
	cout << "median: "<< ptr2.median() << endl;
	ptr2.insert(5); 
	ptr2.insert(1);
	cout << endl;
	ptr2.traverse();
	cout << "median: " << ptr2.median() << endl;
	ptr2.insert(100);
	ptr2.insert(101);
	ptr2.insert(103);
	ptr2.deleteKey(103);
	ptr2.deleteKey(101);
	cout << endl;
	ptr2.traverse();
	cout << "median:   " << ptr2.median() << endl;
	ptr2.deleteKey(100);
	ptr2.deleteKey(1);
	cout << endl;
	ptr2.traverse();
	cout << "median:   " << ptr2.median() << endl;
	ptr2.deleteKey(5);
	ptr2.deleteKey(33);
	cout << endl;
	ptr2.traverse();
	cout << "median:   " << ptr2.median() << endl;


	/* intersection*/
	cout << endl;
	RBTree t1, t2;
	for (int i = 1; i <= 10; i++)	t1.insert(i);
	for (int i = 7; i <= 20; i++)	t2.insert(i);
	vector<int> result = intersection(t1, t2);
	cout << "Same key: ";
	for (auto i : result) cout << i<<"  ";
	cout << endl;

	
	return 1;
}
