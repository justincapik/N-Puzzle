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

	//cout << "[closed] " << endl;
	delete _node->data;
	delete _node;
}

template <class T>
void BinaryTree<T>::add(T var) {
	_node* newNode = new _node;
	newNode->data = var;
	newNode->left = NULL;
	newNode->right = NULL;

	//cout << "new _node with data = " << var->getHash() << " and adress = " << var << endl;
	if (!this->_root) {
		this->_root = newNode;
	}
	else {
		_node* parent = NULL;
		//cout << "1" << endl;
		_node *tree = this->_root;
		while (tree) {
			parent = tree;
			if (var->getHash() > tree->data->getHash())
				tree = tree->left;
			else
				tree = tree->right;
		}
		if (var->getHash() > parent->data->getHash())
			parent->left = newNode;
		else
			parent->right = newNode;
		//cout << "Placed" << endl;
	}
}

template <class T>
void BinaryTree<T>::replace(_node *replaced) {
	_node *tree = this->_root;
	while(tree) {
		if (replaced->data->getHash() > tree->data->getHash())
			tree = tree->left;
		else
			tree = tree->right;
	}
	tree = replaced;

}

template <class T>
T	BinaryTree<T>::isInTree(T var) {
	if (this->_root == NULL) {
		//cout << "not in tree, root doesn't exist." << endl;
		return nullptr;
	}
	else {
		_node *tree = this->_root;
		while (tree && tree->data) {
			if (var->getHash() == tree->data->getHash())
				return tree->data;
			else if (var->getHash() > tree->data->getHash())
				tree = tree->left;
			else
				tree = tree->right;
		}
	}
	//cout << "not in tree." << endl;
	return nullptr;
}

template <class T>
void BinaryTree<T>::remove(T var) {
	//cout << "REMOVED" << endl;
	_node *tree = this->_root;
	_node *parent;
	while (tree && var->getHash() != tree->data->getHash()) {
		parent = tree;
		if (var->getHash() > tree->data->getHash())
			tree = tree->left;
		else
			tree = tree->right;
	}
	if (!tree)
		return;

	if ((tree->left && !tree->right) || (tree->right && !tree->left))
	{
		if (parent->left == tree)
			parent->left = (tree->right) ? tree->right : tree->left;
		else
			parent->right = (tree->right) ? tree->right : tree->left;
	}
	else if (!tree->left && !tree->right) {
		if (parent->left == tree)
			parent->left = NULL;
		else
			parent->right = NULL;
	}
	else if (tree->right && tree->left) {
		if (parent->left == tree)
			parent->left = tree->right;
		else
			parent->right = tree->right;
		this->replace(tree->left);
	}

	//cout << "removed : " << tree << endl;

	delete tree->data;
	delete tree;
}
