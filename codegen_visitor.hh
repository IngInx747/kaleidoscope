#ifndef KS_CODEGEN_VISITOR_HH
#define KS_CODEGEN_VISITOR_HH

#include "visitor.hh"


struct codegen_impl;

class codegen_visitor : public visitor
{
public:
    codegen_visitor();
    ~codegen_visitor();
    
    void initialize();
    void terminate();

    virtual int visit(number_node*);
    virtual int visit(variable_node*);
    virtual int visit(binary_expression_node*);
    virtual int visit(call_function_node*);
    virtual int visit(function_declaration_node*);
    virtual int visit(function_definition_node*);

protected:
    codegen_impl* impl {nullptr};
};

#endif // KS_CODEGEN_VISITOR_HH