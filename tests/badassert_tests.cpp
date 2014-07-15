#include <iostream>

#include <unittest++/UnitTest++.h>

#include "../badassert.h"

using namespace std;
using namespace badassert;

#define GETLINE() __LINE__
#define GETFILE() __FILE__
#define ADD_TEST_SUCCESS() CHECK_EQUAL(1, 1)
#define ADD_TEST_FAILURE() CHECK_EQUAL(1, 0)

#ifndef NDEBUG

TEST(ExceptionContents)
{
    CHECK_EQUAL(numeric_limits<double>::digits10, round(-log10(dlim)) - 1);

    try
    {
        BADAssert(0, ==, 1, "Zero should not be one!", [&] (const AssertException &exc)
        {
            CHECK_EQUAL(0, atoi(exc.leftHandSide().c_str()));
            CHECK_EQUAL(1, atoi(exc.rightHandSide().c_str()));
            CHECK_EQUAL("==", exc.comparisonOperator());

            CHECK_EQUAL(GETFILE(), exc.whichFile());
            CHECK_EQUAL("virtual void TestExceptionContents::RunImpl() const", exc.whichFunction());
            CHECK_EQUAL(GETLINE(), exc.whichLine());
            CHECK_EQUAL("Zero should not be one!", exc.description());

        });
    }
    catch (const AssertException &exc)
    {

    }

}

TEST(RandomCases)
{
    try
    {
        BADAssert(1, ==, 0);
        ADD_TEST_FAILURE();
    }
    catch(const AssertException &exc)
    {
        ADD_TEST_SUCCESS();
    }


    try
    {
        BADAssert(1, ==, 0, "testing testing");
        ADD_TEST_FAILURE();
    }
    catch(const AssertException &exc)
    {
        CHECK_EQUAL("testing testing", exc.description());
    }

    try
    {
        BADAssert(0, ==, 0);
        ADD_TEST_SUCCESS();
    }
    catch(const AssertException &exc)
    {
        ADD_TEST_FAILURE();
    }


    try
    {
        BADAssertEqual(0, true);
        ADD_TEST_FAILURE();
    }
    catch(const AssertException &exc)
    {
        ADD_TEST_SUCCESS();
        CHECK_EQUAL("true", exc.rightHandSide());
        CHECK_EQUAL("==", exc.comparisonOperator());
    }

    try
    {
        BADAssertEqual(1, 0);
        ADD_TEST_FAILURE();
    }
    catch(const AssertException &exc)
    {
        ADD_TEST_SUCCESS();
        CHECK_EQUAL("1", exc.leftHandSide());
        CHECK_EQUAL("0", exc.rightHandSide());
        CHECK_EQUAL("==", exc.comparisonOperator());
    }

    try
    {
        BADAssertClose(1, 1.1, 0.2);
        ADD_TEST_SUCCESS();

        BADAssertClose(1, 1.1, 0.1);
        ADD_TEST_SUCCESS();

        BADAssertClose(1, 1.1, 0.05);
        ADD_TEST_FAILURE();
    }
    catch(const AssertException &exc)
    {
        ADD_TEST_SUCCESS();
        CHECK_EQUAL("1.1 - 1", exc.leftHandSide());
        CHECK_EQUAL("0.05 + dlim", exc.rightHandSide());
        CHECK_EQUAL("<=", exc.comparisonOperator());
    }


    try
    {
        BADAssertBreak("Oh no!", [&] (const AssertException &exc)
        {
            CHECK_EQUAL("Oh no!", exc.description());
        });

        ADD_TEST_FAILURE();
    }
    catch(const AssertException &exc)
    {
        ADD_TEST_SUCCESS();
    }

}

#endif

int main()
{
    cout << "Running badassert tests." << endl;

    return UnitTest::RunAllTests();
}
