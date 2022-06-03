#include "visitor.hh"


static visitor* __the_visitor = nullptr;


visitor* get_the_visitor()
{
    return __the_visitor;
}

void set_the_visitor(visitor* the_visitor)
{
    __the_visitor = the_visitor;
}
