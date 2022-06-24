#ifndef KS_PRINT_JSON_VISITOR_HH
#define KS_PRINT_JSON_VISITOR_HH

#include <cstdio> // FILE
#include "visitor.hh"


class print_json_visitor : public visitor
{
public:
    void initialize();
    void terminate();
    void set_output_file_descriptor(FILE*);
    void print_indentation(int);

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
    int node_count {};
    int current_indent {};
    FILE* fd {stdout};
};

#endif // KS_PRINT_JSON_VISITOR_HH