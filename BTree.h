#ifndef __BTREE_H__
#define __BTREE_H__

#include <iostream>
using namespace std;

template<class K,int M = 3>
struct BTreeNode
{
	K _key[M];                   //多放一个空间方便分裂
	BTreeNode<K, M>* _subs[M + 1];
	BTreeNode<K, M>* _parent;
	size_t _size;

	BTreeNode()
		:_size(0)
	{
		_parent = NULL;
		for (int i = 0; i <= M; i++)
		{
			_subs[i] = NULL;
		}
	}
};


template<class K,class V>
struct Pair
{
	K _first;
	V _second;

	Pair(const K& key, const V& value)
		:_first(key)
		, _second(value)
	{}
};

template<class K,int M = 3>
class BTree
{
	typedef BTreeNode<K, M> Node;
public:
	BTree()
		:_root(NULL)
	{}

	Pair<K, int> Find(const K& key)
	{
		Node* cur = _root;
		Node* parent = NULL;
		int index = 0;
		while (cur)
		{
			while (index < cur->_size)
			{
				if (cur->_key[index] < key)
					++index;
				else if (cur->_key[index] == key)
				{
					return Pair<K, M>(cur, index);
				}
				else
					break;
			}
			parent = cur;
			cur = cur->_subs[index];
			index = 0;
		}
		return Pair<K, M>(parent, -1);
	}

	bool Insert(const K& key)
	{
		//没有节点，直接修改root
		if (_root == NULL)
		{
			_root = new Node();
			_root->_key[0] = key; 
			_root->_size++;
			return true;
		}

		Pair<K, M> pair = Find(key);
		if (pair._second != -1)
		{
			return -1;//已经存在
		}
		Node* cur = pair._first;
		Node* parent = cur->_parent;
		K insertKey = key;

		while (1)
		{
			_InsertKey(cur,insertKey);//直接插入

			//小于M，无需分裂
			if (cur->_size < M)
			{
				return true;
			}

			//分裂调整
			int mid = (cur->_size - 1) / 2;//减1是为了防止M为偶数
			Node* right = new Node();
			int index = 0;
			//拷贝_key[]
			for (int i = mid + 1; i < cur->_size; i++)
			{
				right->_key[index++] = cur->_key[i];
				++right->_size;
			}
			index = 0;
			//拷贝subs[]
			for (int i = mid + 1; i < cur->_size; ++i)
			{
				right->_subs[index++] = cur->_subs[i];
				if (cur->_subs[i])
				{
					cur->_subs[i]->_parent = right;
				}
			}
			insertKey = cur->_key[mid];
			cur->_size = (cur->_size - 1) / 2;
			if (parent == NULL)//cur为根节点
			{
				Node* tmp = new Node();
				tmp->_key[0] = insertKey;
				++tmp->_size;
				tmp->_subs[0] = cur;
				tmp->_subs[1] = right;
				cur->_parent = right->_parent = tmp;
				_root = tmp;
				break;
			}
			else
			{
				right->_parent = parent;
				index = M - 1;
				while (parent->_subs[index] != cur)
				{
					parent->_subs[index + 1] = parent->_subs[index];
					--index;
				}

				parent->_subs[index + 1] = right;
				cur = parent;
				parent = cur->_parent;
			}
		}
		return true;
	}

	void _InsertKey(Node* cur, cosnt K& key)
	{
		//插入排序
		it index = cur->_size - 1;
		while (index >= 0 && key < cur->_key[index])
		{
			cur->_key[index + 1] = cur->_key[index];
			--index;
		}
		cur->_key[index + 1] = key;
		++cur->_size;
	}
protected:
	Node* _root;
};






#endif //__BTREE_H__