#ifndef KS_DEALLOC_VISITOR_HH
#define KS_DEALLOC_VISITOR_HH

#include "visitor.hh"


class dealloc_visitor : public visitor
{
public:
    virtual int visit(top_level_node*);
    virtual int visit(number_node*);
    virtual int visit(variable_node*);
    virtual int visit(binary_expression_node*);
    virtual int visit(call_function_node*);
    virtual int visit(function_declaration_node*);
    virtual int visit(function_definition_node*);
    virtual int visit(block_node*);
    virtual int visit(assignment_node*);
    virtual int visit(if_else_node*);
    virtual int visit(for_loop_node*);
};

dealloc_visitor* get_dealloc_visitor();

#endif // KS_DEALLOC_VISITOR_HH