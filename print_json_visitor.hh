#ifndef KS_PRINT_JSON_VISITOR_HH
#define KS_PRINT_JSON_VISITOR_HH

#include "visitor.hh"


class print_json_visitor : public visitor
{
public:
    virtual int visit(number_node*);

    virtual int visit(variable_node*);

    virtual int visit(binary_expression_node*);

    virtual int visit(call_function_node*);

    virtual int visit(function_declaration_node*);

    virtual int visit(function_definition_node*);

protected:
    int current_indent {};
};

#endif // KS_PRINT_JSON_VISITOR_HH