#ifndef TEST_H__
#define TEST_H__

#include <vector>
#include <tuple>

struct Test
{
    std::tuple<int, char> t;
    std::vector<std::tuple<int, char>> vt;
};

#endif
