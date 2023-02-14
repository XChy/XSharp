#pragma once

#include <vector>
#include "XSharp/TypeSystem.h"
#include "xsharp_global.h"
#include "ASTNodes.h"
#include "Tokens.h"
#include "XSharpUtils.h"
#include "OperInfo.h"

class XSharp_EXPORT Parser
{
   public:
    typedef std::vector<Token>::const_iterator Iterator;
    ASTNode* parse(const std::vector<Token>&
                       tokenList);  // The astnode pointer is managed by caller
   private:
    DefinitionsNode* definitions();
    ClassDeclarationNode* classDeclaration();
    FunctionDeclarationNode* functionDeclaration();
    VariableDeclarationNode* variableDeclaration(
        std::vector<TokenType> stopwords);

    std::vector<VariableDeclarationNode*> paramsDefinition();
    std::vector<ASTNode*> paramsList();

    BlockNode* block();
    ASTNode* statement();
    ASTNode* expression(std::vector<TokenType> stopwords);
    ASTNode* operand();

    TypeNode type();
    static int priority(BinaryOperatorNode* oper);
    static int priority(UnaryOperatorNode* oper);
    static Assoc assoc(BinaryOperatorNode* oper);

    bool isStopwords(Iterator tokenIter,
                     std::vector<TokenType> stopwords) const;

    void forward();
    void backward();
    Iterator current;
    Iterator end;
};
