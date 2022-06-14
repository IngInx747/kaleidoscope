#include "main.hh"
#include "visitor.hh"
#include "print_json_visitor.hh"
#include "codegen_visitor.hh"


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [source]\n", argv[0]);
        return 1;
    }

    FILE* fd = fopen(argv[1], "r");

    if (!fd)
    {
        fprintf(stderr, "Cannot open file \"%s\".\n", argv[1]);
        return 1;
    }
    
    // set yyin which is used by Lexer
    yyin = fd;

    //print_json_visitor the_visitor;
    //the_visitor.set_output_file_descriptor(stdout);
    codegen_visitor the_visitor(argv[1]);

    // initialize the visitor
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
