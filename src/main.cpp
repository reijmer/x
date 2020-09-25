#include <stdio.h>

#include "node.h"
#include "parser.hpp"


using namespace std;


extern int yyparse();

extern TERNARY_TREE programBlock;

char* nodeNames[] = {
    "PROGRAM",
    "STATEMENTS",
    "STATEMENT",
    "EXPRESSION"
};


void printTree(TERNARY_TREE t) {
	if(t == NULL)
		return;

	printf("identifier %s", nodeNames[t->identifier]);

	printTree(t->left);
	printTree(t->center);
	printTree(t->right);
}


int main(int argc, char **argv)
{
	if (yyparse() == 0) {
		printTree(programBlock);
	} else {
		printf("error");
	}

	return 0;
}

