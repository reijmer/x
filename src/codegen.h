#include <stack>
#include <typeinfo>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Bitcode/BitstreamReader.h>
#include <llvm/Bitcode/BitstreamWriter.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

class NBlock;

static LLVMContext MyContext;

class CodeGenBlock {
public:
    BasicBlock *block;
    Value *returnValue;
    std::map<std::string, Value*> locals;

};

class CodeGenContext {
    std::vector<CodeGenBlock *> blocks;
    Function *mainFunction;

public:

    Module *module;
    CodeGenContext() { module = new Module("main", MyContext); }
    
    void generateCode(NBlock& root);
    GenericValue runCode();

    std::vector<CodeGenBlock *>& getBlocks() {
        return blocks;
    }
    
    std::map<std::string, Value*>& locals() {
        return blocks[blocks.size() - 1]->locals;
    }
    
    BasicBlock *currentBlock() {
        return blocks[blocks.size() - 1]->block;
    }
    
    void pushBlock(BasicBlock *block) {
        blocks.push_back(new CodeGenBlock());
        blocks[blocks.size() - 1]->returnValue = NULL;
        blocks[blocks.size() - 1]->block = block;
    }

    void popBlock() {
        CodeGenBlock *top = blocks[blocks.size() - 1];
        blocks.pop_back();
        delete top;
    }
    
    void setCurrentReturnValue(Value *value) {
        blocks[blocks.size() - 1]->returnValue = value;
    }

    Value* getCurrentReturnValue() {
        return blocks[blocks.size() - 1]->returnValue;
    }

    void setCurrentBlock(BasicBlock * block) {
        blocks[blocks.size() - 1]->block = block;
    }
};
