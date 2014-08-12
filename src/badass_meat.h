#pragma once

#include <functional>
#include <string>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <stdio.h>

using std::string;
using std::cerr;
using std::endl;

namespace badass
{

class BADAssException : public std::exception
{

public:
    BADAssException(const char * OP,
                    const char * A,
                    const char * B,
                    const char * file,
                    const char * func,
                    int line,
                    string what,
                    string message) :
        std::exception(),
        m_message(message),
        m_comparisonOperator(OP),
        m_leftHandSide(A),
        m_rightHandSide(B),
        m_whichFile(file),
        m_whichFunction(func),
        m_whichLine(line),
        m_description(what)
    {

    }

    const char *what() const throw ()
    {
        return m_message.c_str();
    }

    const string comparisonOperator() const
    {
        return m_comparisonOperator;
    }

    const string leftHandSide() const
    {
        return m_leftHandSide;
    }

    const string rightHandSide() const
    {
        return m_rightHandSide;
    }

    const string whichFile() const
    {
        return m_whichFile;
    }

    const string whichFunction() const
    {
        return m_whichFunction;
    }

    int whichLine() const
    {
        return m_whichLine;
    }

    const std::string description() const
    {
        return m_description;
    }

private:

    const string m_message;

    const string m_comparisonOperator;
    const string m_leftHandSide;
    const string m_rightHandSide;

    const string m_whichFile;
    const string m_whichFunction;
    const int m_whichLine;

    const string m_description;


};

inline void _searchRepl(string & _string, string _find, string _repl)
{

    int position = _string.find(_find);
    while (position != (int)string::npos)
    {
        _string.replace(position, _find.size(), _repl);
        position = _string.find(_find, position + 1);
    }
}

template<typename aT, typename bT>
inline
string
getAssertMessage(aT Aval,
                 bT Bval,
                 const char * OP,
                 const char * A,
                 const char * B,
                 const char * file,
                 const char * func,
                 int line,
                 string what)
{
    std::stringstream OP_B, assertMessage, assertCore;
    string OP_B_repl, A_repl;


    OP_B  << " " << OP << " " << B;

    OP_B_repl = OP_B.str();
    _searchRepl(OP_B_repl, " == true", "");


    A_repl    = string(A);
    _searchRepl(A_repl   , "false", "");


    assertMessage <<  file << ":" << line << ": " << func << ": ";

    assertCore << A_repl << OP_B_repl;

    if (!assertCore.str().empty())
    {

        assertMessage << "Assertion '" << assertCore.str() << "' failed: ";

        OP_B.str(string());

        OP_B << "!" << OP;

        OP_B_repl = OP_B.str();

        _searchRepl(OP_B_repl, "!==", "!=");
        _searchRepl(OP_B_repl, "!!=", "==");
        _searchRepl(OP_B_repl, "!>=",  "<");
        _searchRepl(OP_B_repl, "!<=",  ">");
        _searchRepl(OP_B_repl, "!>" , "<=");
        _searchRepl(OP_B_repl, "!<" , ">=");


        assertMessage << Aval << " " <<  OP_B_repl << " " << Bval;

    }

    else
    {
        assertMessage << "Assertion failed";
    }

    if (what.empty())
    {
        assertMessage << ".";
    }
    else
    {
        assertMessage << " : " << what;
    }

    return assertMessage.str();

}

template<class T>
struct checkEmptyArgs;

template<class Ret, class T, class... Args>
struct checkEmptyArgs<Ret(T::*)(Args...) const>
{
    constexpr static bool value = sizeof...(Args) == 0;
};

template<class T>
struct checkValidType;

template<class Ret, class T, class... Args>
struct checkValidType<Ret(T::*)(Args...) const>
{
    constexpr static bool value = std::is_same<const BADAssException &, Args...>::value ||
            std::is_same<const BADAssException, Args...>::value ||
            std::is_same<BADAssException, Args...>::value;
};

template<typename aT, typename bT, typename fT>
inline
typename std::enable_if<!checkEmptyArgs<decltype(&fT::operator())>::value, void>::type
fireAssert(aT Aval,
           bT Bval,
           const char * OP,
           const char * A,
           const char * B,
           const char * file,
           const char * func,
           int line,
           string what,
           const fT onFireFunc)
{
    static_assert(checkValidType<decltype(&fT::operator())>::value, "invalid argument type in assert fire function.");

    BADAssException exc(OP,
                        A,
                        B,
                        file,
                        func,
                        line,
                        what,
                        getAssertMessage(Aval, Bval, OP, A, B, file, func, line, what));

    onFireFunc(exc);

    throw exc;

}

template<typename aT, typename bT, typename fT>
inline
typename std::enable_if<checkEmptyArgs<decltype(&fT::operator())>::value, void>::type
fireAssert(aT Aval,
           bT Bval,
           const char * OP,
           const char * A,
           const char * B,
           const char * file,
           const char * func,
           int line,
           string what,
           const fT onFireFunc)
{
    fireAssert(Aval, Bval, OP, A, B, file, func, line, what, [&onFireFunc] (const BADAssException &exc)
    {
        (void) exc;
        onFireFunc();
    });
}

template<typename aT, typename bT>
inline
void
fireAssert(aT Aval,
           bT Bval,
           const char * OP,
           const char * A,
           const char * B,
           const char * file,
           const char * func,
           int line,
           string what = "")
{
    fireAssert(Aval, Bval, OP, A, B, file, func, line, what, [] () {});
}

}
