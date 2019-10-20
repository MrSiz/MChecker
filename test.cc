#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "mchecker.h"

using namespace std;


void test_normal() {
    // normal 
    std::cout << "------test normal------\n";
    int *a = (int*)malloc(sizeof(a));
    free(a);

    int *b = (int*)calloc(3, sizeof(b));
    free(b);

    int *c = (int*)malloc(100);
    int *d = (int*)realloc(c, 200);
    free(d);

    int *e = new int(5);
    delete e;

    int *f = new int[5];
    delete[] f;
    std::cout << "--------------------\n\n";
}

void test_leak() {
    // leak
    std::cout << "------test leak------\n";
    int *a = (int*)malloc(sizeof(a));

    int *b = new int(2);

    struct Node {
        int *a;
    }*node, *leaf;
    node = (Node*)malloc(sizeof(Node));
    node->a = new int[100];
    free(node);  
    std::cout << "--------------------\n\n";  
}


int main() {
    test_normal();
    test_leak();
    return 0;
}