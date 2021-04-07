#include <stdio.h>
#include "ex.h"
#include "prog1.h"

void Test_maxargs(void) {
    // a := 5 + 3; b := (print(a, a-1), 10*a); print(b)
    A_stm p = prog();
    printf("statement is: a := 5 + 3; b := (print(a, a-1), 10*a); print(b) \n");
    printf("max args is: %d\n", maxargs(p));
}

int main() {
    Test_maxargs();
}