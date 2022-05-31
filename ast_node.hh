#ifndef KS_AST_NODE_HH
#define KS_AST_NODE_HH

#include <vector>
#include <string>


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
    {}

public:
    std::string value;
};


class variable_node : public ast_node
{
public:
    virtual int accept(visitor*);

    virtual ~variable_node()
    {}

public:
    std::string name;
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
        for (ast_node* node : arguments)
            delete node;
        arguments.clear();
    }

public:
    std::string callee; // the name of the function called
    std::vector<ast_node*> arguments;
};


class function_declaration_node : public ast_node
{
public:
    virtual int accept(visitor*);

    virtual ~function_declaration_node()
    {
        for (ast_node* node : arguments)
            delete node;
        arguments.clear();
    }

public:
    std::string name; // function name
    std::vector<ast_node*> arguments;
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
    function_declaration_node* declaration;
    ast_node* definition;
};


number_node* make_number_node(std::string&);

variable_node* make_variable_node(std::string&);

binary_expression_node* make_binary_expression_node(ast_node*, ast_node*, char);

call_function_node* make_call_function_node(std::string&, std::vector<ast_node*>&);

function_declaration_node* make_function_declaration_node(std::string&, std::vector<ast_node*>&);

function_definition_node* make_function_definition_node(function_declaration_node*, ast_node*);


#endif