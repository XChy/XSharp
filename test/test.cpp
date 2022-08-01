#pragma once

#include <XSharp/Lexer.h>
#include <iostream>

int main() {
	Lexer lexer;
	auto tokens = lexer.tokenize("return(abc+abc)+1+1.23+0b10+0x23*[1,23]");
	for (auto token : tokens) {
		std::cout << token.dump().toStdString()<<"  ";
	}
	return 0;
}