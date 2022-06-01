#include "ast_node.hh"

#include "visitor.hh"


int number_node::accept(visitor* visitor)
{
    return visitor->visit(this);
}

int variable_node::accept(visitor* visitor)
{
    return visitor->visit(this);
}

int binary_expression_node::accept(visitor* visitor)
{
    return visitor->visit(this);
}

int call_function_node::accept(visitor* visitor)
{
    return visitor->visit(this);
}

int function_declaration_node::accept(visitor* visitor)
{
    return visitor->visit(this);
}

int function_definition_node::accept(visitor* visitor)
{
    return visitor->visit(this);
}


number_node* make_number_node(std::string& text)
{
    number_node* node = new number_node();

    if (node)
    {
        std::swap(node->value, text);
    }

    return node;
}

variable_node* make_variable_node(std::string& text)
{
    variable_node* node = new variable_node();

    if (node)
    {
        std::swap(node->name, text);
    }

    return node;
}

binary_expression_node* make_binary_expression_node(ast_node* lhs, ast_node* rhs, char operation)
{
    binary_expression_node* node = new binary_expression_node();

    if (node)
    {
        node->lhs = lhs;
        node->rhs = rhs;
        node->operation = operation;
    }

    return node;
}

call_function_node* make_call_function_node(std::string& text, std::vector<ast_node*>& nodes)
{
    call_function_node* node = new call_function_node();

    if (node)
    {
        std::swap(node->callee, text);
        std::swap(node->arguments, nodes);
    }

    return node;
}

function_declaration_node* make_function_declaration_node(std::string& text, std::vector<ast_node*>& nodes)
{
    function_declaration_node* node = new function_declaration_node();

    if (node)
    {
        std::swap(node->name, text);
        std::swap(node->arguments, nodes);
    }

    return node;
}

function_definition_node* make_function_definition_node(function_declaration_node* declaration, ast_node* definition)
{
    function_definition_node* node = new function_definition_node();

    if (node)
    {
        node->declaration = declaration;
        node->definition = definition;
    }

    return node;
}