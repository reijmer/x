#include "node.h"
#include "codegen.h"
#include "parser.hpp"

using namespace std;

void CodeGenContext::generateCode(NBlock& root) {
    vector<Type*> argTypes;
    FunctionType *ftype = FunctionType::get(Type::getVoidTy(MyContext), makeArrayRef(argTypes), false);
    mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module);
    BasicBlock *bblock = BasicBlock::Create(MyContext, "entry", mainFunction, 0);

    pushBlock(bblock);
    root.codeGen(*this);
    ReturnInst::Create(MyContext, this->currentBlock());
    popBlock();

    legacy::PassManager pm;
    pm.add(createPrintModulePass(outs()));
    pm.run(*module);
}

GenericValue CodeGenContext::runCode() {
    ExecutionEngine *ee = EngineBuilder( unique_ptr<Module>(module) ).create();
    ee->finalizeObject();
    vector<GenericValue> noargs;
    GenericValue v = ee->runFunction(mainFunction, noargs);
    return v;
}

Value* NInteger::codeGen(CodeGenContext& context) {
    return ConstantInt::get(Type::getInt64Ty(MyContext), value, true);
}

Value* NDouble::codeGen(CodeGenContext& context) {
    return ConstantFP::get(Type::getDoubleTy(MyContext), value);
}

Value* NString::codeGen(CodeGenContext& context) {
    IRBuilder<> builder(context.currentBlock());
    return builder.CreateGlobalStringPtr(StringRef(value));
}

Value* NIdentifier::codeGen(CodeGenContext& context) {
    std::vector<CodeGenBlock *>& blocks = context.getBlocks();

    for (auto block = blocks.rbegin(); block != blocks.rend(); ++block) {
        auto scope = *block;
        std::map<std::string, Value*> names = scope->locals;
        if (names.find(name) != names.end()) {
            return new LoadInst(names[name], "", false, context.currentBlock());
        }
    }
    return NULL;
}

Value* NMethodCall::codeGen(CodeGenContext& context) {
    Function *function = context.module->getFunction(id.name.c_str());
    if (function == NULL) {
        std::cerr << "no such function " << id.name << endl;
    }
    std::vector<Value*> args;
    ExpressionList::const_iterator it;
    for (it = arguments.begin(); it != arguments.end(); it++) {
        args.push_back((**it).codeGen(context));
    }
    CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
    return call;
}

Value* NBinaryOperator::codeGen(CodeGenContext& context) {
    Instruction::BinaryOps instr;
    IRBuilder<> builder(context.currentBlock());
    switch (op) {
        case PLUS:
            instr = Instruction::Add;
            goto math;
        case MINUS:
            instr = Instruction::Sub;
            goto math;
        case MULT:
            instr = Instruction::Mul;
            goto math;
        case DIV:
            instr = Instruction::SDiv;
            goto math;
        case MOD:
            instr = Instruction::SRem;
            goto math;
    }

    return NULL;
math:
    return BinaryOperator::Create(instr, lhs.codeGen(context), 
        rhs.codeGen(context), "", context.currentBlock());
}

Value* NBooleanOperator::codeGen(CodeGenContext& context) {
    Instruction::BinaryOps instr;
    IRBuilder<> builder(context.currentBlock());
    switch (op) {
        case TCNE:
            return builder.CreateICmpNE(lhs.codeGen(context), rhs.codeGen(context), "");
        case TCEQ:
            return builder.CreateICmpEQ(lhs.codeGen(context), rhs.codeGen(context), "");
        case TCLE:
            return builder.CreateICmpSLE(lhs.codeGen(context), rhs.codeGen(context), "");
        case TCGE:
            return builder.CreateICmpSGE(lhs.codeGen(context), rhs.codeGen(context), "");
        case TCGT:
            return builder.CreateICmpSGT(lhs.codeGen(context), rhs.codeGen(context), "");
        case TCLT:
            return builder.CreateICmpSLT(lhs.codeGen(context), rhs.codeGen(context), "");
    }
    return NULL;
}

