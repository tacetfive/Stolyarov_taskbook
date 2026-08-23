/* Recursive thinking:
 * Max value of one-element array is the element.
 * Max value of any other array is max value between its 0th element
 * and max value of remaining subarray.
 * Max value of empty array is undefined.
 */
#include <stdio.h>
#define ARRLEN 10

int max_val(int a, int b)
{
    return a > b ? a : b;
}

int intvecmax(const int *arr, int len)
{
    if (len == 1) return *arr;
    return max_val(*arr, intvecmax(arr + 1, len - 1)); // right fold actually
}
/* test */
int main()
{
    int arr[ARRLEN] = { 4, 3, -3, 234, 52435, 0, -345435, 45, 109, 53333 };
    printf("%d\n", intvecmax(arr, ARRLEN));
    return 0;
}
/* rephrase solution by chatgpt:
 * The maximum of a singleton array is its only element.
 * The maximum of a longer array is the larger of its first element 
 * and the maximum of the remaining subarray.
 */
