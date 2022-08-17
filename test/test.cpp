#pragma once

#include <XSharp/Lexer.h>
#include <XSharp/Parser.h>
#include <iostream>

int main() {
	Lexer lexer;
	auto tokens = lexer.tokenize("int i;String a;");
	for (auto token : tokens) {
		std::cout << token.dump().toStdString()<<"  ";
	}

	std::cout << std::endl;
	Parser parser;
	std::cout<<parser.parse(tokens)->dump().toStdString();
	return 0;
}