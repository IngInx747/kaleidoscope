#include "print_json_visitor.hh"
#include <cstdio>
#include <string>


void print_json_visitor::initialize()
{
    fprintf(fd, "{");
    current_indent = 2;
}

void print_json_visitor::terminate()
{
    fprintf(fd, "\b ");
    current_indent = 0;
    fprintf(fd, "\n}");
}

void print_json_visitor::set_output_file_descriptor(FILE* _fd)
{
    fd = _fd;
}

void print_json_visitor::print_indentation(int indent)
{
    fprintf(fd, "\n");
    for (int i = 0; i < indent; ++i)
        fprintf(fd, " ");
}

int print_json_visitor::visit(number_node* node)
{
    std::string title("node " + std::to_string(++node_count));
    print_indentation(current_indent);
    fprintf(fd, "\"%s\": {", title.c_str());
    current_indent += 2;
        print_indentation(current_indent);
        fprintf(fd, "\"type\": \"number_node\",");
        print_indentation(current_indent);
        fprintf(fd, "\"value\": \"%s\"", node->value);
    current_indent -= 2;
    print_indentation(current_indent);
    fprintf(fd, "},");
    return 0;
}

int print_json_visitor::visit(variable_node* node)
{
    std::string title("node " + std::to_string(++node_count));
    print_indentation(current_indent);
    fprintf(fd, "\"%s\": {", title.c_str());
    current_indent += 2;
        print_indentation(current_indent);
        fprintf(fd, "\"type\": \"variable_node\",");
        print_indentation(current_indent);
        fprintf(fd, "\"name\": \"%s\"", node->name);
    current_indent -= 2;
    print_indentation(current_indent);
    fprintf(fd, "},");
    return 0;
}

int print_json_visitor::visit(binary_expression_node* node)
{
    std::string title("node " + std::to_string(++node_count));
    print_indentation(current_indent);
    fprintf(fd, "\"%s\": {", title.c_str());
    current_indent += 2;
        print_indentation(current_indent);
        fprintf(fd, "\"type\": \"binary_expression_node\",");
        print_indentation(current_indent);
        fprintf(fd, "\"operation\": \"%c\",", node->operation);
        print_indentation(current_indent);
        fprintf(fd, "\"left\": {");
        current_indent += 2;
            node->lhs->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "},");
        print_indentation(current_indent);
        fprintf(fd, "\"right\": {");
        current_indent += 2;
            node->rhs->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "}");
    current_indent -= 2;
    print_indentation(current_indent);
    fprintf(fd, "},");
    return 0;
}

int print_json_visitor::visit(call_function_node* node)
{
    std::string title("node " + std::to_string(++node_count));
    print_indentation(current_indent);
    fprintf(fd, "\"%s\": {", title.c_str());
    current_indent += 2;
        print_indentation(current_indent);
        fprintf(fd, "\"type\": \"call_function_node\",");
        print_indentation(current_indent);
        fprintf(fd, "\"callee\": \"%s\",", node->callee);
        print_indentation(current_indent);
        fprintf(fd, "\"arguments\": [");
        current_indent += 2;
        for (double_linked_list_node<ast_node*>* child = first(node->arguments); child != nullptr; child = next(child))
        {
            print_indentation(current_indent);
            fprintf(fd, "{");
            current_indent += 2;
                child->data->accept(this);
                fprintf(fd, "\b ");
            current_indent -= 2;
            print_indentation(current_indent);
            fprintf(fd, "},");
        }
        fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "]");
    current_indent -= 2;
    print_indentation(current_indent);
    fprintf(fd, "},");
    return 0;
}

int print_json_visitor::visit(function_declaration_node* node)
{
    std::string title("node " + std::to_string(++node_count));
    print_indentation(current_indent);
    fprintf(fd, "\"%s\": {", title.c_str());
    current_indent += 2;
        print_indentation(current_indent);
        fprintf(fd, "\"type\": \"function_declaration_node\",");
        print_indentation(current_indent);
        fprintf(fd, "\"name\": \"%s\",", node->name);
        print_indentation(current_indent);
        fprintf(fd, "\"arguments\": [");
        current_indent += 2;
        for (double_linked_list_node<ast_node*>* child = first(node->arguments); child != nullptr; child = next(child))
        {
            print_indentation(current_indent);
            fprintf(fd, "{");
            current_indent += 2;
                child->data->accept(this);
                fprintf(fd, "\b ");
            current_indent -= 2;
            print_indentation(current_indent);
            fprintf(fd, "},");
        }
        fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "]");
    current_indent -= 2;
    print_indentation(current_indent);
    fprintf(fd, "},");
    return 0;
}

int print_json_visitor::visit(function_definition_node* node)
{
    std::string title("node " + std::to_string(++node_count));
    print_indentation(current_indent);
    fprintf(fd, "\"%s\": {", title.c_str());
    current_indent += 2;
        print_indentation(current_indent);
        fprintf(fd, "\"type\": \"function_definition_node\",");
        print_indentation(current_indent);
        fprintf(fd, "\"declaration\": {");
        current_indent += 2;
            node->declaration->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "},");
        print_indentation(current_indent);
        fprintf(fd, "\"definition\": {");
        current_indent += 2;
            node->definition->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "}");
    current_indent -= 2;
    print_indentation(current_indent);
    fprintf(fd, "},");
    return 0;
}
