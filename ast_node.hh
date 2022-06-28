#ifndef KS_AST_NODE_HH
#define KS_AST_NODE_HH

#include "linked_list.hh"


class visitor;


struct ast_node
{
    virtual int accept(visitor*) = 0;
};


struct top_level_node : public ast_node
{
    virtual int accept(visitor*);

    ast_node* content {nullptr};
};


struct number_node : public ast_node
{
    virtual int accept(visitor*);

    const char* value {nullptr};
};


struct variable_node : public ast_node
{
    virtual int accept(visitor*);

    const char* name {nullptr};
};


struct binary_expression_node : public ast_node
{
    virtual int accept(visitor*);

    ast_node* lhs {nullptr};
    ast_node* rhs {nullptr};
    char operation {0};
};


struct call_function_node : public ast_node
{
    virtual int accept(visitor*);

    const char* callee {nullptr}; // the name of the function called
    double_linked_list_node<ast_node*>* arguments {nullptr};
};


struct function_declaration_node : public ast_node
{
    virtual int accept(visitor*);

    const char* name {nullptr}; // function name (use "" for anonymous expression)
    double_linked_list_node<variable_node*>* arguments {nullptr};
};


struct function_definition_node : public ast_node
{
    virtual int accept(visitor*);

    function_declaration_node* declaration {nullptr};
    ast_node* definition {nullptr};
};


struct block_node : public ast_node
{
    virtual int accept(visitor*);

    double_linked_list_node<ast_node*>* expressions {nullptr};
};


struct assignment_node : public ast_node
{
    virtual int accept(visitor*);

    const char* variable {nullptr}; // variable name
    ast_node* expression {nullptr}; // RHS
};


struct if_else_node : public ast_node
{
    virtual int accept(visitor*);

    ast_node* condition {nullptr};
    ast_node* then_expr {nullptr};
    ast_node* else_expr {nullptr};
};


struct for_loop_node : public ast_node
{
    virtual int accept(visitor*);

    ast_node* init {nullptr};
    ast_node* cond {nullptr};
    ast_node* step {nullptr};
    ast_node* expr {nullptr};
};


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