#include <cstdio>

/* [] operator is left-associative, so the expression is evaluated 
 * from left to right. e[1][2] is parsed as (e[1])[2]
 */

class Aux { // Intermediate class that saves first index.
    int value;
public:
    int operator[](int idx_2) {
        return value - idx_2;
    }
    friend class E; // now E can see fields inside Aux without Set() or Get()
//    void Set_val(int other) { value = other; } 
};

class E {
public:
    Aux operator[](int idx_1) {
        Aux a;
        a.value = idx_1;
        return a;
    }
//    friend class Aux;
};

int main()
{
    E e;
    printf("%d %d %d\n", e[0][0], e[100][100], e[-10][-10]); // "0 0 0"
    printf("%d %d %d\n", e[1500][7], e[7][55], e[-8][-16]); //  "1493 -48 8"
    return 0;
}
