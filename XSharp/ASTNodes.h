#pragma once

#include "xsharp_global.h"
#include "XString.h"

class XSharp_EXPORT ASTNode {
public:
	virtual XString dump() const=0;

	template<typename T>
	T* to() {
		return dynamic_cast<T*>(this);
	}

	template<typename T>
	bool is() const {
		return bool(dynamic_cast<T*>(this));
	}

	virtual ~ASTNode() = default;
};

class XSharp_EXPORT IntegerNode:public ASTNode {
public:
	virtual XString dump();

	void setValue(int64_t value);
	int64_t value() const;
private:
	int64_t _value;
};

class XSharp_EXPORT DecimalFractionNode :public ASTNode {
public:
	virtual XString dump();

	void setValue(double value);
	double value() const;
private:
	double _value;
};

class XSharp_EXPORT BooleanNode :public ASTNode {
public:
	virtual XString dump();

	void setValue(bool value);
	bool value() const;
private:
	bool _value;
};

class XSharp_EXPORT StringNode :public ASTNode {
public:
	virtual XString dump();

	void setValue(XString value);
	XString value() const;
private:
	XString _value;
};