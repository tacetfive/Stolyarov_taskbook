#include <cassert>
#include <limits>
#include <iostream>
#include <cstdio>
#include "p10t08.h"
// #include <cmath>

void test_addition(Rational a, Rational b, Rational expected, int test_counter)
{
    assert((a + b) == expected);
    std::cout << "Test " << test_counter << " completed.\n";
}

void test_subtraction(Rational a, Rational b, Rational expected, int test_counter)
{
    assert((a - b) == expected);
    std::cout << "Test " << test_counter << " completed.\n";
}

void test_multiplication(Rational a, Rational b, Rational expected, int test_counter)
{
    assert((a * b) == expected);
    std::cout << "Test " << test_counter << " completed.\n";
}

void test_division(Rational a, Rational b, Rational expected, int test_counter)
{
    assert((a / b) == expected);
    std::cout << "Test " << test_counter << " completed.\n";
}

int test_overload(Rational a, Rational factor, int test_counter)
{
    sll sll_limit_max = std::numeric_limits<sll>::max();
    sll sll_limit_min = std::numeric_limits<sll>::min();
    ull ull_limit = std::numeric_limits<ull>::max();
    for (int i = 0; i < 50; i++) {
        if (a.Get_num() > 0 && factor.Get_num() > 0) {
            if (sll_limit_max / factor.Get_num() < a.Get_num()) { 
                std::cout << "Test " << test_counter 
                          << ": numerator overload detected.\n"
                          << "a = " << a.Get_num() << " /" << a.Get_den() << "\n";
                return 1;
            }
        }
        /************************* END IT UP
        if (a.Get_num() < 0 || factor.Get_num() < 0) {


                sll_limit_min / factor.Get_num() > a.Get_num()) {
********************/
        if (ull_limit / factor.Get_den() < a.Get_den()) {
            std::cout << "Test " << test_counter 
                      << ": denumerator overload detected.\n"
                      << "a = " << a.Get_num() << " /" << a.Get_den() << "\n";
            return 1;
        }
        a *= factor;
    }
    std::cout << "Test " << test_counter << ": overload was not detected.\n";
    return 0;
}

int main()
{
    Rational a(7,8);
    Rational b(22,7);
    int test_counter = 0;
    test_addition(a, b, Rational(225,56), ++test_counter);
    test_subtraction(a, b, Rational(-127,56), ++test_counter);
    test_multiplication(a, b, Rational(154,56), ++test_counter);
    test_division(a, b, Rational(49,176), ++test_counter);
    test_overload(a, Rational(666, 200), ++test_counter);
    test_overload(a, Rational(66, 888), ++test_counter);
    test_overload(a, Rational(-766, 77), ++test_counter);
    test_overload(a, Rational(-5766, 77), ++test_counter);
    b += 4;
    printf("b += 4  %lld / %llu\n", b.Get_num(), b.Get_den());
    b += 7.67;
    printf("b += 7.67  %lld / %llu\n", b.Get_num(), b.Get_den());
    a = Rational(-14);
    printf("%lld / %llu\n", a.Get_num(), a.Get_den());
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
