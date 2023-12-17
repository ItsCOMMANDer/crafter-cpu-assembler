#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "..\..\include\preprocessor.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("No files, so no");
        return -1;
    }
    //remove_comments(argv[1], argv[2]);
    remove_trailing_whitespaces(argv[1], argv[2]);

    return 42;
}