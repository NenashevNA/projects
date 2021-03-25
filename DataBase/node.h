#pragma once
#include <bits/stdc++.h>
#include "date.h"

enum class LogicalOperation {
	Or,
	And
};

enum class Comparison {
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	NotEqual
};

class Node {
public:
	Node() {};
	virtual bool Evaluate(Date date, string event) const = 0;
};

class EmptyNode : public Node {
public:
	EmptyNode() {};
	bool Evaluate(Date date, string event) const override {
		return true;
	}
private:
	bool tr = true;
};

class DateComparisonNode : public Node {
public:
	DateComparisonNode(Comparison cmp_, Date d_) :
		cmp(cmp_), d(d_) {};
	bool Evaluate(Date date, string event) const override {
		switch (cmp)
		{
		case Comparison::Less: return date < d;
		case Comparison::LessOrEqual: return date <= d;
		case Comparison::Greater: return date > d;
		case Comparison::GreaterOrEqual: return date >= d;
		case Comparison::Equal: return date == d;
		case Comparison::NotEqual: return date != d;
		default: return false;
		}
	}
private:
	Date d;
	Comparison cmp;
};

class EventComparisonNode : public Node {
public:
	EventComparisonNode(Comparison cmp_, string ev_) :
		cmp(cmp_), ev(ev_) {};
	bool Evaluate(Date date, string event) const override {
		switch (cmp)
		{
		case Comparison::Less: return event < ev;
		case Comparison::LessOrEqual: return event <= ev;
		case Comparison::Greater: return event > ev;
		case Comparison::GreaterOrEqual: return event >= ev;
		case Comparison::Equal: return event == ev;
		case Comparison::NotEqual: return event != ev;
		default: return false;
		}
	}
private:
	string ev;
	Comparison cmp;
};

class LogicalOperationNode : public Node{
public:
	LogicalOperationNode(LogicalOperation op_,
		shared_ptr<Node> left_, shared_ptr<Node> right_) : logical_operation(op_),
		left(left_), right(right_) {};
	bool Evaluate(Date date, string event) const override {
		switch (logical_operation)
		{
		case LogicalOperation::Or: return (*left).Evaluate(date, event) || (*right).Evaluate(date, event);
		case LogicalOperation::And: return (*left).Evaluate(date, event) && (*right).Evaluate(date, event);
		default: return false;
		}
	}
private:
	LogicalOperation logical_operation;
	shared_ptr<Node> left;
	shared_ptr<Node> right;
};
