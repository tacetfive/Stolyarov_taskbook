#include <stdio.h>
// #include <iostream.h>

class A {
    int n;
public:
    A(int n_u) { n = n_u; }
    int operator[](int i) { // overloading Subscript operator
        return n + i;
    }
};

int main()
{
    A first = 1;
    A second(10);
    printf("first: %d %d\n", first[100], first[200]); // ~$ first: 101 201
    printf("second: %d %d\n", second[100], second[200]); // ~$ second: 110 210
    return 0;
}
