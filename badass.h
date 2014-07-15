#pragma once

#include <limits>
#include <cmath>

namespace badass {}
constexpr double dlim = std::pow(10, -(std::numeric_limits<double>::digits10 + 1l));

#ifndef NDEBUG
#include "src/badass_meat.h"

#define BADAss(A, OP, B, ...) \
    (((A) OP (B)) \
    ? static_cast<void>(0) \
    : badass::fireAssert(A, B, #OP, #A, #B, \
    __FILE__, \
    __PRETTY_FUNCTION__, \
    __LINE__, ##__VA_ARGS__))

#else
#define BADAss(...) static_cast<void>(0)
#endif


#define BADAssEqual(A, B, ...) \
    BADAss(A, ==, B, ##__VA_ARGS__)

#define BADAssClose(A, B, lim, ...) \
    ((A > B) \
    ? BADAss(A - B, <=, lim + dlim, ##__VA_ARGS__) \
    : BADAss(B - A, <=, lim + dlim, ##__VA_ARGS__))

#define BADAssBool(expr, ...) \
    BADAssEqual((expr), true, ##__VA_ARGS__)

#define BADAssBreak(...) \
    BADAssBool(false, ##__VA_ARGS__)
