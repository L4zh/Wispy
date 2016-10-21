
#ifndef KDTREE_H_
#define KDTREE_H_

#include <vector>
#include <assert.h>
#include "k-d.cpp"


class KDTreeNode
{
private:

    int _split_dim;  // 分割超集
    _Example _data;  // 数据
    _HyperRectangle _rectangle;  // 超矩阵结构

public:

    KDTreeNode *_leftChild, *_rightChild, *_parent;  // 左右孩子父节点

public:

    KDTreeNode() : _leftChild(0), _rightChild(0), _parent(0), _split_dim(0) { }

    KDTreeNode(KDTreeNode *leftChild, KDTreeNode *rightChild, KDTreeNode *parent, int split_dim, _Example data, _HyperRectangle rectangle) :
            _leftChild(leftChild), _rightChild(rightChild), _parent(parent), _split_dim(split_dim), _data(data), _rectangle(rectangle) { }

    KDTreeNode(const KDTreeNode & rhs)
    {
        this->_leftChild = rhs._leftChild;
        this->_rightChild = rhs._rightChild;
        this->_parent = rhs._parent;
        this->_split_dim = rhs._split_dim;
        this->_data = rhs._data;
        this->_rectangle = rhs._rectangle;
    }

    KDTreeNode & operator=(const KDTreeNode & rhs)
    {
        if (this == &rhs)
            return *this;

        (*this)._split_dim = rhs._split_dim;
        (*this)._data = rhs._data;
        (*this)._rectangle = rhs._rectangle;
        (*this)._leftChild = rhs._leftChild;
        (*this)._rightChild = rhs._rightChild;
        (*this)._parent = rhs._parent;
        return *this;
    }

    int splitDim() { return _split_dim; }

    _Example getData() { return _data; }

    _HyperRectangle getHyperRectangle() { return _rectangle; }

    void create(KDTreeNode *leftChild, KDTreeNode *rightChidl, KDTreeNode *parent, int split_dim, _Example data, _HyperRectangle rectangle)
    {
        this->_leftChild = leftChild;
        this->_rightChild = rightChidl;
        this->_parent = parent;
        this->_split_dim = split_dim;
        this->_data = data;
        this->_rectangle = rectangle;
    }
};



class KDTree
{
public:

    KDTreeNode *_root;  // 根节点

public:

    KDTree() : _root(nullptr) { }

    KDTree(KDTreeNode * root) : _root(root) { }

    void create(const ExampleSet & exm_set);

    ~KDTree() { destroyKDTree(_root); }

    std::pair<_Example, double> findNearest(_Example target);  //查找最近邻近节点

    int findNearest(_Example target, double range, std::vector<std::pair<_Example, double>> &res_nearest);

private:

    KDTreeNode * createKDTree(const ExampleSet &exm_set);

    void destroyKDTree(KDTreeNode * root);

    std::pair<_Example, double> findNearest_i(KDTreeNode *root, _Example target);

    int findNearest_range(KDTreeNode * root, _Example target, double range,
                          std::vector<std::pair<_Example, double>> &res_nearest);
};

#endif