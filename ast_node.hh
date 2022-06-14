#ifndef KS_AST_NODE_HH
#define KS_AST_NODE_HH

#include "linked_list.hh"


class ast_node;
class number_node;
class variable_node;
class binary_expression_node;
class call_function_node;
class function_declaration_node;
class function_definition_node;


class visitor;


class ast_node
{
public:
    virtual int accept(visitor*) = 0;

    virtual ~ast_node()
    {}

public:
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


number_node* make_number_node(const char*);

variable_node* make_variable_node(const char*);

binary_expression_node* make_binary_expression_node(ast_node*, ast_node*, char);

call_function_node* make_call_function_node(const char*, double_linked_list_node<ast_node*>*);

function_declaration_node* make_function_declaration_node(const char*, double_linked_list_node<variable_node*>*);

function_definition_node* make_function_definition_node(function_declaration_node*, ast_node*);


#endif