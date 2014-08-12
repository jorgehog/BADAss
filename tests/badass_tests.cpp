#include <iostream>

#include <unittest++/UnitTest++.h>

#include "../badass.h"

using namespace std;
using namespace badass;

#define GETLINE() __LINE__
#define GETFILE() __FILE__
#define ADD_TEST_SUCCESS() CHECK_EQUAL(1, 1)
#define ADD_TEST_FAILURE() CHECK_EQUAL(1, 0)

TEST(ExceptionContents)
{
    CHECK_EQUAL(numeric_limits<double>::digits10, round(-log10(dlim)) - 1);

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
        CHECK_EQUAL("1.1 - 1", exc.leftHandSide());
        CHECK_EQUAL("0.05 + dlim", exc.rightHandSide());
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

int main()
{
    cout << "Running badass tests." << endl;

#ifdef NDEBUG
    cerr << "Debug mode needed." << endl;
    return 1;
#endif

    return UnitTest::RunAllTests();
}
