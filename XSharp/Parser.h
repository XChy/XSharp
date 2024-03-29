#pragma once

#include <vector>
#include "XSharp/ASTNodes.h"
#include "XSharp/ControlFlow/ControlFlowAST.h"
#include "XSharp/Class/ClassAST.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"
#include "xsharp_global.h"
#include "Tokens.h"
#include "XSharpUtils.h"
#include "OperInfo.h"

namespace XSharp {

class XSharp_EXPORT Parser
{
   public:
    typedef std::vector<Token>::const_iterator Iterator;
    ASTNode* parse(const std::vector<Token>&
                       tokenList);  // The astnode pointer is managed by caller
   private:
    DefinitionsNode* definitions();
    ClassNode* classDecl();

    // <type> <name> (paramlist)
    bool isFunctionDecl() const;
    FunctionNode* functionDecl();
    MemberMethodNode* memberMethodDecl();
    ConstructorNode* constructor();

    bool isVariableDecl() const;

    // end after the stopword
    // <type> <name> (( = <initial value> ))
    VarDeclNode* variableDecl(const std::vector<TokenType>& stopwords);

    std::vector<VarDeclNode*> parameters();

    // end at the right parenthesis
    std::vector<ASTNode*> argsList();

    BlockNode* block();
    ASTNode* statement();

    IfNode* ifStatement();
    WhileNode* whileStatement();

    // end at the pos of stopwords
    ASTNode* expr(std::vector<TokenType> stopwords, int priority = 0);
    ASTNode* operand();
    ASTNode* factor();

    TypeNode* type();

    static int priority(const XString& op);
    static int priority(BinaryOperatorNode* oper);
    static int priority(UnaryOpNode* oper);
    static Assoc assoc(const XString& op);
    static Assoc assoc(BinaryOperatorNode* oper);

    bool shouldStopOn(Iterator tokenIter,
                      std::vector<TokenType> stopwords = {}) const;

    void forward();
    void backward();
    Iterator current;
    Iterator end;
};
}  // namespace XSharp
