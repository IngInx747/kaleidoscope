#include "codegen_visitor.hh"

#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>

#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/Utils.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Support/TargetRegistry.h> // llvm-10
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#define OUTPUT_IR
#define FUNCTION_OPTIMIZATION
#define GENERATE_OBJECT

using namespace llvm;


// Generate target's object code
static int generate_target_code(Module& module, std::string& output_filename);


struct codegen_visitor::codegen_impl
{
    codegen_impl() :
    context(),
    builder(context),
    module("kaleidoscope", context),
    FPM(&module)
    {
#ifdef FUNCTION_OPTIMIZATION
        // Promote allocas to registers.
        FPM.add(createPromoteMemoryToRegisterPass());
        // Do simple "peephole" optimizations and bit-twiddling optzns.
        FPM.add(createInstructionCombiningPass());
        // Reassociate expressions.
        FPM.add(createReassociatePass());
        // Eliminate Common SubExpressions.
        FPM.add(createGVNPass());
        // Simplify the control flow graph (deleting unreachable blocks, etc).
        FPM.add(createCFGSimplificationPass());
        FPM.doInitialization();
#endif
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
    std::unordered_map<std::string, AllocaInst*> value_table;
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
    if (impl)
    {
        impl->value_stack.clear();
        impl->value_table.clear();
    }
}

void codegen_visitor::terminate()
{
    if (impl)
    {
#ifdef OUTPUT_IR
        impl->module.print(outs(), nullptr);
#endif
#ifdef GENERATE_OBJECT
        std::string output_filename = impl->module.getSourceFileName() + ".o";
        generate_target_code(impl->module, output_filename);
#endif
    }
}

int codegen_visitor::visit(top_level_node* node)
{
    if (node->content->accept(this) != 0)
        return 1;
    Value* content = impl->pop_value();

#ifdef OUTPUT_IR
    content->print(outs()); // dump info
    std::cout << std::endl;
#endif

    impl->value_stack.clear();
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

    impl->push_value(impl->builder.CreateLoad(iter->second)); // load from var's address
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
    case '|':
        // TODO
        break;
    case '&':
        // TODO
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
    for (Argument& argument : function->args())
    {
        argument.setName(data(child)->name);
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

    for (Argument& argument : function->args())
    {
        //impl->value_table[argument.getName()] = &argument;
        AllocaInst* address = impl->builder.CreateAlloca(Type::getDoubleTy(impl->context), nullptr, argument.getName());
        impl->value_table[argument.getName()] = address;
        impl->builder.CreateStore(&argument, address);
    }

    if (node->definition->accept(this) != 0)
    {
        function->eraseFromParent(); // Error reading body, remove function.
        return 1;
    }

    Value* definition = impl->pop_value(); // returned Value
    impl->builder.CreateRet(definition); // Finish off the function.
    verifyFunction(*function); // Validate the generated code, checking for consistency.

    // Optimize the function(optional)
#ifdef FUNCTION_OPTIMIZATION
    impl->FPM.run(*function);
#endif
    // Remove the anonymous expression.
    //if (strcmp(node->declaration->name, "") == 0)
    //    function->eraseFromParent();

    impl->push_value(function);
    return 0;
}

int codegen_visitor::visit(block_node* node)
{
    Value* valrep {nullptr};

    for (auto* child = first(node->expressions); child != nullptr; child = next(child))
    {
        if (data(child)->accept(this) != 0)
            return 1;
        valrep = impl->pop_value();
    }

    impl->push_value(valrep); // return value of the last expression

    return 0;
}

int codegen_visitor::visit(assignment_node* node)
{
    // Evaluate RHS.
    if (node->expression->accept(this) != 0)
        return 1;
    Value* rhs = impl->pop_value();

    // Emit LHS i.e. the named variable.
    auto iter = impl->value_table.find(std::string(node->variable));
    AllocaInst* address {nullptr};

    if (iter == impl->value_table.end())
    {
        address = impl->builder.CreateAlloca(Type::getDoubleTy(impl->context), nullptr, node->variable);
        impl->value_table[std::string(node->variable)] = address;
    }
    else address = iter->second;

    impl->builder.CreateStore(rhs, address);
    impl->push_value(rhs); // return a value instead of address
    return 0;
}

int codegen_visitor::visit(if_else_node* node)
{
    Function* function = impl->builder.GetInsertBlock()->getParent();

    // Create blocks for the "then" and "else" cases.
    BasicBlock* then_block = BasicBlock::Create(impl->context, "if-true");
    BasicBlock* else_block = BasicBlock::Create(impl->context, "if-false");
    BasicBlock* merg_block = BasicBlock::Create(impl->context, "if-merge");

    // Emit "condition" value
    if (node->condition->accept(this) != 0)
        return 1;
    Value* condition = impl->pop_value(); // Convert condition to a bool by comparing non-equal to 0.0.
    condition = impl->builder.CreateFCmpONE(condition, ConstantFP::get(impl->context, APFloat(0.0)), "if_cond");

    // Create conditional branch.
    impl->builder.CreateCondBr(condition, then_block, else_block);

    // Emit value in "then" block.
    function->getBasicBlockList().push_back(then_block); // Insert "then" block at the end of the function.
    impl->builder.SetInsertPoint(then_block);
    if (node->then_expr->accept(this) != 0)
        return 1;
    Value* then_expr = impl->pop_value();
    impl->builder.CreateBr(merg_block);
    BasicBlock* phi_pred_then = impl->builder.GetInsertBlock();

    // Emit value in "else" block.
    function->getBasicBlockList().push_back(else_block);
    impl->builder.SetInsertPoint(else_block);
    if (node->else_expr->accept(this) != 0)
        return 1;
    Value* else_expr = impl->pop_value();
    impl->builder.CreateBr(merg_block);
    BasicBlock* phi_pred_else = impl->builder.GetInsertBlock();

    // Emit value in "merge" block.
    function->getBasicBlockList().push_back(merg_block);
    impl->builder.SetInsertPoint(merg_block);
    PHINode* phi = impl->builder.CreatePHI(Type::getDoubleTy(impl->context), 2, "if_phi");
    phi->addIncoming(then_expr, phi_pred_then);
    phi->addIncoming(else_expr, phi_pred_else);

    impl->push_value(phi);
    return 0;
}

int codegen_visitor::visit(for_loop_node* node)
{
    Function* function = impl->builder.GetInsertBlock()->getParent();

    BasicBlock* cond_block = BasicBlock::Create(impl->context, "for-cond"); // loop condition
    BasicBlock* loop_block = BasicBlock::Create(impl->context, "for-loop"); // loop body + step
    BasicBlock* next_block = BasicBlock::Create(impl->context, "for-term"); // where loop terminates

    // Emit "init" value.
    if (node->init->accept(this) != 0)
        return 1;
    Value* init = impl->pop_value();
    impl->builder.CreateBr(cond_block);

    // Emit "condition" value
    function->getBasicBlockList().push_back(cond_block);
    impl->builder.SetInsertPoint(cond_block);
    if (node->cond->accept(this) != 0)
        return 1;
    Value* condition = impl->pop_value();
    condition = impl->builder.CreateFCmpONE(condition, ConstantFP::get(impl->context, APFloat(0.0)), "for_cond");
    impl->builder.CreateCondBr(condition, loop_block, next_block);

    // Emit "loop" and "step" values
    function->getBasicBlockList().push_back(loop_block);
    impl->builder.SetInsertPoint(loop_block);
    if (node->expr->accept(this) != 0)
        return 1;
    Value* loop = impl->pop_value();
    if (node->step->accept(this) != 0)
        return 1;
    impl->pop_value();
    impl->builder.CreateBr(cond_block);

    // Terminate the loop
    function->getBasicBlockList().push_back(next_block);
    impl->builder.SetInsertPoint(next_block);

    impl->push_value(loop);
    return 0;
}


static int generate_target_code(Module& module, std::string& output_filename)
{
    static int _is_initialized = 0;

    if (!_is_initialized)
    {
        // Initialize the target registry etc.
        InitializeAllTargetInfos();
        InitializeAllTargets();
        InitializeAllTargetMCs();
        InitializeAllAsmParsers();
        InitializeAllAsmPrinters();
        _is_initialized = 1;
    }

    auto target_triple = sys::getDefaultTargetTriple();
    module.setTargetTriple(target_triple);

    std::string error;
    auto target = TargetRegistry::lookupTarget(target_triple, error);
    if (!target)
    {
        errs() << error;
        return 1;
    }

    TargetOptions opt;
    auto RM = Optional<Reloc::Model>();
    auto target_machine = target->createTargetMachine(target_triple, "generic", "", opt, RM);
    module.setDataLayout(target_machine->createDataLayout());

    std::error_code error_code;
    raw_fd_ostream dest(output_filename, error_code, sys::fs::OF_None);
    if (error_code)
    {
        errs() << "Could not open file: " << error_code.message() << "\n";
        return 1;
    }

    legacy::PassManager pass;
    if (target_machine->addPassesToEmitFile(pass, dest, nullptr, CGFT_ObjectFile))
    {
        errs() << "The Target Machine can't emit a file of this type" << "\n";
        return 1;
    }

    pass.run(module);
    dest.flush();

    return 0;
}
