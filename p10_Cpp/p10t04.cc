#include <cstdio>

class I {
    int row, col;
public:
    I(int r, int c) { 
        row = r; 
        col = c; 
    }
    int Get_row() { return row; }
    int Get_col() { return col; }
};

class M {
    int matrix[9];
    int size = sizeof(matrix) / sizeof(matrix[0]);
public:
    M() : matrix{ 1, 0, 0, 0, 1, 0, 0, 0, 1, } { } 
    M(const M& other) {
        for ( int idx = 0; idx < size; idx++ )
            matrix[idx] = other.matrix[idx];
    }
    int& operator[]( I idx ) {
        int array_idx = ( idx.Get_row() - 1 ) * 3 + ( idx.Get_col() - 1 );
        return matrix[array_idx];
    }
    M operator+(const M& matrix_2) {
        M result;
        int idx;
        for ( idx = 0; idx < size; ++idx )
            result.matrix[idx] = matrix[idx] + matrix_2.matrix[idx];
        return result;
    }
};

int main()
{
    M m1;
    printf("%d %d %d\n", m1[I(1,1)], m1[I(2,2)], m1[I(2,3)]); // "1 1 0"
    M m2;
    m1[I(2,3)] = 7;
    m2[I(2,3)] = 350;
    M m3(m1 + m2);
    printf("%d %d %d\n", m3[I(1,1)], m3[I(2,2)], m3[I(2,3)]); // "2 2 357"
    return 0;
}
