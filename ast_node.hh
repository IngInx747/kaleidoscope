#ifndef KS_AST_NODE_HH
#define KS_AST_NODE_HH

#include "linked_list.hh"


class visitor;


class ast_node
{
public:
    virtual int accept(visitor*) = 0;

    virtual ~ast_node()
    {}

public:
};


class top_level_node : public ast_node
{
public:
    virtual int accept(visitor*);

    virtual ~top_level_node()
    {
        if (content)
        {
            delete content;
            content = nullptr;
        }
    }

public:
    ast_node* content {nullptr};
};


class number_node : public ast_node
{
public:
    virtual int accept(visitor*);

    virtual ~number_node()
    {
        if (value)
        {
            delete value;
            value = nullptr;
        }
    }

public:
    const char* value {nullptr};
};


class variable_node : public ast_node
{
public:
    virtual int accept(visitor*);

    virtual ~variable_node()
    {
        if (name)
        {
            delete name;
            name = nullptr;
        }
    }

public:
    const char* name {nullptr};
};


class binary_expression_node : public ast_node
{
public:
    virtual int accept(visitor*);

    virtual ~binary_expression_node()
    {
        if (lhs)
        {
            delete lhs;
            lhs = nullptr;
        }
        if (rhs)
        {
            delete rhs;
            rhs = nullptr;
        }
    }

public:
    ast_node* lhs {nullptr};
    ast_node* rhs {nullptr};
    char operation {0};
};


class call_function_node : public ast_node
{
public:
    virtual int accept(visitor*);

    virtual ~call_function_node()
    {
        if (callee)
        {
            delete callee;
            callee = nullptr;
        }
        if (arguments)
        {
            for (double_linked_list_node<ast_node*>* child = first(arguments); child != nullptr; child = next(child))
                delete child->data;
            deallocate(arguments);
            arguments = nullptr;
        }
    }

public:
    const char* callee {nullptr}; // the name of the function called
    double_linked_list_node<ast_node*>* arguments {nullptr};
};


class function_declaration_node : public ast_node
{
public:
    virtual int accept(visitor*);

    virtual ~function_declaration_node()
    {
        if (name)
        {
            delete name;
            name = nullptr;
        }
        if (arguments)
        {
            for (double_linked_list_node<variable_node*>* child = first(arguments); child != nullptr; child = next(child))
                delete child->data;
            deallocate(arguments);
            arguments = nullptr;
        }
    }

public:
    const char* name {nullptr}; // function name
    double_linked_list_node<variable_node*>* arguments {nullptr};
};


class function_definition_node : public ast_node
{
public:
    virtual int accept(visitor*);

    virtual ~function_definition_node()
    {
        if (declaration)
        {
            delete declaration;
            declaration = nullptr;
        }
        if (definition)
        {
            delete definition;
            definition = nullptr;
        }
    }

public:
    function_declaration_node* declaration {nullptr};
    ast_node* definition {nullptr};
};


class block_node : public ast_node
{
public:
    virtual int accept(visitor*);

    virtual ~block_node()
    {
        if (expressions)
        {
            for (double_linked_list_node<ast_node*>* child = first(expressions); child != nullptr; child = next(child))
                delete child->data;
            deallocate(expressions);
            expressions = nullptr;
        }
    }

public:
    double_linked_list_node<ast_node*>* expressions {nullptr};
};


class assignment_node : public ast_node
{
public:
    virtual int accept(visitor*);

    virtual ~assignment_node()
    {
        if (variable)
        {
            delete variable;
            variable = nullptr;
        }
        if (expression)
        {
            delete expression;
            expression = nullptr;
        }
    }

public:
    const char* variable {nullptr}; // variable name
    ast_node* expression {nullptr}; // RHS
};


class if_else_node : public ast_node
{
public:
    virtual int accept(visitor*);

    virtual ~if_else_node()
    {
        if (condition)
        {
            delete condition;
            condition = nullptr;
        }
        if (then_expr)
        {
            delete then_expr;
            then_expr = nullptr;
        }
        if (else_expr)
        {
            delete else_expr;
            else_expr = nullptr;
        }
    }

public:
    ast_node* condition {nullptr};
    ast_node* then_expr {nullptr};
    ast_node* else_expr {nullptr};
};


class for_loop_node : public ast_node
{
public:
    virtual int accept(visitor*);
    
    virtual ~for_loop_node()
    {
        if (init)
        {
            delete init;
            init = nullptr;
        }
        if (cond)
        {
            delete cond;
            cond = nullptr;
        }
        if (step)
        {
            delete step;
            step = nullptr;
        }
        if (expr)
        {
            delete expr;
            expr = nullptr;
        }
    }

public:
    ast_node* init {nullptr};
    ast_node* cond {nullptr};
    ast_node* step {nullptr};
    ast_node* expr {nullptr};
};


top_level_node* make_top_level_node(ast_node*);

number_node* make_number_node(const char*);

variable_node* make_variable_node(const char*);

binary_expression_node* make_binary_expression_node(ast_node*, ast_node*, char);

call_function_node* make_call_function_node(const char*, double_linked_list_node<ast_node*>*);

function_declaration_node* make_function_declaration_node(const char*, double_linked_list_node<variable_node*>*);

function_definition_node* make_function_definition_node(function_declaration_node*, ast_node*);

block_node* make_block_node(double_linked_list_node<ast_node*>*);

assignment_node* make_assignment_node(const char*, ast_node*);

if_else_node* make_if_else_node(ast_node*, ast_node*, ast_node*);

for_loop_node* make_for_loop_node(ast_node*, ast_node*, ast_node*, ast_node*);


#endif