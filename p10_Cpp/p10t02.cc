#include <cstdio>

class B {
    int number;
public:
    B(int n) { number = n; }
    int Get() { return number; }
    B operator+=(int summand) { number += summand; return *this; }
    // rewrite operator with argument type int and B
/* ???? my getter function returns field 'number', 
 * but operator += returns whole object 
 */
};


int main() 
{
    B first(1), second = 2;
    first += 10; second += 1000;
    printf("%d %d\n", first.Get(), second.Get());
    return 0;
}
