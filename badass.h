#pragma once

#include <limits>
#include <cmath>

inline double dlim()
{
    return std::pow(10, -(std::numeric_limits<double>::digits10 + 1l));
}
#include "src/badass_meat.h"

#ifndef NDEBUG

#define BADAss(A, OP, B, ...) \
    do { \
        const auto &_aV = A; \
        const auto &_bV = B; \
        badass::check(_aV, _bV, (_aV) OP (_bV), #OP, #A, #B, \
                      __FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while(0)

#define BADAssEqual(A, B, ...) \
    BADAss(A, ==, B, ##__VA_ARGS__)

#define BADAssClose(A, B, lim, ...) \
    BADAss(fabs(A - (B)), <=, lim + dlim(), ##__VA_ARGS__)

#define BADAssDiffer(A, B, lim, ...) \
    BADAss(fabs(A - (B)), >, lim + dlim(), ##__VA_ARGS__)

#define BADAssBool(expr, ...) \
    BADAssEqual((expr), true, ##__VA_ARGS__)

#define BADAssBreak(...) \
    BADAssBool(false, ##__VA_ARGS__)

#define BADAssSimpleDump(...) \
    badass::simpleDump(#__VA_ARGS__, ##__VA_ARGS__)

#else
#define BADAss(...) static_cast<void>(0)
#define BADAssEqual(A, B, ...) static_cast<void>(0)
#define BADAssClose(A, B, lim, ...) static_cast<void>(0)
#define BADAssDiffer(A, B, lim, ...) static_cast<void>(0)
#define BADAssBool(expr, ...) static_cast<void>(0)
#define BADAssBreak(...) static_cast<void>(0)
#define BADAssSimpleDump(...) static_cast<void>(0)
#endif


