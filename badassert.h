#pragma once

#include <limits>
#include <cmath>

constexpr double dlim = std::pow(10, -(std::numeric_limits<double>::digits10 + 1l));

#ifndef NDEBUG
#include "src/badassert_meat.h"

#define BADAssert(A, OP, B, ...) \
    (((A) OP (B)) \
    ? static_cast<void>(0) \
    : badassert::fireAssert(A, B, #OP, #A, #B, \
    __FILE__, \
    __PRETTY_FUNCTION__, \
    __LINE__, ##__VA_ARGS__))

#else
#define BADAssert(...) static_cast<void>(0)
#endif


#define BADAssertEqual(A, B, ...) \
    BADAssert(A, ==, B, ##__VA_ARGS__)

#define BADAssertClose(A, B, lim, ...) \
    ((A > B) \
    ? BADAssert(A - B, <=, lim + dlim, ##__VA_ARGS__) \
    : BADAssert(B - A, <=, lim + dlim, ##__VA_ARGS__))

#define BADAssertBool(expr, ...) \
    BADAssertEqual((expr), true, ##__VA_ARGS__)

#define BADAssertBreak(...) \
    BADAssertBool(false, ##__VA_ARGS__)
