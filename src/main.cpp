#include <iostream>

#include "node.h"
#include "parser.hpp"


using namespace std;

extern int yyparse();


int main(int argc, char **argv)
{
	if (yyparse() == 0) {

	}

	return 0;
}

