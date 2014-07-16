#pragma once

#include <functional>
#include <string>
#include <stdexcept>

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

    const string comparisonOperator() const;

    const string leftHandSide() const;

    const string rightHandSide() const;

    const string whichFile() const;

    const string whichFunction() const;

    int whichLine() const;

    string description() const;

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

typedef std::function<void(const BADAssException &exc)> assertFireFuncType;

void _searchRepl(string & _string, string _find, string _repl)
{

    int position = _string.find(_find);
    while (position != (int)string::npos)
    {
        _string.replace(position, _find.size(), _repl);
        position = _string.find(_find, position + 1);
    }
}

template<typename aT, typename bT>
static string getAssertMessage(aT Aval,
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

template<typename aT, typename bT>
void
fireAssert(aT Aval,
           bT Bval,
           const char * OP,
           const char * A,
           const char * B,
           const char * file,
           const char * func,
           int line,
           string what,
           const assertFireFuncType &onFireFunc)
{

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

template<typename aT, typename bT>
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
    fireAssert(Aval, Bval, OP, A, B, file, func, line, what, [] (const BADAssException &exc) {(void) exc;});
}


const std::string BADAssException::comparisonOperator() const
{
    return m_comparisonOperator;
}

const std::string BADAssException::leftHandSide() const
{
    return m_leftHandSide;
}

const std::string BADAssException::rightHandSide() const
{
    return m_rightHandSide;
}

const std::string BADAssException::whichFile() const
{
    return m_whichFile;
}

const std::string BADAssException::whichFunction() const
{
    return m_whichFunction;
}

int BADAssException::whichLine() const
{
    return m_whichLine;
}

string BADAssException::description() const
{
    return m_description;
}


}
