#include <iostream>

#include <unittest++/UnitTest++.h>

#include "../badass.h"

using namespace std;
using namespace badass;

#define GETLINE() __LINE__
#define GETFILE() __FILE__
#define ADD_TEST_SUCCESS() CHECK_EQUAL(1, 1)
#define ADD_TEST_FAILURE() CHECK_EQUAL(1, 0)

struct TEST_COUT
{
    int m_v;

    TEST_COUT(int v)
    {
        m_v = v;
    }

    bool operator == (const TEST_COUT &other) const
    {
        return m_v == other.m_v;
    }
};

struct TEST_COUT_STREAM
{
    int m_v;

    TEST_COUT_STREAM(int v)
    {
        m_v = v;
    }

    bool operator == (const TEST_COUT &other) const
    {
        return m_v == other.m_v;
    }
};


ostream &operator << (ostream &os, const TEST_COUT_STREAM &cs)
{
    os << cs.m_v;

    return os;
}

TEST(coutable)
{
    CHECK(is_coutable<TEST_COUT>::value == false);
    CHECK(is_coutable<int>::value == true);
    CHECK(is_coutable<double>::value == true);
    CHECK(is_coutable<char*>::value == true);
    CHECK(is_coutable<string>::value == true);
    CHECK(is_coutable<TEST_COUT_STREAM>::value == true);

    try
    {
        int one = 0;
        BADAss(one, ==, 1);
    }
    catch (const BADAssException &exc)
    {
        cout << "Does contain     0 != 1? " << exc.message() << endl;
    }

    try
    {
        auto t1 = TEST_COUT(1);
        auto t2 = TEST_COUT(2);

        BADAss(t1, ==, t2);
    }
    catch (const BADAssException &exc)
    {
        cout << "Does not contain 0 != 1? " << exc.message() << endl;
    }
}

TEST(ExceptionContents)
{
    CHECK_EQUAL(numeric_limits<double>::digits10, round(-log10(dlim())) - 1);

    try
    {
        int one = 0;
        BADAss(one, ==, 1, "One should be 1!", [&] (const BADAssException &exc)
        {
            CHECK_EQUAL("one", exc.leftHandSide());
            CHECK_EQUAL(1, atoi(exc.rightHandSide().c_str()));
            CHECK_EQUAL("==", exc.comparisonOperator());

            CHECK_EQUAL(GETFILE(), exc.whichFile());
            CHECK_EQUAL("virtual void TestExceptionContents::RunImpl() const", exc.whichFunction());
            CHECK_EQUAL(GETLINE(), exc.whichLine());
            CHECK_EQUAL("One should be 1!", exc.description());

        });
    }
    catch (const BADAssException &exc)
    {

    }

}

TEST(SimpleMode)
{
    int a = 0;
    try
    {
        BADAss(1, ==, 0, "fail", [&a] ()
        {
            a = 1;
        });
    }
    catch(const BADAssException &exc)
    {
        CHECK_EQUAL("fail", exc.description());
        CHECK_EQUAL(1, a);
    }
}

TEST(RandomCases)
{
    try
    {
        BADAss(1, ==, 0);
        ADD_TEST_FAILURE();
    }
    catch(const BADAssException &exc)
    {
        ADD_TEST_SUCCESS();
    }


    try
    {
        BADAss(1, ==, 0, "testing testing");
        ADD_TEST_FAILURE();
    }
    catch(const BADAssException &exc)
    {
        CHECK_EQUAL("testing testing", exc.description());
    }

    try
    {
        BADAss(0, ==, 0);
        ADD_TEST_SUCCESS();
    }
    catch(const BADAssException &exc)
    {
        ADD_TEST_FAILURE();
    }


    try
    {
        BADAssEqual(0, true);
        ADD_TEST_FAILURE();
    }
    catch(const BADAssException &exc)
    {
        ADD_TEST_SUCCESS();
        CHECK_EQUAL("true", exc.rightHandSide());
        CHECK_EQUAL("==", exc.comparisonOperator());
    }

    try
    {
        BADAssEqual(1, 0);
        ADD_TEST_FAILURE();
    }
    catch(const BADAssException &exc)
    {
        ADD_TEST_SUCCESS();
        CHECK_EQUAL("1", exc.leftHandSide());
        CHECK_EQUAL("0", exc.rightHandSide());
        CHECK_EQUAL("==", exc.comparisonOperator());
    }

    try
    {
        BADAssClose(1, 1.1, 0.2);
        ADD_TEST_SUCCESS();

        BADAssClose(1, 1.1, 0.1);
        ADD_TEST_SUCCESS();

        BADAssClose(1, 1.1, 0.05);
        ADD_TEST_FAILURE();
    }
    catch(const BADAssException &exc)
    {
        ADD_TEST_SUCCESS();
        CHECK_EQUAL("fabs(1 - (1.1))", exc.leftHandSide());
        CHECK_EQUAL("0.05 + dlim()", exc.rightHandSide());
        CHECK_EQUAL("<=", exc.comparisonOperator());
    }


    try
    {
        BADAssBreak("Oh no!", [&] (const BADAssException &exc)
        {
            CHECK_EQUAL("Oh no!", exc.description());
        });

        ADD_TEST_FAILURE();
    }
    catch(const BADAssException &exc)
    {
        ADD_TEST_SUCCESS();
    }

}

class CallCounter
{
public:
    CallCounter() {}

    uint m_count = 0;

    int magicFunction()
    {
        m_count++;
        return 1;
    }
};

TEST(double_function_call)
{
    CallCounter cc;

    BADAssEqual(cc.magicFunction(), cc.magicFunction());

    CHECK_EQUAL(2, cc.m_count);
}

class CopyCounter : public CallCounter
{
public:

    CopyCounter() : CallCounter() {}

    CopyCounter(const CopyCounter &other)
    {
        (void) other;
        m_copies++;
    }

    static uint m_copies;

    bool operator==(const CopyCounter &other) const
    {
        (void) other;
        return true;
    }
};

uint CopyCounter::m_copies = 0;

TEST(copy_call)
{
    CopyCounter cc;
    CopyCounter cc2;

    CopyCounter cc3 = cc;
    (void) cc3;

    BADAss(cc, ==, cc2);

    CHECK_EQUAL(1, CopyCounter::m_copies);

}


int main()
{
    cout << "Running badass tests." << endl;

#ifdef NDEBUG
    cerr << "Debug mode needed." << endl;
    return 1;
#endif

    return UnitTest::RunAllTests();
}
