#include "codegen_visitor.hh"

#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>

#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>

using namespace llvm;


static void dump(Value* value)
{
    value->print(outs());
    std::cout << std::endl;
}


struct codegen_visitor::codegen_impl
{
    codegen_impl() :
    context(),
    builder(context),
    module("kaleidoscope", context),
    FPM(&module)
    {
        // Do simple "peephole" optimizations and bit-twiddling optzns.
        FPM.add(createInstructionCombiningPass());
        // Reassociate expressions.
        FPM.add(createReassociatePass());
        // Eliminate Common SubExpressions.
        FPM.add(createGVNPass());
        // Simplify the control flow graph (deleting unreachable blocks, etc).
        FPM.add(createCFGSimplificationPass());
        FPM.doInitialization();
    }

    void push_value(Value* value)
    {
        value_stack.push_back(value);
    }

    Value* pop_value()
    {
        Value* value = value_stack.back();
        value_stack.pop_back();
        return value;
    }

    // llvm base
    LLVMContext context;
    IRBuilder<> builder;
    Module module;

    // llvm optimization
    legacy::FunctionPassManager FPM;

    // customized info
    std::vector<Value*> value_stack;
    std::unordered_map<std::string, Value*> value_table;
};

codegen_visitor::codegen_visitor(const char* source_filename)
{
    if (!impl)
    {
        impl = new codegen_impl();
        impl->module.setSourceFileName(source_filename);
    }
}

codegen_visitor::~codegen_visitor()
{
    if (impl)
    {
        delete impl;
        impl = nullptr;
    }
}

void codegen_visitor::initialize()
{
}

void codegen_visitor::terminate()
{
    if (impl)
    {
        impl->value_stack.clear();
        impl->value_table.clear();
        impl->module.print(outs(), nullptr);
    }
}

int codegen_visitor::visit(top_level_node* node)
{
    if (node->content->accept(this) != 0)
        return 1;

    Value* content = impl->pop_value();
    impl->value_stack.clear();

    dump(content); // dump info
    return 0;
}

int codegen_visitor::visit(number_node* node)
{
    double dvalue = std::stod(std::string(node->value));
    impl->push_value(ConstantFP::get(impl->context, APFloat(dvalue)));
    return 0;
}

int codegen_visitor::visit(variable_node* node)
{
    auto iter = impl->value_table.find(std::string(node->name));

    if (iter == impl->value_table.end())
    {
        fprintf(stderr, "[ERROR] Unknown variable \"%s\".\n", node->name);
        return 1;
    }

    impl->push_value(iter->second);
    return 0;
}

int codegen_visitor::visit(binary_expression_node* node)
{
    if (node->lhs->accept(this) != 0)
        return 1;
    if (node->rhs->accept(this) != 0)
        return 1;

    Value* valrep {nullptr};
    Value* rval = impl->pop_value();
    Value* lval = impl->pop_value();

    switch (node->operation)
    {
    case '+':
        valrep = impl->builder.CreateFAdd(lval, rval, "addtmp");
        break;
    case '-':
        valrep = impl->builder.CreateFSub(lval, rval, "subtmp");
        break;
    case '*':
        valrep = impl->builder.CreateFMul(lval, rval, "multmp");
        break;
    case '/':
        valrep = impl->builder.CreateFDiv(lval, rval, "divtmp");
        break;
    case '<':
        valrep = impl->builder.CreateFCmpULT(lval, rval, "cmplttmp");
        valrep = impl->builder.CreateUIToFP(valrep, Type::getDoubleTy(impl->context), "booltmp"); // Convert bool 0/1 to double 0.0 or 1.0
        break;
    case '>':
        valrep = impl->builder.CreateFCmpUGT(lval, rval, "cmpgttmp");
        valrep = impl->builder.CreateUIToFP(valrep, Type::getDoubleTy(impl->context), "booltmp"); // Convert bool 0/1 to double 0.0 or 1.0
        break;
    case '=':
        break;
    default:
        fprintf(stderr, "[ERROR] Invalid operation \"%c\".\n", node->operation);
    }

    if (!valrep) return 1;

    impl->push_value(valrep);

    return 0;
}

int codegen_visitor::visit(call_function_node* node)
{
    Function* callee = impl->module.getFunction(node->callee);

    if (!callee)
    {
        fprintf(stderr, "[ERROR] Unknown referenced function \"%s\".\n", node->callee);
        return 1;
    }

    if (callee->arg_size() != size(node->arguments))
    {
        fprintf(stderr, "[ERROR] Incorrect number of arguments passed to the function \"%s\".\n", node->callee);
        return 1;
    }

    for (auto* child = first(node->arguments); child != nullptr; child = next(child))
        if (data(child)->accept(this) != 0)
            return 1;

    std::vector<Value*> arguments;
    for (size_t i = 0; i < callee->arg_size(); ++i)
        arguments.push_back(impl->pop_value());
    std::reverse(arguments.begin(), arguments.end());

    impl->push_value(impl->builder.CreateCall(callee, arguments, "calltmp"));
    return 0;
}

