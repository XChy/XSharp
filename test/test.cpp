#pragma once

#include <XSharp/Lexer.h>
#include <XSharp/Parser.h>
#include <iostream>

int main() {
	while(true){
		std::string a;
		std::cin >> a;
	Lexer lexer;
    auto tokens = lexer.tokenize("int main(int a,String b){"+XString::fromStdString(a) + "}");
	for (auto token : tokens) {
		std::cout << token.dump().toStdString() << "  ";
	}

	std::cout << std::endl;
	Parser parser;
	std::cout << parser.parse(tokens)->dump().toStdString();
	}
	return 0;
}
