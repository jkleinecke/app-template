#include "base.cpp"
#include "stdio.h"

int main(int argc, char** argv)
{
    printf("Operation System: %s\n", os_name());
    printf("Compiler: %s %s\n", compiler_name(), compiler_version());
    printf("Architecture: %s\n", arch_name());

    return 0;
}
