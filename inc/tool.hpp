#ifndef TOOL_H
#define TOOL_H

#include <limits>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

namespace tool {

template <class T>
bool is_zero(const T&);

}

#include "../src/tool.inl"
#endif  // TOOL_H