#include <iostream>
#include "codegen.h"
#include "node.h"
#include "parser.hpp"

using namespace std;

extern int yyparse();

extern NBlock *programBlock;

void createCoreFunctions(CodeGenContext& context);

int main(int argc, char **argv)
{
	if (yyparse() == 0) {

		cout << programBlock << endl;
		InitializeNativeTarget();
		InitializeNativeTargetAsmPrinter();
		InitializeNativeTargetAsmParser();

		CodeGenContext context;
		createCoreFunctions(context);

		context.generateCode(*programBlock);
		context.runCode();
	}

	return 0;
}

