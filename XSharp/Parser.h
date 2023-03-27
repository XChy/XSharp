#pragma once

#include <vector>
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
    ClassNode* classDeclaration();
    // <type> <name> (paramlist)
    bool isFunctionDecl() const;
    FunctionNode* functionDeclaration();

    // end after the stopword
    // <type> <name> (( = <initial value> ))
    bool isVariableDecl() const;
    VariableNode* variableDeclaration(const std::vector<TokenType>& stopwords);

    std::vector<VariableNode*> paramsDefinition();

    // end at the right parenthesis
    std::vector<ASTNode*> paramsList();

    BlockNode* block();
    ASTNode* statement();

    IfNode* ifStatement();
    WhileNode* whileStatement();

    // end at the pos of stopwords
    ASTNode* expression(std::vector<TokenType> stopwords, int priority = 0);
    ASTNode* operand();

    Type* type();

    static int priority(const XString& op);
    static int priority(BinaryOperatorNode* oper);
    static int priority(UnaryOperatorNode* oper);
    static Assoc assoc(const XString& op);
    static Assoc assoc(BinaryOperatorNode* oper);

    bool isStopwords(Iterator tokenIter,
                     std::vector<TokenType> stopwords) const;

    void forward();
    void backward();
    Iterator current;
    Iterator end;
};
}  // namespace XSharp
