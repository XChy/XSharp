#include <XSharp/Lexer.h>
#include <XSharp/Parser.h>
#include <iostream>

int main()
{
	while (true)
	{
		try
		{
			char a[1024] = "1+1;";
			// std::cin.getline(a, 1024);
			std::cout << "h";
			Lexer lexer;
			auto tokens = lexer.tokenize("int main(int a,String b){" + XString(a) + "}");
			for (auto token : tokens)
			{
				std::cout << token.dump().toStdString() << "  ";
			}

			std::cout << std::endl;
			Parser parser;
			std::cout << parser.parse(tokens)->dump().toStdString();
		}
		catch (XSharpError e)
		{
			std::cout << e.errorInfo.toStdString();
		}
	}
	return 0;
}
