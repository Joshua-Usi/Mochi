<p align="center">
<img width="256" height="256" src="./Mochi.png">
</p>

# Mochi 
 A C++ header-only testing framework with similiar syntax to Mocha.js
```C++
Mochi::Suite example("Mochi tests", [&](Mochi::Suite* suite) {
	suite->AddCase("Asserts if 2 values are equal", [&]() {
		return Mochi::Assert::That(3).Is->EqualTo(3)->AssertsTrue(); // True
	});
	suite->AddCase("Asserts if a value is less than another", [&]() {
		return Mochi::Assert::That(9.3f).Is->LessThan(6.4f)->AssertsTrue(); // False
	});
});
example.RunAllTests();
example.PrintReport();
 ```
 Output from PrintReport():
 ```
 Suite: Mochi tests
----------------------------------------------------------------
Total Tests: 2
Tests Passed: 1
Tests Failed: 1
Test Execution Time: X.XXXXXs
----------------------------------------------------------------
Failed Tests:
Asserts if a value is less than another
 ```
# Usage
## Suites
```C++
Mochi::Suite suite("Icecream", [&](Mochi::Suite* suite) {

});
 ```
A suite is a collection of cases. A suite can have a name, the name of the suite above is called "Icecream".
Each suite can execute and test all their cases at once using 
```C++
suite.RunAllTests();
 ```
 Which saves the results of each test and notes the total execution time of all the tests
 
 To get a formatted from the results through the console, Use the function 
 
```C++
suite.PrintReport();
 ```
 To get a succint report of all tests that passed, failed, which specific tests failed and total execution time. This suite has no test cases so the resulting output from ```suite.PrintReport();``` will be:
  ```
 Suite: Icecream
----------------------------------------------------------------
Total Tests: 0
Tests Passed: 0
Tests Failed: 0
Test Execution Time: X.XXXXXs
----------------------------------------------------------------
 ```
 ## Cases
 A case tests a value or a functions return value for correctness using Assertions
 ```C++
 Mochi::Suite example("Mochi tests", [&](Mochi::Suite* suite) {
 	suite->AddCase("Checks if two values are equal to each other", [&]() {
		return Mochi::Assert::That(3).Is->EqualTo(3)->AssertsTrue(); // True
	});
});
 ```
 You may add as many cases to a suite as you like. But it is highly recommended to put similiar cases in one suite or use one suite for each module that is being tested
 ## Assertions
 The assertion system is my favourite part of this framework. It allows you to use almost english like grammar while also being able to generate a test. While Mocha.js looks much cleaner. This is the best that C++ can possibly do for now
 ```C++
 // Assert that 3 is equal to 3
 Mochi::Assert::That(3).Is->EqualTo(3);
 ```
 There are many assertion types, these are:
 ### Integral and floating equalities
 ```C++
 // Returns true if the number from That() is equal to n
 Mochi::Assert::That(x).Is->EqualTo(n);
 // Returns true if the number from That() is less than n
 Mochi::Assert::That(x).Is->LessThan(n);
 // Returns true if the number from That() is greater than n
 Mochi::Assert::That(x).Is->GreaterThan(n);
 ```
  ### Floating value tests
 ```C++
 // Returns true is the floating point number from That() is NaN (Not a Number)
 Mochi::Assert::That(x).Is->NaN();
  // Returns true is the floating point number from That() is Finite (Not Infinite)
 Mochi::Assert::That(x).Is->Finite();
 ```
  ### Boolean value tests
 ```C++
 // Returns true if the boolean from That() is true
 Mochi::Assert::That(x).Is->True();
 ```
  ### Memory comparison tests
 ```C++
 // Returns true if the memory region from the pointer from That() is equal to each other
 Mochi::Assert::That(x).Is->Equals(void* memoryregion);
 // Returns true if the pointer from That() is null
 Mochi::Assert::That(x).Is->Null();
 ```
  ### Test Operators
 On their own the above comparators are powerful but not enough to encompass every possible test that might be conceived. This is where the Assertion libraries operators come into play, allowing comparators to be chained through boolean logic
 ### Not operand
  ```C++
 // Inverts the boolean value from the succeeding statement
 // Assert that 5 is not equal to 3
 Mochi::Assert::That(5).Is->Not()->EqualTo(3);
 ```
  ### And operand
  ```C++
 // Only returns true if both the preceeding and succeeding statement is true
 // Assert that 5 is greater than 2 and less than 7
 Mochi::Assert::That(5).Is->GreaterThan(2)->And()->LessThan(7);
 ```
  ### Or operand
  ```C++
 // Returns true if either the preceeding or succeedin statement is true, or even both!
 // Assert that 5 is less than or equal to 7
 Mochi::Assert::That(5).Is->LessThan(7)->Or()->EqualTo();
 ```
 ### Getting the value out of assertions
 To get the value of an assertion, call ```AssertsTrue();``` at the end of the assert chain
   ```C++
 // Returns true if either the preceeding or succeedin statement is true, or even both!
 // Assert that 5 is less than or equal to 7
 Mochi::Assert::That(5).Is->LessThan(7)->Or()->EqualTo()->AssertsTrue() // Returns true;
 ```
 Asserts can be chained as long as you want. However it is very cumbersome and usually not required
 ```C++
 // Assert that 7 is not equal to 5 or les than 9 and greater than 2 and not equal to 3 asserts true
 // AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
 return Mochi::Assert::That(7).Is->Not()->EqualTo(5)->Or()->LessThan(9)->And()->GreaterThan(2)->And()->Not()->EqualTo(3)->AssertsTrue();
 ```
 ## Testing the testing framework
 Under ```./tests``` there is a file called ```meta.h``` which contains a function ```void meta()``` that defines the meta-tests required to assert that this framework is working correctly.
