#ifndef KS_CODEGEN_VISITOR_HH
#define KS_CODEGEN_VISITOR_HH

#include "visitor.hh"


class codegen_visitor : public visitor
{
protected:
    struct codegen_impl;

public:
    codegen_visitor(const char*);
    ~codegen_visitor();

    void initialize();
    void terminate();

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

protected:
    codegen_impl* impl {nullptr};
};

#endif // KS_CODEGEN_VISITOR_HH