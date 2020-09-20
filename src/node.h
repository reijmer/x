#include <iostream>
#include <vector>

class CodeGenContext;
class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;

class Node {
public:
    virtual ~Node() {}
};

class NExpression : public Node {};

class NStatement : public Node {};

class NInteger : public NExpression {
public:
    long long value;
    NInteger(long long value) : value(value) {}
};

class NDouble : public NExpression {
public:
    double value;
    NDouble(double value) : value(value) {}
};

class NIdentifier : public NExpression {
public:
    std::string name;
    NIdentifier(const std::string& name) : name(name) {}
};

class NString : public NExpression {
public:
    std::string value;
    NString(const std::string& value) : value(value) {}
};

class NMethodCall : public NExpression {
public:
    const NIdentifier& id;
    ExpressionList arguments;
    NMethodCall(const NIdentifier& id, ExpressionList& arguments) :
        id(id), arguments(arguments) {}
    NMethodCall(const NIdentifier& id) : id(id) {}
};

class NBinaryOperator : public NExpression {
public:
    int op;
    NExpression& lhs;
    NExpression& rhs;
    NBinaryOperator(NExpression& lhs, int op, NExpression& rhs) :
        lhs(lhs), rhs(rhs), op(op) {}
};

class NBooleanOperator : public NExpression {
public:
    int op;
    NExpression& lhs;
    NExpression& rhs;
    NBooleanOperator(NExpression& lhs, int op, NExpression& rhs) :
        lhs(lhs), rhs(rhs), op(op) {}
};

class NAssignment : public NExpression {
public:
    NIdentifier& lhs;
    NExpression& rhs;
    NAssignment(NIdentifier& lhs, NExpression& rhs) : lhs(lhs), rhs(rhs) {}
};

class NBlock : public NExpression {
public:
    StatementList statements;
    NBlock() { }
};

class NExpressionStatement : public NStatement {
public:
    NExpression& expression;
    NExpressionStatement(NExpression& expression) : expression(expression) {}
};

class NVariableDeclaration : public NStatement {
public:
    NIdentifier& id;
    NExpression *assignmentExpr;
    NVariableDeclaration(NIdentifier& id) : id(id) { assignmentExpr = NULL; }
    NVariableDeclaration(NIdentifier& id, NExpression *assignmentExpr) : id(id), assignmentExpr(assignmentExpr) {}
};

class NBool : public NExpression {
public:
    bool value = false;

    NBool(bool val) {
        value = val;
    }
};

class NIfStatement : public NStatement {
public:
    NBlock *truecond;
    NBlock *falsecond;
    NBooleanOperator *cond;

    NIfStatement(NExpression *exprNode, NBlock *true_blockNode, NBlock *false_blockNode = nullptr) :
            cond((NBooleanOperator*)exprNode),
            truecond(true_blockNode),
            falsecond(false_blockNode) {}

};

class NFunctionDeclaration : public NStatement {
public:
	const NIdentifier& type;
	const NIdentifier& id;
	VariableList arguments;
	NBlock& block;
	NFunctionDeclaration(const NIdentifier& id, const VariableList& arguments, NBlock& block) :
		type(type), id(id), arguments(arguments), block(block) {}
};

class NReturnStatement : public NStatement {
public:
    NExpression& expression;
    NReturnStatement(NExpression& expression) : expression(expression) {}
};

class NWhileLoop : public NStatement {
public:
    NBlock *loopBlock;
    NBlock *elseBlock;
    NBooleanOperator *cond;

    NWhileLoop(NExpression *exprNode, NBlock *blockNode, NBlock *elseBlock = nullptr) :
            cond((NBooleanOperator*)exprNode),
            loopBlock(blockNode),
            elseBlock(elseBlock) {}

};