#pragma once


#include <string>
#include <string.h>

#include <iostream>
#include <stdio.h>
#include <iomanip>

#include <sstream>

#include <functional>

#include <stdexcept>

#include <vector>


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

inline
void
_searchRepl(string & _string, string _find, string _repl)
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

//http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}



//http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
inline
std::vector<std::string> &
split(const std::string &s, char delim, std::vector<std::string> &elems, bool trimElements = false)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(trimElements? trim(item) : item);
    }
    return elems;
}

inline
std::vector<std::string>
split(const std::string &s, char delim, bool trimElements = false)
{
    std::vector<std::string> elems;
    split(s, delim, elems, trimElements);
    return elems;
}



template<typename T>
inline
void
_simpleDump_meat(const uint maxLength, const std::vector<string> &names, const uint level, const T &val)
{
    string name = names.at(level);
    cout << "    " << std::setw(maxLength) << std::left << name << " " << *val << endl;
}

template<typename T, typename ...Args>
inline
void
_simpleDump_meat(const uint maxLength, const std::vector<string> &names, const uint level, const T &val, const Args&... args)
{
    _simpleDump_meat(maxLength, names, level, val);
    _simpleDump_meat(maxLength, names, level + 1, args...);
}

template<typename ...Args>
inline
void
simpleDump(const char* concatenatedNamesCStr, const Args&... args)
{
    string concatenatedNames = string(concatenatedNamesCStr);
    std::vector<string> names = split(concatenatedNames, ',', true);

    if (names.empty()) return;

    uint maxLength = names.at(0).size();

    for (uint i = 1; i < names.size(); ++i)
    {
        if (names.at(i).size() > maxLength)
        {
            maxLength = names.at(i).size();
        }
    }

    cout << std::setw(maxLength/2) << std::setfill('-') << std::right
         << " " << "BADAss::simpleDump"
         << std::setw(maxLength/2) << std::setfill('-') << std::left << " " << std::setfill(' ') << endl;

    _simpleDump_meat(maxLength, names, 0, &args...);

    cout << std::setw(maxLength + 18) << std::setfill('-') << "" << std::setfill(' ') << endl;

}



}
