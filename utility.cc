#include "utility.hh"
#include <cstring>


const char* make_c_str(const char* text)
{
    int len = strlen(text);
    char* copy = new char[len + 1];
    strcpy(copy, text);
    return copy;
}
