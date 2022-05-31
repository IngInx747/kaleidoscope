#ifndef KS_VISITOR_HH
#define KS_VISITOR_HH


#include "ast_node.hh"


class visitor
{
public:
    virtual int visit(number_node*) = 0;

    virtual int visit(variable_node*) = 0;

    virtual int visit(binary_expression_node*) = 0;

    virtual int visit(call_function_node*) = 0;

    virtual int visit(function_declaration_node*) = 0;

    virtual int visit(function_definition_node*) = 0;
};

#endif // KS_VISITOR_HH
