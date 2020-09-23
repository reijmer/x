#include <iostream>
#include <vector>

class Statement;
class Expression;
class VariableDeclaration;

typedef std::vector<Statement*> StatementList;
typedef std::vector<Expression*> ExpressionList;
typedef std::vector<VariableDeclaration*> VariableList;


enum NodeType {
    Expression,
    Statement
}



class Node {

    public:
        Node(std::string name, std::string value, NodeType *type, Node* left, Node* right) :
            name(name),
            value(value),
            type(type),
            left(left),
            right(right) {}
        
        virtual ~Node() {}


    private:
        
        std::string name;
        std::string value;

        NodeType type;
        Node *left;
        Node *right;
};
