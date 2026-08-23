// #include <cstdio>
#include <cmath>
#include "p10t08.h"

Rational Rational::operator+(Rational addend) {
    Rational sum((num * addend.Get_den() + addend.Get_num() * den),
                    den * addend.Get_den());
    return sum;
}
Rational Rational::operator-(Rational subtr) {
    Rational difference((num * subtr.Get_den() - subtr.Get_num() * den),
                    den * subtr.Get_den());
    return difference;
}
Rational Rational::operator*(Rational factor) {
    Rational product((num * factor.Get_num()), den * factor.Get_den());
    return product;
}
Rational Rational::operator/(Rational divisor) {
    Rational quotient((num * divisor.Get_den()), den * divisor.Get_num());
    return quotient;
}
bool Rational::operator==(const Rational& other) {
    return num == other.num && den == other.den;
}
// override assignment operators
const Rational& Rational::operator+=(const Rational& addend) {
    num = num * addend.den + addend.num * den;
    den = den * addend.den;
    return *this;
}
const Rational& Rational::operator-=(const Rational& subtr) {
    num = num * subtr.den - subtr.num * den;
    den = den * subtr.den;
    return *this;
}
const Rational& Rational::operator*=(const Rational& factor) {
    num *= factor.num;
    den *= factor.den;
    return *this;
}
const Rational& Rational::operator/=(const Rational& divisor) {
    num *= divisor.den;
    den *= divisor.num;
    return *this;
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
