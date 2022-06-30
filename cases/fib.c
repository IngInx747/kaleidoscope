#include <stdio.h>
#include <stdlib.h>

double fib_1(double);
double fib_2(double);

int main(int argc, char** argv)
{
    double a = 10;
    if (argc > 1) a = (double)atoi(argv[1]);
    printf("%lf\n", fib_1(a)); fflush(stdout);
    printf("%lf\n", fib_2(a)); fflush(stdout);
}