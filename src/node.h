#include <iostream>
#include <vector>

#define SYMTABSIZE  50
#define IDLENGTH    15
#define NOTHING     -1
#define IDENTOFFSET 2

enum NodeType {
    PROGRAM,
    STATEMENTS,
    STATEMENT,
    EXPRESSION
};

struct Node {
    int identifier;
    int value;
    struct Node *left;
    struct Node *center;
    struct Node *right;
};


typedef struct Node TREE_NODE;
typedef TREE_NODE *TERNARY_TREE;


TERNARY_TREE create_node(int, int, TERNARY_TREE, TERNARY_TREE, TERNARY_TREE);


struct symTabeNode {
    char identifier[IDLENGTH];
};

typedef struct symTabeNode SYMTABNODE;
typedef SYMTABNODE *SYMTABNODEPTR;


SYMTABNODE symTab[SYMTABSIZE];