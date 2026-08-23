#include <stdio.h>
#include <stdlib.h>

int n;
int main()
{
    printf("enter size of diamond, odd number: ");
    scanf("%d", &n);
    for (int i = 1; i<=n; i++)
    {
        int spaces_1 = abs((n+1)/2-i);
        for (int j = spaces_1; j>0; j--)
        {
            printf(" "); // тела двух циклов абсолютно одинаковые -- печать пробелов. Это надо заменить процедурой
        }
        printf("*");
        if ((i==1) || (i==n))
            {
                printf("\n");
                continue;
            }
        int spaces_2 = abs((n/2-1) - spaces_1)*2+1;
        for (int j = 0; j<spaces_2; j++)
        {
            printf(" ");
        }
        printf("*\n");
    }
    return 0;
}

/*
//recursion
void printstring(n)
{

    printstring(n)
}
*/
