#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "mchecker.h"

using namespace std;


void test_normal() {
    // normal 
    std::cout << "------test normal------\n";
    // int *a = (int*)malloc(sizeof(a));
    // free(a);

    // int *b = (int*)calloc(3, sizeof(b));
    // free(b);

    int *c = (int*)malloc(100);
    int *d = (int*)realloc(c, 200);
    free(d);

    // int *e = new int(5);
    // delete e;

    // int *f = new int[5];
    // delete[] f;
    std::cout << "--------------------\n";
}

int main() {
    test_normal();

    return 0;
}