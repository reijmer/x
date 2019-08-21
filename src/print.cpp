#include <iostream>
#include "codegen.h"
#include "node.h"

using namespace std;

llvm::Function* createPrintfFunction(CodeGenContext& context) {
    std::vector<llvm::Type*> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(MyContext)); //char*

    llvm::FunctionType* printf_type =
        llvm::FunctionType::get(
            llvm::Type::getInt32Ty(MyContext), printf_arg_types, true);

    llvm::Function *func = llvm::Function::Create(
                printf_type, llvm::Function::ExternalLinkage,
                llvm::Twine("printf"),
                context.module
           );
    func->setCallingConv(llvm::CallingConv::C);
    return func;
}

void createPrintFunction(CodeGenContext& context, llvm::Function* printfFn) {
    std::vector<llvm::Type*> echo_arg_types;
    echo_arg_types.push_back(llvm::Type::getInt8PtrTy(MyContext));

    llvm::FunctionType* echo_type =
        llvm::FunctionType::get(
            llvm::Type::getVoidTy(MyContext), echo_arg_types, true);

    llvm::Function *func = llvm::Function::Create(
                echo_type, llvm::Function::InternalLinkage,
                llvm::Twine("print"),
                context.module
           );
    llvm::BasicBlock *bblock = llvm::BasicBlock::Create(MyContext, "entry", func, 0);
    context.pushBlock(bblock);

    std::vector<Value*> args;

    for (auto arg = func->arg_begin(); arg != func->arg_end(); &*arg++) {
        Value* val = arg;
        args.push_back(val);
    }

    CallInst *call = CallInst::Create(printfFn, makeArrayRef(args), "", bblock);
    ReturnInst::Create(MyContext, bblock);
    context.popBlock();
}

void createCoreFunctions(CodeGenContext& context) {
    llvm::Function* printfFn = createPrintfFunction(context);
    createPrintFunction(context, printfFn);
}