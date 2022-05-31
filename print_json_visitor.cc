#include "print_json_visitor.hh"

#include <cstdio>


static FILE* fd = stdin;


static void print_indentation(int indent)
{
    for (int i = 0; i < indent; ++i)
        fprintf(fd, " ");
}

int print_json_visitor::visit(number_node* node)
{
    print_indentation(this->current_indent);
    fprintf(fd, "\"type\": \"number_node\",\n");
    print_indentation(this->current_indent);
    fprintf(fd, "\"value\": \"%s\"\n", node->value.c_str());
}

int print_json_visitor::visit(variable_node* node)
{
    print_indentation(this->current_indent);
    fprintf(fd, "\"type\": \"variable_node\",\n");
    print_indentation(this->current_indent);
    fprintf(fd, "\"name\": \"%s\"\n", node->name.c_str());
}

int print_json_visitor::visit(binary_expression_node* node)
{
    print_indentation(this->current_indent);
    fprintf(fd, "\"type\": \"binary_expression_node\",\n");
    print_indentation(this->current_indent);
    fprintf(fd, "\"operation\": \"%c\"\n", node->operation);
    print_indentation(this->current_indent);
    fprintf(fd, "\"left\": {\n");
    this->current_indent += 2;
    node->lhs->accept(this);
    this->current_indent -= 2;
    print_indentation(this->current_indent);
    fprintf(fd, "},\n");
    print_indentation(this->current_indent);
    fprintf(fd, "\"right\": {\n");
    this->current_indent += 2;
    node->rhs->accept(this);
    this->current_indent -= 2;
    print_indentation(this->current_indent);
    fprintf(fd, "}\n");
}

int print_json_visitor::visit(call_function_node* node)
{
    print_indentation(this->current_indent);
    fprintf(fd, "\"type\": \"call_function_node\",\n");
    print_indentation(this->current_indent);
    fprintf(fd, "\"callee\": \"%s\"\n", node->callee.c_str());
    print_indentation(this->current_indent);
    fprintf(fd, "\"arguments\": [\n");
    this->current_indent += 2;
    for (ast_node* child : node->arguments)
        child->accept(this);
    this->current_indent -= 2;
    print_indentation(this->current_indent);
    fprintf(fd, "]\n");
}

int print_json_visitor::visit(function_declaration_node* node)
{
    print_indentation(this->current_indent);
    fprintf(fd, "\"type\": \"function_declaration_node\",\n");
    print_indentation(this->current_indent);
    fprintf(fd, "\"name\": \"%s\"\n", node->name.c_str());
    print_indentation(this->current_indent);
    fprintf(fd, "\"arguments\": [\n");
    this->current_indent += 2;
    for (ast_node* child : node->arguments)
        child->accept(this);
    this->current_indent -= 2;
    print_indentation(this->current_indent);
    fprintf(fd, "]\n");
}

int print_json_visitor::visit(function_definition_node* node)
{
    print_indentation(this->current_indent);
    fprintf(fd, "\"type\": \"function_definition_node\",\n");
    print_indentation(this->current_indent);
    fprintf(fd, "\"declaration\": {\n");
    this->current_indent += 2;
    node->declaration->accept(this);
    this->current_indent -= 2;
    print_indentation(this->current_indent);
    fprintf(fd, "},\n");
    print_indentation(this->current_indent);
    fprintf(fd, "\"definition\": {\n");
    this->current_indent += 2;
    node->definition->accept(this);
    this->current_indent -= 2;
    print_indentation(this->current_indent);
    fprintf(fd, "}\n");
}