Value* NBlock::codeGen(CodeGenContext& context) {
    StatementList::const_iterator it;
    Value *last = NULL;
    for (it = statements.begin(); it != statements.end(); it++) {
        last = (**it).codeGen(context);
    }
    return last;
}

Value* NExpressionStatement::codeGen(CodeGenContext& context) {
    return expression.codeGen(context);
}

Value* NAssignment::codeGen(CodeGenContext& context) {
    Value* value = rhs.codeGen(context);
    Type* ty = value->getType();
    AllocaInst* var = new AllocaInst(ty, 0, lhs.name.c_str(), context.currentBlock());
    context.locals()[lhs.name] = var;

    return new StoreInst(value, context.locals()[lhs.name], false, context.currentBlock());
}

Value* NVariableDeclaration::codeGen(CodeGenContext& context) {
    if (assignmentExpr != NULL) {
        NAssignment assn(id, *assignmentExpr);
        auto val = assn.codeGen(context);
        
        return val;
    } else {
        // set default type to int, can be changed on assignment.
        AllocaInst *alloc = new AllocaInst(Type::getInt64Ty(MyContext), 0, id.name.c_str(), context.currentBlock());
	    context.locals()[id.name] = alloc;

        return alloc;
    }
}

Value *NBool::codeGen(CodeGenContext &context) {
    IRBuilder<> builder(context.currentBlock());
    if (value)
        return builder.getTrue();
    else
        return builder.getFalse();
}

Value *NIfStatement::codeGen(CodeGenContext &context) {
    
    Function *function = context.currentBlock()->getParent();
    BasicBlock *thenBlock = BasicBlock::Create(MyContext, "then", function);
    BasicBlock *elseBlock = BasicBlock::Create(MyContext, "else");
    BasicBlock *mergeBlock = BasicBlock::Create(MyContext, "cont");

    function->getBasicBlockList().push_back(thenBlock);
    Value *condValue = cond->codeGen(context);

    if (falsecond != nullptr)
        BranchInst::Create(thenBlock, elseBlock, condValue, context.currentBlock());
    else
        BranchInst::Create(thenBlock, mergeBlock, condValue, context.currentBlock());

    context.pushBlock(thenBlock);
    Value *thenValue = truecond->codeGen(context);
    BranchInst::Create(mergeBlock, context.currentBlock());
    context.popBlock();

    if (falsecond != nullptr) {
        function->getBasicBlockList().push_back(elseBlock);
        context.pushBlock(elseBlock);
        Value *elseValue = falsecond->codeGen(context);
        
        BranchInst::Create(mergeBlock, context.currentBlock());
        context.popBlock();
    }

    function->getBasicBlockList().push_back(mergeBlock);
    context.pushBlock(mergeBlock);

    return function;
}

Value* NFunctionDeclaration::codeGen(CodeGenContext& context) {
    std::vector<Type*> argTypes;
    VariableList::const_iterator it;

    for (it = arguments.begin(); it != arguments.end(); it++) {
        argTypes.push_back(Type::getInt64Ty(MyContext));//typeOf((**it).type));
    }

    FunctionType *ftype = FunctionType::get(Type::getVoidTy(MyContext), makeArrayRef(argTypes), false);
    Function *function = Function::Create(ftype, GlobalValue::InternalLinkage, id.name.c_str(), context.module);
    BasicBlock *bblock = BasicBlock::Create(MyContext, "entry", function, 0);

    context.pushBlock(bblock);

    Function::arg_iterator argsValues = function->arg_begin();
    Value* argumentValue;

    for (it = arguments.begin(); it != arguments.end(); it++) {
        //(**it).codeGen(context);
        AllocaInst *alloc = new AllocaInst(Type::getInt64Ty(MyContext), 0, (*it)->id.name.c_str(), context.currentBlock());
	    context.locals()[(*it)->id.name] = alloc;

        argumentValue = &*argsValues++;
        StoreInst *inst = new StoreInst(argumentValue, context.locals()[(*it)->id.name], false, bblock);
    }

    block.codeGen(context);
    ReturnInst::Create(MyContext, context.getCurrentReturnValue(), context.currentBlock() );

    context.popBlock();
    return function;
}