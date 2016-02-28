#pragma once

#include "MonteCarlo\TreeNode.h"
#include "MonteCarlo\UctMonteCarloTree.h"

namespace RamAi
{
	/*class State
	{
	public:
		State() = default;
		State(int number) : m_number(number) {}
		~State() = default;

		const int GetNumber() const { return m_number; }

		bool operator== (const State &other) const
		{
			return m_number == other.m_number;
		}

	private:
		int m_number;
	};

	class MonteCarloTest : public UctMonteCarloTree<State>
	{
	public:
		MonteCarloTest(const State &initialState, const float bias)
			: UctMonteCarloTree(initialState, bias)
		{
		}

		MonteCarloTest(State &&initialState, const float bias)
			: UctMonteCarloTree(std::move(initialState), bias)
		{
		}

		MonteCarloTest(const MonteCarloTest &other)
			: UctMonteCarloTree(other)
		{
		}

		MonteCarloTest(MonteCarloTest &&other)
			: UctMonteCarloTree(std::move(other))
		{
		}

		virtual ~MonteCarloTest() override
		{
		}

	public:
		MonteCarloTest &operator= (const MonteCarloTest &other)
		{
			m_root = other.m_root;
			m_bias = other.m_bias;
			return *this;
		}

		MonteCarloTest &operator= (MonteCarloTest &&other)
		{
			m_root = std::move(other.m_root);
			m_bias = std::move(other.m_bias);
			return *this;
		}

	protected:
		virtual void PerformExpansion(TreeNode<NodeType> &root) override
		{
			int number = root.GetValue().state.GetNumber();

			root.AddChild(State(number + 1));
			root.AddChild(State(number + 2));
			root.AddChild(State(number + 3));
		}

		virtual const float Simulate(const State &state) override
		{
			return (state.GetNumber() % 2) == 0 ? 1.0f : 0.0f;
		}
	};*/
};