/* First solution is not a tail recursion, because second parameter
 * of max_val() is temporary value and maximum value is unnown until 
 * convolution of recursive calls, so it is right fold approach.
 * I also can not modify it to left fold because it will call max_val()
 * after the recursive call returns.
 * To make solution tail-recursive I need to introduce accumulator
 * that will transit current maximum value to recursive calls. After that,
 * I will can not state solution in pure recursive manner because now I need
 * explicitly state "give and store the current maximum value". However,
 * the immutability requirement will be maintained because accumulator will
 * be initialized in each recursive call and not being reassigned.
 */
#include <stdio.h>
#define ARRLEN 10

int intvecmax(const int *arr, int len, int curr_max)
{
    if (len == 0) return curr_max;
    curr_max = curr_max > *(arr+1) ? curr_max : *(arr+1);
    return intvecmax(&arr[1], len-1, curr_max); /* same as arr+1 */
}
/* test */
int main()
{
    int arr[ARRLEN] = { 4, 20, -3, 234, 52435, 0, -345435, 45, 54345, 87000 };
    printf("%d\n", intvecmax(arr, ARRLEN, *arr));
    return 0;
}
