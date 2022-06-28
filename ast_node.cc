#include "ast_node.hh"

#include "visitor.hh"


int top_level_node::accept(visitor* visitor)
{
    return visitor->visit(this);
}

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

int block_node::accept(visitor* visitor)
{
    return visitor->visit(this);
}

int assignment_node::accept(visitor* visitor)
{
    return visitor->visit(this);
}

int if_else_node::accept(visitor* visitor)
{
    return visitor->visit(this);
}

int for_loop_node::accept(visitor* visitor)
{
    return visitor->visit(this);
}


//top_level_node* make_top_level_node(ast_node* content)
//{
//    top_level_node* node = new top_level_node();
//
//    if (node)
//    {
//        node->content = content;
//    }
//
//    return node;
//}


number_node* make_number_node(const char* text)
{
    number_node* node = new number_node();

    if (node)
    {
        node->value = text;
    }

    return node;
}

variable_node* make_variable_node(const char* text)
{
    variable_node* node = new variable_node();

    if (node)
    {
        node->name = text;
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

call_function_node* make_call_function_node(const char* text, double_linked_list_node<ast_node*>* nodes)
{
    call_function_node* node = new call_function_node();

    if (node)
    {
        node->callee = text;
        node->arguments = nodes;
    }

    return node;
}

function_declaration_node* make_function_declaration_node(const char* text, double_linked_list_node<variable_node*>* nodes)
{
    function_declaration_node* node = new function_declaration_node();

    if (node)
    {
        node->name = text;
        node->arguments = nodes;
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

block_node* make_block_node(double_linked_list_node<ast_node*>* nodes)
{
    block_node* node = new block_node();

    if (node)
    {
        node->expressions = nodes;
    }

    return node;
}

assignment_node* make_assignment_node(const char* text, ast_node* expression)
{
    assignment_node* node = new assignment_node();

    if (node)
    {
        node->variable = text;
        node->expression = expression;
    }

    return node;
}

if_else_node* make_if_else_node(ast_node* condition, ast_node* then_expr, ast_node* else_expr)
{
    if_else_node* node = new if_else_node();

    if (node)
    {
        node->condition = condition;
        node->then_expr = then_expr;
        node->else_expr = else_expr;
    }

    return node;
}

for_loop_node* make_for_loop_node(ast_node* init, ast_node* cond, ast_node* step, ast_node* expr)
{
    for_loop_node* node = new for_loop_node();

    if (node)
    {
        node->init = init;
        node->cond = cond;
        node->step = step;
        node->expr = expr;
    }

    return node;
}
