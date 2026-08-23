

int main() 
{
    B b; // A::i == 0; "first\n third\n"
    A *p = &b; // Pointer type: A*; object type: B. 
               // There's no new object created on this line.
    printf("result = (%d ; %d)\n", p->f(), b.f()); 
    return 0; // destructors for b. "fourth\n second\n"
}

/* p->f() is A::f(). g() is virtual so called B::g() " 14 "
 * b.f() if B::f()  " 3 "






