#pragma once
#include <vector>

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
		std::vector<Gate*> inputs;
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
		void SetInput(unsigned int inputNumber, Gate* state)
		{
			inputs[inputNumber] = state;
		}
		/// <summary>
		/// Set the input of a gate to a boolean value. Useful for using true booleans rather than the emulated gates
		/// </summary>
		/// <param name="inputNumber">The index of the input to set</param>
		/// <param name="state">The new state of the gate using the Gate* singleton instance</param>
		void SetInputBool(unsigned int inputNumber, bool state)
		{
			Gate* constant = new Gate((state) ? Operator::ConstantTrue : Operator::ConstantFalse);
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
				bool result = inputs[0]->Output();
				for (int i = 1; i < inputs.size(); i++)
				{
					result |= inputs[i]->Output();
				}
				return result;
			};
			case Operator::And:
			{
				bool result = inputs[0]->Output();
				for (int i = 1; i < inputs.size(); i++)
				{
					result &= inputs[i]->Output();
				}
				return result;
			};
			case Operator::Not: return !inputs[0]->Output();
			}
		}
	};
}