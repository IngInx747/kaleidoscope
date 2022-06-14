#include "codegen_visitor.hh"

#include <vector>
#include <memory>
#include <unordered_map>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

using namespace llvm;

struct codegen_impl
{
    codegen_impl()
    : context(), builder(context), module("kaleidoscope", context)
    {}

    void push_value(Value* value)
    { value_stack.push_back(value); }

    Value* pop_value()
    { Value* value = value_stack.back(); value_stack.pop_back(); return value; }

    LLVMContext context;
    IRBuilder<> builder;
    Module module;

    std::vector<Value*> value_stack;
    std::unordered_map<std::string, Value*> value_table;
};

codegen_visitor::codegen_visitor()
{
    if (!impl)
    {
        impl = new codegen_impl();
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
    }
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
        impl->push_value(impl->builder.CreateFAdd(lval, rval, "addtmp"));
        break;
    case '-':
        impl->push_value(impl->builder.CreateFSub(lval, rval, "subtmp"));
        break;
    case '*':
        impl->push_value(impl->builder.CreateFMul(lval, rval, "multmp"));
        break;
    case '/':
        impl->push_value(impl->builder.CreateFDiv(lval, rval, "divtmp"));
        break;
    default:
        fprintf(stderr, "[ERROR] Invalid operation \"%c\".\n", node->operation);
        return 1;
    }

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

    if (!function && node->declaration->accept(this) != 0)
        return 1;

    function = static_cast<Function*>(impl->pop_value());

    //if (!function)
    //    return 1;

    if (!function->empty())
    {
        fprintf(stderr, "[ERROR] Redefined function \"%s\".\n", node->declaration->name);
        return 1;
    }

    BasicBlock *block = BasicBlock::Create(impl->context, "entry", function);
    impl->builder.SetInsertPoint(block);

    impl->value_table.clear();
    for (Argument& arg : function->args())
        impl->value_table[arg.getName()] = &arg;

    if (node->definition->accept(this) != 0)
    {
        function->eraseFromParent();
        return 1;
    }

    Value* retval = impl->pop_value();
    impl->builder.CreateRet(retval);
    verifyFunction(*function);
    impl->push_value(function);

    return 0;
}