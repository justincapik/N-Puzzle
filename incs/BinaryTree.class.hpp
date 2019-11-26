#ifndef BINARY_TREE_CLASS_HPP
# define BINARY_TREE_CLASS_HPP

# include <iostream>
using namespace std;



template <class T>
class BinaryTree
{
	private:
		struct _node
		{
			T data;
			struct _node *left;
			struct _node *right;
		};

		_node* _root;


	public:

	    BinaryTree();
	    ~BinaryTree();

		void freeNodes(_node* _node);
		void add(T var);
		void replace(_node *replaced);
		T	 isInTree(T var);
		void remove(T var);
};

#endif
