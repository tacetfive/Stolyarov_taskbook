
void f(I& x, I y)
{              // calling copy constructor of i2 object. "earth 850" 
    y += 1000; // type of `operator+=` argument is I. calling constr with parameter.
               // "venus 1000"
    x += y;
                // destruct copy of i2 and 1000. "moon\n moon"
}

int main()
{
    I i1;     // calling constructor with no parameters. "sun", i1.i == 6
    I i2(50); // calling constructor with parameter. "venus 50", i2.i == 50
    i2 += 800; // type of `operator+=` argument is I. calling constr with parameter.
               // "venus 800" then i2.i == 850.
               // when `operator+=` ends, destruct 800. Prints "moon"
    f(i1, i2);
    printf("%d %d\n", i1.Get(), i2.Get()); // "1856 850"
    return 0; // i1 and i2 destruct, "moon moon"
}
