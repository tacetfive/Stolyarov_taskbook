#include <cstdio>
#include <cmath>

typedef long long sll;
// typedef unsigned long long ull;

class Rational {
    sll num; // numerator
    sll den; // denominator
public:
    sll Get_num() { return num; }
    sll Get_den() { return den; }
// constructors
    Rational(sll m) { num = m; den = 1; }
    Rational(sll m, sll n) {
        sll common_div = FindGCD(m, n);
        num = m/common_div;
        den = n/common_div;
    }
// override arithmetic operators
    Rational operator+(Rational addend);
    Rational operator-(Rational subtr);
    Rational operator*(Rational factor);
    Rational operator/(Rational divisor);
// override assignment operators
    const Rational& operator+=(const Rational& addend);
    const Rational& operator-=(const Rational& subtr);
    const Rational& operator*=(const Rational& factor);
    const Rational& operator/=(const Rational& divisor);
// cast operators
    operator double() { return num/den; }
    operator int() { return round(num/den); }
private:
    static sll FindGCD(sll num1, sll num2); // FindGCD not required Rational object so it static.
};

Rational Rational::operator+(Rational addend) 
{
    Rational sum((num * addend.Get_den() + addend.Get_num() * den),
                    den * addend.Get_den());
    return sum;
}
Rational Rational::operator-(Rational subtr) 
{
    Rational difference((num * subtr.Get_den() - subtr.Get_num() * den),
                    den * subtr.Get_den());
    return difference;
}
Rational Rational::operator*(Rational factor) 
{
    Rational product((num * factor.Get_num()), den * factor.Get_den());
    return product;
}
Rational Rational::operator/(Rational divisor) 
{
    Rational quotient((num * divisor.Get_den()), den * divisor.Get_num());
    return quotient;
}
// override assignment operators
const Rational& Rational::operator+=(const Rational& addend) 
{
    num = num * addend.den + addend.num * den;
    den = den * addend.den;
    return *this;
}
const Rational& Rational::operator-=(const Rational& subtr) 
{
    num = num * subtr.den - subtr.num * den;
    den = den * subtr.den;
    return *this;
}
const Rational& Rational::operator*=(const Rational& factor) 
{
    num *= factor.num;
    den *= factor.den;
    return *this;
}
const Rational& Rational::operator/=(const Rational& divisor) 
{
    num *= divisor.den;
    den *= divisor.num;
    return *this;
}

sll Rational::FindGCD(sll num1, sll num2)
{
    sll dividend;
    sll divisor;
    sll remainder = 1;
    if (num1 == num2) { return (num1); }
    if (num1 > num2) {
        dividend = num1;
        divisor  = num2;
    } else {
        dividend = num2;
        divisor  = num1;
    }
    while(true) {
        remainder = dividend % divisor;
        dividend = divisor;
        if (!remainder) break;
        divisor  = remainder;
    }
    if (divisor < 0) {
        divisor *= (-1);
    }
    printf("%lld and %lld GCD is %lld\n", num1, num2, divisor);
    return divisor;
}

int main()
{
    Rational a(7, 8);
    Rational b(-22, 6);
    printf("a =  %lld / %lld\n", a.Get_num(), a.Get_den());
    printf("b =  %lld / %lld\n", b.Get_num(), b.Get_den());
    Rational c = a + b;
    printf("a + b  %lld / %lld\n", c.Get_num(), c.Get_den());
    c = a - b;
    printf("a - b  %lld / %lld\n", c.Get_num(), c.Get_den());
    c = a * b;
    printf("a * b  %lld / %lld\n", c.Get_num(), c.Get_den());
    c = a / b;
    printf("a / b  %lld / %lld\n", c.Get_num(), c.Get_den());
    b += 4;
    printf("b += 4  %lld / %lld\n", b.Get_num(), b.Get_den());
    b += 7.67;
    printf("b += 7.67  %lld / %lld\n", b.Get_num(), b.Get_den());
    a = b;
    a *= Rational(78,8889);
    printf("%lld / %lld\n", a.Get_num(), a.Get_den());
    a *= Rational(78,8889);
    printf("%lld / %lld\n", a.Get_num(), a.Get_den());
    a *= Rational(78,8889);
    printf("%lld / %lld\n", a.Get_num(), a.Get_den());
    a *= Rational(78,8889);
    printf("%lld / %lld\n", a.Get_num(), a.Get_den());
    a *= Rational(78,8889);
    printf("%lld / %lld\n", a.Get_num(), a.Get_den());
    a *= Rational(78,8889);
    printf("%lld / %lld\n", a.Get_num(), a.Get_den());
    a *= Rational(78,8889);
    printf("%lld / %lld\n", a.Get_num(), a.Get_den());
    a *= Rational(78,8889);
    printf("%lld / %lld\n", a.Get_num(), a.Get_den());
    a = Rational(-14);
    printf("%lld / %lld\n", a.Get_num(), a.Get_den());
    return 0;
}

/* this code causes error:
In member function ‘const Rational& Rational::operator-=(const Rational&)’:
p10t08.cpp:42:34: error: passing ‘const Rational’ as ‘this’ argument discards qualifiers [-fpermissive]
42 |         num = num * subtr.Get_den() - subtr.Get_num() * den;
   |                     ~~~~~~~~~~~~~^~/
When using getters instead of just field names:
    const Rational& operator-=(const Rational& subtr) {
        num = num * subtr.Get_den() - subtr.Get_num() * den;
        den = den * subtr.Get_den();
        return *this;
    }
*/
