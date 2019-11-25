#include "BinaryTree.class.hpp"
#include <iostream>
using namespace std;

template <class T>
BinaryTree<T>::BinaryTree() {
	this->_root = NULL;
}
template <class T>
BinaryTree<T>::~BinaryTree() {
	freeNodes(this->_root);
}

template <class T>
void BinaryTree<T>::freeNodes(_node* _node) {
	if (!_node)
		return;

	freeNodes(_node->left);
	freeNodes(_node->right);

	free(_node);
}

template <class T>
void BinaryTree<T>::add(T var) {
	_node* newNode = new _node;
	newNode->data = &var;
	newNode->left = NULL;
	newNode->right = NULL;

	cout << "new _node with data = " << var->getHash() << endl;
	if (!this->_root) {
		cout << "root don't exist " << endl;
		this->_root = newNode;
	}
	else {
		_node* parent = NULL;
		cout << "1" << endl;
		_node *tree = this->_root;
		while (tree) {
			cout << "check : " << (*tree->data)->getHash() << endl;
			parent = tree;
			if (var->getHash() > (*tree->data)->getHash())
				tree = tree->left;
			else
				tree = tree->right;
			if (tree)
				cout << "go to : " << (*tree->data)->getHash() << endl;
		}
		if (var->getHash() > (*parent->data)->getHash())
			parent->left = newNode;
		else
			parent->right = newNode;
		cout << "Placed" << endl;
	}
}

template <class T>
void BinaryTree<T>::replace(_node *replaced) {
	_node *tree = this->_root;
	while(tree) {
		if ((*replaced->data)->getHash() > (*tree->data)->getHash())
			tree = tree->left;
		else
			tree = tree->right;
	}
	tree = replaced;

}

template <class T>
T	BinaryTree<T>::isInTree(T var) {
	if (this->_root == NULL)
		return nullptr;
	else {
		_node *tree = this->_root;
		while (tree && (*tree->data)) {
			if (var->getHash() == (*tree->data)->getHash())
				return *(tree->data);
			else if (var->getHash() > (*tree->data)->getHash())
				tree = tree->left;
			else
				tree = tree->right;
		}
	}
	return nullptr;
}

template <class T>
void BinaryTree<T>::remove(T var) {
	_node *tree = this->_root;
	_node *parent;
	while (var->getHash() != (*tree->data)->getHash()) {
		parent = tree;
		if (var->getHash() > (*tree->data)->getHash())
			tree = tree->left;
		else
			tree = tree->right;
	}

	_node *tmp = new _node;

	tmp->data = tree->data;
	tmp->left = tree->left;
	tmp->right = tree->right;

	delete tree;

	if (tmp->left) {
		cout << "3" << endl;
		tree = tmp->left;

		if (tmp->right)
			replace(tmp->right);
	}
	else if (tmp->right) {
		tree = tmp->right;
	}
	else
		tree = NULL;

	if (this->_root->data == tmp->data)
		this->_root = tree;

	delete tmp;

}
