#include "dealloc_visitor.hh"


static dealloc_visitor __dealloc;

dealloc_visitor* get_dealloc_visitor()
{
    return &__dealloc;
}

int dealloc_visitor::visit(top_level_node* node)
{
    if (node->content)
    {
        node->content->accept(this);
        delete node->content;
        node->content = nullptr;
    }

    return 0;
}

int dealloc_visitor::visit(number_node* node)
{
    if (node->value)
    {
        delete [] node->value;
        node->value = nullptr;
    }

    return 0;
}

int dealloc_visitor::visit(variable_node* node)
{
    if (node->name)
    {
        delete [] node->name;
        node->name = nullptr;
    }

    return 0;
}

int dealloc_visitor::visit(binary_expression_node* node)
{
    if (node->lhs)
    {
        node->lhs->accept(this);
        delete node->lhs;
        node->lhs = nullptr;
    }
    if (node->rhs)
    {
        node->rhs->accept(this);
        delete node->rhs;
        node->rhs = nullptr;
    }

    return 0;
}

int dealloc_visitor::visit(call_function_node* node)
{
    if (node->callee)
    {
        delete [] node->callee;
        node->callee = nullptr;
    }
    if (node->arguments)
    {
        for (
            auto* child = first(node->arguments);
            child != nullptr;
            child = next(child))
        {
            data(child)->accept(this);
            delete data(child);
        }
        deallocate(node->arguments);
        node->arguments = nullptr;
    }

    return 0;
}

int dealloc_visitor::visit(function_declaration_node* node)
{
    if (node->name && node->name[0] != '\0')
    {
        delete [] node->name;
        node->name = nullptr;
    }
    if (node->arguments)
    {
        for (
            auto* child = first(node->arguments);
            child != nullptr;
            child = next(child))
        {
            data(child)->accept(this);
            delete data(child);
        }
        deallocate(node->arguments);
        node->arguments = nullptr;
    }

    return 0;
}

int dealloc_visitor::visit(function_definition_node* node)
{
    if (node->declaration)
    {
        node->declaration->accept(this);
        delete node->declaration;
        node->declaration = nullptr;
    }
    if (node->definition)
    {
        node->definition->accept(this);
        delete node->definition;
        node->definition = nullptr;
    }

    return 0;
}

int dealloc_visitor::visit(block_node* node)
{
    if (node->expressions)
    {
        for (
            auto* child = first(node->expressions);
            child != nullptr;
            child = next(child))
        {
            data(child)->accept(this);
            delete data(child);
        }
        deallocate(node->expressions);
        node->expressions = nullptr;
    }

    return 0;
}

int dealloc_visitor::visit(assignment_node* node)
{
    if (node->variable)
    {
        delete [] node->variable;
        node->variable = nullptr;
    }
    if (node->expression)
    {
        node->expression->accept(this);
        delete node->expression;
        node->expression = nullptr;
    }

    return 0;
}

int dealloc_visitor::visit(if_else_node* node)
{
    if (node->condition)
    {
        node->condition->accept(this);
        delete node->condition;
        node->condition = nullptr;
    }
    if (node->then_expr)
    {
        node->then_expr->accept(this);
        delete node->then_expr;
        node->then_expr = nullptr;
    }
    if (node->else_expr)
    {
        node->else_expr->accept(this);
        delete node->else_expr;
        node->else_expr = nullptr;
    }

    return 0;
}

int dealloc_visitor::visit(for_loop_node* node)
{
    if (node->init)
    {
        node->init->accept(this);
        delete node->init;
        node->init = nullptr;
    }
    if (node->cond)
    {
        node->cond->accept(this);
        delete node->cond;
        node->cond = nullptr;
    }
    if (node->step)
    {
        node->step->accept(this);
        delete node->step;
        node->step = nullptr;
    }
    if (node->expr)
    {
        node->expr->accept(this);
        delete node->expr;
        node->expr = nullptr;
    }

    return 0;
}
