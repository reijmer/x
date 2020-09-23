#include <iostream>

#include "node.h"
#include "parser.hpp"


using namespace std;

extern int yyparse();


extern Block programBlock;


void printTree(branch) {




}


int main(int argc, char **argv)
{
	if (yyparse() == 0) {
		printTree(programBlock);
	} else {
		cout << "error";
	}

	return 0;
}

