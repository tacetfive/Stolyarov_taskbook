// #include <cstdio>
 #include <cmath>

typedef long long sll;
typedef unsigned long long ull;

class Rational {
    sll num; // numerator
    ull den; // denominator
public:
    sll Get_num() { return num; }
    ull Get_den() { return den; }
// constructors
    Rational(sll m) { num = m; den = 1; }
    Rational(sll m, ull n) { num = m; den = n; }
// override arithmetic operators
    Rational operator+(Rational addend);
    Rational operator-(Rational subtr);
    Rational operator*(Rational factor);
    Rational operator/(Rational divisor);
// override assignment operators
    bool operator==(const Rational& other);
    const Rational& operator+=(const Rational& addend);
    const Rational& operator-=(const Rational& subtr);
    const Rational& operator*=(const Rational& factor);
    const Rational& operator/=(const Rational& divisor); 
// cast operators
    operator double() { return num/den; }
    operator int() { return round(num/den); }
    ~Rational() {}
};
