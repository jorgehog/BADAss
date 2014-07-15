##Beautiful And Descriptive Assert
----------------------------

###BADAssert is an assert framework desgined for easy and clean yet very descriptive assertion calls.
###The goal is to improve efficiency and transparency of assertion calls by using user defined functions which trigger only on assertion breaks.

###Please see the test source file for examples of use.


```
#include <badassert.h>
#include <iostream>

using namespace std;
using namespace badassert;

try
{
    BADAssert(0, ==, 1, "Zero should not be one!", [&] (const AssertException &exc)
    {

        cout << exc.leftHandSide() << " mismatched " << exc.rightHandSide()
             << " according to the " << exc.comparisonOperator() << " operator. "
             << endl;

        cout << "assert call came from the file " << exc.whichFile()
             << " at line " << exc.whichLine()
             << " in function " << exc.whichFunction()
             << endl;

        cout << " this is your description of the issue: " << exc.description()
             << endl;

        cout << "Now use this information to do whatever preparations necessary "
             << "such as really expensive calculations you only want to do if the"
             << "progam is crashing in a debug build."
             << endl;

    });
}
catch (const AssertException &exc)
{

}

```
