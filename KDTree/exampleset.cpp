
#include <vector>
#include <assert.h>
#include <cmath>
#include <fstream>


// 单个数据向量定义定义
struct _Example
{
public:

    _Example() : dom_dims(0) { }

    _Example(const std::vector<double> elt, int dims)
    {
        if (dims > 0) {
            data = elt;
            dom_dims = dims;
        }
        else
            dom_dims = 0;
    }

    _Example(int dims)
    {
        if (dims > 0)
        {
            dom_dims = dims;
            data.resize(dims);
        }
        else
            dom_dims = 0;
    }

    _Example(const _Example & rhs)
    {
        if (rhs.dom_dims > 0)
        {
            dom_dims = rhs.dom_dims;
            data = rhs.data;
        }
        else
            dom_dims = 0;
    }

    _Example& operator=(const _Example & rhs)
    {
        if (this == &rhs)
            return (*this);

        releaseExample();

        if (rhs.dom_dims > 0)
        {
            dom_dims = rhs.dom_dims;
            data = rhs.data;
        }

        return *this;
    }

    ~_Example() { }


    double & dataAt(int dim)
    {
        assert(dim < dom_dims);
        return data[dim];
    }

    double & operator[](int dims)
    {
        return dataAt(dims);
    }

    const double & dataAt(int dims) const
    {
        assert(dims < dom_dims);
        return dataAt(dims);
    }

    const double & operator[](int dims) const
    {
        return dataAt(dims);
    }

    // 增加数组元素个数
    void create(int dims)
    {
        releaseExample();
        if (dims > 0)
        {
            dom_dims = dims;
            data.resize( dims);
        }
    }

    int getDomDims() const
    {
        return dom_dims;
    }

    void setTo(double val)
    {
        if (dom_dims > 0)
        {
            for (int i = 0; i < dom_dims; ++i)
                data[i] = val;
        }
    }

private:

    void releaseExample()
    {
        dom_dims = 0;
        data.clear();
    }

private:

    std::vector<double> data;  // 数据
    int dom_dims;  // 数组向量维度
};



// 距离计算
double Distance(const _Example & x, const _Example & y)
{
    double distance = 0.0;

    if (x.getDomDims() == y.getDomDims() && x.getDomDims() > 0 && y.getDomDims() > 0)
    {
        for (int i = 0; i < x.getDomDims(); ++i)
            distance += (x[i] - y[i]) * (x[i] - y[i]);
        distance = sqrt(distance);
    }

    return distance;
}



// 比较数据向量
class ExampleCompare
{
public:

    ExampleCompare(const int index) : _dim(index) { }

    bool operator()(const _Example & x, const _Example & y) const
    {
        return x[_dim] < y[_dim];
    }

private:

    int _dim;
};



// 数据范围内的超矩形结构
struct _HyperRectangle
{
    _Example min;  // 数据集中所有数据向量每个维度上最小值组成的数据向量
    _Example max;  // 数据集中所有数据向量每个维度上最大值组成的数据向量

    _HyperRectangle() { }

    _HyperRectangle(_Example mi, _Example mx)
    {
        assert(mx.getDomDims() == mi.getDomDims());
        min = mi;
        max = mx;
    }

    _HyperRectangle(const _HyperRectangle & rhs)
    {
        max = rhs.max;
        min = rhs.min;
    }

    _HyperRectangle & operator=(const _HyperRectangle & rhs)
    {
        if (this == &rhs)
            return *this;
        min = rhs.min;
        max = rhs.max;
        return *this;
    }

    void create(_Example mx, _Example mn)
    {
        assert(mx.getDomDims() == mn.getDomDims());
        min = mn;
        max = mx;
    }
};



class ExampleSet
{
private:

    std::vector<_Example> _set;
    int _size;
    int _dims;

public:

    ExampleSet() : _size(0), _dims(0) { }

    ExampleSet(std::vector<_Example> set, int size, int dims)
    {
        if (size > 0)
            _size = size;
        else
            _size = 0;
        if (dims > 0)
            _dims = dims;
        else
            _dims = 0;
        _set = set;
    }

    ExampleSet(int size, int dims)
    {
        if (size > 0)
            _size = size;
        else
            _size = 0;
        if (dims > 0)
            _dims = dims;
        else
            _dims = 0;
    }

    ExampleSet(const ExampleSet & rhs)
    {
        if (rhs._size > 0)
            _size = rhs._size;
        else
            _size = 0;
        if (rhs._dims > 0)
            _dims = rhs._dims;
        else
            _dims = 0;
        _set = rhs._set;
    }

    ExampleSet& operator=(const ExampleSet & rhs)
    {
        if (this == &rhs)
            return *this;
        releaseExampleSet();
        if (rhs._size > 0)
            _size = rhs._size;
        else
            _size = 0;
        if (rhs._dims > 0)
            _dims = rhs._dims;
        else
            _dims = 0;
        _set = rhs._set;
        return *this;
    }

    ~ExampleSet() { }

    _Example & exampleAt(int index)
    {
        assert(index < _size);
        return _set[index];
    }

    _Example & operator[](int index)
    {
        return exampleAt(index);
    }

    const _Example & exampleAt(int index) const
    {
        assert(index < _size);
        return _set[index];
    }

    void create(int size, int dims)
    {
        releaseExampleSet();
        if (size > 0 && dims > 0)
        {
            _set.resize(size);
            _size = size;
            _dims = dims;
            for (int i = 0; i < _size; ++i)
                _set[i].create(_dims);
        }
    }

    int getDims() const
    {
        return _dims;
    }

    int getSize() const
    {
        return _size;
    }

    // 计算超矩阵
    _HyperRectangle calculateRange()
    {
        assert(_size > 0 && _dims > 0);
        _Example _min(_dims);
        _Example _max(_dims);
        for (int i = 0; i < _dims; ++i)
        {
            _min.dataAt(i) = (*this)[0][i];
            _max.dataAt(i) = (*this)[0][i];
        }
        for (int i = 0; i < _size; ++i)
        {
            for (int j = 0; j < _dims; ++j)
            {
                if ((*this)[i][j] < _min[j])
                    _min[j] = (*this)[i][j];
                if ((*this)[i][j] > _max[j])
                    _max[j] = (*this)[i][j];
            }
        }
        _HyperRectangle rectangle(_max, _min);
        return rectangle;
    }

    bool empty() const
    {
        return (_size == 0);
    }

    // 按某个方向维排序函数
    void sortByDim(int dim)
    {
        ExampleCompare cmp(dim);
        std::sort(_set.begin(), _set.end(), cmp);
    }

    bool remove(int index)
    {
        if (index >= 0 && index < _size)
        {
            _set.erase(_set.begin() + index);
            _size -= 1;
            return true;
        }
        else
            return false;
    }

    void push_back(const _Example & ex)
    {
        _set.push_back(ex); _size += 1;
    }

    // 从文件中读取
    bool readData(char * strFilePath)
    {
        std::ifstream fin(strFilePath);
        if (fin != nullptr)
            return false;
        double temp;
        int row_id = 0, column_id;  // 行/列对应 (_size)/(_dims)

        while (!fin.eof())
        {
            for (column_id = 0; column_id < _dims; column_id++)
            {
                fin >> temp;
                (*this)[row_id][column_id] = temp;
            }
            row_id += 1;
            if (row_id == _size)
                break;
        }

        fin.close();
        return true;
    }

private:

    void releaseExampleSet()
    {
        _set.clear();
        _size = 0;
    }
};