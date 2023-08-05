#pragma once

#include "XSharp/Parser.h"

using namespace XSharp;

ClassNode* Parser::classDecl()
{
    ClassNode* classNode = new ClassNode;
    // skip 'class'
    forward();

    if (!current->is(Identifier)) throw XSharpError("No name for class");

    // get the name of class
    classNode->name = current->value;
    forward();

    // [optional] superclass
    if (current->isKeyword("extends")) {
        forward();
        if (current->is(Identifier)) classNode->superClass = current->value;
        forward();
    }

    // skip '{'
    if (current->is(OpenBrace))
        forward();
    else
        throw XSharpError("Expected '{' is missing");

    while (!current->is(CloseBrace)) {
        if (isVariableDecl()) {
            auto var = variableDecl({SentenceEnd});
            classNode->members.push_back(var);
        } else if (isFunctionDecl()) {
            auto func = memberMethodDecl();
            func->selfClass = classNode;
            classNode->methods.push_back(func);
        } else if (current->isKeyword("new")) {
            auto cons = constructor();
            cons->selfClass = classNode;
            classNode->constructors.push_back(cons);
        } else {
            throw XSharpError("Not a field in class");
        }
    }

    // skip '}'
    forward();

    // analyze the class block
    return classNode;
}

