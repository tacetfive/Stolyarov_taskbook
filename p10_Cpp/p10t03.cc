#include <cstdio>

class D {
    int number;
public:
    D() { number = 0; } // default constructor. From gpt: D() : number(0) {}
    D( const D& obj ) { number = obj.number + 1; } // Copy constructor
    int Get() { return number; }
    void operator=(const D& arg) { number = arg.number + 1;}
};

int main ()
{
    D x;
    D y(x); // constructor with parameter.
    D z = y;
    printf("%d %d %d\n", x.Get(), y.Get(), z.Get()); // "0 1 2"
    return 0;
}


