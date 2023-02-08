<p align="center">
<img width="256" height="256" src="./Mochi.png">
</p>

# Mochi 

 A C++ header-only testing library with similiar syntax to Mocha.js
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
