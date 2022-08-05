#pragma once

#include "xsharp_global.h"
#include "XString.h"
#include "TypeSystem.h"

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

	virtual ~ASTNode() = default;//Is set to delete the children
};

class XSharp_EXPORT IntegerNode:public ASTNode {
public:
	XString dump() const;

	void setValue(int64_t value);
	int64_t value() const;
private:
	int64_t _value;
};

class XSharp_EXPORT DecimalFractionNode :public ASTNode {
public:
	XString dump() const;

	void setValue(double value);
	double value() const;
private:
	double _value;
};

class XSharp_EXPORT BooleanNode :public ASTNode {
public:
	XString dump() const;

	void setValue(bool value);
	bool value() const;
private:
	bool _value;
};

class XSharp_EXPORT StringNode :public ASTNode {
public:
	XString dump() const;

	void setValue(XString value);
	XString value() const;
private:
	XString _value;
};

class XSharp_EXPORT AddNode :public ASTNode {
public:
	XString dump() const;

	void setLeft(ASTNode* left);
	ASTNode* left();

	void setRight(ASTNode* right);
	ASTNode* right();

	~AddNode();
private:
	ASTNode* _left;
	ASTNode* _right;
};

class XSharp_EXPORT SubNode :public ASTNode {
public:
	XString dump() const;

	void setLeft(ASTNode* left);
	ASTNode* left();

	void setRight(ASTNode* right);
	ASTNode* right();

	~SubNode();
private:
	ASTNode* _left;
	ASTNode* _right;
};

class XSharp_EXPORT MulNode :public ASTNode {
public:
	XString dump() const;

	void setLeft(ASTNode* left);
	ASTNode* left();

	void setRight(ASTNode* right);
	ASTNode* right();

	~MulNode();
private:
	ASTNode* _left;
	ASTNode* _right;
};

class XSharp_EXPORT DivNode :public ASTNode {
public:
	XString dump() const;

	void setLeft(ASTNode* left);
	ASTNode* left();

	void setRight(ASTNode* right);
	ASTNode* right();

	~DivNode();
private:
	ASTNode* _left;
	ASTNode* _right;
};

class XSharp_EXPORT FunctionDeclarationNode :public ASTNode {
public:
	XString dump() const;
private:
	XString _name;
	XSharp::Type _returnType;
	std::vector<std::pair<XSharp::Type, XString>> paramsType;// <type name,param name>
};

class XSharp_EXPORT ClassDeclarationNode :public ASTNode {
public:
	XString dump() const;
private:
	XString _name;
};

class XSharp_EXPORT VariableDeclarationNode :public ASTNode {
public:
	XString dump() const;
private:
	XString _name;
};

class XSharp_EXPORT BlockNode :public ASTNode {
public:
	XString dump() const;
private:
	std::vector<ASTNode*> _statements;
};

class XSharp_EXPORT DefinitionsNode :public ASTNode {
public:
	XString dump() const;
private:
	std::vector<ClassDeclarationNode*> _classDeclarations;
	std::vector<FunctionDeclarationNode*> _functionDeclarations;
	std::vector<VariableDeclarationNode*> _variableDeclarations;
};