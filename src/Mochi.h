#include <string>
#include <functional>
#include <iostream>
#include <chrono>

#include "Assert/Assert.h"

namespace Mochi
{
	/// <summary>
	/// Defines a collection of test cases in a class
	/// </summary>
	class Suite
	{
	public:
		Suite(std::string name, std::function<void(Suite*)> func)
		{
			suiteName = name;
			casesPassed = 0;
			func(this);
		}
		/// <summary>
		/// Add a test case to the suite
		/// </summary>
		/// <param name="caseName">Name or description of the test</param>
		/// <param name="func">Lambda function that contains test contents</param>
		void AddCase(std::string caseName, std::function<bool(void)> func)
		{
			caseNames.push_back(caseName);
			cases.push_back(func);
		}
		/// <summary>
		/// Runs all test cases
		/// </summary>
		void RunAllTests()
		{
			failedTestIDs.clear();
			start = std::chrono::high_resolution_clock::now();
			for (std::size_t i = 0, len = cases.size(); i < len; i++)
			{
				bool result = cases[i]();
				casesPassed += result;
				if (!result)
				{
					failedTestIDs.push_back(i);
				}
			}
			end = std::chrono::high_resolution_clock::now();
		}
		/// <summary>
		/// Prints the test report detailing passes, fails and which specific ones failed
		/// </summary>
		void PrintReport()
		{
			std::cout << "Suite: " << suiteName << std::endl;
			std::cout << "----------------------------------------------------------------" << std::endl;
			std::cout << "Total Tests: " << cases.size() << std::endl;
			std::cout << "Tests Passed: " << casesPassed << std::endl;
			std::cout << "Tests Failed: " << cases.size() - casesPassed << std::endl;
			std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
			std::cout << "Test Execution Time: " << duration.count() << "s" << std::endl;
			if (cases.size() - casesPassed > 0)
			{
				std::cout << "----------------------------------------------------------------" << std::endl;
				std::cout << "Failed Tests:" << std::endl;
				for (int i = 0; i < failedTestIDs.size(); i++)
				{
					std::cout << caseNames[failedTestIDs[i]] << std::endl;
				}
			}
			std::cout << "----------------------------------------------------------------" << std::endl;
		}
	private:
		std::chrono::high_resolution_clock::time_point start;
		std::chrono::high_resolution_clock::time_point end;
		std::string suiteName;
		std::vector<std::function<bool(void)>> cases;
		std::vector<std::string> caseNames;
		std::vector<int> failedTestIDs;
		int casesPassed;
	};
}