int codegen_visitor::visit(function_declaration_node* node)
{
    Type* ret_type = Type::getDoubleTy(impl->context);
    std::vector<Type*> args_types(size(node->arguments), Type::getDoubleTy(impl->context));
    FunctionType* function_type = FunctionType::get(ret_type, args_types, false);
    Function* function = Function::Create(function_type, Function::ExternalLinkage, node->name, impl->module);

    if (!function) return 1;
        
    auto* child = first(node->arguments);
    for (Argument& arg : function->args())
    {
        arg.setName(data(child)->name);
        child = next(child);
    }

    impl->push_value(function);
    return 0;
}

int codegen_visitor::visit(function_definition_node* node)
{
    Function *function = impl->module.getFunction(node->declaration->name);

    if (!function)
    {
        if (node->declaration->accept(this) != 0) return 1;
        function = static_cast<Function*>(impl->pop_value());
    }

    if (!function->empty())
    {
        fprintf(stderr, "[ERROR] Redefined function \"%s\".\n", node->declaration->name);
        return 1;
    }

    // Create a new basic block to start insertion into.
    BasicBlock *block = BasicBlock::Create(impl->context, "entry", function);
    impl->builder.SetInsertPoint(block);

    // Record the function arguments in the NamedValues map.
    impl->value_table.clear();
    for (Argument& arg : function->args())
        impl->value_table[arg.getName()] = &arg;

    if (node->definition->accept(this) != 0)
    {
        function->eraseFromParent(); // Error reading body, remove function.
        return 1;
    }

    Value* definition = impl->pop_value(); // returned Value
    impl->builder.CreateRet(definition); // Finish off the function.
    verifyFunction(*function); // Validate the generated code, checking for consistency.

    // Optimize the function(optional)
    //impl->FPM.run(*function);

    // Remove the anonymous expression.
    //if (strcmp(node->declaration->name, "") == 0)
    //    function->eraseFromParent();

    impl->push_value(function);
    return 0;
}

int codegen_visitor::visit(block_node* node)
{
    return 0;
}

int codegen_visitor::visit(assignment_node* node)
{
    return 0;
}

int codegen_visitor::visit(if_else_node* node)
{
    if (node->condition->accept(this) != 0)
        return 1;
    Value* condition = impl->pop_value();

    // Convert condition to a bool by comparing non-equal to 0.0.
    condition = impl->builder.CreateFCmpONE(condition, ConstantFP::get(impl->context, APFloat(0.0)), "cond");
    Function* function = impl->builder.GetInsertBlock()->getParent();

    // Create blocks for the "then" and "else" cases. Insert the "then" block at the end of the function.
    BasicBlock* then_block = BasicBlock::Create(impl->context, "then", function);
    BasicBlock* else_block = BasicBlock::Create(impl->context, "else");
    BasicBlock* merg_block = BasicBlock::Create(impl->context, "merge");
    impl->builder.CreateCondBr(condition, then_block, else_block);

    // Emit "then" value.
    impl->builder.SetInsertPoint(then_block);
    if (node->then_expr->accept(this) != 0)
        return 1;
    Value* then_expr = impl->pop_value();
    impl->builder.CreateBr(merg_block);
    then_block = impl->builder.GetInsertBlock(); // Codegen of 'then' can change the current block, update "then" block for the PHI.

    // Emit "else" block.
    function->getBasicBlockList().push_back(else_block);
    impl->builder.SetInsertPoint(else_block);
    if (node->else_expr->accept(this) != 0)
        return 1;
    Value* else_expr = impl->pop_value();
    impl->builder.CreateBr(merg_block);
    else_block = impl->builder.GetInsertBlock(); // Codegen of 'else' can change the current block, update "else" block for the PHI.

    // Emit "merge" block.
    function->getBasicBlockList().push_back(merg_block);
    impl->builder.SetInsertPoint(merg_block);
    PHINode* phi = impl->builder.CreatePHI(Type::getDoubleTy(impl->context), 2, "iftmp");
    phi->addIncoming(then_expr, then_block);
    phi->addIncoming(else_expr, else_block);

    impl->push_value(phi);
    return 0;
}

int codegen_visitor::visit(for_loop_node* node)
{
    return 0;
}
