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

int print_json_visitor::visit(top_level_node* node)
{
    node->content->accept(this);
    return 0;
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
        //
        print_indentation(current_indent);
        fprintf(fd, "\"operation\": \"%c\",", node->operation);
        //
        print_indentation(current_indent);
        fprintf(fd, "\"left\": {");
        current_indent += 2;
            node->lhs->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "},");
        //
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
        //
        print_indentation(current_indent);
        fprintf(fd, "\"callee\": \"%s\",", node->callee);
        //
        print_indentation(current_indent);
        fprintf(fd, "\"arguments\": [ ");
        current_indent += 2;
        for (double_linked_list_node<ast_node*>* child = first(node->arguments); child != nullptr; child = next(child))
        {
            print_indentation(current_indent);
            fprintf(fd, "{");
            current_indent += 2;
                data(child)->accept(this);
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
        //
        print_indentation(current_indent);
        fprintf(fd, "\"name\": \"%s\",", node->name);
        //
        print_indentation(current_indent);
        fprintf(fd, "\"arguments\": [ ");
        current_indent += 2;
        for (double_linked_list_node<variable_node*>* child = first(node->arguments); child != nullptr; child = next(child))
        {
            print_indentation(current_indent);
            fprintf(fd, "{");
            current_indent += 2;
                data(child)->accept(this);
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
        //
        print_indentation(current_indent);
        fprintf(fd, "\"declaration\": {");
        current_indent += 2;
            node->declaration->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "},");
        //
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

int print_json_visitor::visit(block_node* node)
{
    std::string title("node " + std::to_string(++node_count));
    print_indentation(current_indent);
    fprintf(fd, "\"%s\": {", title.c_str());
    current_indent += 2;
        print_indentation(current_indent);
        fprintf(fd, "\"type\": \"block_node\",");
        //
        print_indentation(current_indent);
        fprintf(fd, "\"expressions\": [ ");
        current_indent += 2;
        for (double_linked_list_node<ast_node*>* child = first(node->expressions); child != nullptr; child = next(child))
        {
            print_indentation(current_indent);
            fprintf(fd, "{");
            current_indent += 2;
                data(child)->accept(this);
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

int print_json_visitor::visit(assignment_node* node)
{
    std::string title("node " + std::to_string(++node_count));
    print_indentation(current_indent);
    fprintf(fd, "\"%s\": {", title.c_str());
    current_indent += 2;
        print_indentation(current_indent);
        fprintf(fd, "\"type\": \"assignment_node\",");
        //
        print_indentation(current_indent);
        fprintf(fd, "\"variable\": \"%s\",", node->variable);
        //
        print_indentation(current_indent);
        fprintf(fd, "\"RHS\": {");
        current_indent += 2;
            node->expression->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "}");
    current_indent -= 2;
    print_indentation(current_indent);
    fprintf(fd, "},");
    return 0;
}

int print_json_visitor::visit(if_else_node* node)
{
    std::string title("node " + std::to_string(++node_count));
    print_indentation(current_indent);
    fprintf(fd, "\"%s\": {", title.c_str());
    current_indent += 2;
        print_indentation(current_indent);
        fprintf(fd, "\"type\": \"if_else_node\",");
        //
        print_indentation(current_indent);
        fprintf(fd, "\"condition\": {");
        current_indent += 2;
            node->condition->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "},");
        //
        print_indentation(current_indent);
        fprintf(fd, "\"then\": {");
        current_indent += 2;
            node->then_expr->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "},");
        //
        print_indentation(current_indent);
        fprintf(fd, "\"else\": {");
        current_indent += 2;
            node->else_expr->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "}");
    current_indent -= 2;
    print_indentation(current_indent);
    fprintf(fd, "},");
    return 0;
}

int print_json_visitor::visit(for_loop_node* node)
{
    std::string title("node " + std::to_string(++node_count));
    print_indentation(current_indent);
    fprintf(fd, "\"%s\": {", title.c_str());
    current_indent += 2;
        print_indentation(current_indent);
        fprintf(fd, "\"type\": \"for_loop_node\",");
        //
        print_indentation(current_indent);
        fprintf(fd, "\"initialization\": {");
        current_indent += 2;
            node->init->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "},");
        //
        print_indentation(current_indent);
        fprintf(fd, "\"condition\": {");
        current_indent += 2;
            node->cond->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "},");
        //
        print_indentation(current_indent);
        fprintf(fd, "\"step\": {");
        current_indent += 2;
            node->step->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "},");
        //
        print_indentation(current_indent);
        fprintf(fd, "\"expression\": {");
        current_indent += 2;
            node->expr->accept(this);
            fprintf(fd, "\b ");
        current_indent -= 2;
        print_indentation(current_indent);
        fprintf(fd, "}");
    current_indent -= 2;
    print_indentation(current_indent);
    fprintf(fd, "},");
    return 0;
}
