#include <cstdio>

#define MSIZE 9

class M {
    int matrix[MSIZE];
public:
    M() : matrix{ 1, 0, 0, 0, 1, 0, 0, 0, 1, } { } 
    M(const M& other) { // copy constructor
        for ( int idx = 0; idx < MSIZE; idx++ )
            matrix[idx] = other.matrix[idx];
    }
    class Row_index {
        int row_idx;
        M *current_matrix;
    public:
        Row_index(int r_idx, M *c_matrix) 
            : row_idx(r_idx), current_matrix(c_matrix) { }
        int& operator[](int idx) {
            int array_idx = ( row_idx - 1 ) * 3 + ( idx - 1 );
            return current_matrix->matrix[array_idx];
        }
    };
    friend class Row_index;
    Row_index operator[]( int row ) {
        return Row_index(row, this);
    }
    M operator+(const M& matrix_2) {
        M result;
        int idx;
        for ( idx = 0; idx < MSIZE; ++idx )
            result.matrix[idx] = matrix[idx] + matrix_2.matrix[idx];
        return result;
    }
};

int main()
{
    M m1;
    printf("%d %d %d\n", m1[1][1], m1[2][2], m1[2][3]); // "1 1 0"
    M m2;
    m1[2][3] = 7;
    m2[2][3] = 350;
    M m3(m1 + m2);
    printf("%d %d %d\n", m3[1][1], m3[2][2], m3[2][3]); // "2 2 357"
    return 0;
}
