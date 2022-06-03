#include "main.hh"
#include "visitor.hh"
#include "print_json_visitor.hh"


int main(int argc, char** argv)
{
    FILE* fd {nullptr};

    if (argc >= 2)
    {
        fd = fopen(argv[1], "r");
        if (fd) yyin = fd; // yyin is used by Lexer
    }

    // initialize the visitor
    print_json_visitor the_visitor;
    the_visitor.set_output_file_descriptor(stdout);
    the_visitor.initialize();
    set_the_visitor(&the_visitor);

    // parsing routine
    yyparse();

    // clean
    set_the_visitor(nullptr);
    the_visitor.terminate();
    if (fd) { fclose(fd); fd = nullptr; }

    return 0;
}
