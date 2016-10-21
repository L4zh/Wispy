
#include "kdtree.h"
#include <vector>
#include <assert.h>


void KDTree::create(const ExampleSet &exm_set)
{
    _root = createKDTree(exm_set);
}


KDTreeNode* KDTree::createKDTree(const ExampleSet &exm_set)
{
    if (exm_set.empty())
        return nullptr;

    ExampleSet exm_set_copy(exm_set);
    int dims = exm_set_copy.getDims();
    int size = exm_set_copy.getSize();

    // 计算每个维的方差,选出方差值最大的维
    double var_max = -0.1;
    double avg, var;
    int dim_max_var = -1;

    for (int i = 0; i < dims; i++)
    {
        avg = 0.0;
        var = 0.0;
        for (int j = 0; j < size; ++j)
        {
            avg += exm_set_copy[i][j];
        }
        avg /= size;
        for (int j = 0; j < size; ++j)
        {
            var += (exm_set_copy[i][j] - avg) * (exm_set_copy[i][j]);
        }
        var /= size;
        if(var > var_max)
        {
            var_max = var;
            dim_max_var = i;
        }
    }

    // 确定节点的数据矢量
    _HyperRectangle hr = exm_set_copy.calculateRange();
    exm_set_copy.sortByDim(dim_max_var);
    int mid = size / 2;
    _Example exm_split = exm_set_copy.exampleAt(mid);
    exm_set_copy.remove(mid);

    ExampleSet exm_set_left(0, exm_set_copy.getDims());
    ExampleSet exm_set_right(0, exm_set_copy.getDims());
    exm_set_right.remove(0);

    int size_new = exm_set_copy.getSize();

    for (int i = 0; i < size_new; i++)
    {
        _Example temp = exm_set_copy[i];
        if (temp.dataAt(dim_max_var) < exm_split.dataAt(dim_max_var))
            exm_set_left.push_back(temp);
        else
            exm_set_right.push_back(temp);
    }

    KDTreeNode *pNewNode = new KDTreeNode(0, 0, 0, dim_max_var, exm_split, hr);
    pNewNode->_leftChild = createKDTree(exm_set_left);
    if (pNewNode->_leftChild != nullptr)
        pNewNode->_leftChild->_parent = pNewNode;
    pNewNode->_rightChild = createKDTree(exm_set_right);
    if (pNewNode->_rightChild != nullptr)
        pNewNode->_rightChild->_parent = pNewNode;

    return pNewNode;
}


void KDTree::destroyKDTree(KDTreeNode *root)
{
    if (root != nullptr)
    {
        destroyKDTree(root->_leftChild);
        destroyKDTree(root->_rightChild);
        delete root;
    }
}


std::pair<_Example, double> KDTree::findNearest_i(KDTreeNode *root, _Example target)
{
    KDTreeNode * pSearch = root;
    std::vector<KDTreeNode*> search_path;  // 保存搜索路径
    _Example nearest;
    double max_dist;

    while (pSearch != nullptr)
    {
        search_path.push_back(pSearch);
        int s = pSearch->splitDim();
        if (target[s] <= pSearch->getData()[s])
        {
            pSearch = pSearch->_leftChild;
        }
        else
        {
            pSearch = pSearch->_rightChild;
        }
    }

    nearest = search_path.back()->getData();
    max_dist = Distance(nearest, target);

    search_path.pop_back();

    // 回溯搜索路径
    while (!search_path.empty())
    {
        KDTreeNode *pBack = search_path.back();
        search_path.pop_back();
        if (pBack->_leftChild == nullptr && pBack->_rightChild == nullptr)
        {
            if (Distance(nearest, target) > Distance(pBack->getData(), target))
            {
                nearest = pBack->getData();
                max_dist = Distance(pBack->getData(), target);
            }
        }
        else
        {
            int s = pBack->splitDim();
            if (abs(static_cast<int>(pBack->getData()[s]) - static_cast<int>(target[s])) < max_dist)
            {
                if (Distance(nearest, target) > Distance(pBack->getData(), target))
                {
                    nearest = pBack->getData();
                    max_dist = Distance(pBack->getData(), target);
                }
                if (target[s] <= pBack->getData()[s])
                    pSearch = pBack->_rightChild;
                else
                    pSearch = pBack->_leftChild;
                if (pSearch != nullptr)
                    search_path.push_back(pSearch);
            }
        }
    }

    std::pair<_Example, double> result(nearest, max_dist);
    return result;
}


std::pair<_Example, double> KDTree::findNearest(_Example target)
{
    std::pair<_Example, double> result;

    if (_root == NULL)
    {
        result.second = std::numeric_limits<double>::infinity();
        return result;
    }
    else
        return findNearest_i(_root, target);
}


int KDTree::findNearest(_Example target, double range, std::vector<std::pair<_Example, double>> &res_nearest)
{
    return findNearest_range(_root, target, range, res_nearest);
}


int KDTree::findNearest_range(KDTreeNode *root, _Example target, double range,
                                      std::vector<std::pair<_Example, double>> &res_nearest)
{
    if (root == NULL)
        return 0;

    double dist_sq, dx;
    int ret, added_res = 0;
    dist_sq = 0;
    dist_sq = Distance(root->getData(), target);

    if (dist_sq <= range)
    {
        std::pair<_Example, double> temp(root->getData(), dist_sq);
        res_nearest.push_back(temp);
        added_res = 1;
    }

    dx = target[root->splitDim()] - root->getData()[root->splitDim()];

    // 左子树或右子树递归的查找
    ret = findNearest_range(dx <= 0.0 ? root->_leftChild : root->_rightChild, target, range, res_nearest);

    if (ret >= 0 && fabs(dx) < range)
    {
        added_res += ret;
        ret = findNearest_range(dx <= 0.0 ? root->_rightChild : root->_leftChild, target, range, res_nearest);
    }

    added_res += ret;
    return added_res;
}