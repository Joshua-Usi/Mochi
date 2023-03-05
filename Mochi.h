#include <string>
#include <functional>
#include <iostream>
#include <chrono>
#include <cstring>
#include <vector>

#include <assert.h>

namespace Mochi
{
	namespace Logic
	{
		enum class Operator
		{
			None,
			ConstantTrue,
			ConstantFalse,
			Or,
			And,
			Not,
			//Nand,
			//Nor,
			//Xor,
			//Xnor,
		};
		class Gate
		{
		private:
			std::vector<std::shared_ptr<Gate>> inputs;
			Operator operation;
		public:
			/// <summary>
			/// Note that this method does not initialise Input values, you must initialise them yourself
			/// </summary>
			/// <param name="oper">Logical operation to undertake</param>
			/// <param name="inputCount">Number of inputs taken</param>
			Gate(Operator oper, unsigned int inputCount = 0)
			{
				inputs.resize(inputCount);
				operation = oper;
			}
			/// <summary>
			/// Sets the input of a gate to another gates output
			/// </summary>
			/// <param name="inputNumber">The index of the input to set</param>
			/// <param name="state">The new state of the gate using the Gate* singleton instance</param>
			void SetInput(unsigned int inputNumber, std::shared_ptr<Gate> state)
			{
				if (inputNumber < inputs.size())
				{
					inputs[inputNumber] = state;
				}
				else
				{
					throw "inputNumber out of range";
				}
			}
			void SetInput(unsigned int inputNumber, Gate* state)
			{
				this->SetInput(inputNumber, std::shared_ptr<Gate>(state));
			}
			/// <summary>
			/// Set the input of a gate to a boolean value. Useful for using true booleans rather than the emulated gates
			/// </summary>
			/// <param name="inputNumber">The index of the input to set</param>
			/// <param name="state">The new state of the gate using the Gate* singleton instance</param>
			void SetInputBool(unsigned int inputNumber, bool state)
			{
				std::shared_ptr<Gate> constant = std::make_shared<Gate>((state) ? Operator::ConstantTrue : Operator::ConstantFalse);
				inputs[inputNumber] = constant;
			}
			/// <summary>
			/// Changes the boolean operator of the gate
			/// </summary>
			/// <param name="op">New operator the gate takes on</param>
			void SetOperator(Operator op)
			{
				operation = op;
			}
			/// <summary>
			/// Changes the input count to a certain value
			/// </summary>
			/// <param name="inputCount">The new number of inputs the gate has</param>
			void SetInputCount(unsigned int inputCount)
			{
				inputs.resize(inputCount);
			}
			/// <summary>
			/// The result of the gate when it is run with all values and sub-gates
			/// </summary>
			/// <returns>A boolean output of the value of logical gate value</returns>
			bool Output()
			{
				switch (operation)
				{
				case Operator::ConstantTrue: return true;
				case Operator::ConstantFalse: return false;
				case Operator::Or:
				{
					// 0 input gates should fail
					assert(inputs.size() > 0);
					bool result = inputs[0]->Output();
					for (int i = 1; i < inputs.size(); i++)
					{
						result |= inputs[i]->Output();
					}
					return result;
				};
				case Operator::And:
				{
					// 0 input gates should fail
					assert(inputs.size() > 0);
					bool result = inputs[0]->Output();
					for (int i = 1; i < inputs.size(); i++)
					{	
						result &= inputs[i]->Output();
					}
					return result;
				};
				case Operator::Not:
				{
					// 0 input gates should fail
					assert(inputs.size() > 0);
					return !inputs[0]->Output();
				}
				default:
					throw "Invalid Gate type";
				}
			}
		};
	}
	namespace Assert
	{
		template <typename type>
		class That
		{
		private:
			// One input AND gates act like repeaters, propagating signals through
			std::shared_ptr<Logic::Gate> satisfaction;
			type state;
			int chainSize;
		public:
			That& Is = *this;
			That(type value)
			{
				satisfaction.reset(new Logic::Gate(Logic::Operator::And, 1));
				state = value;
				chainSize = 0;
			}
			/// <summary>
			/// Logical NOT gate that inverts the next operation to return the opposite boolean value
			/// </summary>
			That& Not()
			{
				std::shared_ptr<Logic::Gate> s = satisfaction;
				satisfaction = std::make_shared<Logic::Gate>(Logic::Operator::Not, 1);
				satisfaction->SetInput(0, s);
				return *this;
			}
			/// <summary>
			/// Logical OR gate that returns true if one or more inputs is true
			/// </summary>
			That& Or()
			{
				satisfaction->SetOperator(Logic::Operator::Or);
				return *this;
			}
			/// <summary>
			/// Logical AND gate that only returns true if all inputs are true
			/// </summary>
			/// <param name="value">The values to compare with</param>
			That& And()
			{
				satisfaction->SetOperator(Logic::Operator::And);
				return *this;
			}
			/// <summary>
			/// Comparator that returns true if the given value is equal to the asserted value
			/// </summary>
			/// <param name="value">Value to compare</param>
			That& EqualTo(type value)
			{
				satisfaction->SetInputCount(chainSize + 1);
				satisfaction->SetInputBool(chainSize, state == value);
				chainSize++;
				return *this;
			}
			/// <summary>
			/// Comparator that returns true if the given value is less than the asserted value
			/// </summary>
			/// <param name="value">Value to compare</param>
			That& LessThan(type value)
			{
				satisfaction->SetInputCount(chainSize + 1);
				satisfaction->SetInputBool(chainSize, state < value);
				chainSize++;
				return *this;
			}
			/// <summary>
			/// Comparator that returns true if the given value is greater than the asserted value
			/// </summary>
			/// <param name="value">Value to compare</param>
			That& GreaterThan(type value)
			{
				satisfaction->SetInputCount(chainSize + 1);
				satisfaction->SetInputBool(chainSize, state > value);
				chainSize++;
				return *this;
			}
			/// <summary>
			/// Floating point comparator that returns true if the asserted value is NaN
			/// </summary>
			That& NaN()
			{
				satisfaction->SetInputCount(chainSize + 1);
				satisfaction->SetInputBool(chainSize, std::isnan(state));
				chainSize++;
				return *this;
			}
			/// <summary>
			/// Floating point comparator that returns true if the asserted value is Finite
			/// </summary>
			That& Finite()
			{
				satisfaction->SetInputCount(chainSize + 1);
				satisfaction->SetInputBool(chainSize, !std::isinf(state));
				chainSize++;
				return *this;
			}
			/// <summary>
			/// Boolean comparator that returns true if the asserted value is true or truthy
			/// </summary>
			That& True()
			{
				satisfaction->SetInputCount(chainSize + 1);
				satisfaction->SetInputBool(chainSize, state);
				chainSize++;
				return *this;
			}
			/// <summary>
			/// Memory comparator that returns true if the 2 memory locations are exactly equal bit for bit
			/// </summary>
			/// <param name="memregion">Starting address of memory region to compare</param>
			That& Equals(type memregion)
			{
				int size = sizeof(type);
				satisfaction->SetInputCount(chainSize + 1);
				satisfaction->SetInputBool(chainSize, memcmp(state, memregion, size) == 0);
				chainSize++;
				return *this;
			}
			/// <summary>
			/// Memory comparator that returns true if the memory address points to a Null Address (0x0)
			/// </summary>
			That& Null()
			{
				satisfaction->SetInputCount(chainSize + 1);
				satisfaction->SetInputBool(chainSize, state == 0);
				chainSize++;
				return *this;
			}
			bool AssertsTrue()
			{
				return satisfaction->Output();
			}
		};
	}
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